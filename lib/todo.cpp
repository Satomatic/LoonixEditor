#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern vector<string> raw;
extern int index;
extern int cury;
extern int curx;

class Todo{
	public:
		vector<string> flags = {"TODO:", "FIX:", "BUG:", "DONE:"};
		vector<vector<string>> search;

		int cursor = 0;
		int precursor = 0;

		Box TodoContainer;

	void draw(){
		// draw container //
		TodoContainer.width = 40;
		TodoContainer.height = 10;
		TodoContainer.center = true;
		TodoContainer.escape = "\u001b[1m";
		TodoContainer.title = "Todo";
		TodoContainer.footer = "Ctrl + X - Close";

		// clear search vector //
		search.clear();

		// search for flags //
		for (int i = 0; i < raw.size(); i++){
			// for char in line
			for (int b = 0; b < raw[i].size(); b++){
				// for flag in flags //
				for (int c = 0; c < flags.size(); c++){
					if (raw[i].substr(b, flags[c].size()) == flags[c]){
						search.push_back({flags[c], to_string(i), to_string(b)});
					}
				}
			}
		}

		// display search results //
		for (int i = 0; i < search.size(); i++){
			int ycoord = stoi(search[i][1]);
			int xcoord = stoi(search[i][2]);
			vector<string> flagmessage = splitIndex(raw[ycoord], xcoord);

			string item = "\u001b[1mX\u001b[0m {\u001b[3mY,Z\u001b[0m} P";
			item = replace_all(item, "X", search[i][0]);
			item = replace_all(item, "Y", search[i][1]);
			item = replace_all(item, "Z", search[i][2]);
			item = replace_all(item, "P", flagmessage[1].substr(search[i][0].size() + 1, flagmessage[1].size()));

			TodoContainer.message += item;
			TodoContainer.message += "\\";
		}

		TodoContainer.draw();

		if (search.size() > 0){
			updateTodoCursor();
		}

		while (true){
			string key = getInput();

			if (key == "CTRLX"){
				break;

			}else if (key == "UpArrow"){
				if (cursor != 0){
					cursor --;
				}

				if (search.size() > 0){
					updateTodoCursor();
				}

			}else if (key == "DownArrow"){
				if (cursor < search.size() - 1){
					cursor ++;
				}

				if (search.size() > 0){
					updateTodoCursor();
				}
			}else if (key == "Return"){
				if (search.size() > 0){
					int ycoord = stoi(search[cursor][1]);
					int xcoord = stoi(search[cursor][2]);

					index = ycoord - 1;
					cury = 1;
					curx = xcoord;

					clearFromPoint(0);
					updateViewport();
					drawFromPoint(0);
					updateCursor();

					TodoContainer.draw();
					updateTodoCursor();
				}
			}
		}


		TodoContainer.undraw();
		drawFromPoint(0);
		updateCursor();
	}

	void updateTodoCursor(){
		int ycoord = stoi(search[cursor][1]);
		int xcoord = stoi(search[cursor][2]);

		int ycoordB = stoi(search[precursor][1]);
		int xcoordB = stoi(search[precursor][2]);

		vector<string> flagmessage = splitIndex(raw[ycoord], xcoord);
		vector<string> flagmessageb = splitIndex(raw[ycoordB], xcoordB);

		string item = "X {Y,Z} P";
		item = replace_all(item, "X", search[cursor][0]);
		item = replace_all(item, "Y", search[cursor][1]);
		item = replace_all(item, "Z", search[cursor][2]);
		item = replace_all(item, "P", flagmessage[1].substr(search[cursor][0].size() + 1, flagmessage[1].size()));

		string itemb = "\u001b[0m\u001b[1mX\u001b[0m {\u001b[3mY,Z\u001b[0m} P";
		itemb = replace_all(itemb, "X", search[precursor][0]);
		itemb = replace_all(itemb, "Y", search[precursor][1]);
		itemb = replace_all(itemb, "Z", search[precursor][2]);
		itemb = replace_all(itemb, "P", flagmessageb[1].substr(search[precursor][0].size() + 1, flagmessageb[1].size()));

		setCursorPosition(TodoContainer.posx + 1, TodoContainer.posy + precursor + 1);
		cout << itemb;

		setCursorPosition(TodoContainer.posx + 1, TodoContainer.posy + cursor + 1);
		cout << "\u001b[0m\u001b[107m\u001b[30m" << item;

		precursor = cursor;
	}
};
