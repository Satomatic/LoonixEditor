#include <iostream>
#include <vector>
#include <string>

using namespace std;

class helpMenu{
	public:
		int width = 40;
		int height = 13;
		int tabIndex = 0;
		
		vector<string> tabs = {
			"file",
			"edtior",
			"about"
		};

		vector<vector<vector<string>>> data = {
			// file menu //
			{
				{"Ctrl + N", "New file"},
				{"Ctrl + O", "Open file"},
				{"Ctrl + S", "Save file"},
				{"Ctrl + A", "Save as file"},
				{"Ctrl + X", "Close file"}
			},
			
			// editor menu //
			{
				{"Ctrl + Alt + Right", "Next file"},
				{"Ctrl + Alt + Left", "Previous file"},
				{"Ctrl + F", "Find string"},
				{"Ctrl + R", "Fine and Replace"},
				{"Ctrl + U", "Jump line"},
				{"Ctrl + L", "Line info"}
			},
			
			// about menu //
			{
				{"Written by", "Satomatic (Brian Thomson)"}
			}
		};

		Box HelpContainer;
		
	void init(){
		HelpContainer.width = width;
		HelpContainer.height = height;
		HelpContainer.title = "Help";
		HelpContainer.center = true;
		HelpContainer.draw();

		loadTabData();
		updateTabs();

		while (true){
			string key = getInput();
			
			if (key == "LeftArrow"){
				if (tabIndex == 0){
					tabIndex = tabs.size() - 1;
				}else{
					tabIndex --;
				}
			
			}else if (key == "RightArrow"){
				if (tabIndex == tabs.size() - 1){
					tabIndex = 0;
				}else{
					tabIndex ++;
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
				cout << "\u001b[107;30m" << tabs[i] << "\u001b[0m";
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
		vector<vector<string>> tabdata = data[tabIndex];
		
		for (int i = 0; i < tabdata.size(); i++){
			setCursorPosition(HelpContainer.posx + 1, HelpContainer.posy + 3 + i);
			cout << "\u001b[1m" << tabdata[i][0] << "\u001b[0m";
			setCursorPosition(HelpContainer.posx + 1 + width - tabdata[i][1].size(), HelpContainer.posy + 3 + i);
			cout << tabdata[i][1];
		}
	}
};
