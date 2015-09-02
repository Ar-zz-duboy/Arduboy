#include <SPI.h>
#include "Arduboy.h"
#include <EEPROM.h>
#include "bitmaps.h"

Arduboy arduboy;

int i = 500;
int x = 63;
int y = 31;
byte countdown = 10;
byte counter = 0;
int j[] = {0,0,0,0,0,0,0,15,30,46,62,78,94,108};
boolean zoom = true;

int k = 130;
float l = 0;
byte m = 30;
int o = 1;

int p1 = 1;
int p2 = 1;
int p3 = 1;
int p4 = 1;
int p5 = 1;
int p6 = 1;
int p7 = 1;

int test1 = 1;
int test2 = 1;
int test3 = 1;
int test4 = 1;
int test5 = 1;
int test6 = 1;
int test7 = 1;

void setup() {
  arduboy.start();
  
  arduboy.drawBitmap(20, 4, JO3RI, 88, 56, 1);
  arduboy.display();
  delay(3000);
  
  arduboy.clearDisplay();
  arduboy.drawBitmap(14, 16, TEAMarg, 96, 32, 1);
  arduboy.display();
  delay(3000);
  //Serial.begin(9600);
}

void loop() {
  arduboy.clearDisplay();
  if(counter < 14){
  switch (counter) {
      case 0:
        characterA(15,31,i);
        break;
      case 1:
        characterR(30,31,i);
        break;
      case 2:
        characterD(46,31,i);
        break;
      case 3:
        characterU(63,31,i);
        break;
      case 4:
        characterB(79,31,i);
        break;
      case 5:
        characterO(96,31,i);
        break;
      case 6:
        characterY(111,31,i);
        break;
  }
  if(counter > 0 && counter < 7)characterA(15,31,20);
  if(counter > 1 && counter < 8)characterR(30,31,20);
  if(counter > 2 && counter < 9)characterD(46,31,20);
  if(counter > 3 && counter < 10)characterU(63,31,20);
  if(counter > 4 && counter < 11)characterB(79,31,20);
  if(counter > 5 && counter < 12)characterO(96,31,20);
  if(counter > 6 && counter < 13)characterY(111,31,20);

  switch(counter){
    case 7:
      characterA(j[counter],31,20);
      break;
    case 8:
      characterR(j[counter],31,20);
      break;
    case 9:
      characterD(j[counter],31,20);
      break;
    case 10:
      characterU(j[counter],31,20);
      break;
    case 11:
      characterB(j[counter],31,20);
      break;
    case 12:
      characterO(j[counter],31,20);
      break;
    case 13:
      characterY(j[counter],31,20);
      break;
  }
  if(i <= 20 && zoom == true){
    i = 500;
    counter++;
    if(counter > 6)zoom = false;
  }
  if(zoom == false && j[counter] < 1){
    counter++;
  }
  j[counter] -= 5;
  i -= 20;
}
  if(counter > 13 && counter < 15){
      characterA(k+15+(m-10*sin(l))-5,31+sin(-l+(0*0.4))*10,20+sin(l+(0*0.4))*5);
      characterR(k+45+(m-10*sin(l))-10,31+sin(-l+(1*0.4))*10,20+sin(l+(1*0.4))*5);
      characterD(k+75+(m-10*sin(l))-15,31+sin(-l+(2*0.4))*10,20+sin(l+(2*0.4))*5);
      characterU(k+105+(m-10*sin(l))-20,31+sin(-l+(3*0.4))*10,20+sin(l+(3*0.4))*5);
      characterB(k+135+(m-10*sin(l))-25,31+sin(-l+(4*0.4))*10,20+sin(l+(4*0.4))*5);
      characterO(k+165+(m-10*sin(l))-30,31+sin(-l+(5*0.4))*10,20+sin(l+(5*0.4))*5);
      characterY(k+195+(m-10*sin(l))-35,31+sin(-l+(6*0.4))*10,20+sin(l+(6*0.4))*5);
  k--;
  l+=0.1;
  if(k == -230){
    k=130;
    if(counter < 15)counter++;
  }
  }
  if(counter > 14 && counter < 22){
    
    switch (counter) {
        case 15:
          characterA(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
          break;
        case 16:
          characterR(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
          break;
        case 17:
          characterD(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
          break;
        case 18:
          characterU(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
          break;
        case 19:
          characterB(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
          break;
        case 20:
          characterO(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
          break;
        case 21:
          characterY(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
          break;
    }
    if(o % 63 == 0)counter++;
    o++;
    l+=0.1;
  }
  if(counter > 21 && counter < 23){
    if(test1 < 192){
      characterA(63+(30*sin(l+1.6)),31+(15*sin(l)),15+(5*sin(l)));
      test1++;
    }
    if(test1 == 192 && p1 < 64){
      characterA(66+p1,16,10);
      p1+=3;
    }
    if(test1 == 9)test2++;
    if(test2 > 1 && test2 < 67+128){
      characterR(63+(30*sin(l-0.9+1.6)),31+(15*sin(l-0.9)),15+(5*sin(l-0.9)));
      test2++;
    }
    if(test2 == 195 && p2 < 64){
      characterR(66+p2,16,10);
      p2+=3;
    }
    if(test2 == 9)test3++;
    if(test3 > 1 && test3 < 70+128){
      characterD(63+(30*sin(l-1.8+1.6)),31+(15*sin(l-1.8)),15+(5*sin(l-1.8)));
      test3++;
    }
    if(test3 == 198 && p3 < 64){
      characterD(66+p3,16,10);
      p3+=3;
    }
    if(test3 == 9)test4++;
    if(test4 > 1 && test4 < 73+128){
      characterU(63+(30*sin(l-2.7+1.6)),31+(15*sin(l-2.7)),15+(5*sin(l-2.7)));
      test4++;
    }
    if(test4 == 201 && p4 < 64){
      characterU(66+p4,16,10);
      p4+=3;
    }
    if(test4 == 9)test5++;
    if(test5 > 1 && test5 < 76+128){
      characterB(63+(30*sin(l-3.6+1.6)),31+(15*sin(l-3.6)),15+(5*sin(l-3.6)));
      test5++;
    }
    if(test5 == 204 && p5 < 64){
      characterB(66+p5,16,10);
      p5+=3;
    }
    if(test5 == 9)test6++;
    if(test6 > 1 && test6 < 79+128){
      characterO(63+(30*sin(l-4.5+1.6)),31+(15*sin(l-4.5)),15+(5*sin(l-4.5)));
      test6++;
    }
    if(test6 == 207 && p6 < 64){
      characterO(66+p6,16,10);
      p6+=3;
    }
    if(test6 == 9)test7++;
    if(test7 > 1 && test7 < 82+128){
      characterY(63+(30*sin(l-5.4+1.6)),31+(15*sin(l-5.4)),15+(5*sin(l-5.4)));
      test7++;
    }
    if(test7 == 210 && p7 < 64){
      characterY(66+p7,16,10);
      p7+=3;
      if(p7 == 64)counter++;
    }
    l+=0.1;
  }
  if(counter > 22 && counter < 24){
    characterA(15,19+(13*sin(l)),15+(5*sin(l)));
    characterR(30,19+(13*sin(l)),15+(5*sin(l)));
    characterD(46,19+(13*sin(l)),15+(5*sin(l)));
    characterU(63,19+(13*sin(l)),15+(5*sin(l)));
    characterB(79,19+(13*sin(l)),15+(5*sin(l)));
    characterO(96,19+(13*sin(l)),15+(5*sin(l)));
    characterY(111,19+(13*sin(l)),15+(5*sin(l)));

    characterA(15,44-(13*sin(l)),15+(5*sin(l)));
    characterR(30,44-(13*sin(l)),15+(5*sin(l)));
    characterD(46,44-(13*sin(l)),15+(5*sin(l)));
    characterU(63,44-(13*sin(l)),15+(5*sin(l)));
    characterB(79,44-(13*sin(l)),15+(5*sin(l)));
    characterO(96,44-(13*sin(l)),15+(5*sin(l)));
    characterY(111,44-(13*sin(l)),15+(5*sin(l)));
    l+=0.1;
  }
  //l+=0.1;
  arduboy.display();
  delay(20);
}

void character0(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
}

void character1(float charX, float charY, float scale)
{
  arduboy.drawLine(charX-scale/10,charY-scale/2,charX+scale/10,charY-scale/2,1);
  arduboy.drawLine(charX-scale/10,charY+scale/2,charX+scale/10,charY+scale/2,1);
  arduboy.drawLine(charX-scale/10,charY-scale/2,charX-scale/10,charY+scale/2,1);
  arduboy.drawLine(charX+scale/10,charY-scale/2,charX+scale/10,charY+scale/2,1);
}

void character2(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX-scale/3,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY-(scale/2-((scale/5)*2)),charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY+scale/3,charX+scale/3,charY+scale/2,1);
}

void character3(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/3,charX-scale/3,charY+scale/2,1);
}

void character4(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX+scale/3,charY-scale/2,1);
}

void character5(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+scale/3,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/3,charX-scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+scale/3,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-scale/3,charY+scale/3,1);
}

void character6(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+scale/3,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-(scale/2-((scale/5)*2)),charX+scale/3,charY+scale/2,1);
}

void character7(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
}

void character8(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
}

void character9(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
}

void characterA(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
}

void characterB(float charX, float charY, float scale){
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY-scale/2,charX-((scale/2-((scale/5)*2))*4),charY+scale/2,1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY+scale/2,charX+((scale/2-((scale/5)*2))*4),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*2),charY-(scale/2-((scale/5)*2)),charX+((scale/2-((scale/5)*2))*2),charY-scale/2,1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY-(scale/2-((scale/5)*2)),charX+((scale/2-((scale/5)*2))*2),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY-scale/2,charX+((scale/2-((scale/5)*2))*2),charY-scale/2,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY+scale/2,charX+((scale/2-((scale/5)*2))*4),charY+scale/2,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY+scale/3,charX+((scale/2-((scale/5)*2))*2),charY+scale/3,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),charX+((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY-(scale/2-((scale/5)*2)),charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY-scale/3,charX,charY-scale/3,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY+scale/3,charX-((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*2),charY+scale/3,charX+((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY-scale/3,charX-((scale/2-((scale/5)*2))*2),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX,charY-scale/3,charX,charY-(scale/2-((scale/5)*2)),1);
}

void characterC(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+scale/3,charY+scale/3,1);
}

void characterD(float charX, float charY, float scale){
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY-scale/2,charX,charY-scale/2,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY-scale/3,charX,charY-scale/3,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY+scale/3,charX,charY+scale/3,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY+scale/2,charX,charY+scale/2,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY-scale/2,charX-((scale/2-((scale/5)*2))*4),charY+scale/2,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY-scale/3,charX-((scale/2-((scale/5)*2))*2),charY+scale/3,1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*2),charY-(scale/2-((scale/5)*2)),charX+((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY-(scale/2-((scale/5)*2)),charX+((scale/2-((scale/5)*2))*4),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX,charY-scale/2,charX+((scale/2-((scale/5)*2))*4),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX,charY-scale/3,charX+((scale/2-((scale/5)*2))*2),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX,charY+scale/3,charX+((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX,charY+scale/2,charX+((scale/2-((scale/5)*2))*4),charY+(scale/2-((scale/5)*2)),1);
}

void characterE(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+scale/3,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-(scale/2-((scale/5)*2)),charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+scale/3,charY+scale/3,1);
}

void characterF(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
}

void characterG(float charX, float charY, float scale){
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY-scale/2,charX-((scale/2-((scale/5)*2))*4),charY-scale/2,1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY-scale/3,charX-((scale/2-((scale/5)*2))*2),charY-scale/3,1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY-scale/2,charX+((scale/2-((scale/5)*2))*4),charY-scale/3,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY-scale/2,charX-((scale/2-((scale/5)*2))*4),charY+scale/2,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY+scale/3,charX-((scale/2-((scale/5)*2))*2),charY-scale/3,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*4),charY+scale/2,charX+((scale/2-((scale/5)*2))*4),charY+scale/2,1);
  arduboy.drawLine(charX-((scale/2-((scale/5)*2))*2),charY+scale/3,charX+((scale/2-((scale/5)*2))*2),charY+scale/3,1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY+scale/2,charX+((scale/2-((scale/5)*2))*4),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*2),charY+scale/3,charX+((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*2),charY+(scale/2-((scale/5)*2)),charX,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+((scale/2-((scale/5)*2))*4),charY-(scale/2-((scale/5)*2)),charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX,charY+(scale/2-((scale/5)*2)),charX,charY-(scale/2-((scale/5)*2)),1);
}

void characterH(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
}

void characterI(float charX, float charY, float scale){
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
}

void characterJ(float charX, float charY, float scale){
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+(scale/2-((scale/5)*2)),charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
}

void characterK(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY,charX+scale/3,charY+scale/2,1);
}

void characterL(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+scale/3,charY+scale/3,1);
}

void characterM(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/2,charY-scale/2,charX-scale/3,charY-scale/2,1);
  arduboy.drawLine(charX+scale/2,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/2,charY+scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/2,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/2,charY-scale/2,charX-scale/2,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/3,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/2,charY-scale/2,charX+scale/2,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/3,charX,charY+(scale/2-((scale/5)*2)),1);
}

void characterN(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/2,charY-scale/2,charX-scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/2,charY+scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/2,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX+scale/2,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/2,charY-scale/2,charX-scale/2,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX+scale/2,charY-scale/2,charX+scale/2,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX+scale/3,charY+scale/2,1);
}

void characterO(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
}

void characterP(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
}

void characterQ(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/3,charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/3,charX+scale/3,charY+scale/2,1);
}

void characterR(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX+scale/3,charY+scale/2,1);
}

void characterS(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),charX+scale/3,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),charX-scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/3,charX-scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+scale/3,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-scale/3,charY+scale/3,1);
}

void characterT(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX-(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/3,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
}

void characterU(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/2,1);
}

void characterV(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX,charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX,charY+scale/2,1);
}

void characterW(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/2,charY-scale/2,charX-scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/2,charY+scale/2,charX-scale/3,charY+scale/2,1);
  arduboy.drawLine(charX+scale/2,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX+scale/2,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/2,charY-scale/2,charX-scale/2,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX+scale/2,charY-scale/2,charX+scale/2,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX-scale/3,charY+scale/3,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY+scale/3,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX,charY+(scale/2-((scale/5)*2)),1);
}

void characterX(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-(scale/2-((scale/5)*2)),charY,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/2,charX,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY+scale/2,charX,charY+(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+(scale/2-((scale/5)*2)),charY,1);
}

void characterY(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY-scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/2,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY,charX-(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY,charX+(scale/2-((scale/5)*2)),charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-(scale/2-((scale/5)*2)),charY,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY-scale/2,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/2,charX,charY-(scale/2-((scale/5)*2)),1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+(scale/2-((scale/5)*2)),charY,1);
}

void characterZ(float charX, float charY, float scale){
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX+scale/3,charY-scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/3,charX+(scale/2-((scale/5)*2)),charY-scale/3,1);
  arduboy.drawLine(charX-(scale/2-((scale/5)*2)),charY+scale/3,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX+scale/3,charY+scale/2,1);
  arduboy.drawLine(charX-scale/3,charY-scale/2,charX-scale/3,charY-scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/2,charX+scale/3,charY-scale/3,1);
  arduboy.drawLine(charX-scale/3,charY+scale/2,charX-scale/3,charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY+scale/2,charX+scale/3,charY+scale/3,1);
  arduboy.drawLine(charX+(scale/2-((scale/5)*2)),charY-scale/3,charX-scale/3,charY+scale/3,1);
  arduboy.drawLine(charX+scale/3,charY-scale/3,charX-(scale/2-((scale/5)*2)),charY+scale/3,1);
}
