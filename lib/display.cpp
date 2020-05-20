#include <iostream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

extern vector<string> viewport;
extern int screenWidth;
extern int screenHeight;
extern int XOffset;

class HeaderDrop{
	public:
		string message;
		string previous;
		string styling;
		int count = 0;

		bool showing = false;

	void draw(){
		showing = true;
		count = 0;
	
		update();
	}

	void update(){
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
	}

	void undraw(){
		int pos = int(screenWidth / 2) - int(message.size() / 2) - 1;
		resetColor();

		for (int i = 0; i < message.size() + 4; i++){
			setCursorPosition(pos + i, 1);
			cout << " ";
		}

		showing = false;
		
		setCursorPosition(XOffset, 1);
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
		bool nofill = false;

		enum align{
			LEFT,
			RIGHT,
			CENTER,
			VERTICAL,
			HORIZONTAL
		};
		
		align titleAlign;
		align footerAlign;

		vector<pair<int, align>> seperators;
		
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
			if (nofill == true){
				for (int i = 0; i < height; i++){
					setCursorPosition(posx, posy + i + 1);
					cout << "│";
					setCursorPosition(posx + width + 1, posy + i + 1);
					cout << "│";
				}
			}else{
			
				string linefill = "";
				for (int i = 0; i < width; i++){
					linefill += " ";
				}

				for (int i = 0; i < height; i++){
					setCursorPosition(posx, posy + i + 1);
					cout << "│" << linefill << "│";
				}
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
				int titlepos;
				
				if (titleAlign == LEFT){
					titlepos = posx + 1;
				
				}else if (titleAlign == RIGHT){
					titlepos = posx + width - title.size() - 1;
				
				}else{
					titlepos = int(width / 2) - int(title.size() / 2) + posx;
				}
			
				setCursorPosition(titlepos, posy);
				cout << "┤" << escape << title << "\u001b[0m├";
			}

			// draw footer //
			if (footer != ""){
				int titlepos;
	
				if (footerAlign == LEFT){
					titlepos = posx + 1;
				
				}else if (footerAlign == RIGHT){
					titlepos = posx + width - footer.size() - 1;
				
				}else{
					titlepos = int(width / 2) - int(footer.size() / 2) + posx;
				}

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
			
			// draw seperators //
			for (int i = 0; i < seperators.size(); i++){
				drawSeperator(seperators[i].second, seperators[i].first);
			}
			
			showing = true;
		}

		void undraw(){
			resetColor();
			for (int i = 0; i < height + 2; i++){
				setCursorPosition(posx, posy + i);
				for (int b = 0; b < width + 2; b++){
					cout << " ";
				}
			}

			newRefresh();
			drawHeader();

			setCursorPosition(0,0);
			showing = false;
		}
		
		void addSeperator(align anchor, int position){
			pair<int, align> spair;
			
			spair.first = position;
			spair.second = anchor;
			
			seperators.push_back(spair);
		}
		
		void drawSeperator(align anchor, int position){
			int pos;
			
			vector<string> chars = {
				"┬",
				"┴",
				"│",
				"┤",
				"├",
				"─",
			};
			
			if (anchor == VERTICAL){
				pos = posx + width - position;
			
				// draw connectors //
				setCursorPosition(pos, posy);
				cout << chars[0];
				setCursorPosition(pos, posy + height + 2);
				cout << chars[1];
				
				// draw filler //
				for (int i = 0; i < height; i++){
					setCursorPosition(pos, posy + i + 1);
					cout << chars[2];
				}
			
			}else if (anchor == HORIZONTAL){
				pos = posy + position;
				
				// draw connectors //
				setCursorPosition(posx, pos);
				cout << chars[4];
				setCursorPosition(posx + width + 1, pos);
				cout << chars[3];
				
				// draw filler //
				for (int i = 0; i < width; i++){
					setCursorPosition(posx + i + 1, pos);
					cout << chars[5];
				}
			}
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
		int height = 1;
	
		bool border = false;
		bool showing = true;
		bool center = false;
		
		string title = "";
		string style = "";
		string closeMessage = "";
		string prefix = "";

		vector <string> customClose;
		
		Box container;

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
			container.posx = x - 1;
			container.posy = y - 1;
			container.width = maxx + prefix.size() + 1;
			container.height = height;
			container.titleAlign = container.CENTER;

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
		cout << "\u001b[107;30m \u001b[0m";
	
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

class ScrollBar{
	public:
		int height = 10;
		int size = 30;
		int position = 10;
		int x = 0;
		int y = 0;
		
	void draw(){
		cout << "\u001b[0m";

		// draw bar background //
		for (int i = 0; i < height; i++){
			setCursorPosition(x, y + i);
			cout << "\u001b[38;5;242m" << "░";
		}
		
		if (size <= height){return;}
		
		// get scroll bar size //
		double a = (double)height / 100;
		double b = (double)100 / size;
		double c = (double)height * b;
		double d = (double)a * c;
		
		if (ceil(d) > height){
			d = height;
		}
		
		// get position //
		double e = (double)height / 100;
		double si = (double)((double)100 / (double)size);
		double f = (double)( ((double)100 / (double)size) * position );
		double g = (double)e * f;
		
		if (ceil(g) > height - ceil(d)){
			g = height - ceil(d);
		}
		
		cout << "\u001b[0m";
		for (int i = 0; i < ceil(d); i++){
			setCursorPosition(x, y + i + ceil(g));
			cout << "░";
		}
	}
};

