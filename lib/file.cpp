#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

extern vector<string> raw;
extern vector<string> lines;
extern vector<string> viewport;
extern string currentfile;

void loadFile(string filepath){
	lines.clear();
	raw.clear();
	lines.push_back(" if you can see this, somethings gone wrong ");
	raw.push_back(" if you can see this, somethings gone wrong ");

	currentfile = filepath;

	ifstream file(filepath);

	string line = "";
	while (getline(file, line)){
		// tab convert //
		string convert = "";
		for (int i = 0; i < line.size(); i++){
			if (line[i] == '\t'){
				convert += "    ";
			}else{
				convert += line[i];
			}
		}
		raw.push_back(convert);
		lines.push_back(syntaxLine(convert));
	}
	file.close();

	updateViewport();
}

void openFile(){
	system("setterm -cursor on");
	setCursorPosition(0,0);
	clear();
	while (true){
		string input = "";
		cout << "Open file >> ";
		getline(cin, input);

		ifstream file(input);

		if (file.good()){
			loadFile(input);

			// redraw display //
			system("setterm -cursor off");
			clear();
			drawScreen();
			drawHeader();
			updateCursor();

			break;
		}else{
			clear();
			cout << "Error loading file" << endl << input;
		}
	}
}

void saveFile(){
	ofstream file(currentfile);

	for (int i = 1; i < lines.size(); i++){
		file << lines[i] << endl;
	}

	file.close();
}

void saveAsFile(){
	system("setterm -cursor on");
	setCursorPosition(0,0);
	clear();
	string input = "";
	cout << "Save as >> ";
	getline(cin, input);

	currentfile = input;

	// redraw screen //
	system("setterm -cursor off");
	setCursorPosition(0,0);
	clear();
	drawScreen();
	drawHeader();
	updateCursor();

	saveFile();
}

void newFile(){
	lines.clear();
	lines.push_back(" if you can see this, somethings gone wrong ");
	lines.push_back(" ");
	lines.push_back(" ");

	raw.clear();
	raw.push_back(" if you can see this, somethings gone wrong ");
	raw.push_back(" ");
	raw.push_back(" ");

	currentfile = "newfile";

	updateViewport();

	curx = 0;
	cury = 1;

	setCursorPosition(0,0);
	clear();
	drawScreen();
	drawHeader();
	updateCursor();
}
