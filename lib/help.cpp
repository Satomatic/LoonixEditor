#include <iostream>
#include <vector>
#include <string>

using namespace std;

class helpMenu{
	public:
		int width = 20;
		int height = 10;

		vector<vector<string>> items = {
			{"\u001b[0mKeys", ""},
			{"^N", " ~ \u001b[3mNew"},
			{"^O", " ~ \u001b[3mOpen"},
			{"^S", " ~ \u001b[3mSave"},
			{"^A", " ~ \u001b[3mSave as"},
			{"^F", " ~ \u001b[3mFind"},
			{"^K", " ~ \u001b[3mCut line"},
			{"^X", " ~ \u001b[3mExit / Close"}
		};

		Box HelpMenu;

	void draw(){
		HelpMenu.title = "Help";
		HelpMenu.center = true;
		HelpMenu.width = width;
		HelpMenu.height = height;

		for (int i = 0; i < items.size(); i++){
			string item = "\u001b[30;107m";
			item += items[i][0];
			item += "\u001b[0m\u001b[38;5;242m";
			item +=  items[i][1] + " \\";
			item += "\u001b[0m";
			HelpMenu.message += item;
		}

		HelpMenu.draw();

		while (true){
			string key = getInput();

			if (key == "CTRLX"){
				setCursorPosition(0,0);
				break;
			}
		}

		HelpMenu.undraw();
	}
};
