#include <iostream>
#include <signal.h>
#include <math.h>
#include <vector>
#include <string>
#include "lexer/main.cpp"
#include "lib/keyboard.cpp"
#include "lib/display.cpp"
#include "lib/render.cpp"
#include "lib/replace.cpp"
#include "lib/help.cpp"
#include "lib/file.cpp"
#include "lib/find.cpp"
#include "lib/todo.cpp"
#include "lib/jump.cpp"

using namespace std;

// global variables //
vector <string> raw;
vector <string> lines;
vector <string> viewport;
vector <string> rawViewport;

int curx = 0;
int cury = 1;
int prex = 0;
int prey = 1;
int index = 0;

int screenHeight;
int screenWidth;

bool hasEdited = false;

HeaderDrop headerMessage;

string currentfile = "";

Box infoBox(string title, string message){
	Box MessageBox;
	MessageBox.title = title;
	MessageBox.width = message.size();
	MessageBox.height = 1;
	MessageBox.message = message;
	MessageBox.center = true;

	return MessageBox;
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
		if (FileExists(argv[1]) == true){
			loadFile(argv[1]);
			drawScreen();
			drawHeader();
			updateCursor();
		}else{
			newFile();
		}
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

		if (headerMessage.showing == true){
			if(headerMessage.count >= 5){
				headerMessage.undraw();
			}else{
				headerMessage.count ++;
			}
		}

		if (key == "DownArrow"){
			if (index + cury != raw.size() - 1){
				string nextline = raw[index + cury + 1];
				if (curx >= nextline.size()){
					curx = nextline.size();
				}

				if (cury == viewport.size() - 1){
					updateCursor();
					index ++;
					//refresh();
					//drawHeader();
					newRefresh();

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
					newRefresh();
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
						refresh();
						drawHeader();
						curx = unilen(raw[index  + cury]);//.length();
					}
				}else{
					curx = unilen(raw[index + cury - 1]);//.length();
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
			if (curx == unilen(raw[index + cury])){
				if (cury == viewport.size() - 1){
					if (index + cury != raw.size() - 1){
						curx = 0;
						index ++;
						refresh();
						drawHeader();
					}
				}else{
					curx = 0;
					cury ++;
					refresh();
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

		}else if (key == "PGUP"){
			if (index - screenHeight <= 0){
				index = 0;
				cury = 1;
				curx = 0;
			}else{
				index -= screenHeight;
				cury = 1;
				curx = 0;
			}

			newRefresh();
			updateCursor();
			drawHeader();

		}else if (key == "PGDN"){
			if (lines.size() >= screenHeight - 1){
				index += screenHeight;
			
				if (testViewport() < screenHeight - 1){
					index = lines.size() - screenHeight + 1;
					cury = viewport.size() - 1;
				
				}else{
					cury = 1;
					curx = 0;
				}

				newRefresh();
				updateCursor();
				drawHeader();
			
			}else{
				cury = viewport.size() - 1;
				updateCursor();
			}

		}else if (key == "Backspace" && curx == 0 && cury != 1){ // At beginning of line
			string currentline = raw[index + cury];
			string previousline = raw[index + cury - 1];
			lines[index + cury - 1] = syntaxLine(raw[index + cury - 1] + currentline);
			raw[index + cury - 1] = raw[index + cury - 1] + currentline;

			lines.erase(lines.begin() + index + cury);
			raw.erase(raw.begin() + index + cury);

			refresh();
			drawHeader();

			curx = previousline.size();
			cury --;

			updateCursor();

			hasEdited = true;

		}else if (key == "Backspace" && curx != 0){
			string currentline = raw[index + cury];
			string newline = "";
			string extend = "";

			if (curx >= 4 && currentline.substr(curx - 4, 4) == "    "){ // Tab backspace
				vector<string> linesplit = splitIndex(currentline, curx);
				newline = linesplit[0].substr(0, curx - 4);

				if (linesplit.size() == 2){
					newline += linesplit[1];
				}

				curx -= 4;
				extend = "     ";
				
			}else if (currentline.substr(curx - 1, 1) == "/" && currentline.size() >= curx + 2  && currentline.substr(curx, 2) == "  "){
				vector<string> linesplit = splitIndex(currentline, curx);
				newline = linesplit[0].substr(0, curx - 1);
				
				newline += " ";
				newline += linesplit[1];
				
				curx --;
				extend = "  ";
			
			}else{
				if (curx == currentline.size()){ // end of line
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

			// auto indent
			if (curx > 0){
				int spaceCount = 0;
				for (int i = 0; i < currentline.size(); i++){
					char chara = currentline.at(i);
					if (chara == ' '){
						spaceCount ++;
					}else{
						break;
					}
				}

				int tabCount = floor(spaceCount / 4);

				for (int i = 0; i < tabCount; i++){
					raw[index + cury + 1].insert(0, "    ");
					lines[index + cury + 1].insert(0, "    ");
				}

				curx = tabCount * 4;
			}

			// move viewport down
			if (cury == screenHeight - 2){
				index ++;
			}else{
				cury ++;
			}

			refresh();
			drawHeader();
			updateCursor();

			hasEdited = true;

		}else if (key == "CTRLX"){
			if (hasEdited == true){
				OptionDialog save;
				save.message = "Save changes?";
				save.items = {"Yes", "No", "Cancel"};
				save.centerText = true;
				save.draw();
				
				if (save.selected == 0){
					saveFile();
					break;
				
				}else if (save.selected == 1){
					break;
				}
			
			}else{
				break;
			}
			
		}else if (key == "CTRLS"){
			if (currentfile == "" || currentfile == "newfile"){
				saveAsFile();
			}else{
				saveFile();
			}

		}else if (key == "CTRLA"){
			saveAsFile();

		}else if (key == "CTRLO"){
			if (hasEdited == true){
				OptionDialog save;
				save.message = "Save changes?";
				save.items = {"Yes", "No", "Cancel"};
				save.centerText = true;
				save.draw();
				
				if (save.selected == 0){
					saveFile();
					openFile();
				
				}else if (save.selected == 1){
					openFile();
				}

				refresh();
				updateCursor();
				drawHeader();
		
			}else{
				openFile();
			}
			
		}else if (key == "CTRLN"){
			if (hasEdited == true){
				OptionDialog save;
				save.message = "Save changes?";
				save.items = {"Yes", "No", "Cancel"};
				save.centerText = true;
				save.draw();
				
				if (save.selected == 0){
					saveFile();
					newFile();
					
				}else if (save.selected == 1){
					newFile();
				}
			
			}else{
				newFile();
			}

			
		}else if (key == "CTRLH"){
			filemanager fm;
			fm.width = 20;
			fm.title = "Epic";
			fm.draw();
			
		}else if (key == "CTRLU"){
			jumpLine jump;
			jump.draw();
			
		}else if (key == "CTRL/"){
			if (raw[index + cury].size() >= 2){
				if (raw[index + cury].substr(0, 2) == "  "){
					raw[index + cury][0] = '/';
					raw[index + cury][1] = '/';
				}else{
					raw[index + cury].insert(0, "//");
					curx += 2;
				}
			}else{
				raw[index + cury].insert(0, "//");
				curx += 2;
			}
			
			lines[index + cury] = syntaxLine(raw[index + cury]);
			
			newRefresh();
			updateCursor();
			drawHeader();

		}else if (key == "CTRL-UpArrow"){
			if (index + cury != 1){
				string nextline = raw[index + cury - 1];
				string currentline = raw[index + cury];

				raw[index + cury - 1] = currentline;
				raw[index + cury] = nextline;
				lines[index + cury - 1] = syntaxLine(currentline);
				lines[index + cury] = syntaxLine(nextline);

				cury --;
			}

			refresh();
			drawHeader();
			updateCursor();

		}else if (key == "CTRL-DownArrow"){
			if (index + cury != raw.size() - 1){ // not at end of file
				string previousline = raw[index + cury + 1];
				string currentline = raw[index + cury];

				raw[index + cury + 1] = currentline;
				raw[index + cury] = previousline;
				lines[index + cury + 1] = syntaxLine(currentline);
				lines[index + cury ] = syntaxLine(previousline);

				cury ++;
			}

			refresh();
			drawHeader();
			updateCursor();

		}else if (key == "CTRL-RightArrow"){
			int pos = curx;
			for (int i=0; i < raw[index + cury].size(); i++){
				if (i > curx){
					if (isAlpha(raw[index + cury].at(i)) != true){
						pos += (i - curx);
						break;
					}
				}
			}

			if (raw[index + cury].substr(curx, 4) == "    "){
				curx += 4;
			
			}else{            
				if (pos > raw[index + cury].size() - 1 || curx == raw[index + cury].size()){
					if (cury == viewport.size() - 1){
						if (index + cury != raw.size() - 1){
							curx = 0;
							index ++;
							refresh();
							drawHeader();
						}
					}else{
						curx = 0;
						cury += 1;
					}
				}else{
					curx = pos + 1;
				}
			}
			
			updateCursor();
			
		}else if (key == "CTRL-LeftArrow"){
			int pos = 0;
			for (int i=curx - 1; i > 0; i--){
				if (isAlpha(raw[index + cury].at(i)) != true){
					pos = i;
					break;
				}
			}

			if (curx >= 4 && raw[index + cury].substr(curx - 4, 4) == "    "){
				curx -= 4;
			}else{
				if (curx == 0){
					if (cury == 1){
						if (index + cury != 1){ // move viewport up
							index --;
							refresh();
							drawHeader();
							curx = raw[index + cury].length();
						}
					}else{ // move to previous line
						curx = raw[index + cury - 1].size();
						cury -= 1;
					}
				}else{ // move back
					curx = pos;
				}
			}

			updateCursor();

		}else if (key == "CTRLK"){
			lines.erase(lines.begin() + index + cury);
			raw.erase(raw.begin() + index + cury);

			if (raw[index + cury].size() < curx){
				curx = raw[index + cury].size();
			}

			refresh();
			drawHeader();
			updateCursor();

		}else if (key == "CTRLL"){
			string currentline = raw[index + cury];

			string info = "line: [x/y] col: [z/p]";

			info = replace_all(info, "x", to_string(index + cury));
			info = replace_all(info, "y", to_string(raw.size() - 1));
			info = replace_all(info, "z", to_string(curx));
			info = replace_all(info, "p", to_string(currentline.size()));

			headerMessage.message = info;
			headerMessage.styling = "";
			headerMessage.draw();

		}else if (key == "F1"){
			helpMenu help;
			help.draw();

			clearFromPoint(0);
			drawFromPoint(0);
			drawHeader();
			updateCursor();

		}else if (key == "CTRLF"){
			Find findP;
			findP.draw();

		}else if (key == "CTRLR"){
			Replace replaceP; // replaceP = replacePanel
			replaceP.draw();

		}else if (key == "CTRLT"){
			Todo todoP;
			todoP.draw();

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

		}else if (key == "/" && raw[index + cury].size() > curx + 1 && raw[index + cury].substr(curx + 1, 1) == " "){
			raw[index + cury][curx] = '/';
			lines[index + cury] = syntaxLine(raw[index + cury]);
				
			updateViewport();
			curx ++;
				
			resetColor();
			setCursorPosition(0, cury);
			cout << lines[index + cury];
			updateCursor();
			
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

			drawFromPoint(0);
			updateCursor();
		}
	}

	setCursorPosition(0,0);
	clear();
	system("setterm -cursor on");
}
