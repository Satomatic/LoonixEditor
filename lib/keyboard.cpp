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
		}else if (b == 49){
			char c = getch();
			char d = getch();
			char e = getch();

			if (c == 59 && d == 53 && e == 65){
				absolute = "CTRL-UpArrow";
			}else if (c == 59 && d == 53 && e == 66){
				absolute = "CTRL-DownArrow";
			}
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
		}else if (key == 12){
			absolute = "CTRLL";
		}else if (key == 6){
			absolute = "CTRLF";
		}else if (key == 11){
			absolute = "CTRLK";
		}else if (key == 18){
			absolute = "CTRLR";
		}else if (key == 9){
			absolute = "TAB";
		}else if (key == 20){
			absolute = "CTRLT";
		}else if (key == 21){
			absolute = "CTRLU";
		}else{
			absolute = key;
		}

		return absolute;
	}
}
