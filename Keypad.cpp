#include "Keypad.h"
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

// <<constructor>> Allows custom keymap, pin configuration, and keypad sizes.
Keypad::Keypad(char *userKeymap, byte *row, byte *col) {
    rowPins = row;
    columnPins = col;

    keymap = userKeymap;

    debounceTime = 2;
    holdTime = 500;
	keypadEventListener = 0;

    transitionTo(IDLE);
    stateChanged = false;

    initializePins();
}

// New in 2.0 this function lets the end user test for any changes in state
// before deciding if any variables, etc. need to be updated in their code.
boolean Keypad::keyStateChanged() {
    return stateChanged;
}

char Keypad::getKey() {
	// Return the new key value if a keypress was detected. By testing for
	// keyStateChanged() we don't return a keypress more than once.
    if( getKeyState()==PRESSED && keyStateChanged() )
    {
    	return currentKey;
    }

	return NO_KEY;	// Otherwise just return the default key value:
}

char Keypad::waitForKey() {
	char waitKey = NO_KEY;
	while( (waitKey = getKey()) == NO_KEY );	// Do nothing. Waiting for keypress.
	return waitKey;
}

// Private
// Scan the keypad and report whether or not a key (or any key) has been pressed.
// 2011-12-23 - Removed from getKeyState() for readability and ease of maintenance.
boolean Keypad::scanKeys() {
	static unsigned int allKeys=0;
	byte curKey=0;
	boolean anyKey;

	// Assume that some other device is sharing the data pins used by the
	// keypad. If that is the case then the pins will need to be re-intialized
	// each time before they are used.
//     initializePins();
		// The assumption above is currently wrong; therefore, the initialization can be commented out safely. - CapacitorSet

	// I rewrote this method to provide a status change (anyKey OPEN/CLOSED) to the
	// getKeyState() function which handles debouncing. Now we can scan the keypad
	// without having to worry about huge debounce time delays.
	for( int c=0; c < KEYPAD_COLS; c++) {
		digitalWrite(columnPins[c], LOW);
		for( int r=0; r < KEYPAD_ROWS; r++) {
			curKey = digitalRead(rowPins[r]);
			allKeys += curKey;
			if(curKey==0) currentKey = keymap[c+(r*KEYPAD_COLS)];

			// All keys have been scanned. Set 'anyKey' value for use by getKeyState().
			if( r==(KEYPAD_ROWS-1) && c==(KEYPAD_COLS-1) ) {
				if( allKeys==(KEYPAD_ROWS*KEYPAD_COLS) )
					anyKey = OPEN;
				else
					anyKey = CLOSED;
			}
		}
		digitalWrite(columnPins[c], HIGH);
	}
	allKeys = 0;
	return anyKey;		// Status tells if keys are OPEN or CLOSED.
}

// only one key can ever be evaluated at one point in time.
KeyState Keypad::getKeyState() {
	static unsigned long startTime;
    static unsigned long Timer;
	static boolean buttons;
    stateChanged = false;

	// Scan keypad once every 10 mS. This makes the loop() count go from about
	// 4,000 loops per second to about 40,000 loops per second. A 10 fold increase
	// in program speed. It is also responsible for the major portion of time used
	// for debouncing the keys. Most humans can't press a key any faster than 20 mS
	// so the end user won't notice any lag at 10 mS.
	if ( (millis()-startTime)>10 )
		startTime = millis();
	else
		return state;

	// Find out whether or not a key was pressed and if so which one it was.
	buttons = scanKeys();

	switch (state) {
	case IDLE:
		// The only thing to do while idling is to look for a debounced keypress.
		if( buttons==CLOSED && (millis()-Timer)>debounceTime ) {
			transitionTo(PRESSED);
			Timer = millis();
		}
		break;
	case PRESSED:
	/*
		// Waiting for a key hold...
		if ( (millis()-Timer)>holdTime ) {
			transitionTo(HOLD);      // Move to next state.
			Timer = millis();    // Reset debounce timer.
		}
		// Or for the key to be release.
		else*/ if ( buttons==OPEN && (millis()-Timer)>debounceTime ) {
			transitionTo(RELEASED);
			Timer = millis();
		}
		break;
/*	case HOLD:
		// Waiting for the key to be released.
		if ( (buttons==OPEN) && (millis()-Timer)>debounceTime ) {
			transitionTo(RELEASED);
			Timer = millis();
		}
		break;*/
	case RELEASED:
		transitionTo(IDLE);
		break;
	}
    return state;
}

KeyState Keypad::getState() {
	return state;
}

void Keypad::addEventListener(void (*listener)(char)){
	keypadEventListener = listener;
}

void Keypad::transitionTo(KeyState nextState) {
    state = nextState;
    stateChanged = true;
	if (keypadEventListener!=NULL){
		keypadEventListener(currentKey);
	}
}

void Keypad::initializePins() {
    //configure column pin modes and states
    for (byte C=0; C<KEYPAD_COLS; C++) {
        pinMode(columnPins[C],OUTPUT);
        digitalWrite(columnPins[C],LOW);
    }
    //configure row pin modes and states
    for (byte R=0; R<KEYPAD_ROWS; R++) {
        pinMode(rowPins[R], INPUT_PULLUP);
    }
}
