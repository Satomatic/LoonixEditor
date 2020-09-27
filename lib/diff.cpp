#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern vector<vector<string>> fileMemory;
extern vector<string> viewport;
extern vector<string> raw;
extern ConfigManager configManager;
extern bool diffEnable;
extern int screenHeight;
extern int fileIndex;
extern int scroll;
extern int cury;

class DiffManager{
	public:
		vector<vector<int>> diff;
		vector<int> tempDiff;
		string sideChar = "▌";

	void init(){
		vector<int> cdiff;
		for (int i = 1; i < raw.size(); i++){
			cdiff.push_back(0);
		}
		diff.push_back(cdiff);
	}

	void drawDiffBar(){
		if (configManager.getValue("diff_enabled") != "1"){
			if (XOffset == 1){XOffset = 0;}
			return;
		}
		
		if (XOffset == 0){
			XOffset = 1;
			newRefresh();
		}
		
		for (int i = 0; i < screenHeight; i++){
			setCursorPosition(0, i + 1);
			cout << "\u001b[0m";

			if (i < diff[fileIndex].size() && i < viewport.size() - 1){
				if (diff[fileIndex][scroll + i] == 0){
					cout << "\u001b[0m";
				
				}else if (diff[fileIndex][scroll + i] == 1){
					cout << "\u001b[38;5;214m";
				
				}else if (diff[fileIndex][scroll + i] == 2){
					cout << "\u001b[38;5;29m";
				}

			}else{
				cout << "\u001b[38;5;242m";
			}
			
			cout << sideChar;
		}
	}

	void updateLine(int line, int value){
		diff[fileIndex][line] = value;
	}
	
	void insertLine(int curpos, int scroll){
		diff[fileIndex].insert(diff[fileIndex].begin() + curpos + scroll, 1);        
		drawDiffBar();
	}

	void removeLine(int curpos, int scroll){
		diff[fileIndex].erase(diff[fileIndex].begin() + scroll + curpos - 1);
		drawDiffBar();    
	}

	void updateCurrentLine(){
		updateLine(cury + scroll - 1, 1);
		drawDiffBar();
	}

	void zero(){
		for (int i = 0; i < diff[fileIndex].size(); i++){
			if (diff[fileIndex][i] == 1){
				diff[fileIndex][i] = 2;
			}
		}
		
		drawDiffBar();
	}
};

