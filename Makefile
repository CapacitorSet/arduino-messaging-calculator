all: calculator.ino

calculator.ino: base.raw make.js keys.json
	node make.js