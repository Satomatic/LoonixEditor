#include <iostream>
#include <string>
#include "cmd.cpp"

using namespace std;

string getInput(){
	char key = getch();

	if (key == '\033'){
		// special char //
		char a = getch();
		char b = getch();
		string absolute = "";

		if (b == 'A'){
			absolute = "UpArrow";
		}else if (b == 'B'){
			absolute = "DownArrow";
		}else if (b == 'C'){
			absolute = "RightArrow";
		}else if (b == 'D'){
			absolute = "LeftArrow";
		}else{
			if (b == 53){
				char c = getch();
				if (c == 126){
					absolute = "PGUP";
				}
			}else if (54){
				char c = getch();
				if (c == 126){
					absolute = "PGDN";
				}
			}
		}

		return absolute;
	}else{
		string absolute = "";

		if (key == 127){
			absolute = "Backspace";
		}else if (key == 24){
			absolute = "CTRLX";
		}else if (key == 19){
			absolute = "CTRLS";
		}else if (key == 10){
			absolute = "Return";
		}else if (key == 1){
			absolute = "CTRLA";
		}else if (key == 15){
			absolute = "CTRLO";
		}else if (key == 14){
			absolute = "CTRLN";
		}else if (key == 8){
			absolute = "CTRLH";
		}else if (key == 9){
			absolute = "TAB";
		}else{
			absolute = key;
		}

		return absolute;
	}
}
