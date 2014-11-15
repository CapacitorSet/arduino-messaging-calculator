all: calculator.ino

calculator.ino: keys.json key.cpptemplate tapCount.cpptemplate calculator.raw
	./keys.py

calculator.raw: configure.py base.raw
	./configure.py

keys.json:
base.raw: