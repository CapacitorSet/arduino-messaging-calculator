#!/usr/bin/node

fs = require('fs');
keys = JSON.parse(fs.readFileSync('keys.json').toString('utf8'));
originalProgram = fs.readFileSync('Base.raw').toString('utf8');
replacement = "\n";

// All the \ts don't serve any real function and are ignored by the compiler, but they make the code more readable (thus making debugging easier).

for (key in keys) {
	data = keys[key]
	replacement       = fs.readFileSync("Key.cpptemplate").toString("utf8");
	tapCount_template = fs.readFileSync("TapCount.cpptemplate").toString("utf8");

	HandleTapCount = "";
	tapCount = 0;
	data.textMode.forEach(function (key) {
		HandleTapCount += tapCount_template.replace("$TapCount", tapCount).replace("$CurrentTap", setVars({"toBuffer": key, "toExpression": key}));
		tapCount++;
	});

	replacement = substituteVars(replacement, {
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
		returnVal = '\t\tstrcpy(toBuffer, "' + data.toBuffer     + '");strcpy(toExpression, "' + data.toExpression + '");\n'
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