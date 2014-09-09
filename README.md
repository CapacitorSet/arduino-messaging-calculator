#Calculator - an Arduino-based calculator with support for messaging#

###Setup###
For the purposes of this tutorial:

- *Pin `X`* means "the pin labeled as `X` on the Arduino board", unless otherwise specified.
- *Positive rail* means "the `5V` pin".
- *Ground rail* means "the `GND` pin".
- The pins on the HD44780 are numbered from left to right: "pin `1` of the HD44780" being its ground pin (`Gnd`), and "pin `16` of the HD44780" being its backlight cathode (`BLK`).
- The pins on the keypad are numbered from left to right: "pin `1` of the keypad" being its pin for the top row, and "pin `8` of the keypad" being its pin for the rightmost column.
- One pushbutton (of your choice) is "the Shift key"; the other one is "the Ctrl key".
   
1. Get an Arduino Micro, two pushbutton switches, one potentiometer, one HD44780 controller, and one 4x4 keypad.
2. Connect the leftmost pin of the potentiometer to the positive rail.
3. Connect the rightmost pin of the potentiometer to the negative rail.
4. Connect pin `1` of the HD44780 to the ground rail.
5. Connect pin `2` of the HD44780 to the positive rail.
6. Connect pin `3` of the HD44780 to the middle pin of the potentiometer.
7. Connect pin `4` of the HD44780 to the `A0` pin.
8. Connect pin `5` of the HD44780 to the ground rail.
9. Connect pin `6` of the HD44780 to the `A1` pin.
10. Connect pin `11` of the HD44780 to the `A2` pin.
11. Connect pin `12` of the HD44780 to the `A3` pin.
12. Connect pin `13` of the HD44780 to the `A4` pin.
13. Connect pin `14` of the HD44780 to the `A5` pin.
14. Connect one pin of the Shift key to the ground rail, and the other one to the `4` pin.
15. Connect one pin of the Ctrl key to the ground rail, and the other one to the `3` pin.
16. Connect pin `1` of the keypad to the `12` pin.
17. Connect pin `2` of the keypad to the `11` pin.
18. Connect pin `3` of the keypad to the `10` pin.
19. Connect pin `4` of the keypad to the `9` pin.
20. Connect pin `5` of the keypad to the `8` pin.
21. Connect pin `6` of the keypad to the `7` pin.
22. Connect pin `7` of the keypad to the `6` pin.
23. Connect pin `8` of the keypad to the `5` pin.

To generate the code, run "node make.js" (you must have installed node.js). Then, open Calcolatrice.ino in the Arduino IDE, and upload the sketch to your Arduino board.

###Usage###
To begin using the calculator, simply power the Arduino. The LCD display should turn on and display `Tette *-*`. To compute an expression, simply type it out as you would on a regular calculator, then press the `*` key.
To scroll the display to the left, press `Ctrl+Shift+*`; to scroll it to the right, press `Ctrl+Shift+#`.

###Typing###
To delete the last character entered, press `#`.
To type a digit, simply press its key.


    +-Operator-+------Key-----+
    |    +     | Ctrl+Shift+A |
    +----------+--------------+
    |    -     |       B      |
    +----------+--------------+
    |    x     |       C      |
    +----------+--------------+
    |    /     |       D      |
    +----------+--------------+
    | Decimal  |    Shift+0   |
    | dot      |              |
    +----------+--------------+
    |    ^     |    Shift+A   |
    +----------+--------------+
    | Previous |    Shift+B   |
    |  result  |              |
    +----------+--------------+
    |    (     |    Shift+C   |
    +----------+--------------+
    |    )     |    Shift+D   |
    +----------+--------------+
    |    ln    |    Ctrl+A    |
    +----------+--------------+
    |   sin    |    Ctrl+B    |
    +----------+--------------+
    |   cos    |    Ctrl+C    |
    +----------+--------------+
    |   tan    |    Ctrl+D    |
    +----------+--------------+
    |   log    |  Ctrl+Shift+A|
    +----------+--------------+
    |   asin   |  Ctrl+Shift+B|
    +----------+--------------+
    |   acos   |  Ctrl+Shift+C|
    +----------+--------------+
    |   atan   |  Ctrl+Shift+D|
    +----------+--------------+
