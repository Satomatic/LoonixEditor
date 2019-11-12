#include <iostream>
#include <signal.h>
#include <vector>
#include <string>
#include "lexer/main.cpp"
#include "lib/keyboard.cpp"
#include "lib/display.cpp"
#include "lib/render.cpp"
#include "lib/help.cpp"
#include "lib/file.cpp"

using namespace std;

// global variables //
vector <string> raw;
vector <string> lines;
vector <string> viewport;

int curx = 0;
int cury = 1;
int prex = 0;
int prey = 1;
int index = 0;

int screenHeight;
int screenWidth;

bool hasEdited = false;

string currentfile = "";

void infoBox(string title, string message){
	Box MessageBox;
	MessageBox.title = title;
	MessageBox.width = message.size();
	MessageBox.height = 1;
	MessageBox.message = message;
	MessageBox.center = true;
	MessageBox.draw();
}

int main(int argc, char** argv){
	// start up //
	signal(SIGINT,SIG_IGN);

	system("setterm -cursor off");
	system("stty -ixon");
	clear();

	lines.push_back(" if you can see this, somethings gone wrong "); // reserve space for header

	updateScreenSize();

	if (argc == 2){
		loadFile(argv[1]);
		drawScreen();
		drawHeader();
		updateCursor();
	}else{
		newFile();
	}

	// Welcome Message //
	Box WelcomeMessage;
	WelcomeMessage.message = " Welcome to Loonix editor \\ \\";
	WelcomeMessage.message += "  \u001b[30;107m^N\u001b[0m New";
	WelcomeMessage.message += " \u001b[30;107m^O\u001b[0m Open";
	WelcomeMessage.message += " \u001b[30;107m^S\u001b[0m Save";
	WelcomeMessage.width = 26;
	WelcomeMessage.height = 3;
	WelcomeMessage.center = true;
	WelcomeMessage.draw();

	// program loop //
	while (true){
		updateHeader();

		// keyboard stuff //
		string key = getInput();

		if (key == "DownArrow"){
			if (index + cury != raw.size() - 1){
				string nextline = raw[index + cury + 1];
				if (curx >= nextline.size()){
					curx = nextline.size();
				}

				if (cury == viewport.size() - 1){
					index ++;
					updateViewport();
					clear();
					drawScreen();
					drawHeader();

					updateCursor();
				}else{
					cury ++;
					updateCursor();
				}
			}

		}else if (key == "UpArrow"){
			if (index + cury != 1){
				string previousline = raw[index + cury - 1];
				if (curx >= previousline.size()){
					curx = previousline.size();
				}
			}

			if (cury == 1){
				if (cury + index != 1){
					index --;
					updateViewport();
					clear();
					drawScreen();
					drawHeader();
					updateCursor();
				}
			}else{
				cury --;
				updateCursor();
			}

		}else if (key == "LeftArrow"){
			if (curx == 0){
				if (cury == 1){
					if (index + cury != 1){
						index --;
						updateViewport();
						clear();
						drawScreen();
						drawHeader();
						curx = raw[index  + cury].length();
					}
				}else{
					curx = raw[index + cury - 1].length();
					cury --;
				}

			}else{
				if (curx >= 4){
					string currentline = raw[index + cury];
					if (currentline.substr(curx - 4, 4) == "    "){
						curx -= 4;
					}else{
						curx --;
					}
				}else{
					curx --;
				}
			}

			updateCursor();

		}else if (key == "RightArrow"){
			if (curx == raw[index + cury].size()){
				if (cury == viewport.size()){
					if (index + cury != raw.size()){
						curx = 0;
						index ++;
						updateViewport();
						clear();
						drawScreen();
						drawHeader();
					}
				}else{
					curx = 0;
					cury ++;
					clear();
					drawScreen();
					drawHeader();
				}

			}else{
				string currentline = raw[index + cury];
				if (currentline.substr(curx, 4) == "    "){
					curx += 4;
				}else{
					curx ++;
				}
			}

			updateCursor();

		}else if (key == "Backspace" && curx == 0 && cury != 1){ // fix
			string currentline = raw[index + cury];
			string previousline = raw[index + cury - 1];
			lines[index + cury - 1] = syntaxLine(lines[index + cury - 1] + currentline);
			raw[index + cury - 1] = raw[index + cury - 1] + currentline;

			lines.erase(lines.begin() + index + cury);
			raw.erase(raw.begin() + index + cury);

			updateViewport();
			clear();
			drawScreen();
			drawHeader();

			curx = previousline.size();
			cury --;

			updateCursor();

			hasEdited = true;

		}else if (key == "Backspace" && curx != 0){
			string currentline = raw[index + cury];
			string newline = "";
			string extend = "";

			if (curx >= 4 && currentline.substr(curx - 4, 4) == "    "){
				vector<string> linesplit = splitIndex(currentline, curx);
				newline = linesplit[0].substr(0, curx - 4);
				newline += linesplit[1];
				curx -= 4;
				extend = "     ";
			}else{
				if (curx == currentline.size()){
					newline = currentline.substr(0, currentline.size() - 1);
				}else{
					vector<string> linesplit = splitIndex(currentline, curx);
					string beforePoint = linesplit[0].substr(0, linesplit[0].size() - 1);

					newline = beforePoint + linesplit[1];
				}
				extend = "  ";
				curx --;
			}

			lines[index + cury] = syntaxLine(newline);
			raw[index + cury] = newline;
			updateViewport();

			resetColor();
			setCursorPosition(0, cury);
			cout << newline << extend;
			updateCursor();

			hasEdited = true;

		}else if (key == "Return"){
			string currentline = raw[index + cury];
			if (curx == currentline.size()){
				lines.insert(lines.begin() + index + cury + 1, "");
				raw.insert(raw.begin() + index + cury + 1, "");
			}else{
				vector<string> linesplit = splitIndex(currentline, curx);
				lines[index + cury] = syntaxLine(linesplit[0]);
				lines.insert(lines.begin() + index + cury + 1, syntaxLine(linesplit[1]));
				raw[index + cury] = linesplit[0];
				raw.insert(raw.begin() + index + cury + 1, linesplit[1]);
			}

			updateViewport();
			clear();
			drawScreen();
			drawHeader();
			curx = 0;
			cury ++;
			updateCursor();

			hasEdited = true;

		}else if (key == "CTRLX"){
			break;

		}else if (key == "CTRLS"){
			if (currentfile == "" || currentfile == "newfile"){
				saveAsFile();
			}else{
				saveFile();
			}

		}else if (key == "CTRLA"){
			saveAsFile();

		}else if (key == "CTRLO"){
			openFile();

		}else if (key == "CTRLN"){
			newFile();

		}else if (key == "CTRLH"){
			helpMenu help;
			help.draw();

			clear();
			drawScreen();
			drawHeader();
			updateCursor();

		}else if (key == "TAB"){
			string currentline = raw[index + cury];

			currentline.insert(curx, "    ");
			lines[index + cury] = syntaxLine(currentline);
			raw[index + cury] = currentline;
			updateViewport();

			// make update line function at some point //
			resetColor();
			setCursorPosition(0, cury);
			cout << currentline;

			curx += 4;
			updateCursor();

			hasEdited = true;

		}else{
			if (key.size() == 1){
				string currentline = raw[index + cury];
				string newline = "";

				if (curx == currentline.size()){
					newline = currentline + key;
				}else{
					vector<string> linesplit = splitIndex(currentline, curx);
					newline = linesplit[0] + key + linesplit[1];
				}
				lines[index + cury] = syntaxLine(newline);
				raw[index + cury] = newline;
				updateViewport();
				curx ++;

				resetColor();
				setCursorPosition(0, cury);
				cout << newline;
				updateCursor();
			}

			hasEdited = true;
		}

		checkScreenSize();

		if (WelcomeMessage.showing == true){
			WelcomeMessage.undraw();
		}
	}

	setCursorPosition(0,0);
	clear();
	system("setterm -cursor on");
}
