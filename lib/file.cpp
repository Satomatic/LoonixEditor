#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

extern vector<string> raw;
extern vector<string> lines;
extern vector<string> viewport;
extern string currentfile;
extern bool hasEdited;

extern vector<vector<string>> openFiles;
extern vector<vector<string>> fileMemory;
extern int fileIndex;

extern HeaderDrop headerMessage;
extern DiffManager diffManager;

void saveFile();

void loadFile(string filepath){
	lines.clear();
	raw.clear();
	lines.push_back(" if you can see this, somethings gone wrong ");
	raw.push_back(" if you can see this, somethings gone wrong ");

	currentfile = filepath;
	hasEdited = false;

	ifstream file(filepath);

	string line = "";
	while (getline(file, line)){
		// tab convert //
		string convert = "";
		for (int i = 0; i < line.size(); i++){
			if (line[i] == '\t'){
				convert += "    ";
			}else if (line[i] == '\r'){
				
			}else{
				convert += line[i];
			}
		}
		raw.push_back(convert);
		lines.push_back(syntaxLine(convert));
	}
	file.close();

	if (lines.size() <= 1){
		raw.push_back("");
		lines.push_back("");
	}

	updateViewport();
}

void createFile(string filename){
	currentfile = filename;
	hasEdited = false;
	
	ofstream file(filename);
	file << " " << endl << " " << endl;
	file.close();
}

void createFileMemory(string filename){
	openFiles.push_back({filename, "0", "1", "0", "1"});
	fileMemory.push_back({filename, " header space ", " ", " "});
}

void loadFileFromMemory(string filename){
	string currentfile = filename;
	hasEdited = false;
	
	// search for file in scroll //
	int foundIndex = 0;
	
	for (int i = 0; i < fileMemory.size(); i++){
		if (fileMemory[i][0] == filename){
			foundIndex = i;
			break;
		}
	}
	
	// clear viewport, raw, ect... //
	lines.clear();
	raw.clear();
	viewport.clear();
	rawViewport.clear();
	
	// load file into raw and lines //
	for (int i = 1; i < fileMemory[foundIndex].size(); i++){
		raw.push_back(fileMemory[foundIndex][i]);
		lines.push_back(syntaxLine(fileMemory[foundIndex][i]));
	}

	// load cursor position //
	for (int i = 0; i < openFiles.size(); i++){
		if (openFiles[i][0] == currentfile){
			curx = stoi(openFiles[i][1]);
			cury = stoi(openFiles[i][2]);
			scroll = stoi(openFiles[i][3]);
			
			if (openFiles[i][4] == "1"){
				hasEdited = true;
			}else{
				hasEdited = false;
			}
		}
	}
}

void moveFileIntoMemory(){
	// check if file is in memory //
	bool fileFound = false;
	int foundIndex = 0;
	
	for (int i = 0; i < openFiles.size(); i++){
		if (openFiles[i][0] == currentfile){
			fileFound = true;
			foundIndex = i;
			break;
		}
	}
	
	// edit file scroll vector //
	if (fileFound == false){
		openFiles.push_back({currentfile, "0", "1", "0", "0"});
		fileMemory.push_back({currentfile});
		
		for (int i = 0; i < raw.size(); i++){
			fileMemory[fileMemory.size() - 1].push_back(raw[i]);
		}
	}else{
		openFiles[foundIndex][1] = to_string(curx);
		openFiles[foundIndex][2] = to_string(cury);
		openFiles[foundIndex][3] = to_string(scroll);
		openFiles[foundIndex][4] = to_string(hasEdited);
	
		int foundIndex = 0;
	
		// search for file in memory //
		for (int i = 0; i < fileMemory.size(); i++){
			if (fileMemory[i][0] == currentfile){
				foundIndex = i;
				break;
			}
		}
		
		// push file into memory //
		fileMemory[foundIndex].clear();
		fileMemory[foundIndex].push_back(currentfile);

		for (int i = 0; i < raw.size(); i++){
			fileMemory[foundIndex].push_back(raw[i]);
		}
	}
}

void openFile(){
	system("setterm -cursor on");
	setCursorPosition(0,0);
	clear();

	// clear file memory //
	fileMemory.clear();
	openFiles.clear();
	fileIndex = 0;

	string input = "";
	cout << "Open file >> ";
	getline(cin, input);

	ifstream file(input);

	if (file.good()){
		loadFile(input);

		// redraw display //
		scroll = 0;
		curx = 0; // reset cursor
		cury = 1;

		moveFileIntoMemory();

		clear();
		updateViewport();
		drawScreen();
		drawHeader();
		updateCursor();
	}else{
		clear();
		updateViewport();
		drawScreen();
		drawHeader();
		updateCursor();

		headerMessage.message = "File does not exist";
		headerMessage.styling = "\u001b[38;5;124m";
		headerMessage.draw();
	}

	system("setterm -cursor off");
}

int openFileNewBuffer(){
	resetColor();
	setCursorPosition(0, 0);

	string input = "";

	// create input box //
	Input open;
	open.title = "Open file";
	open.prefix = "File > ";
	open.maxx = 15;
	open.center = true;
	open.border = true;
	open.init();
	open.undraw();
	
	input = open.input;

	if (input != ""){
		ifstream file(input);
	
		if (file.good()){
			// check if file is already open //
			OptionDialog overwrite;
			for (int i = 0; i < openFiles.size(); i++){
				if (openFiles[i][0] == input){
					overwrite.message = "This file in already open\\Would you like to overwrite it?";
					overwrite.items = {"Yes", "No", "Cancel"};
					overwrite.height = 4;
					overwrite.width = 33;
					overwrite.centerText = true;
					overwrite.draw();
				}
			}
			
			if (overwrite.selected == 0){
				moveFileIntoMemory();
		
				scroll = 0;
				curx = 0;
				cury = 1;
		
				currentfile = input;
				fileIndex = openFiles.size();
				loadFile(input);
				moveFileIntoMemory();

				return 1;
			}

		}else{
			return 0;
		}
	}

	return 2; // return 2 if file remains unopened //
}

void openFileCurrentBuffer(){
	cout << "\u001b[0m";
	string input;
	bool nofill = false;
	int height = 1;
	
	while (true){
		Input open;
		open.container.nofill = nofill;
		open.height = height;
		open.input = input;
		open.title = "Replace buffer";
		open.prefix = "File > ";
		open.maxx = 15;
		open.center = true;
		open.border = true;
		open.init();
		
		input = open.input;
		
		if (FileExists(input)){
			open.undraw();
			break;
		}else if (input == ""){
			open.undraw();
			break;
		}else{
			height = 2;
			nofill = true;
			setCursorPosition(open.x, open.y + 2);
			cout << "\u001b[38;5;124mFile does not exist";
			open.container.draw();
		}
	}
	
	OptionDialog save;
	if (input != ""){
		if (hasEdited == true){
			save.message = "Save changes?";
			save.items = {"Yes", "No", "Cancel"};
			save.centerText = true;
			save.draw();
			
			if (save.selected == 0){
				saveFile();
			
			}else if (save.selected == 2){
				save.undraw();
				return;
			}
		}
		
		// clear screen //
		clearText();
		
		// clear file memory //
		fileMemory[fileIndex].clear();
		openFiles[fileIndex][0] = input;
		openFiles[fileIndex][1] = to_string(0);
		openFiles[fileIndex][2] = to_string(0);
		openFiles[fileIndex][3] = to_string(0);
		openFiles[fileIndex][4] = "false";
		currentfile = input;
		loadFile(input);
		moveFileIntoMemory();
		
		scroll = 0;
		cury = 1;
		curx = 0;
		
		newRefresh();
		updateHeader();
		updateCursor();
	}
	
	newRefresh();
	updateCursor();
}

void saveFile(){
	diffManager.zero();
	hasEdited = false;

	ofstream file(currentfile);

	if (file.good()){
		for (int i = 1; i < raw.size(); i++){
			string convert = raw[i];

			// convert spaces to tabs
			for (int b = 0; b < convert.size(); b++){
				if (convert.substr(b, 4) == "    "){
					convert.replace(b, 4, "\t");
				}else{
					break;
				}
			}

			file << convert << endl;
		}

		headerMessage.styling = "\u001b[0m";    
		headerMessage.message = "File saved";
		headerMessage.draw();
	
	}else{
		headerMessage.message = "Error saving file";
		headerMessage.styling = "\u001b[38;5;124m";
		headerMessage.draw();
	}

	file.close();

}

void saveAsFile(){
	setCursorPosition(0,0);

	string input = "";

	Input save;
	save.title = "Save as";
	save.prefix = "File > ";
	save.maxx = 15;
	save.center = true;
	save.border = true;
	save.init();
	save.undraw();

	input = save.input;

	if (input != ""){
		fileMemory.erase(fileMemory.begin() + fileIndex);
		openFiles.erase(openFiles.begin() + fileIndex);
		
		currentfile = input;

		// move new file into memory //
		moveFileIntoMemory();

		// redraw screen //
		system("setterm -cursor off");
		setCursorPosition(0,0);
		clear();
		drawScreen();
		drawHeader();
		updateCursor();

		saveFile();
	}
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

	scroll = 0;
	curx = 0;
	cury = 1;

	setCursorPosition(0,0);
	clear();
	drawScreen();
	drawHeader();
	updateCursor();
}

