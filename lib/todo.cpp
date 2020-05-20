#include <iostream>
#include <vector>
#include <string>

using namespace std;

extern vector<string> raw;
extern int index;
extern int cury;
extern int curx;

class Todo{
	public:
		vector<vector<string>> results = {};
		vector<string> flags = {"TODO:", "FIX:", "BUG:", "DONE:"};
		vector<string> view;
		
		int posy = 0;
		int scroll = 0;
		
		Box TodoContainer;
		ScrollBar scrollBar;

	void init(){
		TodoContainer.title = "Todo";
		TodoContainer.titleAlign = TodoContainer.CENTER;
		TodoContainer.width = 40;
		TodoContainer.height = 10;
		TodoContainer.center = true;
		TodoContainer.draw();

		scrollBar.height = 10;
		scrollBar.x = TodoContainer.posx + TodoContainer.width;
		scrollBar.y = TodoContainer.posy + 1;
		scrollBar.size = 0;
		scrollBar.position = 0;
		scrollBar.draw();

		// search for flags //
		for (int y = 0; y < raw.size(); y++){
			for (int x = 0; x < raw[y].size(); x++){
				for (int i = 0; i < flags.size(); i++){
					if (raw[y].substr(x, flags[i].size()) == flags[i]){
						vector<string> flagmessage = splitIndex(raw[y], x + flags[i].size() + 1);
						string flag = flags[i].substr(0, flags[i].size() - 1);
						
						results.push_back({flagmessage[1], flag, to_string(x), to_string(y)});
					}
				}
			}
		}
		
		if (results.size() == 0){
			string message = "No results found";
			int x = centerWidth(message, TodoContainer.width + 1);
			
			setCursorPosition(TodoContainer.posx + x + 1, TodoContainer.posy + 1);
			cout << "\u001b[0;1m" << message;
			
			while (true){
				string key = getInput();
				
				if (key == "CTRLX"){
					break;
				}
			}
			
			TodoContainer.undraw();
			return;
		}
		
		// display results //
		updateView();
		while (true){
			scrollBar.size = results.size();
			scrollBar.draw();
			
			string key = getInput();
			
			if (key == "DownArrow"){
				scrollBar.position = scroll;
				
				if (scroll + posy < results.size() - 1){ // if not a bottom of list //
					if (posy == TodoContainer.height - 1){ // if at bottom of view //
						scroll ++;
					}else{
						posy ++;
					}
				}
				
			}else if (key == "UpArrow"){
				scrollBar.position = scroll;
				
				if (scroll + posy != 0){
					if (posy == 0){
						scroll --;
					}else{
						posy --;
					}
				}

			}else if (key == "Return"){
				vector<string> currentLine = results[scroll + posy];
				
				int xcoord = stoi(currentLine[2]);
				int ycoord = stoi(currentLine[3]);
				
				index = ycoord - 1;
				cury = 1;
				curx = xcoord;
			
				if (testViewport() < screenHeight - 1 && index >= screenHeight){
					int difference = screenHeight - 1 - testViewport();
					index -= difference;
					cury += difference;
					
				}else if (index <= screenHeight){
					index = 0;
					cury = ycoord;
				}
			
				refresh();
				updateCursor();
				updateHeader();
				
				TodoContainer.draw();
			
			}else if (key == "CTRLX"){
				break;
			}
		
			updateView();
		}
		
		TodoContainer.undraw();
	}
	
	void updateView(){
		// update viewport //
		view.clear();
		for (int i = 0; i < results.size(); i++){
			if (i >= scroll && i <= scroll + TodoContainer.height - 1){
				string linedata = "[" + results[i][1] + "]{" + results[i][2] + "," + results[i][3] + "} ";
				
				int gap = 0;
				gap = TodoContainer.width - linedata.size() - results[i][0].size();
				
				for (int b = 0; b < gap - 2; b++){
					linedata += " ";
				}
				
				linedata += results[i][0];
				
				if (linedata.size() > TodoContainer.width){
					linedata = linedata.substr(0, TodoContainer.width - 4);
					linedata += "...";
				}
				
				
				view.push_back(linedata);
			}
		}
		
		// display viewport //
		for (int i = 0; i < view.size(); i++){
			setCursorPosition(TodoContainer.posx + 1, TodoContainer.posy + 1 + i);
			if (i == posy){
				cout << "\u001b[107;30m" << view[i] << "\u001b[0m ";
			}else{
				cout << view[i];
			}
		}
		// TODO: This is a test todo
	}
};
