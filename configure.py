#!/usr/bin/env python
import json

strings_file = open("strings.json","r")
strings = json.loads(strings_file.read())
strings_file.close()

template_file = open("base.raw", "r")
template = template_file.read()
template_file.close()

while True:
	lang = raw_input("English (en)/Italiano (it)? [Default: it] ")
	if (lang == ""):
		lang = "it"
	if (any(lang in langitem for langitem in strings)):
		break

string = strings[lang]
yes = string["yes"]
no  = string["no"]

while True:
	debug = raw_input(string["debug"])
	if (debug == ""):
		debug = no
	if (debug == yes):
		replacement = "true"
	if (debug == no):
		replacement = "false"
	if (debug in (yes, no)):
		template = template.replace("$debug", replacement)
		break

while True:
	if (debug == yes):
		messagingInterface = raw_input(string["messagingInterface"])
		if (messagingInterface == ""):
			messagingInterface = "Serial1"
	else:
		isMicro = raw_input(string["typeArduino"])
		if (isMicro == ""):
			isMicro = yes
		if (isMicro in (yes, no)):
			if (isMicro == yes):
				messagingInterface = "Serial1"
			if (isMicro == no):
				messagingInterface = "Serial"
	if (messagingInterface in ("Serial", "Serial1")):
		template = template.replace("$messagingInterface", messagingInterface)
		break

while True:
	shiftPin = raw_input(string["shiftPin"])
	if (shiftPin == ""):
		shiftPin = "4"
	if (shiftPin.isdigit()):
		template = template.replace("$shiftPin", shiftPin)
		break

while True:
	ctrlPin = raw_input(string["ctrlPin"])
	if (ctrlPin == ""):
		ctrlPin = "3"
	if (ctrlPin.isdigit()):
		template = template.replace("$ctrlPin", ctrlPin)
		break

while True:
	lcdCols = raw_input(string["lcdCols"])
	if (lcdCols == ""):
		lcdCols = "16"
	if (lcdCols.isdigit()):
		template = template.replace("$lcdCols", lcdCols)
		break

while True:
	lcdRows = raw_input(string["lcdRows"])
	if (lcdRows == ""):
		lcdRows = "2"
	if (lcdRows.isdigit()):
		template = template.replace("$lcdRows", lcdRows)
		break

calculator_raw_file = open("calculator.raw", "w")
calculator_raw_file.write(template)
calculator_raw_file.close()

print string["done"]