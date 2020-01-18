#include <termios.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/ioctl.h>
#include <algorithm>
#include <unistd.h>
#include <dirent.h>

using namespace std;

vector<string> split(string text, char letter){
	vector<string> returnvector;
	string current = "";

	for (int i = 0; i < text.size(); i++){
		if (text[i] == letter){
			returnvector.push_back(current);
			current = "";
		}else{
			current += text[i];
		}
	}

	returnvector.push_back(current);
	return returnvector;
}

vector<string> splitIndex(string text, int index){
	vector<string> returnvector;
	string current = "";

	for (int i =0; i < text.size(); i++){
		if (i == index){
			returnvector.push_back(current);
			current = text[i];
		}else{
			current += text[i];
		}
	}

	returnvector.push_back(current);
	return returnvector;
}

vector<string> DirView(const string& path){
	struct dirent *de;
	DIR *dr = opendir(path.c_str());

	vector<string> returnarray;

	if (dr == NULL){
		returnarray.push_back("Error while opening directory");
		return returnarray;
	}

	while ((de = readdir(dr)) != NULL){
		string returnstring = de->d_name;

		if (de->d_type == DT_REG){
			returnstring += ("|FILE");
		}else{
			returnstring += ("|DIR");
		}
		returnarray.push_back(returnstring);
	}

	closedir(dr);

	sort(returnarray.begin(), returnarray.end());

	return returnarray;
}

bool FileExists(string filename)
{
	string filepath = ".";
	
	// check for file path
	for (int i = 0; i < filename.size(); i++){
		if (filename.substr(i, 1) == "/"){
			filepath = filename.substr(0, filename.find_last_of("/"));
			break;    
		}    
	}
	
	// get file name //
	vector<string> fileSplit = split(filename, '/');
	filename = fileSplit[fileSplit.size() - 1];
	
	bool exists = false;
	vector<string> dirview = DirView(filepath);
	
	// search for file //
	for (int i = 0; i < dirview.size(); i++){
		vector<string> dirsplit = split(dirview[i], '|');
		if (dirsplit[0] == filename){
			exists = true;
			break;
		}
	}

	return exists;
}

void updateScreenSize(){
	extern int screenHeight;
	extern int screenWidth;

	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	screenHeight = size.ws_row;
	screenWidth = size.ws_col;
}

void resetColor(){
	cout << "\u001b[0m";
}

void setCursorPosition(int x, int y){
	printf("\033[%d;%dH", y+1, x+1);
}

void setConsoleTitle(string title){
	cout << "\u001b]0;" << title << "\007";
}

void clear(){
	printf("\033[0m"); // reset terminal to default
	printf("\033[2J"); // clear screen
	system("clear"); // system clear 'prevents any coloring issues'
}

void checkScreenSize(){
	extern int screenHeight;
	extern int screenWidth;

	extern void drawHeader();
	extern void drawScreen();
	extern void updateCursor();
	extern void updateViewport();
	extern void newRefresh();

	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	if (screenWidth != size.ws_col || screenHeight != size.ws_row){
		updateScreenSize();
		clear();
		
		updateViewport();
		resetColor();
		setCursorPosition(0,0);
		
		drawScreen();
		updateCursor();
		drawHeader();
	}
}

int isAlpha(char letter){
	string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bool found = false;
	
	for (int i = 0; i < chars.size(); i++){
		if (chars.at(i) == letter){
			found = true;
			break;
		}
	}
	
	return found;    
}

string fillX(string text){
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	string returnvalue = text;
	int toadd = w.ws_col - text.size();

	for (int i = 0; i < toadd; i++){
		returnvalue += " ";
	}

	return returnvalue;
}


static struct termios old, current;

void initTermios(int echo)
{
	tcgetattr(0, &old);
	current = old;
	current.c_lflag &= ~ICANON;
	if (echo) {
		current.c_lflag |= ECHO;
	} else {
		current.c_lflag &= ~ECHO;
	}
	tcsetattr(0, TCSANOW, &current);
}

void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

char getch(void)
{
	return getch_(0);
}
