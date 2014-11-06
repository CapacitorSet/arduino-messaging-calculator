all: calculator.ino

calculator.ino: base.raw make.js keys.json Keys.cpptemplate
	node make.js

base.raw:
make.js:
keys.json:
Keys.cpptemplate: TapCount.cpptemplate
TapCount.cpptemplate: