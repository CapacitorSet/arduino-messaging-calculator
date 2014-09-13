MAKER = make.js

calculator.ino : $(MAKER) keys.json Base.raw
	node $(MAKER)
