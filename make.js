#!/usr/bin/node

fs                = require('fs');
keys              = JSON.parse(fs.readFileSync('keys.json').toString('utf8'));
originalProgram   = fs.readFileSync('calculator.raw').toString('utf8');
key_template      = fs.readFileSync("key.cpptemplate").toString("utf8");
tapCount_template = fs.readFileSync("tapCount.cpptemplate").toString("utf8");
replacement       = ""

for (key in keys) {
	data = keys[key];

	HandleTapCount = "";
	tapCount = 0;
	data.textMode.forEach(function (key) {
		HandleTapCount += tapCount_template.replace("$TapCount", tapCount).replace("$CurrentTap", setVars({"toBuffer": key, "toExpression": key}));
		tapCount++;
	});

	replacement += substituteVars(key_template, {
		"case": quote(key),
		"CtrlShift": setVars(data.ctrlShift),
		"CtrlNoshift": setVars(data.ctrl),
		"NoctrlShift": setVars(data.shift),
		"TapCount": data.textMode.length,
		"HandleTapCount": HandleTapCount,
		"Vanilla": setVars(data.vanilla)
	});
}

function quote(text) {
	// Puts single or double quotes around text. Single quotes are preferred, in order to save space.
	if (text.length == 1) {
		return "'" + text + "'";
	} else {
		return '"' + text + '"';
	}
}

function setVars(data) {
	if (typeof data !== 'undefined') {
		if (typeof data.toExpression == 'undefined') data.toExpression = data.toBuffer;
		returnVal = 'strcpy(toBuffer, "' + data.toBuffer     + '");strcpy(toExpression, "' + data.toExpression + '");'
		return returnVal;	
	} else {
		return "";
	}
}

function substituteVars(template, data) {
	for (item in data) {
		template = template.replace("$" + item, data[item]);
	}
	return template;
}

originalProgram = originalProgram.replace("/* KEYS */", replacement);

fs.writeFileSync("calculator.ino", originalProgram);