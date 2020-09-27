#include <iostream>
#include <string>
#include <vector> 
#include "global.h"

using namespace std;

extern DiffManager diffManager;
extern HeaderDrop headerMessage;
extern ScrollBar scrollbar;

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
							cout << "\u001b[7m" << replace << "\u001b[0m";
							
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
								
								newRefresh();
								drawHeader();
								drawGuideLines();
								updateCursor();
								diffManager.drawDiffBar();
								scrollbar.position = scroll;
								scrollbar.draw();
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

class NewReplace{
	public:
		string replacer;
		string replace = "";

		int total = 0;
		int done = 0;
		int skip = 0;

		vector<vector<string>> results;
		
		Box replaceContainer;

	void init(){
		replaceContainer.title = "Replace";
		replaceContainer.width = screenWidth - 2;
		replaceContainer.height = 3;
		replaceContainer.titleAlign = replaceContainer.CENTER;
		replaceContainer.posy = screenHeight - replaceContainer.height - 2;
		replaceContainer.draw();

		// get replace //
		Input input;
		input.prefix = "find: ";
		input.maxx = 19;
		input.x = replaceContainer.posx + 1;
		input.y = replaceContainer.posy + 1;
		input.input = replace;
		input.init();
		replace = input.input;
		; // wait to undraw cursor
		input.undrawCursor();
		
		// get replacer //
		Input binput;
		binput.prefix = "replace: ";
		binput.maxx = 16;
		binput.x = replaceContainer.posx + 1;
		binput.y = replaceContainer.posy + 2;
		binput.init();
		replacer = binput.input;
		; // wait to undraw cursor
		binput.undrawCursor();
	
		if (input.input == "" && binput.input == ""){
			headerMessage.styling = "\u001b[0m";
			headerMessage.message = "No more results";
			headerMessage.draw();
		
			replaceContainer.undraw();
		
			return;
		}

		// build results array //
		for (int i = 1; i < raw.size(); i++){
			for (int b = 0; b < raw[i].size(); b++){
				if (raw[i].substr(b, replace.size()) == replace){
					results.push_back({to_string(i), to_string(b)});
				}
			}
		}
	  
		total = results.size();
		  
		// cycle through results //
		for (int i = 0; i < results.size(); i++){
			int xcoord = stoi(results[i][1]);
			int ycoord = stoi(results[i][0]);
			bool close = false;
			
			scroll = ycoord - 1;
			cury = 1;
			curx = xcoord;
			
			if (raw.size() - 1 < screenHeight - 1){
				scroll = 0;
				cury = ycoord;
				curx = xcoord;
			}else{
				if (testViewport() < screenHeight - 1){
					int difference = screenHeight - testViewport() - 1;
					scroll -= difference;
					cury += difference;
				}
			}
			
			newRefresh();
			updateCursor();
			diffManager.drawDiffBar();
			scrollbar.position = scroll;
			scrollbar.draw();
			drawGuideLines();
			
			setCursorPosition(curx + XOffset, cury);
			cout << "\u001b[7m" << replace << "\u001b[0m";
			
			replaceContainer.draw();
			setCursorPosition(replaceContainer.posx + 1, replaceContainer.posy + 1);
			cout << "find: " << replace;
			setCursorPosition(replaceContainer.posx + 1, replaceContainer.posy + 2);
			cout << "replace: " << replacer;
			setCursorPosition(replaceContainer.posx + 1, replaceContainer.posy + 3);
			cout << "replaced " << done << " / " << total - 1;

			while (true){
				string key = getInput();
				
				if (key == "Return"){
					raw[ycoord] = raw[ycoord].replace(xcoord, replace.size(), replacer);
					lines[ycoord] = syntaxLine(raw[ycoord]);

					setCursorPosition(0 + XOffset, cury);
					cout << lines[ycoord];
					
					if (done + 1 != results.size()){
						int totaldifference = 0;
						
						for (int i = 0; i < results.size(); i++){
							if (results[i][0] == results[done][0]){
								if (stoi(results[i][1]) != xcoord){
									int difference = 0;
									
									if (replacer.size() > replace.size()){
										difference = replacer.size() - replace.size();
									}else{
										difference = replace.size() - replacer.size();
									}
									
									int x = stoi(results[i][1]);
									x += difference + totaldifference;
									results[i][1] = to_string(x);
								}
							}
						}
					}

					updateCursor();

					if (done == results.size()){
						break;
					}
					
					while (true){
						key = getInput();
					
						if (key == "Return"){
							done += 1;
							break;
							
						}else if (key == "CTRLX"){
							replaceContainer.undraw();
							return;
						}
					}
					
					break;
				
				}else if (key == "RightArrow"){
					skip += 1;
					break;
			
				}else if (key == "CTRLX"){
					close = true;
					break;
				}
			}
			
			if (close == true){
				break;
			
			}else if (done + skip == total){
				break;
			}
		}
		
		replaceContainer.undraw();
		newRefresh();
		updateCursor();
	
		if (done + skip == total){
			headerMessage.styling = "\u001b[0m";
			headerMessage.message = "No more results";
			headerMessage.draw();
		}
	}
};
