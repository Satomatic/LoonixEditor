#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern int screenWidth;
extern int screenHeight;

void drawBox(int x, int y, int w, int h, string title = "", string msg="", bool vcenter=false, bool hcenter=false, string escapecode=""){
	h = h - 2;

	if (hcenter == true){
		int pos = int(screenWidth / 2) - int(w / 2) + 1;
		x = pos;
	}

	if (vcenter == true){
		int pos = int(screenHeight / 2) - int(h / 2);
		y = pos;
	}

	setCursorPosition(x,y);
	resetColor();
	string linetop = "";
	for (int i = 0; i < w; i++){
		linetop += "─";
	}

	cout << "┌" << linetop << "┐";

	if (title != ""){
		int titlepos = int(w / 2) - int(title.size() / 2) + x;
		setCursorPosition(titlepos, y);
		cout << "┤" << escapecode << title << "\u001b[0m├";
	}

	string linefill = "";
	for (int i = 0; i < w; i++){
		linefill += " ";
	}

	for (int i = 0; i < h; i++){
		setCursorPosition(x, y + i + 1);
		cout << "│" << linefill << "│";
	}

	string bottomline = "";
	for (int i = 0; i < w; i++){
		bottomline += "─";
	}

	setCursorPosition(x, y + h + 1);
	cout << "└" << bottomline << "┘";

	if (msg != ""){
		int yoff = 0;
		int xoff = 0;
		for (int i = 0; i < msg.size(); i++){
			setCursorPosition(x + 1 + xoff, y + yoff + 1);
			cout << msg[i];

			if (xoff == w){
				xoff = 0;
				yoff ++;
			}else{
				xoff ++;
			}
		}
	}
}

class Box{
	public:
		int posx = 0;
		int posy = 0;
		int width = 0;
		int height = 0;

		string title = "";
		string message = "";
		string escape = "";

		bool showing = true;
		bool vcenter = false;
		bool hcenter = false;
		bool center = false;

		void draw(){
			// center //
		    if (hcenter == true || center == true){
		        int pos = int(screenWidth / 2) - int(width / 2) + 1;
		        posx = pos;
		    }

		    if (vcenter == true || center == true){
		        int pos = int(screenHeight / 2) - int(height / 2);
		        posy = pos - 1;
		    }

			// draw top bar //
    		setCursorPosition(posx, posy);
    		resetColor();
    		string linetop = "";
    		for (int i = 0; i < width; i++){
    		    linetop += "─";
    		}
    		cout << "┌" << linetop << "┐";

			// draw mid bars //
    		string linefill = "";
    		for (int i = 0; i < width; i++){
    		    linefill += " ";
   			}

    		for (int i = 0; i < height; i++){
    		    setCursorPosition(posx, posy + i + 1);
    		    cout << "│" << linefill << "│";
		    }

			// draw bottom line //
			string bottomline = "";
    		for (int i = 0; i < width; i++){
    		    bottomline += "─";
    		}

		    setCursorPosition(posx, posy + height + 1);
		    cout << "└" << bottomline << "┘";

			// draw title //
			if (title != ""){
		        int titlepos = int(width / 2) - int(title.size() / 2) + posx;
		        setCursorPosition(titlepos, posy);
		        cout << "┤" << escape << title << "\u001b[0m├";
		    }

			// draw message //
		    if (message != ""){
		        int yoff = 0;
		        int xoff = 0;

				vector<string> messagesplit = split(message, '\\');
				for (int i = 0; i < messagesplit.size(); i++){
					setCursorPosition(posx + 1, posy + 1 + i);
					cout << messagesplit[i];
				}
		    }
			showing = true;
		}

		void undraw(){
			resetColor();
			for (int i = 0; i < height + 2; i++){
				for (int b = 0; b < width + 2; b++){
					setCursorPosition(posx + b, posy + i);
					cout << " ";
				}
			}
			setCursorPosition(0,0);
			showing = false;
		}
};
