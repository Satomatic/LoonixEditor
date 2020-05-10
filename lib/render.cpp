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

extern int startx;
extern int starty;
extern int endx;
extern int endy;

extern int XOffset;

extern string currentfile;
extern vector<string> raw;
extern vector<string> lines;
extern vector<string> viewport;
extern vector<string> rawViewport;

extern vector<vector<string>> openFiles;
extern int fileIndex;

extern bool hasEdited;
extern bool insertMode;

void drawGuideLines();
void drawGuideLinesC(int pos);

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
			if (unilen(raw[i]) >= screenWidth - 1){
				viewport.push_back(lines[i].substr(0, screenWidth - 2) + "$");
				rawViewport.push_back(raw[i].substr(0, screenWidth - 2) + "$");
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

	cout << fillX(headerdata);

	if (insertMode == true){
		setCursorPosition(screenWidth - 8, 0);
		cout << "[insert]";
	}else{
		setCursorPosition(screenWidth - 8, 0);
		cout << "        ";
	}

	cout << "\u001b[0m";
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

	cout << headerdata;

	if (insertMode == true){
		setCursorPosition(screenWidth - 8, 0);
		cout << "[insert]";
	}else{
		setCursorPosition(screenWidth - 8, 0);
		cout << "        ";
	}

	cout << "\u001b[0m";
}

void drawFooter(){
	setCursorPosition(0, screenHeight - 1);
	cout << "\u001b[0m" << "\u001b[30;107m" << fillX("F1 Help  ^X Exit") << "\u001b[0m";
}

void drawScreen(){
	for (int i = 0; i < viewport.size(); i++){
		setCursorPosition(XOffset, 0 + i);
		cout << viewport[i];
	}
}

void updateCursor(){
	int cursorAbsolute = index + cury;
	string currentline = raw[cursorAbsolute];
	string previousline = raw[index + prey];
	string previousChar = "";


	// get current cursor //
	char cursorChar;
	if (curx >= unilen(currentline)){
		cursorChar = ' ';
	}else{
		cursorChar = nonUniString(currentline).at(curx);
	}

	if (index + prey <= raw.size()){
		previousChar = syntaxLine(previousline);
	}

	// draw over cursor //
	cout << "\u001b[0m";
	setCursorPosition(XOffset, prey);
	cout << previousChar << " ";
	
	if (prex > previousline.size()){
		setCursorPosition(XOffset + prex, prey);
		cout << " ";
	}

	drawGuideLinesC(prey);
	drawGuideLinesC(cury);

	// draw new cursor //
	cout << "\u001b[30m\u001b[107m";
	setCursorPosition(curx + XOffset, cury);
	cout << cursorChar;

	
	prey = cury;
	prex = curx;
}

void updateLine(){
	string oldLine = rawViewport[cury];
	updateViewport();
	string newLine = rawViewport[cury];
	int difference;

	if (oldLine.size() < newLine.size()){
		difference = newLine.size() - oldLine.size();
	}else{
		difference = oldLine.size() - newLine.size();
	}

	setCursorPosition(XOffset, cury);
	cout << lines[index + cury];

	for (int i = 0; i < difference; i++){
		cout << " ";
	}
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
			setCursorPosition(XOffset, i);
			cout << viewport[i];
		}
	}
}

void clearFromPoint(int line){
	resetColor();

	for (int i = 0; i < viewport.size(); i++){
		if (i > line){
			setCursorPosition(XOffset, i);
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

		setCursorPosition(XOffset, i);
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
			setCursorPosition(XOffset, i);
			cout << syntaxLine(newViewport[i]);
		}
	}
	
	drawGuideLines();
}

void drawGuideLines(){
	cout << "\u001b[0m";

	for (int y = 1; y < rawViewport.size(); y++){
		if (rawViewport[y].size() == 0){
			if (rawViewport[y + 1].substr(0, 8) == "        " ||
				rawViewport[y - 1].substr(0, 8) == "        "){
					
				setCursorPosition(XOffset, y);
				cout << "\u001b[38;5;238m    |";
				
				for (int x = 0; x < screenWidth; x++){
					if (isSpecial(rawViewport[y - 1][x])){
						break;
					
					}else if (isAlpha(rawViewport[y + 1][x])){
						break;
					
					}else{
						if ((x & 3) == 0 && x != 0){
							setCursorPosition(XOffset + x, y);
							cout << "\u001b[38;5;238m|";
						}
					}
				}
			}
		}
		
		for (int x = 0; x < rawViewport[y].size() + 1; x++){
			if ((x & 3) == 0){
				if (rawViewport[y][x] != ' '){
					break;
				}
				
				setCursorPosition(XOffset + x, y);
				if (rawViewport[y][x] == ' ' && x > 0 && rawViewport[y].substr(x, 4) == "    "){   
					cout << "\u001b[38;5;238m|";
				}
			}
		}
	}
	
	// highlight current line within scope //
	for (int y = cury; y < rawViewport.size(); y++){
		if (curx == 0){break;}
		
		if (rawViewport[y][curx] == ' ' && (curx & 3) == 0 || rawViewport[y].size() == 0){
			setCursorPosition(XOffset + curx, y);
			cout << "\u001b[38;5;242m|";
		}else{
			break;
		}
	}
	
	for (int y = cury; y > 0; y--){
		if (curx == 0){break;}
		
		if (rawViewport[y][curx] == ' ' && (curx & 3) == 0 || rawViewport[y].size() == 0){
			setCursorPosition(XOffset + curx, y);
			cout << "\u001b[38;5;242m|";
		}else{
			break;
		}
	}
}

void drawGuideLinesC(int pos){
	for (int x = 0; x < rawViewport[pos].size(); x++){
		if ((x & 3) == 0){
			if (rawViewport[pos][x] == ' ' && x > 0 && rawViewport[pos].substr(x, 4) == "    "){
				setCursorPosition(XOffset + x, pos);
				
				if (x == curx){
					cout << "\u001b[0m\u001b[38;5;242m|";
				}else{
					cout << "\u001b[0m\u001b[38;5;238m|";
				}
			}
		}
	}
}

void drawSelection(){
	int start;
	int end;
	
	if (endx < startx){
		start = endx;
		end = startx;
	}else{
		start = startx;
		end = endx;
	}
	
	setCursorPosition(start + XOffset, starty);
	cout << "\u001b[107;30m" << raw[cury + index].substr(start, end - start) << "\u001b[0m";
}

string returnSelection(){
	if (startx == 0 || endx == 0){
		return "";
	}
	
	if (endx < startx){
		return raw[cury + index].substr(endx, startx - endx);
	
	}else{
		return raw[cury + index].substr(startx, endx - startx);
	}
}

void clearText(){
	for (int i = 1; i < viewport.size(); i++){
		for (int b = 0; b < rawViewport[i].size() + 1; b++){
			setCursorPosition(b, i);
			cout << " ";
		}
	}
}

