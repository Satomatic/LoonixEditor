#include <iostream>
#include <vector> 
#include <string>

using namespace std;

extern ConfigManager configManager;
extern DiffManager diffManager;

class OptionMenu{
	public:
		vector<vector<string>> options = {
			{"Block scrolling",   "lowc_enabled", configManager.getSetting("lowc_enabled", "0")},
			{"Indentation guide", "line_enabled", configManager.getSetting("line_enabled", "1")},
			{"Diff bar",          "diff_enabled", configManager.getSetting("diff_enabled", "1")},
			{"Scroll bar",        "scrl_enabled", configManager.getSetting("scrl_enabled", "1")},
			{"Dark header",       "dark_enabled", configManager.getSetting("dark_enabled", "0")}
		};

		int cursorpos = 0;
		
		Box container;

	void init(){
		container.width = 40;
		container.height = 13;
		container.title = "Options";
		container.titleAlign = container.CENTER;
		container.center = true;
		container.draw();
		
		while (true){
			drawOptions();
			
			string key = getInput();
			
			if (key == "UpArrow"){
				if (cursorpos == 0){
					cursorpos = options.size() - 1;
				}else{
					cursorpos --;
				}
			
			}else if (key == "DownArrow"){
				if (cursorpos == options.size() - 1){
					cursorpos = 0;
				}else{
					cursorpos ++;
				}
				
			}else if (key == "Return" || key == " "){
				if (options[cursorpos][2] == "0"){
					options[cursorpos][2] = "1";
				}else{
					options[cursorpos][2] = "0";
				}
				
				configManager.putValue(
					options[cursorpos][1],
					options[cursorpos][2]
				);
				
			}else if (key == "CTRLX"){
				break;
			}
		}
		
		clear();
		diffManager.drawDiffBar();
		newRefresh();
		drawHeader();
		drawFooter();
		updateCursor();
		container.undraw();
	}
	
	void drawOptions(){
		cout << "\u001b[0m";
	
		for (int i = 0; i < options.size(); i++){
			if (i == cursorpos){
				cout << "\u001b[7m";
			}else{
				cout << "\u001b[0m";
			}
		
			setCursorPosition(container.posx + 1, container.posy + 1 + i);
			cout << options[i][0];
			setCursorPosition(container.posx + container.width - 2, container.posy + 1 + i);
			if (options[i][2] == "0"){
				cout << "\u001b[0m[ ]";
			}else{
				cout << "\u001b[0m[x]";
			}
		}
	}
};

