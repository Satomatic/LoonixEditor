#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern vector<string> raw;
extern int index;
extern int posx;
extern int posy;

class jumpLine{
	public:
		string input = "";
		string nums = "0123456789";
	
		Box JumpContainer;

	void draw(){
		JumpContainer.title = "Jump line";
		JumpContainer.posx = 0;
		JumpContainer.posy = 0;
		JumpContainer.width = 26;
		JumpContainer.height = 3;
		JumpContainer.center = true;
		JumpContainer.draw();

		updateInput();

		while (true){
			string key = getInput();
			
			if (key == "Return"){
				bool isnum = false;
				
				for (int i = 0; i < input.size(); i++){
					char cchar = input.at(i);
					bool cisnum = false;
					
					for (int b = 0; b < nums.size(); b++){
						char cnum = nums.at(b);
						
						if (cchar == cnum){
							cisnum = true;
							break;
						}
					}
					
					if (cisnum == true){
						isnum = true;
					}else{
						isnum = false;
						break;
					}
				}
				
				if (isnum == true){
					int line = stoi(input);
					
					if (line < raw.size()){
						index = line - 1;
						cury = 1;
						
						if (curx > raw[line].size()){
							curx = raw[line].size();
						}
						
						refresh();
						updateCursor();
						
						JumpContainer.draw();
						updateInput();
					}else{
						setCursorPosition(JumpContainer.posx + 1, JumpContainer.posy + 3);
						cout << "\u001b[31;2mError: Not a line\u001b[0m";
					}
				}else{
					setCursorPosition(JumpContainer.posx + 1, JumpContainer.posy + 3);
					cout << "\u001b[31;2mError: Not a number\u001b[0m";
				}
				
			}else if (key == "CTRLX"){
				JumpContainer.undraw();
				refresh();
				updateCursor();
				break;

			}else if (key == "UpArrow"){
				break;
			
			}else if (key == "DownArrow"){
				break;
				
			}else if (key == "RightArrow"){
				break;
				
			}else if (key == "LeftArrow"){
				break;

			}else if (key == "Backspace"){
				if (input.size() != 0){    
					input = input.substr(0, input.size() - 1);
				}
			
			}else{
				if (key.size() == 1){
					input += key;
				}
			}
			
			updateInput();
		}
		
		JumpContainer.undraw();
		refresh();
		updateCursor();        
	}  
	
	void updateInput(){
		setCursorPosition(JumpContainer.posx + 1, JumpContainer.posy + 1);

		if (input.size() > 19){
			cout << "\u001b[1mLine: \u001b[0m" << input.substr(input.size() - 19, 19);
		}else{
			cout << "\u001b[1mLine: \u001b[0m" << input;
		}
		
		cout << " " << endl;
	}    
}; 
 
