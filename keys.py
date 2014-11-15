#!/usr/bin/env python
# Automatically generates the code for handling keys, based on keys.json.

import json

keys_file = open("keys.json", "r")
keys      = keys_file.read()
keys      = json.loads(keys)
keys_file.close()

base_file = open("calculator.raw", "r")
base      = base_file.read()
base_file.close()

key_template_file = open("key.cpptemplate", "r")
key_template      = key_template_file.read()
key_template_file.close()

tap_template_file = open("tapCount.cpptemplate", "r")
tap_template      = tap_template_file.read()
tap_template_file.close()

replacement       = ""


def substituteVars(template, data):
	temp = template
	for item in data:
		temp = temp.replace("$" + item, str(data[item]))
	return temp

def quote(key):
	if len(key) == 1:
		return "'" + key + "'"
	else:
		return '"' + key + '"'


def setVars(data):
	if data == "ignore":
		return ""
	else:
		return 'buffer += "' + data["toBuffer"] + '";'

for key in keys:
	data = keys[key]

	HandleTapCount = ""
	tapCount = 0
	for item in data["textMode"]:
		temp = tap_template.replace("$TapCount", str(tapCount))
		temp = temp.replace("$CurrentTap", setVars({"toBuffer": key}))
		HandleTapCount = temp
		tapCount += 1

	if "ctrlShift" not in data:
		data["ctrlShift"] = "ignore"
	if "ctrl" not in data:
		data["ctrl"] = "ignore"
	if "shift" not in data:
		data["shift"] = "ignore"


	replacement += substituteVars(key_template, {
		"case": quote(key),
		"CtrlShift": setVars(data["ctrlShift"]),
		"CtrlNoshift": setVars(data["ctrl"]),
		"NoctrlShift": setVars(data["shift"]),
		"TapCount": len(data["textMode"]),
		"HandleTapCount": HandleTapCount,
		"Vanilla": setVars(data["vanilla"])
	})


program = base.replace("/* KEYS */", replacement)

calculator_file = open("calculator.ino", "w")
calculator_file.write(program)
calculator_file.close()