#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern vector<string> raw;
extern int index;
extern int curx;
extern int cury;

class Find{
	public:
		vector<vector<int>> find_results;

		string find_str;
		int total = 0;
		int current = 0;

		string input = "";
		string lastinput;

		Box FindContainer;

	void draw(){
		FindContainer.width = 26;
		FindContainer.height = 2;
		FindContainer.center = true;
		FindContainer.title = "Find";
		FindContainer.draw();

		updateInput();

		while (true){
			string key = getInput();

			if (key == "Backspace"){
				if (input.size() != 0){
					input = input.substr(0, input.size() - 1);
				}

				updateInput();

			}else if (key == "Return"){
				if (input.size() != 0){
					if (input != lastinput){
						find_results.clear();
						current = 0;
						total = 0;
						for (int i = 0; i < raw.size(); i++){
							for (int b = 0; b < raw[i].size(); b++){
								if (raw[i].substr(b, input.size()) == input){
									find_results.push_back({i, b});
									total ++;
								}
							}
						}

					}else if (total != 0){
						if (current == total){
							current = 0;
						}

						int csx = find_results[current][1];
						int csy = find_results[current][0];

						index = csy - 1;
						cury = 1;
						curx = csx;

						if (raw.size() - 1 < screenHeight - 1){
							index = 0;
							cury = csy;
							curx = csx;
						}else{
							if (testViewport() < screenHeight - 1){
								int difference = screenHeight - testViewport() - 1;
								index -= difference;
								cury += difference;
							}
						}

						current ++;

						clearFromPoint(0);
						updateViewport();
						drawFromPoint(0);
						drawHeader();
						updateCursor();

						// highlight find
						setCursorPosition(curx, cury);
						cout << "\u001b[30;107m" << input << "\u001b[0m";
					}

					FindContainer.message = "\\Found ";
					FindContainer.message += to_string(current);
					FindContainer.message += "/";
					FindContainer.message += to_string(total);

					FindContainer.draw();
					updateInput();

					lastinput = input;
				}

			}else if (key == "CTRLX"){
				break;

			}else if (key == "LeftArrow" || key == "RightArrow"){
				break;

			}else if (key == "UpArrow" || key == "DownArrow"){
				break;

			}else if (key.size() == 1){
				input += key;
				updateInput();
			}
		}

		FindContainer.undraw();

		drawFromPoint(0);
		updateCursor();
	}

	void updateInput(){
		setCursorPosition(FindContainer.posx + 1, FindContainer.posy + 1);

		if (input.size() > 19){
			cout << "\u001b[1mfind: \u001b[0m" << input.substr(input.size() - 19, 19);
		}else{
			cout << "\u001b[1mfind: \u001b[0m" << input;
		}

		cout << " ";
	}
};
