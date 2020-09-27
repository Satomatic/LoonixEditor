#include <iostream>
#include <signal.h>
#include <math.h>
#include <vector>
#include <string>
#include "lib/keyboard.cpp"
#include "lexer/main.cpp"
#include "lib/config.cpp"
#include "lib/render.cpp"
#include "lib/display.cpp"
#include "lib/diff.cpp"
#include "lib/options.cpp"
#include "lib/help.cpp"
#include "lib/file.cpp"
#include "lib/replace.cpp"
#include "lib/find.cpp"
#include "lib/todo.cpp"
#include "lib/jump.cpp"
#include "lib/command.cpp"

using namespace std;

// current file vectors //
vector <string> raw;
vector <string> lines;
vector <string> viewport;
vector <string> rawViewport;

// multi file vectors //
vector <vector<string>> openFiles = {}; // {"filename", "curx", "cury", "scroll", "has saved (0/1)"}
vector <vector<string>> fileMemory = {};

int fileIndex = 0;

int curx = 0;
int cury = 1;
int prex = 0;
int prey = 1;
int scroll = 0;

int startx = 0;
int starty = 0;
int endx = 0;
int endy = 0;

int screenHeight;
int screenWidth;

int XOffset = 1;

bool hasEdited = false;
bool insertMode = false;

HeaderDrop headerMessage;
DiffManager diffManager;
ConfigManager configManager;
ScrollBar scrollbar;

string currentfile = "";

int main(int argc, char** argv){
	// start up //
	signal(SIGINT,SIG_IGN);

    system("stty discard undef"); // re-enables cirtain blocked keys

	hideCursor();
	system("stty -ixon");
	clear();

	lines.push_back(" if you can see this, somethings gone wrong "); // reserve space for header

	updateScreenSize();

	// config //
	configManager.checkConfig();
	configManager.loadConfig();
	loadTheme(configManager.getSetting("theme", "default"));

	if (configManager.getValue("diff_enabled") != "1"){
		XOffset = 0;
	}

	if (argc >= 2){
		if (FileExists(argv[1]) == true){
			// Open file //
			loadFile(argv[1]);
			moveFileIntoMemory();
			drawScreen();
			drawHeader();
			updateCursor();
		}else{
			if (argc == 3){
				if ((string)argv[2] == "-c"){
					// Create new file and save //
					createFile(argv[1]);
					loadFile(argv[1]);
					moveFileIntoMemory();
				}else{
					// Create new file //
					newFile();
					currentfile = argv[1];
					hasEdited = true;
					moveFileIntoMemory();
				}
			}else{
				newFile();
				currentfile = argv[1];
				hasEdited = true;
				moveFileIntoMemory();
			}
			
			drawScreen();
			drawHeader();
			updateCursor();
		}
	}else{
		newFile();
		moveFileIntoMemory();
	}

	// Welcome Message //
	Box WelcomeMessage;
	WelcomeMessage.message = " Welcome to Loonix editor \\ \\";
	WelcomeMessage.message += "  \u001b[7m^N\u001b[0m New";
	WelcomeMessage.message += " \u001b[7m^O\u001b[0m Open";
	WelcomeMessage.message += " \u001b[7m^S\u001b[0m Save";
	WelcomeMessage.width = 26;
	WelcomeMessage.height = 3;
	WelcomeMessage.center = true;
	WelcomeMessage.draw();

	diffManager.init();

	scrollbar.x = screenWidth - 1;
	scrollbar.y = 1;
	scrollbar.height = screenHeight - 2;
	if (configManager.getValue("scrl_enabled") != "0")
		scrollbar.draw();
	
	// program loop //
	while (true){
		// header message logic //
		if (headerMessage.showing == true){
			if(headerMessage.count >= 5){
				headerMessage.undraw();
			}else{
				headerMessage.count ++;
				headerMessage.update();
			}
		}

		diffManager.drawDiffBar();
		
		// update scroll bar //
		scrollbar.size = lines.size() - 1;
		scrollbar.position = scroll;
		scrollbar.x = screenWidth - 1;
		scrollbar.height = screenHeight - 2;
		scrollbar.y = 1; 
		
		if (configManager.getValue("scrl_enabled") != "0")
			scrollbar.draw();
		updateHeader();
		drawFooter();

		updateCursor();		

		string key = getInput();

		// welcome message logic //
		if (WelcomeMessage.showing == true){
			WelcomeMessage.undraw();
			
			newRefresh();
			updateCursor;
		}

		// selection logic //
		if (key == "RightArrow" && startx > 0 || endx > 0){
			curx = endx - 1;
		}
		
		if (key == "LeftArrow" || key == "RightArrow" || key == "UpArrow" || key == "DownArrow"){
			startx = 0;
			starty = 0;
			endx = 0;
			endy = 0;
		}

		if (key == "DownArrow"){
			drawGuideLines();

			int moveIndex = 1;
			int moveCury = 0;
			
			if (configManager.getValue("lowc_enabled") == "1"){
				moveIndex = 10;
				moveCury = -10;
			}
			
			if (scroll + cury != raw.size() - 1){
				string nextline = raw[scroll + cury + 1];
				if (curx >= nextline.size()){
					curx = nextline.size();
				}

				if (cury == viewport.size() - 1){
					updateCursor();
					scroll += moveIndex;
					cury += moveCury;
					newRefresh();

					updateCursor();
				}else{
					cury ++;
					updateCursor();
				}
			}

		}else if (key == "UpArrow"){
			drawGuideLines();

			int moveIndex = 1;
			int moveCury = 0;

			if (configManager.getValue("lowc_enabled") == "1"){
				moveIndex = 10;
				moveCury = -10;
			}
		
			if (scroll + cury != 1){
				string previousline = raw[scroll + cury - 1];
				if (curx >= previousline.size()){
					curx = previousline.size();
				}
			}

			if (cury == 1){
				if (cury + scroll != 1){
					scroll -= moveIndex;
					cury -= moveCury;
					
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
					if (scroll + cury != 1){
						scroll --;
						refresh();
						curx = unilen(raw[scroll  + cury]);//.length();
					}
				}else{
					curx = unilen(raw[scroll + cury - 1]);//.length();
					cury --;
				}

			}else{
				if (curx >= 4){
					string currentline = raw[scroll + cury];
					if (currentline.substr(curx - 4, 4) == "    "){
						curx -= 4;
					}else{
						curx --;
					}
				}else{
					curx --;
				}
			}

			drawGuideLines();
			updateCursor();

		}else if (key == "RightArrow"){
			if (curx == unilen(raw[scroll + cury])){
				if (cury == viewport.size() - 1){
					if (scroll + cury != raw.size() - 1){
						curx = 0;
						scroll ++;
						refresh();
					}
				}else{
					curx = 0;
					cury ++;
					newRefresh();
				}

			}else{
				string currentline = raw[scroll + cury];
				if (currentline.substr(curx, 4) == "    "){
					curx += 4;
				}else{
					curx ++;
				}
			}

			drawGuideLines();
			updateCursor();

		}else if (key == "PGUP"){
			if (scroll - screenHeight <= 0){
				scroll = 0;
				cury = 1;
				curx = 0;
			}else{
				scroll -= screenHeight;
				cury = 1;
				curx = 0;
			}

			newRefresh();
			updateCursor();

		}else if (key == "PGDN"){
			if (lines.size() >= screenHeight - 1){
				scroll += screenHeight;
			
				if (testViewport() < screenHeight - 1){
					scroll = lines.size() - screenHeight + 1;
					cury = viewport.size() - 1;
				
				}else{
					cury = 1;
					curx = 0;
				}

				newRefresh();
				updateCursor();
			
			}else{
				cury = viewport.size() - 1;
				updateCursor();
			}

		}else if (key == "Insert"){
			if (insertMode == true){
				insertMode = false;
			}else{
				insertMode = true;
			}
			

		}else if (key == "Delete"){
			string currentline = raw[scroll + cury];
			string newline;
			vector<string> linesplit;
			
			if (currentline != ""){
				if (curx == currentline.size()){ // End of line
					newline = currentline.substr(0, currentline.size() - 1);
					curx --;
			
				}else if (currentline.size() == 1){
					newline = " ";
			
				}else if (curx == 0){
					newline = currentline.substr(1, currentline.size());
			
				}else{
					linesplit = splitIndex(currentline, curx);
				
					newline = linesplit[0] + linesplit[1].substr(1, linesplit[1].size());
				}
			
				// update raw and lines //
				raw[scroll + cury] = newline;
				lines[scroll + cury] = syntaxLine(newline);
			
				// update line //
				updateViewport();
				setCursorPosition(0, cury);
				cout << lines[scroll + cury] << "  ";
				
				updateCursor();
			}

		}else if (key == "Backspace" && curx == 0 && cury != 1){ // At beginning of line
			string currentline = raw[scroll + cury];
			string previousline = raw[scroll + cury - 1];
			lines[scroll + cury - 1] = syntaxLine(raw[scroll + cury - 1] + currentline);
			raw[scroll + cury - 1] = raw[scroll + cury - 1] + currentline;

			lines.erase(lines.begin() + scroll + cury);
			raw.erase(raw.begin() + scroll + cury);
			diffManager.removeLine(cury, scroll);

			refresh();
			drawGuideLines();

			curx = previousline.size();
			cury --;

			updateCursor();

			diffManager.updateCurrentLine();
			
			hasEdited = true;

		}else if (key == "Backspace" && curx != 0){
			string currentline = raw[scroll + cury];
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

			lines[scroll + cury] = syntaxLine(newline);
			raw[scroll + cury] = newline;
			updateViewport();

			resetColor();
			setCursorPosition(XOffset, cury);
			cout << newline << extend;
			updateCursor();

			diffManager.updateCurrentLine();

			hasEdited = true;

		}else if (key == "Return"){
			string currentline = raw[scroll + cury];
			if (curx == currentline.size()){
				lines.insert(lines.begin() + scroll + cury + 1, "");
				raw.insert(raw.begin() + scroll + cury + 1, "");
			}else{
				vector<string> linesplit = splitIndex(currentline, curx);
				lines[scroll + cury] = syntaxLine(linesplit[0]);
				lines.insert(lines.begin() + scroll + cury + 1, syntaxLine(linesplit[1]));
				raw[scroll + cury] = linesplit[0];
				raw.insert(raw.begin() + scroll + cury + 1, linesplit[1]);
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
					raw[scroll + cury + 1].insert(0, "    ");
					lines[scroll + cury + 1].insert(0, "    ");
				}

				curx = tabCount * 4;
			}
			
			diffManager.insertLine(cury, scroll);

			// move viewport down
			if (cury == screenHeight - 2){
				scroll ++;
			}else{
				cury ++;
			}

			newRefresh();
			updateCursor();

			hasEdited = true;

		}else if (key == "CTRLX"){
			// search for file in memory //
			int foundIndex;
			
			for (int i = 0; i < fileMemory.size(); i++){
				if (fileMemory[i][0] == currentfile){
					foundIndex = i;
				}
			}
			
			bool close = false;
			
			if (hasEdited == true){
				OptionDialog save;
				save.message = "Save changes?";
				save.items = {"Yes", "No", "Cancel"};
				save.centerText = true;
				save.draw();
				
				if (save.selected == 0){
					saveFile();
					close = true;
				
				}else if (save.selected == 1){
					close = true;
				}
			
				save.undraw();
			
			}else{
				close = true;
			}
			
			// close file //
			if (close == true){
				if (openFiles.size() == 1){
					break;
				}else{
					fileMemory.erase(fileMemory.begin() + foundIndex);
					diffManager.diff.erase(diffManager.diff.begin() + foundIndex);
					
					// search in open files //
					for (int i = 0; i < openFiles.size(); i++){
						if (openFiles[i][0] == currentfile){
							openFiles.erase(openFiles.begin() + i);
							break;
						}
					}
					
					if (fileIndex != 0){
						fileIndex --;
					}
					
					loadFileFromMemory(openFiles[fileIndex][0]);
					
					currentfile = openFiles[fileIndex][0];
					curx = stoi(openFiles[fileIndex][1]);
					cury = stoi(openFiles[fileIndex][2]);
					scroll = stoi(openFiles[fileIndex][3]);
					
					if (openFiles[fileIndex][4] == "1"){
						hasEdited = true;
					}else{
						hasEdited = false;
					}
					
					clear();
					updateViewport();
					newRefresh();
					updateCursor();
					drawHeader();
				}
			}
			
		}else if (key == "CTRLS"){
			if (currentfile == "" || currentfile.substr(0, 7) == "newfile"){
				saveAsFile();
			}else{
				saveFile();
			}

		}else if (key == "CTRLA"){
			saveAsFile();

			fileIndex = openFiles.size() - 1;
			updateHeader();

		}else if (key == "CTRLO"){
			moveFileIntoMemory();            
			int opened = openFileNewBuffer();

			clear();
			newRefresh();
			updateCursor();
			drawHeader();

			if (opened == 0){
				headerMessage.styling = "\u001b[38;5;124m";
				headerMessage.message = "File does not exist";
				headerMessage.draw();
				
			}else if (opened == 1){
				headerMessage.styling = "\u001b[0m";
				headerMessage.message = "File opened";
				headerMessage.draw();
			}
			
			diffManager.init();

		}else if (key == "CTRL-ALT-O"){
			openFileCurrentBuffer();
			
		}else if (key == "CTRLN"){
			string filename = "newfile";
			int count = 0;
			bool cont = false;
			
			// auto increment file //
			while (cont == false){
				bool found = false;
				for (int i = 0; i < fileMemory.size(); i++){
					if (fileMemory[i][0] == filename){
						found = true;
						break;
					}
				}
				
				if (found == true){
					count ++;
					filename = "newfile";
					filename += to_string(count);
				}else{
					setCursorPosition(60, 60);
					cout << filename;
					cont = true;
				}
			}
			
			// setup file memory //
			moveFileIntoMemory();
			createFileMemory(filename);     
			
			// reset cursor values //   
			fileIndex = openFiles.size() - 1;
			currentfile = filename;
			scroll = 0;
			cury = 0;
			curx = 0;
			
			loadFileFromMemory(filename);
			
			// reload editor //
			updateCursor();
			clear();
			updateCursor();
			drawHeader();

			diffManager.init();

		}else if (key == "CTRLU"){
			jumpLine jump;
			jump.draw();

		}else if (key == "CTRL/"){
			string commentString = getCommentString();
		
			if (raw[scroll + cury].size() >= commentString.size()){
				if (raw[scroll + cury].substr(0, commentString.size()) == multString(" ", commentString.size())){
					for (int i = 0; i < commentString.size(); i++){
						raw[scroll + cury][i] = commentString[i];
					}
				}else{
					raw[scroll + cury].insert(0, commentString);
					curx += commentString.size();
				}
			}else{
				raw[scroll + cury].insert(0, commentString);
				curx += commentString.size();
			}
			
			lines[scroll + cury] = syntaxLine(raw[scroll + cury]);
			
			if (cury + scroll < raw.size() - 1){
				cury ++;
				curx = 0;
			}
			
			newRefresh();
			updateCursor();

		}else if (key == "CTRL-UpArrow"){
			if (scroll + cury != 1){
				string nextline = raw[scroll + cury - 1];
				string currentline = raw[scroll + cury];

				raw[scroll + cury - 1] = currentline;
				raw[scroll + cury] = nextline;
				lines[scroll + cury - 1] = syntaxLine(currentline);
				lines[scroll + cury] = syntaxLine(nextline);
				
				// swap values in diff //
				int currentValue = diffManager.diff[fileIndex][scroll + cury - 1];
				int previousValue = diffManager.diff[fileIndex][scroll + cury - 2];
				
				diffManager.diff[fileIndex][scroll + cury - 1] = previousValue;
				diffManager.diff[fileIndex][scroll + cury - 2] = currentValue;

				cury --;
			}

			diffManager.drawDiffBar();
			newRefresh();
			updateCursor();

		}else if (key == "CTRL-DownArrow"){
			if (scroll + cury != raw.size() - 1){ // not at end of file
				string previousline = raw[scroll + cury + 1];
				string currentline = raw[scroll + cury];

				raw[scroll + cury + 1] = currentline;
				raw[scroll + cury] = previousline;
				lines[scroll + cury + 1] = syntaxLine(currentline);
				lines[scroll + cury ] = syntaxLine(previousline);
				
				// swap values in diff //
				int currentValue = diffManager.diff[fileIndex][scroll + cury - 1];
				int previousValue = diffManager.diff[fileIndex][scroll + cury];
				
				diffManager.diff[fileIndex][scroll + cury - 1] = previousValue;
				diffManager.diff[fileIndex][scroll + cury] = currentValue;
				
				cury ++;
			}

			diffManager.drawDiffBar();
			newRefresh();
			updateCursor();

		}else if (key == "CTRL-RightArrow"){
			int pos = curx;
			for (int i=curx; i < raw[scroll + cury].size(); i++){
				if (isAlpha(raw[scroll + cury].at(i)) != true){
					pos += (i - curx);
					break;
				}
			}

			if (raw[scroll + cury].substr(curx, 4) == "    "){
				curx += 4;
			
			}else{            
				if (pos > raw[scroll + cury].size() - 1 || curx == raw[scroll + cury].size()){
					if (cury == viewport.size() - 1){
						if (scroll + cury != raw.size() - 1){
							curx = 0;
							scroll ++;
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
				if (isAlpha(raw[scroll + cury].at(i)) != true){
					pos = i;
					break;
				}
			}

			if (curx >= 4 && raw[scroll + cury].substr(curx - 4, 4) == "    "){
				curx -= 4;
			}else{
				if (curx == 0){
					if (cury == 1){
						if (scroll + cury != 1){ // move viewport up
							scroll --;
							refresh();
							drawHeader();
							curx = raw[scroll + cury].length();
						}
					}else{ // move to previous line
						curx = raw[scroll + cury - 1].size();
						cury -= 1;
					}
				}else{ // move back
					curx = pos;
				}
			}

			updateCursor();

		}else if (key == "CTRLK"){
			if (returnSelection() != ""){
				int start = 0;
				int end = 0;
				
				// selection logic //
				if (endx < startx){
					start = endx;
					end = startx;
				}else{
					start = startx;
					end = endx;
					curx -= (end - start - 1);
				}
				
				// undraw line //
				setCursorPosition(0, cury);
				for (int i = 0; i < raw[cury + scroll].size(); i++){
					cout << " ";
				}
						
				// generate new line //
				string newline = raw[cury + scroll].erase(start, (end - start));
				raw[cury + scroll] = newline;
				lines[cury + scroll] = syntaxLine(newline);
				diffManager.removeLine(cury, scroll);
				
				startx = 0;
				starty = 0;
				endx = 0;
				endy = 0;
				
				// draw new line //
				setCursorPosition(0, cury);
				cout << newline;
				
				updateViewport();    
				newRefresh();
				updateCursor();

			}else if (raw.size() - 1 != 1){
				if (raw.size() - 1 == scroll + cury && cury == 1){ // end of file and at cury 1
					if (scroll != 0){
						lines.erase(lines.begin() + scroll + cury);
						raw.erase(lines.begin() + scroll + cury);
						diffManager.removeLine(cury, scroll);
						
						scroll --;
						
						if (raw[scroll + cury].size() < curx){
							curx = raw[scroll + cury].size();
						}
						
						newRefresh();
						updateCursor();
					}
					
				}else if (raw.size() - 1 == scroll + cury){ // end of file and more than cury 1
					lines.erase(lines.begin() + scroll + cury);
					raw.erase(raw.begin() + scroll + cury);
					diffManager.removeLine(cury, scroll);
					
					cury --;
					
					if (raw[scroll + cury].size() < curx){
						curx = raw[scroll + cury].size();
					}
					
					updateCursor();
					clearFromPoint(cury - 1);
					newRefresh();
					updateCursor();
				
				}else{
					lines.erase(lines.begin() + scroll + cury);
					raw.erase(raw.begin() + scroll + cury);
					diffManager.removeLine(cury, scroll);

					if (raw[scroll + cury].size() < curx){
						curx = raw[scroll + cury].size();
					}

					clearFromPoint(cury - 1);
					newRefresh();
					updateCursor();
				}
			}

			hasEdited = true;

		}else if (key == "CTRLL"){
			string currentline = raw[scroll + cury];

			string info = "line: [x/y] col: [z/p]";

			info = replace_all(info, "x", to_string(scroll + cury));
			info = replace_all(info, "y", to_string(raw.size() - 1));
			info = replace_all(info, "z", to_string(curx));
			info = replace_all(info, "p", to_string(currentline.size()));

			headerMessage.message = info;
			headerMessage.styling = "";
			headerMessage.draw();

		}else if (key == "F1"){
			helpMenu help;
			help.init();

			drawFromPoint(0);
			drawHeader();
			updateCursor();
			
		}else if (key == "F5"){
			OptionMenu options;
			options.init();
		
		}else if (key == "F6"){
			if (configManager.getValue("diff_enabled") == "1"){
				configManager.putValue("diff_enabled", "0");
				XOffset = 0;
				
				headerMessage.message = "Disabled Diff Bar";
			}else{
				configManager.putValue("diff_enabled", "1");
				XOffset = 1;
				
				headerMessage.message = "Enabled Diff Bar";
			}
			
			// undraw side bar //
			for (int i = 0; i < screenHeight; i++){
				setCursorPosition(0, i + 1);
				cout << " ";
			}
			
			newRefresh();
			updateCursor();

			headerMessage.styling = "\u001b[0m";
			headerMessage.draw();

		}else if (key == "F7"){
			if (configManager.getValue("lowc_enabled") == "1"){
				configManager.putValue("lowc_enabled", "0");
				
				headerMessage.message = "Enabled Smooth Scrolling";
			}else{
				configManager.putValue("lowc_enabled", "1");
				
				headerMessage.message = "Disabled Smooth Scrolling";
			}
			
			headerMessage.styling = "\u001b[0m";
			headerMessage.draw();

		}else if (key == "F8"){
			if (configManager.getValue("line_enabled") == "1"){
				configManager.putValue("line_enabled", "0");
			
				headerMessage.message = "Disabled intentation guide";
			}else{
				configManager.putValue("line_enabled", "1");
				
				headerMessage.message = "Enabled intentation guide";
			}
			
			newRefresh();
			
			headerMessage.styling = "\u001b[0m";
			headerMessage.draw();

		}else if (key == "CTRLP"){
			CommandLine cmd;
			cmd.init();
		
		}else if (key == "CTRLF"){
			Find findP;
			findP.input = returnSelection();
			findP.draw();
		
		}else if (key == "CTRLR"){
			NewReplace replace;
			replace.replace = returnSelection();
			replace.init();

		}else if (key == "CTRLT"){
			Todo todo;
			todo.init();

		}else if (key == "CTRLD"){
			raw.insert(raw.begin() + cury + scroll, raw[cury + scroll]);
			lines.insert(lines.begin() + cury + scroll, syntaxLine(raw[cury + scroll]));
			
			if (cury == screenHeight - 2){
				scroll ++;
			}else{
				cury ++;
			}
			
			diffManager.insertLine(cury, scroll);
			
			newRefresh();
			updateCursor();

		}else if (key == "ALT-UpArrow"){
			// move screen //
			if (scroll > 0){
				scroll --;
			
				// handel cursor //
				if (cury != screenHeight - 2){
					cury ++;
				}
			}
			
			newRefresh();
			updateCursor();
		
		}else if (key == "ALT-DownArrow"){
			if (scroll < raw.size() - screenHeight + 1){
				scroll ++;
			
				if (cury > 1){
					cury --;
				}
			}
			
			newRefresh();
			updateCursor();

		}else if (key == "CTRL-ALT-RightArrow" || key == "ALT-RightArrow"){
			if (openFiles.size() == 1){
				headerMessage.message = "No other files open";
				headerMessage.styling = "\u001b[38;5;124m";
				headerMessage.draw();
			}else{
				
				if (fileIndex == openFiles.size() - 1){
					fileIndex = 0;
				}else{
					fileIndex ++;
				}
			
				clearText();

				moveFileIntoMemory();
			
				loadFileFromMemory(openFiles[fileIndex][0]);
				curx = stoi(openFiles[fileIndex][1]);
				cury = stoi(openFiles[fileIndex][2]);
				scroll = stoi(openFiles[fileIndex][3]);
				updateCursor();
		
				currentfile = openFiles[fileIndex][0];
			
				updateViewport();
				drawScreen();
				drawHeader();
				drawGuideLines();
				updateCursor();
			}

		}else if (key == "CTRL-ALT-LeftArrow" || key == "ALT-LeftArrow"){
			if (openFiles.size() == 1){
				headerMessage.message = "No other files open";
				headerMessage.styling = "\u001b[38;5;124m";
				headerMessage.draw();
			}else{
				if (fileIndex == 0){
					fileIndex = openFiles.size() - 1;
				}else{
					fileIndex --;
				}
				
				clearText();

				moveFileIntoMemory();

				loadFileFromMemory(openFiles[fileIndex][0]);
				curx = stoi(openFiles[fileIndex][1]);
				cury = stoi(openFiles[fileIndex][2]);
				scroll = stoi(openFiles[fileIndex][3]);

				currentfile = openFiles[fileIndex][0];

				updateViewport();
				drawScreen();
				drawHeader();
				drawGuideLines();
				updateCursor();
			}

		}else if (key == "SHIFT-RightArrow"){
			// calculate variables //
			if (startx == 0){
				startx = curx;
				starty = cury;
				endx = curx + 1;
				endy = cury;
			}
			
			if (endx < raw[starty + scroll].size()){
				endx ++;
			}
			
			newRefresh();
			drawSelection();
		
		}else if (key == "SHIFT-LeftArrow"){
			if (startx == 0 || endx == 0){
				if (curx > 0){
					startx = curx;
					starty = cury;
					endx = curx - 1;
					endy = cury;
				}
			}else{
				if (endx > 0){
					endx --;
				}
			}
			
			newRefresh();
			
			drawSelection();

		}else if (key == "TAB"){
			string currentline = raw[scroll + cury];

			currentline.insert(curx, "    ");
			lines[scroll + cury] = syntaxLine(currentline);
			raw[scroll + cury] = currentline;
			updateViewport();

			// make update line function at some point //
			resetColor();
			setCursorPosition(0, cury);
			cout << currentline;

			curx += 4;
			updateCursor();

			hasEdited = true;

		}else if (key == "/" && raw[scroll + cury].size() > curx + 1 && raw[scroll + cury].substr(curx + 1, 1) == " "){
			raw[scroll + cury][curx] = '/';
			lines[scroll + cury] = syntaxLine(raw[scroll + cury]);
				
			updateViewport();
			curx ++;
				
			resetColor();
			setCursorPosition(0, cury);
			cout << lines[scroll + cury];
			updateCursor();
			
		}else{
			if (key.size() == 1){
				string currentline = raw[scroll + cury];
				string newline = "";

				if (curx == currentline.size()){
					newline = currentline + key;
				}else{
					if (insertMode == true){
						vector<string> linesplit = splitIndex(currentline, curx);
						newline = linesplit[0];
						newline += key;
						newline += linesplit[1].substr(1, linesplit[1].size());
					}else{
						vector<string> linesplit = splitIndex(currentline, curx);
						newline = linesplit[0] + key + linesplit[1];
					}
				}
				
				lines[scroll + cury] = syntaxLine(newline);
				raw[scroll + cury] = newline;
				updateViewport();
				curx ++;

				resetColor();
				setCursorPosition(XOffset, cury);
				cout << newline;
				updateCursor();
			}

			diffManager.updateLine(cury + scroll - 1, 1);
			diffManager.drawDiffBar();
			
			hasEdited = true;
		}

		checkScreenSize();

		int value = 0;
		string currentline = raw[scroll + cury];
		bool comment = false;
		bool stringmode = false;
		
		vector<char> brackets = {'(', '{', '[', '<'};
		vector<char> closing = {')', '}', ']', '>'};
		char bracket;
		char closer;
		
		for (int i = 0; i < brackets.size(); i++){
			if (currentline[curx] == brackets[i]){
				bracket = brackets[i];
				closer = closing[i];
		
				// check for comments
				for (int i = 0; i < curx; i++){
					if (currentline.substr(i, 2) == "//"){
						comment = true;
					}
				}
			
				for (int i = curx; i < currentline.size(); i++){
					if (currentline[i] == bracket){
						value ++;
						
					}else if (currentline[i] == closer){
						value --;
					}
			
					if (value == 0){
						setCursorPosition(XOffset + i, cury);
						cout << "\u001b[0m";
						if (comment == true){
							cout << "\u001b[38;5;242m";
						}else{
							cout << "\u001b[97m";
						}
						cout << "\u001b[4m" << closer;
						break;
					}
				}
			}
		}
	}

	setCursorPosition(0,0);
	clear();
	showCursor();
}
