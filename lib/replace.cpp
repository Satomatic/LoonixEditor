#include <iostream>
#include <string>
#include <vector> 

using namespace std;

extern vector<string> lines;
extern vector<string> raw;
extern int curx;
extern int cury;
extern int index;

class Replace{
	public:
		string replace = "";
		string replacer = "";
		string inputBuffer = "";
			
		vector<vector<string>> search;
		
		int total = 0;
		int done = 0;
		int stage = 0;
	
		Box ReplaceContainer;
				
	void draw(){
		ReplaceContainer.title = "Replace";
		ReplaceContainer.footer = "Ctrl + X - Close";
		ReplaceContainer.posx = 0;
		ReplaceContainer.posy = 0;
		ReplaceContainer.width = 26;
		ReplaceContainer.height = 3;
		ReplaceContainer.center = true;
		ReplaceContainer.draw();

		string inputTitle = "find";
		int inputIndex = 0;
	
		// get replace input //
		updateInput("find", 0);
		updateInput("replace", 1);

		while (true){
			string key = getInput();
			
			if (key == "Return"){
				if (stage == 0){
					inputTitle = "replace";
					inputIndex = 1;
					
					replace = inputBuffer;
					inputBuffer = "";
					
					stage = 1;
				
				}else if (stage == 1){
					replacer = inputBuffer;
					inputBuffer = "";
					
					// search for keywords //
					for (int i = 0; i < raw.size(); i++){
						for (int b = 0; b < raw[i].size(); b++){
							if (raw[i].substr(b, replace.size()) == replace){
								search.push_back({to_string(i), to_string(b)});
							}
						}
					}
					
					setCursorPosition(ReplaceContainer.posx + 1, ReplaceContainer.posy + 3);
					cout << search.size() << " found";
				
					while (true){
						string key = getInput();
						
						if (key == "Return"){
							int xcoord = stoi(search[done][1]);
							int ycoord = stoi(search[done][0]);

							// move cursor //                            
							curx = xcoord;
							cury = ycoord;
							
							// highlight word //
							setCursorPosition(xcoord, ycoord);
							cout << "\u001b[107;30m" << replace << "\u001b[0m";
							
							updateCursor();
							
							string key = getInput();
							
							if (key == "Return"){
								// replace keyword //
								raw[ycoord].replace(xcoord, replace.size(), replacer);
								lines[ycoord] = syntaxLine(raw[ycoord]);
								
								if (done + 1 != search.size()){
									int totalDifference = 0;
									for (int i = 0; i < search.size(); i++){
										if (search[i][0] == search[done][0]){
											if (stoi(search[i][1]) != xcoord){
												int difference = 0;
											
												if (replacer.size() > replace.size()){
													difference = replacer.size() - replace.size();
												}else{
													difference = replace.size() - replacer.size();
												}
											
												int x = stoi(search[i][1]);
												x += difference + totalDifference;
												search[i][1] = to_string(x);
											}
										}
									}
								}
								
								refresh();
								drawHeader();
								updateCursor();
							}
							
							done ++;

							if (done == search.size()){
								setCursorPosition(ReplaceContainer.posx + 1, ReplaceContainer.posy + 3);
								cout << "no results found";
								
								while (true){
									string key = getInput();
									if (key == "CTRLX"){
										break;
									}
								}
								
								break;
							}
							   
						}else if (key == "CTRLX"){
							break;
						}    
					}
					
					undraw();
					break;
				}

			}else if (key == "Backspace"){
				if (inputBuffer.size() != 0){
					inputBuffer = inputBuffer.substr(0, inputBuffer.size() - 1);
				}
				
				updateInput(inputTitle, inputIndex);

			}else if (key == "CTRLX"){
				undraw();
				break;

			}else if (key.size() == 1){
				inputBuffer += key;
				updateInput(inputTitle, inputIndex);
			}
		}
	}

	void undraw(){
		ReplaceContainer.undraw();
		drawFromPoint(0);
	}
	
	void updateInput(string text, int pos){
		setCursorPosition(ReplaceContainer.posx + 1, ReplaceContainer.posy + 1 + pos);
		
		if (inputBuffer.size() > 26 - text.size() - 3){
			int inputSize = 26 - text.size() - 3;
			string input = inputBuffer.substr(inputBuffer.size() - inputSize, inputSize);
			cout << "\u001b[1m" << text << ": \u001b[0m" << input;
		}else{
			cout << "\u001b[1m" << text << ": \u001b[0m" << inputBuffer;
		}

		cout << " ";
	}
};

