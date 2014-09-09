#!/usr/bin/node

// Insert here additional code to deal with keys.
// Notice how "\" is used to continue the string on a new line.

additionalCode = "";

fs = require('fs');
replacements = JSON.parse(fs.readFileSync('keys.json').toString('utf8'));
originalProgram = fs.readFileSync('Calcolatrice.ino').toString('utf8');

for (textToBeReplaced in replacements) {
	keys = replacements[textToBeReplaced];

	replacement = "\n"

	// All the \ts don't serve any real function and are ignored by the compiler, but they make the code more readable (thus making debugging easier).

	for (key in keys) {
		chars = keys[key]; // Eg. if keys = {"1": ["a", "b". "c", "1"]}, then key = "1" and "chars" = ["a", "b". "c", "1"]
		replacement += '\tcase "' + key + '":\n';
		replacement += '\t\tclearScreen = true;\n\t\tvaScritto = true;\n\t\tswitch (tapCount % ' + chars.length + ') {\n';
		tapCount = 0;
		chars.forEach(function (char) {
			replacement += '\t\t\tcase ' + tapCount + ':\n';
			replacement += "\t\t\t\tkey = '" + char + "';\n\t\t\t\tbreak;\n";
			tapCount++;
		});
		replacement += "\t\t}\n\t\tespressione += key;\n\t\tbreak;\n";
	}

	originalProgram = originalProgram.replace("/* " + textToBeReplaced + " */", replacement);

}
console.log(originalProgram);