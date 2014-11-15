all: calculator.ino

calculator.ino: keys.json calculator.raw
	./keys.py

calculator.raw: configure.py base.raw
	./configure.py

keys.json:
base.raw: