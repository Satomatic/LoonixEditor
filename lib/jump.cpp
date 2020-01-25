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
		JumpContainer.height = 2;
		JumpContainer.center = true;
		JumpContainer.draw();

		while (true){
			Input JumpInput;
			JumpInput.x = JumpContainer.posx + 1;
			JumpInput.y = JumpContainer.posy + 1;
			JumpInput.maxx = 19;
			JumpInput.input = input;
			JumpInput.prefix = "Line: ";
			JumpInput.closeMessage = "input_close";
			
			JumpInput.customClose = {
				"LeftArrow",
				"RightArrow",
				"UpArrow",
				"DownArrow"
			};
			
			JumpInput.init();        
			input = JumpInput.input;
			
			// check if input has been closed //
			if (input == JumpInput.closeMessage){
				break;
			}
			
			int cisnum = true;
			
			// check for non numbers //
			for (int i = 0; i < input.size(); i++){
				bool isnum = false;
				
				for (int b = 0; b < nums.size(); b++){
					if (input[i] == nums[b]){
						isnum = true;
						break;
					}
				}
				
				if (isnum == false){
					setCursorPosition(JumpContainer.posx + 1, JumpContainer.posy + 2);
					cout << "\u001b[0m\u001b[38;5;124mError: Not a number\u001b[0m";
					cisnum = false;
					break;
				}
			}
			
			if (cisnum == true){
				if (stoi(input) <= raw.size() - 1){
					index = stoi(input) - 1;
					cury = 1;
					curx = 0;
					
					if (testViewport() < screenHeight - 1){
						int difference = screenHeight - testViewport();
						index -= difference;
						cury += difference;
					}
					
					newRefresh();
					updateCursor();
					updateHeader();
					
					JumpContainer.draw();
				}else{
					setCursorPosition(JumpContainer.posx + 1, JumpContainer.posy + 2);
					cout << "\u001b[0m\u001b[38;5;124mError: Out of range\u001b[0m";
				}
			}
		} 
	
		JumpContainer.undraw();
		refresh();
		updateCursor();        
	}  

	void sub(){
	
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
 
