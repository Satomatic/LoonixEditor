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
			char e = ' ';
			
			if (c == 53 && d == 126){
				absolute = "F5";
			}else{
				e = getch();
			}

			if (c == 59 && d == 53 && e == 65){
				absolute = "CTRL-UpArrow";
			}else if (c == 59 && d == 53 && e == 66){
				absolute = "CTRL-DownArrow";
			}else if (c == 59 && d == 53 && e == 67){
				absolute = "CTRL-RightArrow";
			}else if (c == 59 && d == 53 && e == 68){
				absolute = "CTRL-LeftArrow";
			
			}else if (c == 59 && d == 51 && e == 67){
				absolute = "ALT-RightArrow";
			}else if (c == 59 && d == 51 && e == 68){
				absolute = "ALT-LeftArrow";
			
			}else if (c == 59 && d == 55 && e == 67){
				absolute = "CTRL-ALT-RightArrow";
			}else if (c == 59 && d == 55 && e == 68){
				absolute = "CTRL-ALT-LeftArrow";
			
			}else if (c == 59 && d == 50 && e == 65){
				absolute = "SHIFT+UpArrow";
			
			}else if (c == 59 && d == 50 && e == 66){
				absolute = "SHIFT-DownArrow";
			
			}else if (c == 59 && d == 50 && e == 67){
				absolute = "SHIFT-RightArrow";
			
			}else if (c == 59 && d == 50 && e == 68){
				absolute = "SHIFT-LeftArrow";
			}
		
		}else if (a == 'O' && b == 'P'){
			absolute = "F1";
		}else if (a == 'O' && b == '5'){
			absolute = "F5";
		}else{
			if (b == 53){
				char c = getch();
				if (c == 126){
					absolute = "PGUP";
				}
			}else if (b == 54){
				char c = getch();
				if (c == 126){
					absolute = "PGDN";
				}
			}else if (b == 50){
				char c = getch();
				if (c == 126){
					absolute = "Insert";
				}
			}else if (b == 51){
				char c = getch();
				if (c == 126){
					absolute = "Delete";
				}
			}
		}

		return absolute;

	}else{
		string absolute = "";

		if (key == 127){
			absolute = "Backspace";
		}else if (key == 8){
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
		}else if (key == 127){
			absolute = "CTRL?";
		}else if (key == 31){
			absolute = "CTRL/";
		}else if (key == 16){
			absolute = "CTRLP";
		}else if (key == 4){
			absolute = "CTRLD";
		}else{
			absolute = key;
		}

		return absolute;
	}
}
