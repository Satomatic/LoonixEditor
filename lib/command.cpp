#include <iostream>
#include <string>
#include "global.h"

using namespace std;

extern ConfigManager configManager;
extern ScrollBar scrollbar;

class CommandLine{
	public:
		string input;
		
		Input inputbox;
	
	void init(){
		undrawFooter();
		
		inputbox.x = 0;
		inputbox.y = screenHeight - inputbox.container.height - 1;
		inputbox.maxx = screenWidth - 5;
		inputbox.prefix = ": ";
		inputbox.init();
		
		process(inputbox.input);
	}
	
	void process(string input){
		vector<string> inputsplit = split(input,' ');
		bool error = false;
		
		if (inputsplit[0] == "set" && inputsplit.size() >= 3){
			string key = inputsplit[1];
			string val = inputsplit[2];
			
			configManager.putValue(key, val);
		
			if (key == "theme"){
				loadTheme(configManager.getSetting("theme", "default"));
				reloadLines();
			
			} else if (key == "diff_enabled"){
				clear();
			}
			
			diffManager.drawDiffBar();
			newRefresh();
			updateCursor();
		
		}else if (inputsplit[0] == "reload" && inputsplit.size() >= 2){
			if (inputsplit[1] == "theme"){
				loadTheme(configManager.getSetting("theme", "default"));
				reloadLines();
			}
		
		}else if (inputsplit[0] == "reload"){
			clear();
			
			for (int i = 0; i < viewport.size(); i++){
				setCursorPosition(XOffset, i);
				cout << viewport[i];
			}
			
			diffManager.drawDiffBar();
		   
			if (configManager.getValue("scrl_enabled") == "1")
				scrollbar.draw();
  
			drawHeader();
			drawFooter();
			updateCursor();
		}
	}
};
