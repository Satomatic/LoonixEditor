#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

bool publicCommentMode = false;

// default colours used by program //
vector<vector<string>> colors = {
	{"statements", "\u001b[38;5;163m"},
	{"variables",  "\u001b[38;5;32m"},
	{"functions",  "\u001b[38;5;36m"},
	{"comments",   "\u001b[38;5;242;3m"},
	{"numbers",    "\u001b[38;5;81m"},
	{"strings",    "\u001b[38;5;214m"},
	{"chars",      "\u001b[38;5;135m"},
	{"tagged",     "\u001b[38;5;32m"}
};

extern string currentfile;
extern int curx;
extern int cury;

string replace_all(string text, string replace, string replacer){
	for (int i = 0; i < text.size(); i++){
		if (text.substr(i, replace.size()) == replace){
			text.replace(i, replace.size(), replacer);
			i += replacer.size();
		}
	}
	return text;
}

int find_str(string text, string finding){
	int results = 0;

	for (int i = 0; i < text.size(); i ++){
		if (text.substr(i, finding.size()) == finding){
			results ++;
		}
	}

	return results;
}

vector<string> split(string text, string splitchar){
	vector<string> returnVector;
	string currentword;
	
	for (int i = 0; i < text.size(); i++){
		if (text.substr(i, splitchar.size()) == splitchar){
			currentword += text[i];
			returnVector.push_back(currentword);
			currentword = "";
		}else{
			currentword += text[i];
		}
	}
	
	returnVector.push_back(currentword);
	return returnVector;
}

string getCommentString(){
	vector<vector<string>> comments = {
		{"lua", "--"},
		{"cpp", "//"},
		{"py", "#"},
		{"html", "<!--"},
		{"asm", ";"},
		{"sh", "#"}
	};
	
	string fileExt = currentfile.substr(currentfile.find_last_of(".") + 1);

	for (int i = 0; i < comments.size(); i++){
		if (comments[i][0] == fileExt){
			return comments[i][1];
		}
	}

	return "//";
}

string getColor(string color){
	for (int i = 0; i < colors.size(); i++){
		if (colors[i][0] == color){
			return colors[i][1];
		}
	}
	
	return "\u001b[0m";
}

bool loadTheme(string name){
	bool loaded = false;
	
	string configDir = "/home/";
	configDir += getenv("USER");
	configDir += "/.ledit";

	if (configDir == "/home/root/.ledit"){
		configDir = "/root/.ledit";
	}

	if (FileExists(configDir + "/themes/" + name)){
		ifstream file(configDir + "/themes/" + name);
		string line;
		while (getline(file, line)){
			vector<string> linesplit = split(line, ':');
			
			for (int i = 0; i < colors.size(); i++){
				if (colors[i][0] == linesplit[0]){
					colors[i][1] = "\u001b[" + linesplit[1];
					break;
				}
			}
		}
		file.close();
	}
	
	return loaded;
}

// add support for lexer file later on //
string syntaxLine(string line){
	string text = line;

	bool commentMode = false;
	bool stringMode = false;
	bool tagMode = false;
	bool charMode = false;

	// assembly stuff //
	vector<string> assembly_op = {"mov", "add", "sub", "inc", "call", "int"};
	vector<string> assembly_re = {"ax", "bx", "cx", "dx", "al", "bl", "cl", "dl", "ah", "bh", "ch", "dh", "di", "si"};
	
	// generic language stuff //
	vector<string> statements = {"if", "else", "return", "for", "while", "elif", "then", "end", "and", "break", "in", "elseif"};
	vector<string> variables = {"bool", "string", "int", "void", "class", "def", "function", "char", "do", "define", "ifndef", "endif", "try", "except", "catch", "const", "auto"};
	vector<string> functions = {"extern", "include", "vector", "from", "import", "export", "pair"};
	vector<string> tags = {
		"html", "body", "footer",
		"title", "link", "head", "script",
		"a", "p", "div", "img", "center",
		"h1", "h2", "h3", "h4", "h5", "h6"
	};

	// languages that should use tag based highlighting //
	vector<string> tagLanguages = {
		"html",
		"htm",
		"xhtml"

	};
	bool isTagged = false;
	string commentString = getCommentString();

	// get comment mode //
	string fileExt = currentfile.substr(currentfile.find_last_of(".") + 1);

	// check if tag language //
	for (int i = 0; i < tagLanguages.size(); i++){
		if (fileExt == tagLanguages[i]){
			isTagged = true;
			break;
		}
	}

	// if

	for (int i = 0; i < text.size(); i ++){
		if (commentMode == true){
		}else if (tagMode == true){
			if (text.substr(i, 1) == ">"){
				string replacer = "\u001b[0m>";
				text.replace(i, 1, replacer);
				i += replacer.size();
				tagMode = false;
			}

		}else if (charMode == true){
			if (text.substr(i, 1) == "'"){
				string replacer = "'\u001b[0m";
				text.replace(i, 1, replacer);
				i += replacer.size();
				charMode = false;
			}

		}else if (stringMode == true){
			if (text.substr(i, 1) == "\"" && text.substr(i - 1, 1) != "\\"){
				string replacer = "\"\u001b[0m";
				text.replace(i, 1, replacer);
				i += replacer.size();
				stringMode = false;
			}
		
		}else{
			if (fileExt == "asm" || fileExt == "ASM" || fileExt == "S" || fileExt == "s"){
				for (int b = 0; b < assembly_op.size(); b++){
					string keyword = assembly_op[b];
					string currentWord = text.substr(i, keyword.size());
					string upper;
					
					for (auto & c: keyword) upper += toupper(c);
					
					if (currentWord == keyword || currentWord == upper){
						string nextchar = text.substr(i + keyword.size(), 1);
						string prevchar = "";
						
						if (i > 0){prevchar = text.substr(i - 1, 1);}
						
						if (nextchar == " " || nextchar == ""){
							string replacer = getColor("variables") + currentWord + "\u001b[0m";
							text.replace(i, keyword.size(), replacer);
							i += replacer.size();
						}
					}
				}
				
				for (int b = 0; b < assembly_re.size(); b++){
					string keyword = assembly_re[b];
					string currentWord = text.substr(i, keyword.size());
					string upper;
					
					for (auto & c: keyword) upper += toupper(c);
					
					if (currentWord == keyword || currentWord == upper){
						string nextchar = text.substr(i + keyword.size(), 1);
						string prevchar = "";
						
						if (i > 0){prevchar = text.substr(i - 1, 1);}
						
						if (nextchar == " " || nextchar == "" || nextchar == "," || nextchar == "]" || nextchar == "+"){
							string replacer = getColor("statements") + currentWord + "\u001b[0m";
							text.replace(i, keyword.size(), replacer);
							i += replacer.size();
						}
					}
				}
			}
			
			for (int b = 0; b < statements.size(); b++){
				string keyword = statements[b];
				string currentWord = text.substr(i, keyword.size());
				string upper;
				
				for (auto & c: keyword) upper += toupper(c);

				if (currentWord == keyword || currentWord == upper){
					string nextchar = text.substr(i + keyword.size(), 1);
					string prevchar = "";

					if (i > 0){
						prevchar = text.substr(i - 1, 1);
					}

					if (nextchar == " " || nextchar == "(" || nextchar == "" || nextchar == "{" || nextchar == ";"){
						if (prevchar == " " || prevchar == "" || i == 0 || prevchar == "}" || prevchar == ")" || prevchar == ":"){
							string replacer = getColor("statements") + currentWord + "\u001b[0m";
							text.replace(i, keyword.size(), replacer);
							i += replacer.size();
						}
					}
				}
			}

			for (int b = 0; b < variables.size(); b++){
				string keyword = variables[b];
				string currentWord = text.substr(i, keyword.size());
				string upper;
				
				for (auto & c: keyword) upper += toupper(c);

				if (currentWord == keyword || currentWord  == upper){
					string nextchar = text.substr(i + keyword.size(), 1);
					string prevchar = "";

					if (i > 0){
						prevchar = text.substr(i - 1, 1);
					}

					if (nextchar == " " || nextchar == "(" || nextchar == "{" || nextchar == "*" || nextchar == ":" || nextchar == ""){
						if (prevchar == " " || prevchar == "" || prevchar == "(" || prevchar == "#" || prevchar == ":" || prevchar == "*" || i == 0){
							string replacer = getColor("variables") + currentWord + "\u001b[0m";
							
							text.replace(i, keyword.size(), replacer);
							i += replacer.size();
						}
					}
				}
			}

			for (int b = 0; b < functions.size(); b++){
				string keyword = functions[b];
				string currentWord = text.substr(i, keyword.size());
				string upper;
				
				for (auto & c: keyword) upper += toupper(c);

				if (currentWord == keyword || currentWord == upper){
					string prevchar = "";

					if (i > 0){
						prevchar = text.substr(i - 1, 1);
					}

					if (prevchar == " " || prevchar == "" || i == 0 || prevchar == "#" || prevchar == ":"){
						string replacer = getColor("functions") + currentWord + "\u001b[0m";
						text.replace(i, keyword.size(), replacer);
						i += replacer.size();
					}
				}
			}

			vector<string> numbers = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
			for (int b = 0; b < numbers.size(); b++){
				if (text.substr(i, 1) == numbers[b]){
					string replacer = getColor("numbers") + numbers[b] + "\u001b[0m";
					text.replace(i, 1, replacer);
					i += replacer.size() - 1;
					break;
				}
			}

			if (text.substr(i, 2) == "</"){
				if (isTagged == true){
					for (int b = 0; b < tags.size(); b++){
						if (text.substr(i + 2, tags[b].size()) == tags[b]){
							string replacer = "</\u001b[38;5;32m";
							replacer += text.substr(i + 2, tags[b].size());
							replacer += "\u001b[0m";
							
							text.replace(i, tags[b].size() + 2, replacer);
							i += replacer.size();
							break;
					   }
				   }
			   }
			}

			if (text.substr(i, 1) == "<"){
				if (isTagged == true){
					for (int b = 0; b < tags.size(); b++){
						if (i + 1 + tags[b].size() <= text.size() && text.substr(i + 1, tags[b].size()) == tags[b]){
							string replacer = "<" + getColor("tagged");
							replacer += text.substr(i + 1, tags[b].size());
							replacer += "\u001b[0m";
							
							text.replace(i, tags[b].size() + 1, replacer);
							i += replacer.size();
							tagMode = true;
						}
					}
				}

				if (
					find_str(line, "#include") >= 1 || 
					find_str(line, "vector<") >= 1 || 
					find_str(line, "vector <") >= 1 ||
					find_str(line, "pair<") >= 1 ||
					find_str(line, "pair <") >= 1
				
				){    
					string replacer = "<" + getColor("tagged");
					text.replace(i, 1, replacer);
					i += replacer.size();
					tagMode = true;
				}
			}

			if (text.substr(i, 1) == "'"){
				string replacer = getColor("chars") + "'";
				text.replace(i, 1, replacer);
				i += replacer.size();
				charMode = true;
			}

			if (text.substr(i, commentString.size()) == commentString){
				string replacer = getColor("comments");
				replacer += commentString;
				text.replace(i, commentString.size(), replacer);
				i += replacer.size();
				commentMode = true;
			}
			
			if (text.substr(i, 2) == "/*"){
				string replacer = "\u001b[38;5;242m\u001b[3m/*";
				text.replace(i, 2, replacer);
				i += replacer.size();
				publicCommentMode = true;
			}
			
			if (text.substr(i, 2) == "*/"){
				string replacer = "\u001b[0m*/";
				text.replace(i, 2, replacer);
				i += replacer.size();
				publicCommentMode = false;
			}

			if (text.substr(i, 1) == "\""){
				if (text.substr(i, 2) == "\"\""){
					string replacer = getColor("strings") + "\"\"\u001b[0m";
					text.replace(i, 2, replacer);
					i += replacer.size();
				}else{
					string replacer = getColor("strings") + "\"";
					text.replace(i, 1, replacer);
					i += replacer.size();
					stringMode = true;
				}
			}
		}
	}

	string returntext = "\u001b[0m" + text;
	return returntext;
}
