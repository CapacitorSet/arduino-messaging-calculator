Settings are stored in a `byte`, `settings`. Bits are allocated thus:

0x01: PrintExpression. Controls whether `expression` should be printed to the LCD display at the end of the loop.
0x02: ClearScreen. Controls whether the LCD screen is cleared at the end of the loop.
0x04: TextMode. 0 if the user is in math mode, 1 if they are in text mode.
0x08: WaitingForData. 1 if the calculator is waiting for the user to enter data (to be stored in the EEPROM).
0x10: WaitingForAddress. 1 if the calculator is waiting for the user to enter a memory address.
0x20: Shift.
0x40: Ctrl.