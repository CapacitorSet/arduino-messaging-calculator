#!/usr/bin/node

fs = require('fs');
keys = JSON.parse(fs.readFileSync('keys.json').toString('utf8'));
originalProgram = fs.readFileSync('Base.raw').toString('utf8');
replacement = "\n";

// All the \ts don't serve any real function and are ignored by the compiler, but they make the code more readable (thus making debugging easier).

for (key in keys) {
	data = keys[key]
	replacement += "case " + quote(key) + ":\n";
	if (data.ctrlShift) {
		replacement += "\tif (ctrl && shift) {\n"
		replacement += setVars(data.ctrlShift);
		replacement += "\t} else\n";
	}
	if (data.ctrl) {
		replacement += "\tif (ctrl && !shift) {\n"
		replacement += setVars(data.ctrl);
		replacement += "\t} else\n";
	}
	if (data.shift) {
		replacement += "\tif (!ctrl && shift) {\n"
		replacement += setVars(data.shift);
		replacement += "\t} else\n";
	}
	replacement += "\tif (!ctrl && !shift) {\n"
	replacement += "\t\tif (textMode) {\n";
	textModeChoices = data.textMode.length;
	replacement += "\t\t\ttapCount = tapCount % " + textModeChoices + ";\n";
	replacement += "\t\t\tswitch (tapCount) {\n"
	tapCount = 0;
	data.textMode.forEach(function (key) {
		replacement += "\t\t\t\tcase " + tapCount + ":\n";
		item = {"toBuffer": key, "toExpression": key};
		replacement += "\t\t\t" + setVars(item);
		replacement += "\t\t\t\t\tbreak;\n"
		tapCount++;
	});
	replacement += "\t\t\t}\n";
	replacement += "\t\t} else {\n";
	replacement += "\t" + setVars(data.vanilla);
	replacement += "\t\t}\n";
	replacement += "\tManageKey(toBuffer, toExpression, buffer, expression, latestExpressionLength);\n";
	replacement += "\t}\n";

	replacement += "\tbreak;\n"
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
	if (typeof data.toExpression == 'undefined') data.toExpression = data.toBuffer;
	returnVal = '\t\tstrcpy(toBuffer, "' + data.toBuffer     + '");\n'
	returnVal+= '\t\tstrcpy(toExpression, "' + data.toExpression + '");\n'
	return returnVal;	
}

originalProgram = originalProgram.replace("/* KEYS */", replacement);

fs.writeFileSync("calculator.ino", originalProgram);