#define DEBUG true
#define SHIFT_KEY_PIN 4
#define CTRL_KEY_PIN 3
#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_0 A0
#define LCD_1 A1
#define LCD_2 A2
#define LCD_3 A3
#define LCD_4 A4
#define LCD_5 A5

#define RESULT_PRECISION 8
#define RESULT_MAX_SIZE 64

#define MAX_MSG_SIZE 64
#define DISPLAY_SIZE 16

#include <StackList.h>
#include "infix_postfix.h"
#include "evaluate_postfix.h"
#include "conversion_tools.h"
#include <LiquidCrystal.h>
#include <Keypad.h>

#define ROWS 4
#define COLS 4
char keys[ROWS][COLS] = {
	{'1','2','3','+'},
	{'4','5','6','-'},
	{'7','8','9','*'},
	{'S','0','D','/'}
};
byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = { 8,  7,  6, 5};


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCD_0, LCD_1, LCD_2, LCD_3, LCD_4, LCD_5);
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
bool clearScreen = true; // Per cancellare lo splash screen

// Per il calcolo
String espressione;
int tmp;
double Ans, result;
String prevAns;
String postfix;
char value[RESULT_MAX_SIZE]; // as string

// Per la comunicazione
char messaggio[MAX_MSG_SIZE+1];
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
bool newMessage = true;

// Per il multitap
char prevKey = '\0';
int tapCount = 0;
bool textMode = false;
unsigned long time = 0;


String process(String infix) {
	#if DEBUG
		Serial.print("Processing ");
		Serial.println(infix);
	#endif
	String replacement = "(";
	replacement.replace('-', '0-'); // Es. se prevAns = -1, replacement = "(0-1)"
	replacement.concat(prevAns);
	replacement.concat(')');
	infix.replace("a", replacement);
	return infix;
}

void setup () {
	#if DEBUG
		Serial.begin (9600);
	#endif
	Serial1.begin(9600);

	// Set up the LCD
	lcd.begin(LCD_COLS, LCD_ROWS);
	lcd.print("Tette *-*");

	pinMode(SHIFT_KEY_PIN, INPUT_PULLUP);
	pinMode(CTRL_KEY_PIN, INPUT_PULLUP);
	delay(5000);
	Serial1.println("Ciao!");
}

void scrivi(String testo, bool raw) {
	if (!raw) {
		testo.replace("l", "ln ");
		testo.replace("L", "log ");
		testo.replace("s", "sin ");
		testo.replace("c", "cos ");
		testo.replace("a", "Ans");
		testo.replace("t", "tan ");
		testo.replace("S", "asin ");
		testo.replace("C", "acos ");
		testo.replace("T", "atan ");
	}
	lcd.print(testo);
        if(testo.length() > DISPLAY_SIZE - 1){
		#if DEBUG
			Serial.println("Auto Scrolling");
		#endif
		for (int i = 0; i <= testo.length() - DISPLAY_SIZE; i++)
			lcd.scrollDisplayLeft();
	}
}

void loop () {
	if (Serial1.available()) {
		newMessage = true;
		inChar = Serial1.read(); // Read a character
		#if DEBUG
			Serial.println(inChar);
		#endif
		if (inChar == '£') {
			for (int i = 0; i < MAX_MSG_SIZE+1; i++) {
				messaggio[i] = '\0';
			}
		}
		if(index < MAX_MSG_SIZE) {
			messaggio[index] = inChar; // Store it
			messaggio[++index] = '\0'; // Null terminate the string
		}
	}

	bool vaScritto = false;
	bool raw = false;
	char key = customKeypad.getKey();
	const bool shift = !digitalRead(SHIFT_KEY_PIN);
	const bool ctrl  = !digitalRead(CTRL_KEY_PIN);
	
	if (key) {
		if (textMode) { raw = true; }
		if (shift && ctrl && (key == '5')) {
			clearScreen = true;
			vaScritto = false;
			textMode = !textMode;
			espressione = "";
			#if DEBUG
				Serial.print("Toggling textMode");
			#endif
		} else
		if (shift && ctrl && (key == 'S')) {
			clearScreen = false;
			vaScritto = false;
			textMode = !textMode;
			lcd.scrollDisplayLeft();
		} else
		if (shift && ctrl && (key == 'D')) {
			clearScreen = false;
			vaScritto = false;
			lcd.scrollDisplayRight();
		} else
		if (!shift && !ctrl && (key == 'D')) {
			clearScreen = true;
			vaScritto = true;
			espressione = espressione.substring(0, espressione.length() - 1);
			prevKey = '\0'; // E' difficile da spiegare, ma va tenuto altrimenti non si comporta in modo naturale per l'utente.
		} else
		if (textMode) {
			if (key == prevKey && (millis() - time) < 1000) {
				tapCount++;
				espressione = espressione.substring(0, espressione.length() - 1);
				time = millis();
			} // Cancella il token precedente
			else {
				tapCount=0;
				prevKey = '\0';
				time = millis();
			}
			prevKey = key;
			switch (key) {
				/* TEXTMODE_KEYS */
				/* Don't delete the comment above, it is used in make.js */
				case 'S':
					// TODO
					break;
			}
		} else if (shift && ctrl) {
				switch (key) {
					case '0':
						clearScreen = true;
						vaScritto = true;
						espressione = messaggio;
						newMessage = false;
						break;
					case '+':
						clearScreen = true;
						vaScritto = true;
						key = 'L';
						espressione += String(key);
						break;
					case '-':
						clearScreen = true;
						vaScritto = true;
						key = 'S';
						espressione += String(key);
						break;
					case '*':
						clearScreen = true;
						vaScritto = true;
						key = 'C';
						espressione += String(key);
						break;
					case '/':
						clearScreen = true;
						vaScritto = true;
						key = 'T';
						espressione += String(key);
						break;
				}
		} else if (shift) {
				switch (key) {
					case '+':
						clearScreen = true;
						vaScritto = true;
						key = '^';
						espressione += String(key);
						break;
					case '-':
						clearScreen = true;
						vaScritto = true;
						key = 'a';
						espressione += String(key);
						break;
					case '*':
						clearScreen = true;
						vaScritto = true;
						key = '(';
						espressione += String(key);
						break;
					case '/':
						clearScreen = true;
						vaScritto = true;
						key = ')';
						espressione += String(key);
						break;
					case '0':
						clearScreen = true;
						vaScritto = true;
						key = '.';
						espressione += String(key);
						break;
				}
		} else if (ctrl) {
				switch (key) {
					case '+':
						clearScreen = true;
						vaScritto = true;
						key = 'l';
						espressione += String(key);
						break;
					case '-':
						clearScreen = true;
						vaScritto = true;
						key = 's';
						espressione += String(key);
						break;
					case '*':
						clearScreen = true;
						vaScritto = true;
						key = 'c';
						espressione += String(key);
						break;
					case '/':
						clearScreen = true;
						vaScritto = true;
						key = 't';
						espressione += String(key);
						break;
				}
		} else {
				switch (key) {
					case 'S':
						clearScreen = false;
						vaScritto = false;
						{
							lcd.setCursor(0, 1);
							String tmp = process(espressione); // try to convert the infix expression to postfix.
							postfix = "";
							if (infix_postfix (tmp, postfix)) {
								#if SERIAL_DEBUG
									Serial.print("Postfix: ");
									Serial.println(postfix);
								#endif

								// try to evaluate the postfix expression.
								if (evaluate_postfix (postfix, result)) {
									Ans = result;
									// convert the result as string.
									double_string (value, result, RESULT_PRECISION);

									// print the result
									lcd.print(value);
									prevAns = value;
								} else {
									lcd.write("Can'tEvalPostfix");
									#if DEBUG
										Serial.println(postfix);
									#endif
								}
							} else {
								lcd.write("CantConvert2Infx");
							}
						} // These braces are magic *-*
						// Don't delete them!
						// [I don't know why I put them here, but it's a bugfix, so don't you dare touch it. - CapacitorSet]
						break;
					default:
						espressione += key;
						vaScritto = true;
						clearScreen = true;
						break;
				}
		}

		if (clearScreen) {
			lcd.clear();
			clearScreen = false;
		}
		if (vaScritto) {
			#if DEBUG
				Serial.print("Scrivendo ");
				Serial.println(espressione);
			#endif
			lcd.setCursor(0, 0);
			scrivi(espressione, raw);
		}
	}
}


