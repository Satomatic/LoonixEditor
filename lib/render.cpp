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

extern bool hasEdited;

void updateViewport(){
	viewport.clear();
	for (int i = 0; i < lines.size(); i++){
		if (i >= index && i <= index + screenHeight - 2){
			viewport.push_back(lines[i]);
		}
	}
}

void drawHeader(){
	setCursorPosition(0,0);
	resetColor();
	cout << "\u001b[30m\u001b[107m";

	string headerdata = "Loonix [cols: ";
	headerdata += to_string(curx);
	headerdata += "][row: ";
	headerdata += to_string(index + cury);
	headerdata += "] :: ";

	headerdata += currentfile;

	if (hasEdited == true){
		headerdata += " *";
	}

	headerdata += "   ";

	cout << fillX(headerdata);
}

void updateHeader(){
	setCursorPosition(0, 0);
	resetColor();
	cout << "\u001b[30m\u001b[107m";

	string headerdata = "Loonix [cols: ";
	headerdata += to_string(curx);
    headerdata += "][row: ";
    headerdata += to_string(index + cury);
    headerdata += "] :: ";

    headerdata += currentfile;

    if (hasEdited == true){
        headerdata += " *";
    }

    headerdata += "   ";

	cout << headerdata;
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
	string cursorChar = "";
	if (curx >= currentline.size()){
		cursorChar = " ";
	}else{
		cursorChar = currentline.at(curx);
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

/*
void clearFromPoint(int line){
	resetColor();

	for (int i = 0; i < viewport.size(); i++){
		if (i > line){
			setCursorPosition(0, i);
			int x = 0;
			string y = 0;
			while (x < raw[i].size()){
				y += " ";
				x++;
			}

			cout << y;
		}
	}
}
*/
