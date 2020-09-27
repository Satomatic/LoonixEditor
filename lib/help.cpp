#include <iostream>
#include <vector>
#include <string>

using namespace std;

class helpMenu{
	public:
		int width = 40;
		int height = 13;
		int tabIndex = 0;
		int scroll = 0;
		
		vector<string> tabs = {
			"file",
			"edtior",
			"options",
			"about",
		};

		vector<vector<vector<string>>> data = {
			// file menu //
			{
				{"Ctrl + N", "New file"},
				{"Ctrl + O", "Open file"},
				{"Ctrl + S", "Save file"},
				{"Ctrl + A", "Save as file"},
				{"Ctrl + X", "Close file"},
				{"Ctrl + Alt + O", "Replace current file"},
				{"", ""},
				{"F1", "Help menu"},
				{"F5", "Option menu"},
			},
			
			// editor menu //
			{
				{"Ctrl + Alt + Right", "Next file"},
				{"Ctrl + Alt + Left", "Previous file"},
				{"Alt + Right", "Next file"},
				{"Alt + Left", "Previous file"},
				{"", ""},
				{"Ctrl + UpArrow", "Move line up"},
				{"Ctrl + DownArrow", "Move line down"},
				{"", ""},
				{"Ctrl + F", "Find string"},
				{"Ctrl + R", "Fine and Replace"},
				{"Ctrl + D", "Duplicate line"},
				{"Ctrl + L", "Line info"},
				{"Ctrl + U", "Jump line"},
				{"Ctrl + T", "Todo list"},
				{"Ctrl + K", "Cut line"},
				{"Ctrl + /", "Comment line"}
			},
			
			// options menu //
			{
				{"F6", "Toggle diff bar"},
				{"F7", "Toggle smooth scrolling"},
				{"F8", "Toggle indentation guide"},
			},

			// about menu //
			{
				{"Written by", "Satomatic (Brian Thomson)"},
				{"Theme", configManager.getSetting("theme", "default")},
			}
		};

		Box HelpContainer;
		
	void init(){
		HelpContainer.width = width;
		HelpContainer.height = height;
		HelpContainer.title = "Help";
		HelpContainer.titleAlign = HelpContainer.CENTER;
		HelpContainer.center = true;
		HelpContainer.draw();

		loadTabData();
		updateTabs();

		while (true){
			setCursorPosition(curx, cury);
		
			string key = getInput();
			
			if (key == "LeftArrow"){
				scroll = 0;
				if (tabIndex == 0){
					tabIndex = tabs.size() - 1;
				}else{
					tabIndex --;
				}
			
			}else if (key == "RightArrow"){
				scroll = 0;
				if (tabIndex == tabs.size() - 1){
					tabIndex = 0;
				}else{
					tabIndex ++;
				}

			}else if (key == "UpArrow" && data[tabIndex].size() > 10){
				if (scroll > 0){
					scroll --;
				}
			
			}else if (key == "DownArrow" && data[tabIndex].size() > 10){
				if (scroll < data[tabIndex].size() - 11){
					scroll ++;
				}
			
			}else if (key == "CTRLX"){
				break;
			}
		
			loadTabData();
			updateTabs();
		}
		
		HelpContainer.undraw();
	}
	
	void updateTabs(){
		cout << "\u001b[0m";
		
		int size = 3;
		
		for (int i = 0; i < tabs.size(); i++){
			size += tabs[i].size();
			
			if (i != tabs.size()){
				size += 3;
			}
		}
		
		setCursorPosition(HelpContainer.posx + 1 - (size / 2) + (width / 2), HelpContainer.posy + 1);
		
		cout << " | ";
		
		// draw tabs //
		for (int i = 0; i < tabs.size(); i++){
			if (i == tabIndex){
				cout << "\u001b[7m" << tabs[i] << "\u001b[0m";
			}else{
				cout << tabs[i];
			}
			
			if (i != tabs.size()){
				cout << " | ";
			}
		}
	}
	
	void loadTabData(){
		HelpContainer.draw();
		vector<vector<string>> tabdata;
		
		if (data[tabIndex].size() > 11){
			for (int i = scroll; i < 11 + scroll; i++){
				tabdata.push_back(data[tabIndex][i]);
			}
		}else{
			tabdata = data[tabIndex];
		}
		
		for (int i = 0; i < tabdata.size(); i++){
			setCursorPosition(HelpContainer.posx + 1, HelpContainer.posy + 3 + i);
			cout << "\u001b[1m" << tabdata[i][0] << "\u001b[0m";
			setCursorPosition(HelpContainer.posx + 1 + width - tabdata[i][1].size(), HelpContainer.posy + 3 + i);
			cout << tabdata[i][1];
		}
	}
};
