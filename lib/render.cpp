#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern int curx;
extern int cury;
extern int prex;
extern int prey;
extern int index; // acts as a start point for view port //

extern int screenHeight; // used as the viewport size //
extern int screenWidth;

extern string currentfile;
extern vector<string> raw;
extern vector<string> lines;
extern vector<string> viewport;
extern vector<string> rawViewport;

extern vector<vector<string>> openFiles;
extern int fileIndex;

extern bool hasEdited;

int unilen(string str) {
	int length = 0;
	for (char c : str) {
		if ((c & 0xC0) != 0x80) {
			length++;
		}
	}

	return length;
}

string nonUniString(string str){
	string returnstring;
	
	for (int i = 0; i < str.size(); i++){
		char c = str.at(i);
		
		if ((c & 0xC0) != 0x80){
			returnstring += c;
		}
	}
	
	return returnstring;
}

void updateViewport(){
	viewport.clear();
	rawViewport.clear();
	for (int i = 0; i < lines.size(); i++){
		if (i >= index && i <= index + screenHeight - 2){
			if (unilen(raw[i]) > screenWidth){
				viewport.push_back(lines[i].substr(0, screenWidth - 1));
				rawViewport.push_back(raw[i].substr(0, screenWidth - 1));
			}else{
				viewport.push_back(lines[i]);
				rawViewport.push_back(raw[i]);
			}
		}
	}
}

int testViewport(){
	int size = 0;
	for (int i = 0; i < lines.size(); i++){
		if (i >= index && i <= index + screenHeight - 2){
			size++;
		}
	}
	
	return size;
}

void drawHeader(){
	setCursorPosition(0,0);
	resetColor();
	cout << "\u001b[30m\u001b[107m";

	string headerdata = "Loonix [cols: X][row: Y] :: Z";

	headerdata = replace_all(headerdata, "X", to_string(curx));
	headerdata = replace_all(headerdata, "Y", to_string(index + cury));
	headerdata = replace_all(headerdata, "Z", currentfile);

	if (openFiles.size() > 1){
		string buffer = " (X/Y)";
		
		buffer = replace_all(buffer, "X", to_string(fileIndex + 1));
		buffer = replace_all(buffer, "Y", to_string(openFiles.size()));
		
		headerdata += buffer;
	}

	if (hasEdited == true){
		headerdata += " *";
	}

	headerdata += "   ";

	cout << fillX(headerdata) << "\u001b[0m";
}

void updateHeader(){
	setCursorPosition(0,0);
	resetColor();
	cout << "\u001b[30m\u001b[107m";

	string headerdata = "Loonix [cols: X][row: Y] :: Z";

	headerdata = replace_all(headerdata, "X", to_string(curx));
	headerdata = replace_all(headerdata, "Y", to_string(index + cury));
	headerdata = replace_all(headerdata, "Z", currentfile);

	if (openFiles.size() > 1){
		string buffer = " (X/Y)";
		
		buffer = replace_all(buffer, "X", to_string(fileIndex + 1));
		buffer = replace_all(buffer, "Y", to_string(openFiles.size()));
		
		headerdata += buffer;
	}

	if (hasEdited == true){
		headerdata += " *";
	}

	headerdata += "   ";

	cout << headerdata << "\u001b[0m";
}

void drawScreen(){
	for (int i = 0; i < viewport.size(); i++){
		cout << viewport[i] << endl;
	}
}

void showDebug(){
	string debuginfo = to_string(raw.size());
	debuginfo += "::";
	debuginfo += to_string(lines.size());
	drawBox(50, 20, 20, 10, "Debug info", debuginfo, true, true, "\u001b[4m\u001b[3m\u001b[38;5;213m");
	setCursorPosition(0,0);
}

void updateCursor(){
	int cursorAbsolute = index + cury;
	string currentline = raw[cursorAbsolute];
	string previousline = raw[index + prey];

	// get current cursor //
	char cursorChar;
	if (curx >= unilen(currentline)){
		cursorChar = ' ';
	}else{
		cursorChar = nonUniString(currentline).at(curx);//currentline.at(curx);
	}

	string previousChar = syntaxLine(previousline);//.at(prex);

	// draw over cursor //
	resetColor();
	setCursorPosition(0, prey);
	cout << previousChar << " ";

	cout << "\u001b[30m\u001b[107m";
	// draw new cursor //
	setCursorPosition(curx, cury);
	cout << cursorChar;

	prey = cury;
	prex = curx;
}

void updateCursorBlank(){
	int cursorAbsolute = index + cury;
	string previousline = raw[index + prey];

	// draw over cursor //
	cout << "\u001b[39;49m";
	setCursorPosition(prex, prey);
	cout << previousline.at(prex);

	cout << "\u001b[30m\u001b[107m";
	// draw new cursor //
	setCursorPosition(curx, cury);
	cout << " ";

	prey = cury;
	prex = curx;
}

void drawFromPoint(int line){
	resetColor();

	for (int i = 0; i < viewport.size(); i ++){
		if (i > line){
			setCursorPosition(0, i);
			cout << viewport[i];
		}
	}
}

void clearFromPoint(int line){
	resetColor();

	for (int i = 0; i < viewport.size(); i++){
		if (i > line){
			setCursorPosition(0, i);
			int x = 0;
			string y = "";
			while (x < viewport[i].size()){
				y += " ";
				x ++;
			}

			cout << y;
		}
	}
}

void renderBox(int x, int y, int w, int h){
	for (int i = 0; i < h; i ++){
		for (int b = 0; b < w; b ++){
			string currentline = raw[index + i + y];
			if (x + b <= currentline.size() - 1){
				setCursorPosition(b + x, i + y);
				cout << currentline.at(b + x);
			}
		}
	}
}

void refresh(){
	resetColor();

	clearFromPoint(0);
	updateViewport();
	drawFromPoint(0);
}

void newRefresh(){
	vector<string> oldViewport = rawViewport;
	updateViewport();
	vector<string> newViewport = rawViewport;
	int offset = 0;

	if (newViewport.size() != oldViewport.size()){
		if (newViewport.size() > oldViewport.size()){
			offset = newViewport.size() - oldViewport.size();
		}else{
			offset = oldViewport.size() - newViewport.size();
		}
	}

	for (int i = 1; i < newViewport.size() - offset; i++){
		int newline = unilen(newViewport[i]);
		int oldline = unilen(oldViewport[i]);

		int difference = 0;

		setCursorPosition(0, i);
		cout << viewport[i];

		if (newline >= oldline){
			difference = newline - oldline;
		}else{
			difference = oldline - newline;
		}

		for (int b = 0; b < difference; b++){
			cout << " ";
		}
	}

	if (offset > 0){
		for (int i = offset; i < newViewport.size(); i++){
			setCursorPosition(0, i);
			cout << syntaxLine(newViewport[i]);
		}
	}
}

