#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern vector<string> viewport;
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

class HeaderDrop{
	public:
		string message;
		string previous;
		string styling;
		int count = 0;

		bool showing = false;

	void draw(){
		int pos;

		if (showing == true){
			pos = int(screenWidth / 2) - int(previous.size() / 2) - 1;
			setCursorPosition(pos, 1);
			cout << "\u001b[0m";
			for (int i = 0; i < previous.size() + 4; i++){
				setCursorPosition(pos + i, 1);
				cout << " ";
			}
		}

		pos = int(screenWidth / 2) - int(message.size() / 2) - 1;

		setCursorPosition(pos, 1);
		cout << "\u001b[0m[ " << styling << message << "\u001b[0m ]";

		previous = message;

		showing = true;
		count = 0;
	}

	void undraw(){
		int pos = int(screenWidth / 2) - int(message.size() / 2) - 1;
		resetColor();

		for (int i = 0; i < message.size() + 4; i++){
			setCursorPosition(pos + i, 1);
			cout << " ";
		}

		showing = false;
		
		setCursorPosition(0, 1);
		cout << viewport[1];
	}

};

class Box{
	public:
		int posx = 0;
		int posy = 0;
		int width = 0;
		int height = 0;

		string title = "";
		string message = "";
		string escape = "";

		string footer = "";
		string footerescape = "";

		bool showing = true;
		bool vcenter = false;
		bool hcenter = false;
		bool center = false;
		bool centerText = false;

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

			// draw footer //
			if (footer != ""){
				int titlepos = int(width / 2) - int(footer.size() / 2) + posx;
				setCursorPosition(titlepos, posy + height + 1);
				cout << "┤" << footerescape << footer << "\u001b[0m├";
			}

			// draw message //
			if (message != ""){
				int xoff = 0;
			
				vector<string> messagesplit = split(message, '\\');
				for (int i = 0; i < messagesplit.size(); i++){
					if (centerText == true){
						xoff = int(width / 2) - int(messagesplit[i].size() / 2);
					}
					
					setCursorPosition(posx + 1 + xoff, posy + 1 + i);
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

class OptionDialog{
	public:
		string title = "";
		string message = "";
		int width = 26; // default width and height
		int height = 3;
		int selected = 0;
		bool centerText = false;
		vector<string> items;
		
		Box messageContainer;

	void draw(){
		messageContainer.title = title;
		messageContainer.message = message;
		messageContainer.width = width;
		messageContainer.height = height;
		messageContainer.center = true;
		messageContainer.centerText = centerText;
		messageContainer.draw();
		updateInput();
		
		while (true){
			string key = getInput();

			if (key == "LeftArrow"){
				if (selected == 0){
					selected = items.size() - 1;
				}else{
					selected -= 1;
				}

			}else if (key == "RightArrow"){
				if (selected == items.size() - 1){
					selected = 0;
				}else{
					selected += 1;
				}
			
			}else if (key == "Return"){
				undraw();
				break;
			}
			
			updateInput();
		}
	}
	
	void updateInput(){
		int curPos = messageContainer.posx + messageContainer.width / 2 - 1;
		int diff = 0;
		
		for (int i = 0; i < items.size(); i++){
			diff += items[i].size();
		}
		
		curPos -= (diff / 2);
		
		setCursorPosition(curPos, messageContainer.posy + messageContainer.height);
		
		for (int i = 0; i < items.size(); i++){
			if (i == selected){
				cout << "\u001b[107;30;4m";
			}else{
				cout << "\u001b[4m";
			}
		
			for (int b = 0; b < items[i].size(); b++){
				if (b == 0){
					cout << items[i][b] << "\u001b[24m";
				}else{
					cout << items[i][b];
				}
			}
			
			cout << "\u001b[0m  ";

		}    
	}
	
	void undraw(){
		messageContainer.undraw();
	}
};

class Input{
	public:
		string input = "";
		string placeholder = "";
		int cursorpos = 0;
		int cursorpre = 0;
		
		string background = "";
		string foreground = "";
	
		int x = 0;
		int y = 0;
		int maxx = 10;
		int scroll = 0;
	
		bool border = false;
		bool showing = true;
		bool center = false;
		
		string title = "";
		string style = "";
		string closeMessage = "";
		string prefix = "";

		vector <string> customClose;

	void init(){
		// center pos //
		if (center == true){
			int width = maxx + prefix.size() + 1;
			int pos = int(screenWidth / 2) - int(width / 2) + 1;
			x = pos;
			
			pos = int(screenHeight / 2);
			y = pos;
		}

		// draw border //
		if (border == true){
			Box container;
			container.posx = x - 1;
			container.posy = y - 1;
			container.width = maxx + prefix.size() + 1;
			container.height = 1;

			if (title != ""){
				container.title = title;
				container.escape = style;
			}

			container.draw();
		}

		update();
		
		while (showing){
			// accept input //
			string key = getInput();
	
			for (int i = 0; i < customClose.size(); i++){
				if (key == customClose[i]){
					input = closeMessage;
					showing = false;
					break;
				}
			}

			if (key == "RightArrow"){
	
			}else if (key == "LeftArrow"){
				
			}else if (key == "Backspace"){
				input = input.substr(0, input.size() - 1);

			}else if (key == "Return"){
				showing = false;

			}else if (key == "CTRLX"){
				input = closeMessage;
				showing = false;
			
			}else{
				if (key.size() == 1){
					input += key;
				}
			}
	
			update();
		}
	}
	
	void update(){
		// display input //
		string displaytext;
	
		if (input.size() > maxx){
			displaytext = input.substr(input.size() - maxx, maxx);
			cursorpos = maxx;
		}else{
			displaytext = input;
			cursorpos = input.size();
		}
	
		setCursorPosition(x, y);
		cout << "\u001b[0m" << prefix << background << foreground << displaytext << " ";

		if (input.size() < maxx){
			cout << " ";
		}
		
		// draw cursor //
		setCursorPosition(x + cursorpos + prefix.size(), y);
		cout << "\u001b[107;30m ";
	
		cursorpre = cursorpos;
	}

	void undrawCursor(){
		setCursorPosition(x + cursorpos + prefix.size(), y);
		cout << "\u001b[0m ";
	}

	void undraw(){
		cout << "\u001b[0m";
		
		for (int cx = 0; cx < maxx + 3 + prefix.size(); cx++){
			for (int cy = 0; cy < 3; cy++){
				setCursorPosition(x + cx - 1, y + cy - 1);
				cout << " ";
			}
		}
	}
};
