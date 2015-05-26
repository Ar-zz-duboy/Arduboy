#include "audio.h"

const byte PROGMEM tune_pin_to_timer_PGM[] = { 3, 1 };
volatile byte *timer1_pin_port;
volatile byte timer1_pin_mask;
volatile byte *timer3_pin_port;
volatile byte timer3_pin_mask;
byte _tune_pins[AVAILABLE_TIMERS];
byte _tune_num_chans = 0;
volatile boolean tune_playing; // is the score still playing?
volatile unsigned wait_timer_frequency2;       /* its current frequency */
volatile unsigned wait_timer_old_frequency2;   /* its previous frequency */
volatile boolean wait_timer_playing = false;   /* is it currently playing a note? */
volatile boolean doing_delay = false;          /* are we using it for a tune_delay()? */
volatile unsigned long wait_toggle_count;      /* countdown score waits */
volatile unsigned long delay_toggle_count;     /* countdown tune_ delay() delays */


// pointers to your musical score and your position in said score
volatile const byte *score_start = 0;
volatile const byte *score_cursor = 0;

// Table of midi note frequencies * 2
//   They are times 2 for greater accuracy, yet still fits in a word.
//   Generated from Excel by =ROUND(2*440/32*(2^((x-9)/12)),0) for 0<x<128
// The lowest notes might not work, depending on the Arduino clock frequency
const unsigned int PROGMEM _midi_note_frequencies[128] = {
16,17,18,19,21,22,23,24,26,28,29,31,33,35,37,39,41,44,46,49,52,55,58,62,65,
69,73,78,82,87,92,98,104,110,117,123,131,139,147,156,165,175,185,196,208,220,
233,247,262,277,294,311,330,349,370,392,415,440,466,494,523,554,587,622,659,
698,740,784,831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,
1865,1976,2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,
4435,4699,4978,5274,5588,5920,6272,6645,7040,7459,7902,8372,8870,9397,9956,
10548,11175,11840,12544,13290,14080,14917,15804,16744,17740,18795,19912,21096,
22351,23680,25088 };


void ArduboyTunes::initChannel(byte pin) {
  byte timer_num;
  tune_playing = false;

  if (_tune_num_chans < AVAILABLE_TIMERS) {
    timer_num = pgm_read_byte(tune_pin_to_timer_PGM + _tune_num_chans);
    _tune_pins[_tune_num_chans] = pin;
    _tune_num_chans++;
    pinMode(pin, OUTPUT);
    switch (timer_num) {
      case 1:
        // 16 bit timer
        TCCR1A = 0;
        TCCR1B = 0;
        bitWrite(TCCR1B, WGM12, 1);
        bitWrite(TCCR1B, CS10, 1);
        timer1_pin_port = portOutputRegister(digitalPinToPort(pin));
        timer1_pin_mask = digitalPinToBitMask(pin);
        break;
      case 3:
        // 16 bit timer
        TCCR3A = 0;
        TCCR3B = 0;
        bitWrite(TCCR3B, WGM32, 1);
        bitWrite(TCCR3B, CS30, 1);
        timer3_pin_port = portOutputRegister(digitalPinToPort(pin));
        timer3_pin_mask = digitalPinToBitMask(pin);
        playNote(0, 60);  /* start and stop channel 0 (timer 3) on middle C so wait/delay works */
        stopNote(0);
        break;
    }
  }
}


void ArduboyTunes::playNote (byte chan, byte note) {
  byte timer_num;
  byte prescalarbits = 0b001;
  unsigned int frequency2; /* frequency times 2 */
  unsigned long ocr;

  if (note>127)
    note = 127;

  if (chan < _tune_num_chans) {
    timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
    frequency2 = pgm_read_word (_midi_note_frequencies + note);

    //******  16-bit timer  *********
    // two choices for the 16 bit timers: ck/1 or ck/64
    ocr = F_CPU / frequency2 - 1;
    prescalarbits = 0b001;
    if (ocr > 0xffff) {
      ocr = F_CPU / frequency2 / 64 - 1;
      prescalarbits = 0b011;
    }
    if (timer_num == 1)
      TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;
    else if (timer_num == 3)
      TCCR3B = (TCCR3B & 0b11111000) | prescalarbits;
    // Set the OCR for the given timer, then turn on the interrupts
    switch (timer_num) {
      case 1:
        OCR1A = ocr;
        bitWrite(TIMSK1, OCIE1A, 1);
        break;
      case 3:
        OCR3A = ocr;
        wait_timer_frequency2 = frequency2;  // for "tune_delay" function
        wait_timer_playing = true;
        bitWrite(TIMSK3, OCIE3A, 1);
        break;
    }
  }
}

void ArduboyTunes::stopNote (byte chan) {
  byte timer_num;
  timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
  switch (timer_num) {
    case 1:
      TIMSK1 &= ~(1 << OCIE1A);                 // disable the interrupt
      *timer1_pin_port &= ~(timer1_pin_mask);   // keep pin low after stop
      break;
    case 3:
      wait_timer_playing = false;
      *timer3_pin_port &= ~(timer3_pin_mask);   // keep pin low after stop
      break;
  }
}

void ArduboyTunes::playScore (const byte *score) {
  score_start = score;
  score_cursor = score;
  tune_stepscore();  /* execute initial commands */
  tune_playing = true;  /* release the interrupt routine */
}

void ArduboyTunes::stopScore (void) {
  int i;
  for (i = 0; i < _tune_num_chans; ++i)
    stopNote(i);
  tune_playing = false;
}

bool ArduboyTunes::playing()
{
  return tune_playing;
}


/* Do score commands until a "wait" is found, or the score is stopped.
This is called initially from tune_playcore, but then is called
from the interrupt routine when waits expire.
*/
/* if CMD < 0x80, then the other 7 bits and the next byte are a 15-bit big-endian number of msec to wait */
void ArduboyTunes::tune_stepscore (void) {
  byte cmd, opcode, chan;
  unsigned duration;
  while (1) {
    cmd = pgm_read_byte(score_cursor++);
    if (cmd < 0x80) { /* wait count in msec. */
      duration = ((unsigned)cmd << 8) | (pgm_read_byte(score_cursor++));
      wait_toggle_count = ((unsigned long) wait_timer_frequency2 * duration + 500) / 1000;
      if (wait_toggle_count == 0) wait_toggle_count = 1;
      break;
    }
    opcode = cmd & 0xf0;
    chan = cmd & 0x0f;
    if (opcode == CMD_STOPNOTE) { /* stop note */
      stopNote(chan);
    }
    else if (opcode == CMD_PLAYNOTE) { /* play note */
      playNote(chan, pgm_read_byte(score_cursor++));
    }
    else if (opcode == CMD_RESTART) { /* restart score */
      score_cursor = score_start;
    }
    else if (opcode == CMD_STOP) { /* stop score */
      tune_playing = false;
      break;
    }
  }
}

void ArduboyTunes::delay (unsigned duration) {
  boolean notdone;
  noInterrupts();
  delay_toggle_count = ((unsigned long) wait_timer_frequency2 * duration + 500) / 1000;
  doing_delay = true;
  interrupts();
  do { // wait until the interrupt routines decrements the toggle count to zero
    noInterrupts();
    notdone = delay_toggle_count != 0;  /* interrupt-safe test */
    interrupts();
  }
  while (notdone);
  doing_delay = false;
}

void ArduboyTunes::stopAll(void) {
  byte chan;
  byte timer_num;
  for (chan=0; chan<_tune_num_chans; ++chan) {
    timer_num = pgm_read_byte(tune_pin_to_timer_PGM + chan);
    switch (timer_num) {
      case 1:
        TIMSK1 &= ~(1 << OCIE1A);
        break;
      case 3:
        TIMSK3 &= ~(1 << OCIE3A);
        break;
    }
    digitalWrite(_tune_pins[chan], 0);
  }
  _tune_num_chans = 0;
}

void ArduboyTunes::soundOutput()
{
  if (wait_timer_playing) { // toggle the pin if we're sounding a note
    *timer3_pin_port ^= timer3_pin_mask;
  }
  if (tune_playing && wait_toggle_count && --wait_toggle_count == 0) {
    // end of a score wait, so execute more score commands
    wait_timer_old_frequency2 = wait_timer_frequency2;  // save this timer's frequency
    ArduboyTunes::tune_stepscore();  // execute commands
    // If this timer's frequency has changed and we're using it for a tune_delay(),
    // recompute the number of toggles to wait for
    if (doing_delay && wait_timer_old_frequency2 != wait_timer_frequency2) {
      if (delay_toggle_count >= 0x20000UL && wait_timer_frequency2 >= 0x4000U) {
        // Need to avoid 32-bit overflow...
        delay_toggle_count = ( (delay_toggle_count+4>>3) * (wait_timer_frequency2+2>>2) / wait_timer_old_frequency2 )<<5;
      }
      else {
        delay_toggle_count = delay_toggle_count * wait_timer_frequency2 / wait_timer_old_frequency2;
      }
    }
  }
  if (doing_delay && delay_toggle_count) --delay_toggle_count;  // countdown for tune_delay()
}

ISR(TIMER1_COMPA_vect) {  // TIMER 1
  *timer1_pin_port ^= timer1_pin_mask;  // toggle the pin
}
ISR(TIMER3_COMPA_vect) {  // TIMER 3
  // Timer 3 is the one assigned first, so we keep it running always
  // and use it to time score waits, whether or not it is playing a note.
  ArduboyTunes::soundOutput();
}

