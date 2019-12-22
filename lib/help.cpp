#include <iostream>
#include <vector>
#include <string>

using namespace std;

class helpMenu{
	public:
		int width = 20;
		int height = 13;

		vector<vector<string>> items = {
			{"\u001b[0mKeys", ""},
			{"^N", " ~ \u001b[3mNew"},
			{"^O", " ~ \u001b[3mOpen"},
			{"^S", " ~ \u001b[3mSave"},
			{"^A", " ~ \u001b[3mSave as"},
			{"^F", " ~ \u001b[3mFind"},
			{"^F", " ~ \u001b[3mReplace"},
			{"^K", " ~ \u001b[3mCut line"},
			{"^T", " ~ \u001b[3mTodo list"},
			{"^J", " ~ \u001b[3mJump line"},
			{"^L", " ~ \u001b[3mLine info"},
			{"^/", " ~ \u001b[3mComment line"},
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

		string key = getInput();
		HelpMenu.undraw();
	}
};
