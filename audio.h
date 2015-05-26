#ifndef ArduboyAudio_h
#define ArduboyAudio_h

#include <Arduino.h>
#include <avr/pgmspace.h>

#define AVAILABLE_TIMERS 2
#define CMD_PLAYNOTE	0x90	/* play a note: low nibble is generator #, note is next byte */
#define CMD_STOPNOTE	0x80	/* stop a note: low nibble is generator # */
#define CMD_RESTART	0xe0	/* restart the score from the beginning */
#define CMD_STOP	0xf0	/* stop playing */



class ArduboyTunes
{
public:
	// Playtune Functions
	void initChannel(byte pin);			// assign a timer to an output pin
	void playScore(const byte *score);	// start playing a polyphonic score
	void stopScore();			// stop playing the score
	void delay(unsigned msec);		// delay in milliseconds
	void stopAll();			// stop all timers
	bool playing();

	// called via interrupt
	void static tune_stepscore (void);
	void static soundOutput();


private:
	void static playNote (byte chan, byte note);
	void static stopNote (byte chan);


};
#endif
