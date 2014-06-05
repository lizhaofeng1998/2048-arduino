/*
	2048 for Arduino
	PCD8544 driver
	
	Based on
	http://playground.arduino.cc/Code/PCD8544
	http://www.microsyl.com/index.php/2010/03/24/nokia-lcd-library/
	
	This file is provided AS IS without any warranty.
*/

#ifndef DISPLAY_H
#define DISPLAY_H

// == Includes ==
#include <Arduino.h>
#include <avr/eeprom.h>
#include "format.h"
#include "button.h"
#include "config.h"

// == Runtime variables ==
#if lcdContrastAdjustable
unsigned char eepromContrast EEMEM = lcdDefaultContrast;
#endif

#define LCD_C		 LOW
#define LCD_D		 HIGH

#define LCD_X		 84
#define LCD_Y		 48

// are we using backlight
bool backlightOn = false;

// current contrast
unsigned int contrast = 0xA5;

const byte ASCII[][5] =
{
 { 0x00, 0x00, 0x00, 0x00, 0x00 } // 20	
,{ 0x00, 0x00, 0x5f, 0x00, 0x00 } // 21 !
,{ 0x00, 0x07, 0x00, 0x07, 0x00 } // 22 "
,{ 0x14, 0x7f, 0x14, 0x7f, 0x14 } // 23 #
/*
,{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 } // 24 $
*/
,{ 0x08, 0x1c, 0x3e, 0x1c, 0x08 } // 24 $ (score icon)
,{ 0x23, 0x13, 0x08, 0x64, 0x62 } // 25 %
/*
,{ 0x36, 0x49, 0x55, 0x22, 0x50 } // 26 &
*/
,{ 0xfc, 0x96, 0x96, 0x96, 0xfc } // 26 & (battery icon)
,{ 0x00, 0x05, 0x03, 0x00, 0x00 } // 27 '
,{ 0x00, 0x1c, 0x22, 0x41, 0x00 } // 28 (
,{ 0x00, 0x41, 0x22, 0x1c, 0x00 } // 29 )
,{ 0x14, 0x08, 0x3e, 0x08, 0x14 } // 2a *
,{ 0x08, 0x08, 0x3e, 0x08, 0x08 } // 2b +
,{ 0x00, 0x50, 0x30, 0x00, 0x00 } // 2c ,
,{ 0x08, 0x08, 0x08, 0x08, 0x08 } // 2d -
,{ 0x00, 0x60, 0x60, 0x00, 0x00 } // 2e .
,{ 0x20, 0x10, 0x08, 0x04, 0x02 } // 2f /
,{ 0x3e, 0x51, 0x49, 0x45, 0x3e } // 30 0
,{ 0x00, 0x42, 0x7f, 0x40, 0x00 } // 31 1
,{ 0x42, 0x61, 0x51, 0x49, 0x46 } // 32 2
,{ 0x21, 0x41, 0x45, 0x4b, 0x31 } // 33 3
,{ 0x18, 0x14, 0x12, 0x7f, 0x10 } // 34 4
,{ 0x27, 0x45, 0x45, 0x45, 0x39 } // 35 5
,{ 0x3c, 0x4a, 0x49, 0x49, 0x30 } // 36 6
,{ 0x01, 0x71, 0x09, 0x05, 0x03 } // 37 7
,{ 0x36, 0x49, 0x49, 0x49, 0x36 } // 38 8
,{ 0x06, 0x49, 0x49, 0x29, 0x1e } // 39 9
,{ 0x00, 0x36, 0x36, 0x00, 0x00 } // 3a :
,{ 0x00, 0x56, 0x36, 0x00, 0x00 } // 3b ;
,{ 0x08, 0x14, 0x22, 0x41, 0x00 } // 3c <
,{ 0x14, 0x14, 0x14, 0x14, 0x14 } // 3d =
,{ 0x00, 0x41, 0x22, 0x14, 0x08 } // 3e >
,{ 0x02, 0x01, 0x51, 0x09, 0x06 } // 3f ?
/*
,{ 0x32, 0x49, 0x79, 0x41, 0x3e } // 40 @
*/
,{ 0x7e, 0x5e, 0x4e, 0x46, 0x7e } // 40 @ (contrast icon)
,{ 0x7e, 0x11, 0x11, 0x11, 0x7e } // 41 A
,{ 0x7f, 0x49, 0x49, 0x49, 0x36 } // 42 B
,{ 0x3e, 0x41, 0x41, 0x41, 0x22 } // 43 C
,{ 0x7f, 0x41, 0x41, 0x22, 0x1c } // 44 D
,{ 0x7f, 0x49, 0x49, 0x49, 0x41 } // 45 E
,{ 0x7f, 0x09, 0x09, 0x09, 0x01 } // 46 F
,{ 0x3e, 0x41, 0x49, 0x49, 0x7a } // 47 G
,{ 0x7f, 0x08, 0x08, 0x08, 0x7f } // 48 H
,{ 0x00, 0x41, 0x7f, 0x41, 0x00 } // 49 I
,{ 0x20, 0x40, 0x41, 0x3f, 0x01 } // 4a J
,{ 0x7f, 0x08, 0x14, 0x22, 0x41 } // 4b K
,{ 0x7f, 0x40, 0x40, 0x40, 0x40 } // 4c L
,{ 0x7f, 0x02, 0x0c, 0x02, 0x7f } // 4d M
,{ 0x7f, 0x04, 0x08, 0x10, 0x7f } // 4e N
,{ 0x3e, 0x41, 0x41, 0x41, 0x3e } // 4f O
,{ 0x7f, 0x09, 0x09, 0x09, 0x06 } // 50 P
,{ 0x3e, 0x41, 0x51, 0x21, 0x5e } // 51 Q
,{ 0x7f, 0x09, 0x19, 0x29, 0x46 } // 52 R
,{ 0x46, 0x49, 0x49, 0x49, 0x31 } // 53 S
,{ 0x01, 0x01, 0x7f, 0x01, 0x01 } // 54 T
,{ 0x3f, 0x40, 0x40, 0x40, 0x3f } // 55 U
,{ 0x1f, 0x20, 0x40, 0x20, 0x1f } // 56 V
,{ 0x3f, 0x40, 0x38, 0x40, 0x3f } // 57 W
,{ 0x63, 0x14, 0x08, 0x14, 0x63 } // 58 X
,{ 0x07, 0x08, 0x70, 0x08, 0x07 } // 59 Y
,{ 0x61, 0x51, 0x49, 0x45, 0x43 } // 5a Z
,{ 0x00, 0x7f, 0x41, 0x41, 0x00 } // 5b [
,{ 0x02, 0x04, 0x08, 0x10, 0x20 } // 5c ¥
,{ 0x00, 0x41, 0x41, 0x7f, 0x00 } // 5d ]
,{ 0x04, 0x02, 0x01, 0x02, 0x04 } // 5e ^
,{ 0x40, 0x40, 0x40, 0x40, 0x40 } // 5f _
,{ 0x00, 0x01, 0x02, 0x04, 0x00 } // 60 `
,{ 0x20, 0x54, 0x54, 0x54, 0x78 } // 61 a
,{ 0x7f, 0x48, 0x44, 0x44, 0x38 } // 62 b
,{ 0x38, 0x44, 0x44, 0x44, 0x20 } // 63 c
,{ 0x38, 0x44, 0x44, 0x48, 0x7f } // 64 d
,{ 0x38, 0x54, 0x54, 0x54, 0x18 } // 65 e
,{ 0x08, 0x7e, 0x09, 0x01, 0x02 } // 66 f
,{ 0x0c, 0x52, 0x52, 0x52, 0x3e } // 67 g
,{ 0x7f, 0x08, 0x04, 0x04, 0x78 } // 68 h
,{ 0x00, 0x44, 0x7d, 0x40, 0x00 } // 69 i
,{ 0x20, 0x40, 0x44, 0x3d, 0x00 } // 6a j 
,{ 0x7f, 0x10, 0x28, 0x44, 0x00 } // 6b k
,{ 0x00, 0x41, 0x7f, 0x40, 0x00 } // 6c l
,{ 0x7c, 0x04, 0x18, 0x04, 0x78 } // 6d m
,{ 0x7c, 0x08, 0x04, 0x04, 0x78 } // 6e n
,{ 0x38, 0x44, 0x44, 0x44, 0x38 } // 6f o
,{ 0x7c, 0x14, 0x14, 0x14, 0x08 } // 70 p
,{ 0x08, 0x14, 0x14, 0x18, 0x7c } // 71 q
,{ 0x7c, 0x08, 0x04, 0x04, 0x08 } // 72 r
,{ 0x48, 0x54, 0x54, 0x54, 0x20 } // 73 s
,{ 0x04, 0x3f, 0x44, 0x40, 0x20 } // 74 t
,{ 0x3c, 0x40, 0x40, 0x20, 0x7c } // 75 u
,{ 0x1c, 0x20, 0x40, 0x20, 0x1c } // 76 v
,{ 0x3c, 0x40, 0x30, 0x40, 0x3c } // 77 w
,{ 0x44, 0x28, 0x10, 0x28, 0x44 } // 78 x
,{ 0x0c, 0x50, 0x50, 0x50, 0x3c } // 79 y
,{ 0x44, 0x64, 0x54, 0x4c, 0x44 } // 7a z
,{ 0x00, 0x08, 0x36, 0x41, 0x00 } // 7b {
,{ 0x00, 0x00, 0x7f, 0x00, 0x00 } // 7c |
,{ 0x00, 0x41, 0x36, 0x08, 0x00 } // 7d }
,{ 0x10, 0x08, 0x08, 0x10, 0x08 } // 7e ←
,{ 0x78, 0x46, 0x41, 0x46, 0x78 } // 7f →
};

// == Function prototypes ==
void lcdWrite( byte dc, byte data );
void lcdCharacter( char character );
void lcdResetCursor();
void lcdClear();
void lcdString( const char *characters );
void lcdInitIo();
void lcdInit( void );
unsigned int lcdGetContrast();
void lcdSetContrast( byte newContrast );
void lcdLoadContrast();
void lcdInteractiveContrast();
void printLine( const char * str );
// == Actual code ==

void lcdWrite( byte dc, byte data ) {
	digitalWrite( pinLcdDc, dc );
	digitalWrite( pinLcdSce, LOW );
	shiftOut( pinLcdSdin, pinLcdSclk, MSBFIRST, data );
	digitalWrite( pinLcdSce, HIGH );
}

void lcdCharacter( char character ) {
	lcdWrite( LCD_D, 0x00 );
	for ( int index = 0; index < 5; index++ ) {
		lcdWrite( LCD_D, ASCII[character - 0x20][index] );
	}
	lcdWrite( LCD_D, 0x00 );
}

void lcdResetCursor() {
	lcdWrite( 0, 0x80 | 0 );
	lcdWrite( 0, 0x40 | 0 );
}

void lcdClear() {
	for ( int index = 0; index < LCD_X * LCD_Y / 8; index++ ) {
		lcdWrite( LCD_D, 0x00 );
	}
	lcdResetCursor();
}

void lcdString( const char *characters ) {
	while ( *characters && *characters != '\0' ) {
		lcdCharacter( *characters++ );
	}
}

void lcdInitIo() {
	pinMode( pinLcdSce, OUTPUT );
	pinMode( pinLcdReset, OUTPUT );
	pinMode( pinLcdDc, OUTPUT );
	pinMode( pinLcdSdin, OUTPUT );
	pinMode( pinLcdSclk, OUTPUT );
	pinMode( pinLcdBl, OUTPUT );
	digitalWrite( pinLcdReset, LOW );
	digitalWrite( pinLcdReset, HIGH );
	digitalWrite( pinLcdBl, LOW );
}

void lcdInit( void ) {
	lcdWrite( LCD_C, 0x21 ); // LCD Extended Commands.
	if ( !backlightOn ) // backlight on?
		lcdWrite( LCD_C, contrast );
	else
		lcdWrite( LCD_C, contrast + lcdContrastStep );

	// Set LCD Vop (Contrast).
	lcdWrite( LCD_C, 0x04 ); // Set Temp coefficent. //0x04
	lcdWrite( LCD_C, 0x13 ); // LCD bias mode 1:48. //0x14
	lcdWrite( LCD_C, 0x0C ); // LCD in normal mode.
	lcdWrite( LCD_C, 0x20 );
	lcdWrite( LCD_C, 0x0C );
}

unsigned int lcdGetContrast() {
	return contrast;
}

void lcdSetContrast( byte newContrast ) {
#if lcdContrastAdjustable
	eeprom_write_byte( &eepromContrast, newContrast );
	contrast = newContrast;
#endif
}

void lcdLoadContrast() {
#if lcdContrastAdjustable
	byte savedContrast = eeprom_read_byte( &eepromContrast );
	if ( savedContrast == 255 ) { // not saved yet
		lcdSetContrast( lcdDefaultContrast );
	} else {
		contrast = savedContrast;
	}
#else
	contrast = lcdDefaultContrast;
#endif
}

// prints a line to LCD
void printLine( const char * str ) {
	char buffer[13]; // don't modify constants!
	strncpy( buffer, str, 12 );
	getLineString( buffer, buffer );
	lcdString( buffer );
}

// let the user adjust contrast settings 
void lcdInteractiveContrast() {
	char buffer[13];
	lcdClear();
	printLine( "contrast" );
	while( true ) { // lock the user in
		if ( isPressed( buttonRight ) ) {
			lcdSetContrast( lcdGetContrast() + 0x01 );
		} else if ( isPressed( buttonLeft ) ) {
			lcdSetContrast( lcdGetContrast() - 0x01 );
		} else if ( isPressed( buttonUp ) ) {
			lcdSetContrast( lcdDefaultContrast );
		} else continue;
		lcdInit();
		lcdClear();
		itoa( lcdGetContrast(), buffer, 10 );
		lcdString( buffer );
	}
}

#endif
