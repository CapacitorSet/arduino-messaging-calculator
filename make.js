#!/usr/bin/node

fs = require('fs');
keys = JSON.parse(fs.readFileSync('keys.json').toString('utf8'));
originalProgram = fs.readFileSync('Base.raw').toString('utf8');
replacement = "\n";

// All the \ts don't serve any real function and are ignored by the compiler, but they make the code more readable (thus making debugging easier).

for (key in keys) {
	data = keys[key]
	replacement += "case '" + key + "':\n";
	if (data.ctrlShift) {
		replacement += "\tif (ctrl && shift) {\n"
		replacement += "\t\tbuffer     += '" + data.ctrlShift.toBuffer + "';\n";
		if (!data.ctrlShift.toExpression) { data.ctrlShift.toExpression = data.ctrlShift.toBuffer; } // default the toExpression property to the value of toBuffer
		replacement += "\t\texpression += \"" + data.ctrlShift.toExpression + "\";\n";
		replacement += "\t\tlatestExpressionLength = " + data.ctrlShift.toExpression.length + ";\n";
		replacement += "\t} else\n";
	}
	if (data.ctrl) {
		replacement += "\tif (ctrl && !shift) {\n"
		replacement += "\t\tbuffer     += '" + data.ctrl.toBuffer + "';\n";
		if (!data.ctrl.toExpression) { data.ctrl.toExpression = data.ctrl.toBuffer; } // default the toExpression property to the value of toBuffer
		replacement += "\t\texpression += \"" + data.ctrl.toExpression + "\";\n";
		replacement += "\t\tlatestExpressionLength = " + data.ctrl.toExpression.length + ";\n";
		replacement += "\t} else\n";
	}
	if (data.shift) {
		replacement += "\tif (!ctrl && shift) {\n"
		replacement += "\t\tbuffer     += '" + data.shift.toBuffer + "';\n";
		if (!data.shift.toExpression) { data.shift.toExpression = data.shift.toBuffer; } // default the toExpression property to the value of toBuffer
		replacement += "\t\texpression += \"" + data.shift.toExpression + "\";\n";
		replacement += "\t\tlatestExpressionLength = " + data.shift.toExpression.length + ";\n";
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
		replacement += "\t\t\t\t\tbuffer     += \"" + key + "\";\n";
		replacement += "\t\t\t\t\texpression += \"" + key + "\";\n";
		replacement += "\t\t\t\t\tlatestExpressionLength = 1;\n";
		replacement += "\t\t\t\t\tbreak;\n"
		tapCount++;
	});
	replacement += "\t\t\t}\n";
	replacement += "\t\t} else {\n";
	replacement += "\t\t\tbuffer     += '" + data.vanilla.toBuffer + "';\n";
	if (!data.vanilla.toExpression) { data.vanilla.toExpression = data.vanilla.toBuffer; } // default the toExpression property to the value of toBuffer
	replacement += "\t\t\texpression += \"" + data.vanilla.toExpression + "\";\n";
	replacement += "\t\t\tlatestExpressionLength = " + data.vanilla.toExpression.length + ";\n";
	replacement += "\t\t}\n";
	replacement += "\t}\n";

	replacement += "\tbreak;\n"
}

originalProgram = originalProgram.replace("/* KEYS */", replacement);

fs.writeFileSync("calculator.ino", originalProgram);