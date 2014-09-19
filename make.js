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
		replacement += "\tif (CTRL && SHIFT) {\n"
		replacement += "\t\tbuffer     += " + quote(data.ctrlShift.toBuffer) + ";\n";
		if (!data.ctrlShift.toExpression) { data.ctrlShift.toExpression = data.ctrlShift.toBuffer; } // default the toExpression property to the value of toBuffer
		replacement += "\t\texpression += " + quote(data.ctrlShift.toExpression) + ";\n";
		replacement += "\t\tlatestExpressionLength = " + data.ctrlShift.toExpression.length + ";\n";
		replacement += "\t} else\n";
	}
	if (data.ctrl) {
		replacement += "\tif (CTRL && !SHIFT) {\n"
		replacement += "\t\tbuffer     += " + quote(data.ctrl.toBuffer) + ";\n";
		if (!data.ctrl.toExpression) { data.ctrl.toExpression = data.ctrl.toBuffer; } // default the toExpression property to the value of toBuffer
		replacement += "\t\texpression += " + quote(data.ctrl.toExpression) + ";\n";
		replacement += "\t\tlatestExpressionLength = " + data.ctrl.toExpression.length + ";\n";
		replacement += "\t} else\n";
	}
	if (data.shift) {
		replacement += "\tif (!CTRL && SHIFT) {\n"
		replacement += "\t\tbuffer     += " + quote(data.shift.toBuffer) + ";\n";
		if (!data.shift.toExpression) { data.shift.toExpression = data.shift.toBuffer; } // default the toExpression property to the value of toBuffer
		replacement += "\t\texpression += " + quote(data.shift.toExpression) + ";\n";
		replacement += "\t\tlatestExpressionLength = " + data.shift.toExpression.length + ";\n";
		replacement += "\t} else\n";
	}
	replacement += "\tif (!CTRL && !SHIFT) {\n"
	replacement += "\t\tif (TEXT_MODE) {\n";
	textModeChoices = data.textMode.length;
	replacement += "\t\t\ttapCount = tapCount % " + textModeChoices + ";\n";
	replacement += "\t\t\tswitch (tapCount) {\n"
	tapCount = 0;
	data.textMode.forEach(function (key) {
		replacement += "\t\t\t\tcase " + tapCount + ":\n";
		replacement += "\t\t\t\t\tbuffer     += " + quote(key) + ";\n";
		replacement += "\t\t\t\t\texpression += " + quote(key) + ";\n";
		replacement += "\t\t\t\t\tlatestExpressionLength = 1;\n";
		replacement += "\t\t\t\t\tbreak;\n"
		tapCount++;
	});
	replacement += "\t\t\t}\n";
	replacement += "\t\t} else {\n";
	replacement += "\t\t\tbuffer     += " + quote(data.vanilla.toBuffer) + ";\n";
	if (!data.vanilla.toExpression) { data.vanilla.toExpression = data.vanilla.toBuffer; } // default the toExpression property to the value of toBuffer
	replacement += "\t\t\texpression += " + quote(data.vanilla.toExpression) + ";\n";
	replacement += "\t\t\tlatestExpressionLength = " + data.vanilla.toExpression.length + ";\n";
	replacement += "\t\t}\n";
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

originalProgram = originalProgram.replace("/* KEYS */", replacement);

fs.writeFileSync("calculator.ino", originalProgram);