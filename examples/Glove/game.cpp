#include <Arduino.h>
#include "globals.h"
#include "prompt.h"
#include "levels.h"
#include "game.h"
#include "records.h"

#define FACE_UP 0
#define FACE_DOWN 1
#define FACE_LEFT 2
#define FACE_RIGHT 3

#define TONE_EXPLORER_FIRE 987
#define TONE_BADGUY_DESTROY 486
#define TONE_SPAWNER_DAMAGE 1200
#define TONE_SPAWNER_DESTROY 300
#define TONE_KEY_ACTIVATE 800

#define TONE_GOLD_GET 600
#define TONE_POO_GET 400
#define TONE_CUP_GET 900
#define TONE_LEMON_GET 1000

#define TREASURE_NONE 0
#define TREASURE_GOLD 1
#define TREASURE_POO 2
#define TREASURE_CUP 3
#define TREASURE_LEMON 4

#define MAP_WIDTH 32
#define MAP_HEIGHT 16

const short gamew = 256;
const short gameh = 128;

short spawnx;
short spawny;

char autoFireTime;
char whiteScreenTime;

unsigned short score;
short rollingScore;
unsigned short rollingHealth;

unsigned char currentLevel;
unsigned char levelsCompleted;

unsigned short gameTime;
short gameTimeTick;

bool quitGame;

PROGMEM const unsigned char badGuySpriteA[] = {
	0b00011100,
	0b00110110,
	0b01000100,
	0b10100100,
	0b10100100,
	0b01000100,
	0b00110110,
	0b00011100,
};

PROGMEM const unsigned char guyWalkLeftA[] = {
	0b00000000,
	0b00000000,
	0b10001100,
	0b11111111,
	0b11111111,
	0b00001100,
	0b00000000,
	0b00000000
};

PROGMEM const unsigned char guyWalkLeftB[] = {
	0b00000000,
	0b10000100,
	0b11001000,
	0b00111111,
	0b00111111,
	0b01000100,
	0b11001100,
	0b00000000
};

PROGMEM const unsigned char guyWalkForward[] = {
	0b00000000,
	0b00001000,
	0b01100100,
	0b00111111,
	0b00111111, // xor bit 2
	0b11001100,
	0b00000000,
	0b00000000
};

PROGMEM const unsigned char guyWalkBackward[] = {
	0b00000000,
	0b00000000,
	0b01101100,
	0b00111111,
	0b00111111,
	0b11001000,
	0b00000000, // xor bit 2
	0b00000000
};

PROGMEM const unsigned char wallSprite[] = {
	0b10000001,
	0b11000010,
	0b11100100,
	0b11111000,
	0b11111000,
	0b11101100,
	0b11010110,
	0b10101011
};

PROGMEM const unsigned char wallSpriteB[] = {
	0b00000000,
	0b11111110,
	0b10100010,
	0b01100010,
	0b10100010,
	0b01100010,
	0b11111110,
	0b00000000
};

PROGMEM const unsigned char spawnerSprite[] = {
	0b00000000,
	0b01010100,
	0b00000000,
	0b00101010,
	0b00000000,
	0b01010100,
	0b00000000,
	0b00101010
};

PROGMEM const unsigned char exitSprite[] = {
	0b01010111,
	0b01110111,
	0b01010101,
	0b00000000,
	0b00010101,
	0b01110010,
	0b00010101,
	0b00000000
};

PROGMEM const unsigned char keySprite[] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b10101110,
	0b11111010,
	0b00001110,
	0b00000000,
	0b00000000
};

PROGMEM const unsigned char goldSprite[] = {
	0b01100010,
	0b01110000,
	0b01111001,
	0b01011100,
	0b00101110,
	0b10010110,
	0b00001010,
	0b01000100
};

PROGMEM const unsigned char pooSprite[] = {
	0b00100000,
	0b01001000,
	0b01010000,
	0b01010100,
	0b01010110,
	0b01010000,
	0b01001000,
	0b00100000
};

PROGMEM const unsigned char cupSprite[] = {
	0b00001100,
	0b00011000,
	0b01011000,
	0b01111000,
	0b01011000,
	0b00011000,
	0b00001100,
	0b00000000
};

PROGMEM const unsigned char lemonSprite[] = {
	0b00010000,
	0b00111000,
	0b01111100,
	0b01110100,
	0b01111100,
	0b01110100,
	0b00111000,
	0b00010000,
};

void displayLoadGame(byte file);
void saveGame(byte file);

void gameSetupWorld();
void gameGotoLevel(unsigned char level);
void fadeOut();
void doGameComplete();
void doGameOver();
void flashScreen();
bool saveHighScore();

void markRoomAsCleared(unsigned char room);
void deleteContinueSave();

short scrollx;
short scrolly;
	
struct Bullet {
	short x;
	short y;
	char vx;
	char vy;
	char lifetime;
	bool active;
};

Bullet bullets[3];
const int numBullets = 3;

void add_bullet(short x, short y, short vx, short vy);
void update_bullet(Bullet &obj);
void draw_bullet(Bullet& obj);

struct BadGuy {
	short x;
	short y;
	bool active;
};

char BadGuy_UpdateDelay;

void add_badguy(short x, short y);
void update_badguy(BadGuy &b);
void draw_badguy(BadGuy &b);
void destroy_badguy(BadGuy &obj);


BadGuy badguys[20];
const int numBadguys = 20;

struct Explorer {
	short x;
	short y;
	char direction;
	bool frame;
	char frameTime;
	bool active;
	short health;
	char nextHealthDecrease;
};
Explorer p1;
void damage_explorer(Explorer& obj, short dmg);

struct Wall {
	char x;
	char y;
	char w;
	char h;
	bool active;
	char style;
};

void add_wall(char x, char y, char w, char h, char style);
void draw_wall(Wall &obj);

Wall walls[16];
const int numWalls = 16;

struct Key {
	char x;
	char y;
	unsigned char target;
	bool active;
};

Key keys[4];
const int numKeys = 4;

struct Treasure {
	char x;
	char y;
	char type;
	bool active;
};
Treasure treasures[9];
const int numTreasures = 9;

void add_treasure(char x, char y, char type);
void activate_treasure(Treasure& obj);

struct Spawner {
	char x;
	char y;
	bool active;
	char spawnDelay;
	char health;
};

void add_spawner(char x, char y);
void draw_spawner(Spawner &obj);
void destroy_spawner(Spawner& obj);
void damage_spawner(Spawner& obj, short dmg);

Spawner spawners[4];
const int numSpawners = 4;

struct Exit{
	char x;
	char y;
	char dest;
	bool active;
};

void add_exit(char x, char y, char dest);
void draw_exit(Wall &obj);

Exit exits[4];
const int numExits = 4;

////////////////// BULLET ////////////////
void add_bullet(short x, short y, short vx, short vy)
{
	for(int i=0; i<numBullets; ++i) {
		if(!bullets[i].active) {
			display.tunes.tone(TONE_EXPLORER_FIRE, 20);
			bullets[i].active = true;
			bullets[i].x = x;
			bullets[i].y = y;
			bullets[i].vx = vx;
			bullets[i].vy = vy;
			bullets[i].lifetime = 20;
			return;
		}
	}
	//error("ERR: Max Bullets XCDd");
}

void update_bullet(Bullet &obj)
{
	short wx, wy, ww, wh;
	int i;

	if(obj.active) {	
		obj.x += obj.vx;
		if((obj.x + 4 < 0) || (obj.x > gamew)) {
			obj.active = false;
			return;
		}
		
		obj.y += obj.vy;
		if((obj.y + 4 < 0) || (obj.y > gameh)) {
			obj.active = false;
			return;
		}
		
		// Collision Test
		
		for(i=0; i<numBadguys; ++i) {
			if(badguys[i].active && (
					(obj.x < badguys[i].x + 8) &&
					(obj.x + 1 > badguys[i].x) &&
					(obj.y < badguys[i].y + 8) &&
					(obj.y + 1 > badguys[i].y)))
			{
				obj.active = false;
				destroy_badguy(badguys[i]);
				return;
			}
		}	
		
		obj.lifetime--;
		obj.active = obj.lifetime > 0;
		
		for(i = 0; i<numWalls; ++i) {
			if(!walls[i].active) continue;
			wx = walls[i].x*8;
			wy = walls[i].y*8;
			ww = walls[i].w*8;
			wh = walls[i].h*8;
			if((obj.x < wx + ww) &&
					(obj.x + 1 > wx) &&
					(obj.y < wy + wh) &&
					(obj.y + 1 > wy))
			{
				obj.active = false;
			}
		}
		
		// Hit spawners
		for(i = 0; i<numSpawners; ++i) {
			if(!spawners[i].active) continue;
			wx = spawners[i].x*8;
			wy = spawners[i].y*8;
			ww = 8;
			wh = 8;
			if((obj.x < wx + ww) &&
					(obj.x + 1 > wx) &&
					(obj.y < wy + wh) &&
					(obj.y + 1 > wy))
			{
				damage_spawner(spawners[i], 10);
				obj.active = false;
			}
		}
	}
}

void draw_bullet(Bullet& obj)
{
	if(obj.active) {
		display.drawPixel(obj.x+scrollx,   obj.y+scrolly,   1);
		display.drawPixel(obj.x+1+scrollx, obj.y+scrolly,   1);
		display.drawPixel(obj.x+scrollx,   obj.y+1+scrolly, 1);
		display.drawPixel(obj.x+1+scrollx, obj.y+1+scrolly, 1);
	}	
}

//////////////// WALL //////////////////////
void add_wall(char x, char y, char w, char h, char style)
{
	for(int i=0; i<numWalls; ++i) {
		if(!walls[i].active) {
			walls[i].active = true;
			walls[i].x = x;
			walls[i].y = y;
			walls[i].w = w;
			walls[i].h = h;
			walls[i].style = style;
			return;
		}
	}
}

void draw_wall(Wall &obj)
{
	if(obj.active) {
		int x, y;
		for(y = obj.y; y < obj.y+obj.h; ++y) {
			for(x = obj.x; x < obj.x+obj.w; ++x) {
				if(obj.style == 0)
					display.drawBitmap(x*8+scrollx, y*8+scrolly, wallSprite, 8, 8, 1);
				else if(obj.style == 1)
					display.drawBitmap(x*8+scrollx, y*8+scrolly, wallSpriteB, 8, 8, 1);
			}
		}
	}
}

///////////////// KEY ////////////////////////
void add_key(char x, char y, unsigned char target)
{
	for(int i=0; i<numKeys; i++) {
		if(!keys[i].active) {
			keys[i].active = true;
			keys[i].x = x;
			keys[i].y = y;
			keys[i].target = target;
			return;
		}
	}
}
void draw_key(Key & obj)
{
	if(obj.active)
		display.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, keySprite, 8, 8, 1);
}
void activate_key(Key& obj)
{
	if(obj.active)
	{
		walls[obj.target].active = false;
		obj.active = false;
		display.tunes.tone(TONE_KEY_ACTIVATE, 20);
	}
}

//////////////// TREASURE /////////////////

void add_treasure(char x, char y, char type)
{
	for(int i=0; i<numTreasures; i++) {
		if(!treasures[i].active) {
			treasures[i].active = true;
			treasures[i].x = x;
			treasures[i].y = y;
			treasures[i].type = type;
			return;
		}
	}
}
void draw_treasure(Treasure& obj)
{
	if(!obj.active) return;
	
	switch(obj.type) {
		case TREASURE_GOLD:
			display.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, goldSprite, 8, 8, 1);
			break;
		case TREASURE_POO:
			display.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, pooSprite, 8, 8, 1);
			break;
		case TREASURE_CUP:
			display.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, cupSprite, 8, 8, 1);
			break;
		case TREASURE_LEMON:
			display.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, lemonSprite, 8, 8, 1);
			break;
	}
}
void addScore(short points)
{
		if(rollingScore < 0) rollingScore = points;
		else rollingScore += points;
}

void activate_treasure(Treasure& obj)
{
	char i;
	obj.active = false;
	switch(obj.type) {
		case TREASURE_GOLD: // Awards 10 points
			addScore(10);
			display.tunes.tone(TONE_GOLD_GET, 20);
			break;
		case TREASURE_POO: // Kills all baddies on screen
			for(i = 0; i < numBadguys; ++i) {
				badguys[i].active = false;
			}
			flashScreen();
			display.tunes.tone(TONE_POO_GET, 20);
			break;
		case TREASURE_CUP: // Awards 6 points
			addScore(6);
			display.tunes.tone(TONE_CUP_GET, 20);
			break;
		case TREASURE_LEMON: // Awards 300 health
			rollingHealth += 300;
			display.tunes.tone(TONE_LEMON_GET, 20);
			break;
	}
	obj.active = false;
}

/////////////// SPAWNER ///////////////////

void add_spawner(char x, char y)
{
	for(int i=0; i<numSpawners; i++) {
		if(!spawners[i].active) {
			spawners[i].active = true;
			spawners[i].x = x;
			spawners[i].y = y;
			spawners[i].spawnDelay = 100;
			spawners[i].health = 50;
			return;
		}
	}
}
void update_spawner(Spawner& obj)
{
	if(!obj.active) return;
	if(obj.spawnDelay <= 0) {
		add_badguy(obj.x*8 + 4, obj.y*8 + 4);
		obj.spawnDelay = 100;
	}
	obj.spawnDelay--;
}
void draw_spawner(Spawner& obj)
{
	if(obj.active)
		display.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, spawnerSprite, 8, 8, 1);
}
void destroy_spawner(Spawner& obj)
{
	obj.active = false;
}
void damage_spawner(Spawner& obj, short dmg)
{
	obj.health -= dmg;
	if(obj.health <= 0)
	{
		obj.active = false;
		addScore(1);
		display.tunes.tone(TONE_SPAWNER_DESTROY, 20);
	} else {
		display.tunes.tone(TONE_SPAWNER_DAMAGE, 20);
	}
}

//////////////// EXIT /////////////////////
void add_exit(char x, char y, char dest)
{
	for(int i=0; i<numSpawners; ++i) {
		if(!exits[i].active) {
			exits[i].active = true;
			exits[i].x = x;
			exits[i].y = y;
			exits[i].dest = dest;
			return;
		}
	}
}

void draw_exit(Exit& obj)
{
	if(obj.active)
		display.drawBitmap(obj.x*8+scrollx, obj.y*8+scrolly, exitSprite, 8, 8, 1);	
}

void activate_exit(Exit& obj)
{
	//fadeOut();
	levelsCompleted++;
	if(!quitGame)
		gameGotoLevel(obj.dest);
}


//////////////// EXPLORER /////////////////
void initialize_explorer(Explorer& obj)
{
	obj.x = spawnx*8;
	obj.y = spawny*8;
	obj.direction = FACE_UP;
	obj.frame = false;
	obj.frameTime = 0;
	obj.active = true;
	obj.health = 1000;
	obj.nextHealthDecrease = 5;
}

void update_explorer(Explorer& obj)
{
	if(!obj.active) {
		if((B_PRESSED || A_PRESSED) && !quitGame) {
			doGameOver();
		}
		return;
	}
	if(UP_DOWN && DOWN_DOWN && LEFT_DOWN && RIGHT_DOWN && B_DOWN && A_PRESSED) {
		//gameGotoLevel(0);
	}
	char i, vx, vy;
	
	if(rollingHealth > 0) {
		rollingHealth -= 10;
		obj.health += 10;
		if(obj.health > 1000) {
			obj.health = 1000;
			rollingHealth = 0;
		}
		
	}
	
	short wx, wy, ww, wh;
	
	if(obj.nextHealthDecrease <= 0) {
		obj.nextHealthDecrease = 5;
		damage_explorer(obj, 1);
	}
	obj.nextHealthDecrease--;
	
	if(LEFT_DOWN || RIGHT_DOWN || UP_DOWN || DOWN_DOWN) {
		obj.frameTime ++;
		if(obj.frameTime > 8) {
			obj.frame = !obj.frame;
			obj.frameTime = 0;
		}
	} else {
		if(obj.direction == FACE_LEFT || obj.direction == FACE_RIGHT)
			obj.frame = true;
	}
	
	//if(A_PRESSED || B_PRESSED || (B_DOWN && autoFireTime <= 0)) {
	if(A_PRESSED) {
		autoFireTime = 8;
		short nvx = (obj.direction == FACE_LEFT || LEFT_DOWN) ? -2 : 0;
		nvx = (obj.direction == FACE_RIGHT || RIGHT_DOWN) ? 2 : nvx;
		short nvy = (obj.direction == FACE_UP || UP_DOWN) ? -2 : 0;
		nvy = (obj.direction == FACE_DOWN || DOWN_DOWN) ? 2 : nvy;
		
		add_bullet(obj.x+3, obj.y+3, nvx+vx, nvy+vy);
	}
	if(B_DOWN && (autoFireTime>0)) autoFireTime--;
	
	if(RIGHT_DOWN) {
		vx = 1;
		obj.direction = FACE_RIGHT;
	} else if(LEFT_DOWN) {
		vx = -1;
		obj.direction = FACE_LEFT;
	} else {
		vx = 0;
	}
	
	obj.x += vx;
	obj.x = (obj.x + 8 > gamew) ? gamew - 8 : obj.x;
	obj.x = (obj.x < 0) ? 0 : obj.x;
	
	// WALL COLLISION
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		if((obj.x < wx + ww) &&
				(obj.x + 8 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 8 > wy))
		{
			if(vx > 0) {
				obj.x = wx - 8;
				vx = 0;
			} else {
				obj.x = wx + ww;
				vx = 0;
			}
		}
	}
	// SPAWNER COLLISION
	for(i = 0; i<numSpawners; ++i) {
		if(!spawners[i].active) continue;
		wx = spawners[i].x*8;
		wy = spawners[i].y*8;
		ww = 8;
		wh = 8;
		if((obj.x < wx + ww) &&
				(obj.x + 8 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 8 > wy))
		{
			if(vx > 0) {
				obj.x = wx - 8;
				vx = 0;
			} else {
				obj.x = wx + ww;
				vx = 0;
			}
		}
	}
	
	// BADGUY COLLISION
	for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		if((obj.x < wx + 8) &&
			(obj.x + 8 > wx) &&
			(obj.y < wy + 8) &&
			(obj.y + 8 > wy))
		{
			if(vx > 0) {
				obj.x = wx - 8;
				vx = 0;
			} else if(vx < 0) {
				obj.x = wx + 8;
				vx = 0;
			}
			damage_explorer(p1, 1);
		}
	}
		
	if(DOWN_DOWN) {
		vy = 1;
		obj.direction = FACE_DOWN;
	} else if(UP_DOWN) {
		vy = -1;
		obj.direction = FACE_UP;
	} else {
		vy = 0;
	}
	
	obj.y += vy;
	obj.y = (obj.y + 8 > gameh) ? gameh - 8 : obj.y;
	obj.y = (obj.y < 0) ? 0 : obj.y;
	
	// WALL COLLISION
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		if((obj.x < wx + ww) &&
				(obj.x + 8 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 8 > wy))
		{
			if(vy > 0) {
				obj.y = wy - 8;
				vy = 0;
			} else {
				obj.y = wy + wh;
				vy = 0;
			}
		}
	}
	
	// WALL COLLISION
	for(i = 0; i<numSpawners; ++i) {
		if(!spawners[i].active) continue;
		wx = spawners[i].x*8;
		wy = spawners[i].y*8;
		ww = 8;
		wh = 8;
		if((obj.x < wx + ww) &&
				(obj.x + 8 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 8 > wy))
		{
			if(vy > 0) {
				obj.y = wy - 8;
				vy = 0;
			} else {
				obj.y = wy + wh;
				vy = 0;
			}
		}
	}
	
	// BADGUY COLLISION
		for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		if((obj.x < wx + 8) &&
			(obj.x + 8 > wx) &&
			(obj.y < wy + 8) &&
			(obj.y + 8 > wy))
		{
			if(vy > 0) {
				obj.y = wy - 8;
				vy = 0;
			} else if(vy < 0) {
				obj.y = wy + 8;
				vy = 0;
			}
			damage_explorer(p1, 1);
		}
	}
	
	// KEY COLLISION
	
	for(i = 0; i<numKeys; ++i) {
		if(!keys[i].active) continue;
		wx = keys[i].x * 8;
		wy = keys[i].y * 8;
		if((obj.x < wx + 8) &&
			(obj.x + 8 > wx) &&
			(obj.y < wy + 8) &&
			(obj.y + 8 > wy))
		{
			activate_key(keys[i]);
		}
	}
	
	// EXIT COLLISION
	
	for(i = 0; i<numExits; ++i) {
		if(!exits[i].active) continue;
		wx = exits[i].x * 8;
		wy = exits[i].y * 8;
		if((obj.x < wx + 8) &&
			(obj.x + 8 > wx) &&
			(obj.y < wy + 8) &&
			(obj.y + 8 > wy))
		{
			activate_exit(exits[i]);
		}
	}
	
	// TREASURE COLLISION
	
	for(i = 0; i<numTreasures; ++i) {
		if(!treasures[i].active) continue;
		wx = treasures[i].x * 8;
		wy = treasures[i].y * 8;
		if((obj.x < wx + 8) &&
			(obj.x + 8 > wx) &&
			(obj.y < wy + 8) &&
			(obj.y + 8 > wy))
		{
			activate_treasure(treasures[i]);
		}
	}
	
	scrollx = 64 - obj.x;
	scrolly = 32 - obj.y;
}

void draw_explorer(Explorer& obj)
{
	if(obj.active) {
		switch(obj.direction) {
			case FACE_UP:
				display.drawBitmap(obj.x+scrollx, obj.y+scrolly, guyWalkBackward, 8, 8, 1, obj.frame);
				break;
			case FACE_DOWN:
				display.drawBitmap(obj.x+scrollx, obj.y+scrolly, guyWalkForward, 8, 8, 1, obj.frame);
				break;
			case FACE_LEFT:
				if(obj.frame) display.drawBitmap(obj.x+scrollx, obj.y+scrolly, guyWalkLeftA, 8, 8, 1, false);
				else display.drawBitmap(obj.x+scrollx, obj.y+scrolly, guyWalkLeftB, 8, 8, 1, false);
				break;
			case FACE_RIGHT:
				if(obj.frame) display.drawBitmap(obj.x+scrollx, obj.y+scrolly, guyWalkLeftA, 8, 8, 1, true);
				else display.drawBitmap(obj.x+scrollx, obj.y+scrolly, guyWalkLeftB, 8, 8, 1, true);
				break;
		}
	}
	display.setCursor(0, scrh - 8);
	display.print(obj.health);
	//display.print(gameTime);
}

void damage_explorer(Explorer& obj, short dmg)
{
	obj.health -= dmg;
	if(obj.health <= 0) {
		obj.active = false;
		obj.health = 0;
	}
}

///////////////// BADGUY //////////////////

void add_badguy(short x, short y)
{
	bool found = false;
	int foundIndex;
	for(foundIndex=0; foundIndex<numBadguys; ++foundIndex) {
		if(!badguys[foundIndex].active) {
			badguys[foundIndex].active = true;
			badguys[foundIndex].x = x;
			badguys[foundIndex].y = y;
			found = true;
			break;
		}
	}
	
	if(!found) return;
	
	bool valid = true;
	for(int j = 0; j<numBadguys; ++j) {
		if(!badguys[j].active || (foundIndex == j)) continue;
		short wx = badguys[j].x;
		short wy = badguys[j].y;
		if((badguys[foundIndex].x < wx + 8) &&
			(badguys[foundIndex].x + 8 > wx) &&
			(badguys[foundIndex].y < wy + 8) &&
			(badguys[foundIndex].y + 8 > wy))
		{
			valid = false;
			break;
		}
	}
	
	if(!valid) badguys[foundIndex].active = false;
}
void update_badguy(BadGuy &obj)
{
	if(!obj.active) return;
	char i;
	char vx, vy;
	short wx, wy, ww, wh;
	
	if(obj.x > p1.x) vx = -1;
	else if(obj.x < p1.x) vx = 1;
	else vx = 0;
	
	obj.x += vx;
	obj.x = (obj.x + 8 > gamew) ? gamew - 8 : obj.x;
	obj.x = (obj.x < 0) ? 0 : obj.x;
	
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		if((obj.x < wx + ww) &&
				(obj.x + 8 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 8 > wy))
		{
			if(vx > 0) {
				obj.x = wx - 8;
				vx = 0;
			} else {
				obj.x = wx + ww;
				vx = 0;
			}
		}
	}
	for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active || (&obj == &badguys[i])) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		if((obj.x < wx + 8) &&
			(obj.x + 8 > wx) &&
			(obj.y < wy + 8) &&
			(obj.y + 8 > wy))
		{
			if(vx > 0) {
				obj.x = wx - 8;
				vx = 0;
			} else if(vx < 0) {
				obj.x = wx + 8;
				vx = 0;
			}
		}
	}
	if((obj.x < p1.x + 8) &&
		(obj.x + 8 > p1.x) &&
		(obj.y < p1.y + 8) &&
		(obj.y + 8 > p1.y))
	{
		if(vx > 0) {
			obj.x = p1.x - 8;
			vx = 0;
		} else if(vx < 0) {
			obj.x = p1.x + 8;
			vx = 0;
		}
		damage_explorer(p1, 1);
	}
	
	if(obj.y > p1.y) vy = -1;
	else if(obj.y < p1.y) vy = 1;
	else vy = 0;
	
	obj.y += vy;
	obj.y = (obj.y + 8 > gameh) ? gameh - 8 : obj.y;
	obj.y = (obj.y < 0) ? 0 : obj.y;
	
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		if((obj.x < wx + ww) &&
				(obj.x + 8 > wx) &&
				(obj.y < wy + wh) &&
				(obj.y + 8 > wy))
		{
			if(vy > 0) {
				obj.y = wy - 8;
				vy = 0;
			} else {
				obj.y = wy + wh;
				vy = 0;
			}
		}
	}
	
	for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active || (&obj == &badguys[i])) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		if((obj.x < wx + 8) &&
			(obj.x + 8 > wx) &&
			(obj.y < wy + 8) &&
			(obj.y + 8 > wy))
		{
			if(vy > 0) {
				obj.y = wy - 8;
				vy = 0;
			} else if(vy < 0) {
				obj.y = wy + 8;
				vy = 0;
			}
		}
	}
	if((obj.x < p1.x + 8) &&
		(obj.x + 8 > p1.x) &&
		(obj.y < p1.y + 8) &&
		(obj.y + 8 > p1.y))
	{
		if(vy > 0) {
			obj.y = p1.y - 8;
			vy = 0;
		} else if(vy < 0) {
			obj.y = p1.y + 8;
			vy = 0;
		}
		damage_explorer(p1, 1);
	}
}

void draw_badguy(BadGuy &b)
{
	if(b.active)
		display.drawBitmap(b.x+scrollx, b.y+scrolly, badGuySpriteA, 8, 8, 1);
}

void destroy_badguy(BadGuy &obj)
{
	if(obj.active) {
		display.tunes.tone(TONE_BADGUY_DESTROY, 20);
		obj.active = false;
	}		
}

////////////////// GAME FUNCTIONS /////////////////

void gameDeleteObjects();
void gameGotoLevel(unsigned char level)
{
	markRoomAsCleared(currentLevel);
	if(level == 0) level = currentLevel + 1;
	if(level == 255) level = 0;
	if(level >= numLevels) {
		doGameComplete();
		return;
	}
	
	if(rollingScore > 0)
		score += rollingScore;
	rollingScore = -30;
	
	if(rollingHealth > 0)
		p1.health += rollingHealth;
	rollingHealth = 0;
	
	currentLevel = level;
	saveGame(GAME_SAVE_FILE);
	gameDeleteObjects();
	
	display.clearDisplay();
	display.setCursor(0, 64-8);
	//display.print(F("Level "));
	display.print(rps(&(levelNames[level])));
	//display.print(level+1);
	display.display();
	delay(1000);
	
	// Read shorts from the memory
	unsigned short dataItem;
	
	for(char i=0; i<32; ++i) {
		//dataItem = pgm_read_word(&(levelData[level]) + i);
		
		//dataItem = pgm_read_word_far(levelData + i + level*64);
		
		
		switch(level) {
			case 0: dataItem = pgm_read_word(levelData_0 + i); break;
			case 1: dataItem = pgm_read_word(levelData_1 + i); break;
			case 2: dataItem = pgm_read_word(levelData_2 + i); break;
			case 3: dataItem = pgm_read_word(levelData_3 + i); break;
			case 4: dataItem = pgm_read_word(levelData_4 + i); break;
			case 5: dataItem = pgm_read_word(levelData_5 + i); break;
			case 6: dataItem = pgm_read_word(levelData_6 + i); break;
			case 7: dataItem = pgm_read_word(levelData_7 + i); break;
			case 8: dataItem = pgm_read_word(levelData_8 + i); break;
			case 9: dataItem = pgm_read_word(levelData_9 + i); break;
			case 10: dataItem = pgm_read_word(levelData_10 + i); break;
			case 11: dataItem = pgm_read_word(levelData_11 + i); break;
			case 12: dataItem = pgm_read_word(levelData_12 + i); break;
			case 13: dataItem = pgm_read_word(levelData_13 + i); break;
			case 14: dataItem = pgm_read_word(levelData_14 + i); break;
			case 15: dataItem = pgm_read_word(levelData_15 + i); break;
			case 16: dataItem = pgm_read_word(levelData_16 + i); break;
			case 17: dataItem = pgm_read_word(levelData_17 + i); break;
			case 18: dataItem = pgm_read_word(levelData_18 + i); break;
			case 19: dataItem = pgm_read_word(levelData_19 + i); break;
			case 20: dataItem = pgm_read_word(levelData_20 + i); break;
			case 21: dataItem = pgm_read_word(levelData_21 + i); break;
			case 22: dataItem = pgm_read_word(levelData_22 + i); break;
			case 23: dataItem = pgm_read_word(levelData_23 + i); break;
			case 24: dataItem = pgm_read_word(levelData_24 + i); break;
			case 25: dataItem = pgm_read_word(levelData_25 + i); break;
			case 26: dataItem = pgm_read_word(levelData_26 + i); break;
			case 27: dataItem = pgm_read_word(levelData_27 + i); break;
			case 28: dataItem = pgm_read_word(levelData_28 + i); break;
			case 29: dataItem = pgm_read_word(levelData_29 + i); break;
		}
		
		
		//if(level<4)	dataItem = pgm_read_word(levelData_1 + level*32 + i);
		//else if(level<8) dataItem = pgm_read_word(levelData_1 + level*32 + i);
		//else if(level<12) dataItem = pgm_read_word(levelData_2 + level*32 + i);
		
		unsigned char x, y, w, h, dest, target;
		char style;
		bool direction;
		
		if(i < 16) { // Read a wall
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			dest = (dataItem >> 2) & 31;
			style = (dataItem >> 1) & 1;
			direction = (dataItem) & 1;
			w = direction ? 1 : dest+1;
			h = direction ? dest+1 : 1;
			add_wall(x, y, w, h, style);
		} else if(i >= 16 && i < 20) { // Read a spawner
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			add_spawner(x, y);
		} else if(i >= 20 && i < 24) { // Read a key
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			target = (dataItem >> 3) & 15;
			add_key(x, y, target);
		} else if(i >= 24 && i < 25) { // Read a start
			spawnx = (dataItem >> 11) & 31;
			spawny = (dataItem >> 7) & 15;
		} else if(i >= 25 && i < 29) { // Read an exit
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			dest = (dataItem) & 127;
			add_exit(x, y, dest);
		} else if(i >= 29 && i < 32) { // Read a treasure
			x = (dataItem >> 11) & 31;
			y = (dataItem >> 7) & 15;
			target = (dataItem >> 4) & 7; // type
			dest = (dataItem) & 15; // layout
			
			if(target == 0) continue; // If the type is zero, don't do squat with it
			
			switch(dest) { case 0: case 1: case 2: case 3: case 5: case 6: case 9: case 10: case 11: case 12: case 14:
					add_treasure(x, y, target);
			break; }
			switch(dest) { case 2: case 4: case 7: case 9: case 13: case 15:
					add_treasure(x+1, y, target);
			break; }
			switch(dest) { case 8: case 14:
					add_treasure(x+2, y, target);
			break; }
			
			switch(dest) { case 1: case 4: case 8: case 11: case 15:
					add_treasure(x, y+1, target);
			break; }
			switch(dest) { case 3: case 10:
					add_treasure(x+1, y+1, target);
			break; }
			switch(dest) { case 5: case 9:
					add_treasure(x+2, y+1, target);
			break; }
			
			switch(dest) { case 7: case 12: case 13:
					add_treasure(x, y+2, target);
			break; }
			switch(dest) { case 6: case 11: case 14: case 15:
					add_treasure(x+1, y+2, target);
			break; }
			switch(dest) { case 10: case 12: case 13:
					add_treasure(x+2, y+2, target);
			break; }
		}
	}
	p1.x = spawnx*8;
	p1.y = spawny*8;
}

void gameDeleteObjects()
{	
	char i;
	for(i=0; i<numBadguys; ++i) {
		badguys[i].active = false;
	}
	for(i=0; i<numSpawners; ++i) {
		spawners[i].active = false;
	}
	for(i=0; i<numKeys; ++i) {
		keys[i].active = false;
	}
	for(i=0; i<numExits; ++i) {
		exits[i].active = false;
	}
	for(i=0; i<numWalls; ++i) {
		walls[i].active = false;
	}
	for(i=0; i<numBullets; ++i) {
		bullets[i].active = false;
	}
	for(i=0; i<numTreasures; ++i) {
		treasures[i].active = false;
	}
}

void gameSetupWorld()
{
	/*
	scrollx = 0;
	scrolly = 0;

	
	add_wall(2,4,16,1,0);
	add_wall(18,0,1,5,0);
	add_wall(5,9,1,7,0);
	add_wall(11,9,11,1,0);
	add_wall(26,0,1,10,0);
	add_wall(16,10,1,6,0);
	add_wall(0,4,2,1,1);
	
	add_spawner(10,1);
	add_spawner(23,1);
	add_spawner(13,12);
	
	add_key(14, 14, 6);
	
	spawnx = 30;
	spawny = 1;
	
	add_exit(16,1,0);
	
	initialize_explorer(p1);
	*/
}

void gameSetup()
{
	levelsCompleted = 0;
	autoFireTime = 0;
	rollingScore = -30;
	quitGame = false;
	initialize_explorer(p1);
	currentLevel = 0;
	//gameDeleteObjects();
	//gameSetupWorld();
	/*
	add_wall(0, 2, 5, 1);
	add_wall(6, 5, 4, 1);
	add_wall(10, 0, 1, 6);
	add_wall(2, 2, 1, 4);
	add_wall(13, 4, 3, 1);
	
	add_spawner(15, 7);
	add_spawner(0, 4);
	
	add_exit(15, 0);
	*/

}

void gameUpdate()
{
	int i;

	for(i=0; i<numSpawners; ++i) {
		update_spawner(spawners[i]);
	}
	if(BadGuy_UpdateDelay <= 0) {
		for(i=0; i<numBadguys; ++i) {
			update_badguy(badguys[i]);
		}
		BadGuy_UpdateDelay = 3;
	}
	BadGuy_UpdateDelay--;
	for(i=0; i<numBullets; ++i) {
		update_bullet(bullets[i]);
	}
	update_explorer(p1);
	
	if(rollingScore > 0) {
		score++;
	}
	if(rollingScore > -30) {
		rollingScore--;
	}
	
	if(gameTimeTick <= 0) {
		gameTime ++;
		gameTimeTick = 46;
	}
	gameTimeTick--;
}

void flashScreen()
{
	display.fillRect(0, 0, scrw, scrh, 1);
	display.display();
	whiteScreenTime = 2;
}

void gameDraw()
{
	if(whiteScreenTime > 0) {
		whiteScreenTime --;
		return;
	}
	
	short i;
	display.clearDisplay();
	
	// Draw level border
	
	draw_explorer(p1);
	
	for(i=0; i<numWalls; ++i) {
		draw_wall(walls[i]);
	}
	for(i=-1; i<MAP_WIDTH; i++) {
		display.drawBitmap(i*8+scrollx, -8+scrolly, wallSprite, 8, 8, 1);
		display.drawBitmap(i*8+scrollx, gameh+scrolly, wallSprite, 8, 8, 1);
	}
	for(i=-1; i<=MAP_HEIGHT; i++) {
		display.drawBitmap(-8+scrollx, i*8+scrolly, wallSprite, 8, 8, 1);
		display.drawBitmap(gamew+scrollx, i*8+scrolly, wallSprite, 8, 8, 1);
	}
	for(i=0; i<numSpawners; ++i) {
		draw_spawner(spawners[i]);
	}
	for(i=0; i<numTreasures; ++i) {
		draw_treasure(treasures[i]);
	}
	for(i=0; i<numKeys; ++i) {
		draw_key(keys[i]);
	}
	for(i=0; i<numExits; ++i) {
		draw_exit(exits[i]);
	}
	for(i=0; i<numBadguys; ++i) {
		draw_badguy(badguys[i]);
	}
	for(i=0; i<numBullets; ++i) {
		draw_bullet(bullets[i]);
	}
	
	if(rollingScore > -30) {
		display.setCursor(0, scrh-8);
		display.print(score);
		display.print("00 pts");
	}
	
	display.display();
}

void doGameComplete()
{
	display.clearDisplay();
	display.setCursor(3*6, 8*1);
	display.print(F("Game Complete!"));
	display.setCursor(6, 8*3);
	display.print(F("You cleared "));
	display.print(levelsCompleted);
	display.print(F(" rooms"));
	display.setCursor(6*6, 8*4);
	display.print(F("In "));
	printTime(gameTime);
	display.setCursor(2*8, 8*5);
	display.print(F("With a score of"));
	display.setCursor(6*5, 8*6);
	display.print(score);
	display.print(F("00 points"));
	display.display();
	
	tautInput();
	while(true) {
		updateNewInput();
		if(A_PRESSED || B_PRESSED) break;
		updateOldInput();
	}
	quitGame = true;
	deleteContinueSave();
	saveHighScore();
}

void doGameOver()
{
	saveGame(GAME_SAVE_FILE);
	
	display.clearDisplay();
	display.setCursor(5*8-4, 8*1);
	display.print(F("Game Over"));
	display.setCursor(6, 8*3);
	display.print(F("You cleared "));
	display.print(levelsCompleted);
	display.print(F(" rooms"));
	display.setCursor(6*6, 8*4);
	display.print(F("In "));
	printTime(gameTime);
	display.setCursor(2*8, 8*5);
	display.print(F("With a score of"));
	display.setCursor(6*5, 8*6);
	display.print(score);
	display.print(F("00 points"));
	display.display();

	tautInput();
	while(true) {
		updateNewInput();
		if(A_PRESSED) break;
		updateOldInput();
	}
	quitGame = true;
	deleteContinueSave();
	saveHighScore();
}

void gameLoop()
{
	while(!quitGame) {
		updateNewInput();
		delay(10);
		gameUpdate();
		if(quitGame) break;
		gameDraw();
		updateOldInput();
	}
}


void displayLoadGame(byte file)
{
	rollingScore = -30;
	quitGame = false;
	int address = file * 10 * 5;

	if(EEPROM.read(10+address+FILE_CONTINUE) == 1) {
		gameSetup();
		
		byte hi = EEPROM.read(10+address+FILE_SCORE);
		byte lo = EEPROM.read(10+address+FILE_SCORE+1);
		score = (hi << 8) | lo;
		
		hi = EEPROM.read(10+address+FILE_HEALTH);
		lo = EEPROM.read(10+address+FILE_HEALTH+1);
		p1.health = (hi << 8) | lo;
		
		levelsCompleted = EEPROM.read(10+address+FILE_LEVEL);
		currentLevel = EEPROM.read(10+address+FILE_CURRENT_LEVEL);
		
		hi = EEPROM.read(10+address+FILE_TIME);
		lo = EEPROM.read(10+address+FILE_TIME+1);
		gameTime = (hi << 8) | lo;
		
		if(currentLevel == 0)
			currentLevel = 255;
				
		gameGotoLevel(currentLevel);
		gameLoop();
	}
		
	quitGame = true;
}

void saveGame(byte file)
{
	int address = file * 10 * 5;
	EEPROM.write(10+address+FILE_SCORE, ((score >> 8) & 0xFF));
	EEPROM.write(10+address+FILE_SCORE+1, (score & 0xFF));
	EEPROM.write(10+address+FILE_HEALTH, ((p1.health >> 8) & 0xFF));
	EEPROM.write(10+address+FILE_HEALTH+1, (p1.health & 0xFF));
	EEPROM.write(10+address+FILE_TIME, ((gameTime >> 8) & 0xFF));
	EEPROM.write(10+address+FILE_TIME+1, (gameTime & 0xFF));
	EEPROM.write(10+address+FILE_LEVEL, levelsCompleted);
	EEPROM.write(10+address+FILE_CURRENT_LEVEL, currentLevel);
	EEPROM.write(10+address+FILE_CONTINUE, 1);
}

void deleteContinueSave()
{
	int address = GAME_SAVE_FILE * 10 * 5;
	EEPROM.write(10+address+FILE_CONTINUE, 0);
}

void displayGame()
{
	gameSetup();
	score = 0;
	gameTime = 0;
	gameTimeTick = 0;
	gameGotoLevel(255);
	gameLoop();
}

void fadeOut()
{
	for(unsigned char i=0; i<8; ++i) {
		for(unsigned char j=0; j<255; ++j) {
			display.drawPixel(random(0,128), random(0,64), 0);
		}
		display.display();
	}
}

void swipeOut()
{
	for(unsigned char y=0; y<64; y+=3) {
		for(unsigned char x=0; x<128; ++x) {
			display.drawPixel(x, y, 0);
		}
	}
	display.display();
	for(unsigned char y=1; y<64; y+=3) {
		for(unsigned char x=0; x<128; ++x) {
			display.drawPixel(x, y, 0);
		}
	}
	display.display();
	for(unsigned char y=2; y<64; y+=3) {
		for(unsigned char x=0; x<128; ++x) {
			display.drawPixel(x, y, 0);
		}
	}
	display.display();
}

void markRoomAsCleared(unsigned char room)
{
	if(room > numLevels) return;
	int address = GAME_SAVE_FILE * 10 * 5;
	int pos = address+FILE_COMPLETION+(room/8);
	char data = EEPROM.read(pos);
	data |= 1 << (room%8);
	EEPROM.write(pos, data);
}

bool saveHighScore()
{
	int address = GAME_SAVE_FILE * 10 * 5;
	
	char myScoreIndex;
	short compareScore;
	
	int myScoreAddress;
	int srcScoreAddress;
	int destScoreAddress;
	
	byte hi;
	byte lo;
	
	for(myScoreIndex=0; myScoreIndex<3; ++myScoreIndex) {
		srcScoreAddress = 20 + (myScoreIndex*10) + address;
		hi = EEPROM.read(srcScoreAddress+FILE_SCORE);
		lo = EEPROM.read(srcScoreAddress+FILE_SCORE+1);
		compareScore = (hi << 8) | lo;
		if(score > compareScore) break;
	}
	
	if(myScoreIndex < 3) {
		// Input name
		char input;
		char nameLength = 0;
		char inputName[5];
		
		for(char i=0; i<4; ++i) {
			inputName[i] = ' ';
		}
		inputName[4] = '\0';
		
		char cursorPos = 0;
		
		while(true) {
			display.clearDisplay();
			display.setCursor(0,0);
			display.print(F("You set a high score!"));
			display.setCursor(0,8);
			display.print(F(" Enter name: \""));
			display.print(inputName);
			display.setCursor(6*18, 8);
			display.print("\"");
			input = prompt_keyboard(cursorPos);
			if(input == 0) {
				if(nameLength > 0) nameLength--;
				if(nameLength < 4) inputName[nameLength] = ' ';
			} else {
				if(nameLength >= 4) break;
				inputName[nameLength] = input;
				nameLength++;
			}
		}
		
		
		for(char i=2; i>myScoreIndex; --i) {
			srcScoreAddress = 20 + ((i-1)*10) + address;
			destScoreAddress = 20 + (i*10) + address;
			for(char data=0; data<10; ++data) {
				hi = EEPROM.read(srcScoreAddress+data);
				EEPROM.write(destScoreAddress+data, hi);
			}
		}
		
		myScoreAddress = 20 + (myScoreIndex*10) + address;
		EEPROM.write(myScoreAddress+FILE_SCORE, ((score >> 8) & 0xFF));
		EEPROM.write(myScoreAddress+FILE_SCORE+1, (score & 0xFF));
		EEPROM.write(myScoreAddress+FILE_TIME, ((gameTime >> 8) & 0xFF));
		EEPROM.write(myScoreAddress+FILE_TIME+1, (gameTime & 0xFF));
		EEPROM.write(myScoreAddress+FILE_LEVEL, levelsCompleted);
		EEPROM.write(myScoreAddress+FILE_NAME, inputName[0]);
		EEPROM.write(myScoreAddress+FILE_NAME+1, inputName[1]);
		EEPROM.write(myScoreAddress+FILE_NAME+2, inputName[2]);
		EEPROM.write(myScoreAddress+FILE_NAME+3, inputName[3]);
		
		displayRecords();
		
		return true;
	}
	return false;
}
