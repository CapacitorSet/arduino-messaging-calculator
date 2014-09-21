#ifndef KEYPAD_H
#define KEYPAD_H

// Arduino versioning.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"	// for digitalRead, digitalWrite, etc
#else
#include "WProgram.h"
#endif

#define OFF LOW
#define ON HIGH

#define CLOSED LOW
#define OPEN HIGH

#define makeKeymap(x) ((char*)x)

typedef char KeypadEvent;

typedef enum {IDLE, PRESSED, /*HOLD, */RELEASED} KeyState;  // KeyState was KeypadState

const char NO_KEY = '\0';
#define KEY_RELEASED NO_KEY

class Keypad {
public:
	Keypad(char *userKeymap, byte *row, byte *col);

	void begin(char *userKeymap);
	char getKey();
	KeyState getState();
	void setDebounceTime(unsigned int);
	void setHoldTime(unsigned int);
	void addEventListener(void (*listener)(char));

	char waitForKey();
	boolean keyStateChanged();

private:
	void transitionTo(KeyState);
	void initializePins();

	char *keymap;
    byte *rowPins;
    byte *columnPins;
	KeyState state;
	char currentKey;
	unsigned int debounceTime;
	unsigned int holdTime;
	void (*keypadEventListener)(char);

	boolean scanKeys();
	KeyState getKeyState();

	boolean buttons;
	boolean stateChanged;
};

#endif