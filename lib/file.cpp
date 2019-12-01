#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

extern vector<string> raw;
extern vector<string> lines;
extern vector<string> viewport;
extern string currentfile;
extern bool hasEdited;

extern HeaderDrop headerMessage;

void loadFile(string filepath){
	lines.clear();
	raw.clear();
	lines.push_back(" if you can see this, somethings gone wrong ");
	raw.push_back(" if you can see this, somethings gone wrong ");

	currentfile = filepath;
	hasEdited = false;

	ifstream file(filepath);

	string line = "";
	while (getline(file, line)){
		// tab convert //
		string convert = "";
		for (int i = 0; i < line.size(); i++){
			if (line[i] == '\t'){
				convert += "    ";
			}else{
				convert += line[i];
			}
		}
		raw.push_back(convert);
		lines.push_back(syntaxLine(convert));
	}
	file.close();

	updateViewport();
}

void openFile(){
	system("setterm -cursor on");
	setCursorPosition(0,0);
	clear();

	string input = "";
	cout << "Open file >> ";
	getline(cin, input);

	ifstream file(input);

	if (file.good()){
		loadFile(input);

		// redraw display //
		index = 0;
		curx = 0; // reset cursor
		cury = 1;

		clear();
		updateViewport();
		drawScreen();
		drawHeader();
		updateCursor();
	}else{
		clear();
		updateViewport();
		drawScreen();
		drawHeader();
		updateCursor();

		headerMessage.message = "File does not exist";
		headerMessage.styling = "\u001b[2m\u001b[38;5;124m";
		headerMessage.draw();
	}

	system("setterm -cursor off");
}

void saveFile(){
	hasEdited = false;

	ofstream file(currentfile);

	for (int i = 1; i < raw.size(); i++){
		//file << raw[i] << endl;
		string convert = raw[i];

		// convert spaces to tabs
		for (int b = 0; b < convert.size(); b++){
			if (convert.substr(b, 4) == "    "){
				convert.replace(b, 4, "\t");
			}else{
				break;
			}
		}

		file << convert << endl;

	}

	file.close();

	headerMessage.message = "File saved";
	headerMessage.styling = "";
	headerMessage.draw();
}

void saveAsFile(){
	system("setterm -cursor on");
	setCursorPosition(0,0);
	clear();
	string input = "";
	cout << "Save as >> ";
	getline(cin, input);

	currentfile = input;

	// redraw screen //
	system("setterm -cursor off");
	setCursorPosition(0,0);
	clear();
	drawScreen();
	drawHeader();
	updateCursor();

	saveFile();
}

void newFile(){
	lines.clear();
	lines.push_back(" if you can see this, somethings gone wrong ");
	lines.push_back(" ");
	lines.push_back(" ");

	raw.clear();
	raw.push_back(" if you can see this, somethings gone wrong ");
	raw.push_back(" ");
	raw.push_back(" ");

	currentfile = "newfile";

	updateViewport();

	curx = 0;
	cury = 1;

	setCursorPosition(0,0);
	clear();
	drawScreen();
	drawHeader();
	updateCursor();
}

class filemanager{
	public:
		int posx = 0;
		int posy = 0;
		int width = 10;
		int height = 10;

		string title = "";

		vector<string> view;
		vector<string> currentdir;
		vector<string> currentsort;

		int start = 0;
		int index = 0;
		int lastIndex = 0;

		string selected = "";
		string dir = ".";

		Box FileManagerBox;

		void draw(){
			currentdir = DirView(".");

			// draw box //
			FileManagerBox.title = title;
			FileManagerBox.width = width;
			FileManagerBox.height = height;
			FileManagerBox.posx = posx;
			FileManagerBox.posy = posy;
			FileManagerBox.center = true;
			FileManagerBox.draw();

			// display dir //
			updateView();
			updateList();
			updateCursor();

			while(true){
				string key = getInput();

				if (key == "DownArrow"){
					if (start + index != currentdir.size() - 1){
						if (start + index == height - 1){
							// at bottom //
							clearView();
							start ++;
							updateView();
							updateList();
							updateCursor();
						}else{
							index ++;
							updateCursor();
						}
					}

				}else if (key == "UpArrow"){
					if (start + index > 0){
						if (index == 0){
							clearView();
							start --;
							updateView();
							updateList();
							updateCursor();
						}else{
							index --;
							updateCursor();
						}
					}

				}else if (key == "Return"){
					string currentItem = currentsort[int(start + index)];
					vector<string> itemsplit = split(currentItem, '|');

					if (itemsplit[1] == "FILE"){
						selected = dir + "/" + itemsplit[0];
						break;
					}else{
						dir += "/";
						dir += itemsplit[0];

						clearView();
						currentdir.clear();
						currentdir = DirView(dir);
						index = 0;
						start = 0;
						updateView();
						updateList();
						updateCursor();
					}

				}else if (key == "CTRLX"){
					selected = "exit";
					break;
				}

			}
		}

		void updateView(){
			view.clear();
			currentsort.clear();
			// for directories //
			for (int i = 0; i < currentdir.size(); i++){
				if (i >= start && i < height){
					vector<string> linesplit = split(currentdir[i], '|');
					if (linesplit[1] == "DIR"){
						view.push_back(linesplit[0]);
						currentsort.push_back(linesplit[0] + "|" + linesplit[1]);
					}
				}
			}

			// for files //
			for (int i = 0; i < currentdir.size(); i++){
				if (i >= start && i < height){
					vector<string> linesplit = split(currentdir[i], '|');
					if (linesplit[1] == "FILE"){
						view.push_back(linesplit[0]);
						currentsort.push_back(linesplit[0] + "|" + linesplit[1]);
					}
				}
			}

		}

		void clearView(){
			resetColor();

			// clear list //
			for (int i = 0; i < view.size(); i++){
				for (int b = 0; b < view[i].size(); b++){
					setCursorPosition(FileManagerBox.posx + 1 + b, FileManagerBox.posy + 1 + i);
					cout << " ";
				}
			}
		}

		void updateList(){
			clearView();

			// display list//
			for (int i = 0; i < view.size(); i++){
				setCursorPosition(FileManagerBox.posx + 1, FileManagerBox.posy + 1 + i);
				cout << view[i];
			}
		}

		void updateCursor(){
			// overwrite previous line //
			setCursorPosition(FileManagerBox.posx + 1, FileManagerBox.posy + 1 + lastIndex);
			resetColor();
			cout << view[lastIndex];

			// write new cursor //
			setCursorPosition(FileManagerBox.posx + 1, FileManagerBox.posy + 1 + index);
			cout << "\u001b[30m\u001b[107m";
			cout << view[index];

			lastIndex = index;
		}
};
