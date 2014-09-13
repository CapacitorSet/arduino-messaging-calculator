MAKER = make.js

calculator.ino : $(MAKER) keys.json Base.raw Keypad.cpp Keypad.h QueueArray.h StackList.h conversion_tools.cpp conversion_tools.h evaluate_postfix.cpp evaluate_postfix.h infix_postfix.cpp infix_postfix.h parse_tools.cpp parse_tools.h recovery_tools.cpp recovery_tools.h
	node $(MAKER)