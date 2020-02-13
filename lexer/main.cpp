#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

bool publicCommentMode = false;

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

// add support for lexer file later on //
string syntaxLine(string line){
	string text = line;

	bool commentMode = false;
	bool stringMode = false;
	bool tagMode = false;
	bool charMode = false;

	vector<string> statements = {"if", "else", "return", "for", "while", "elif", "then", "end"};
	vector<string> variables = {"bool", "string", "int", "void", "class", "def", "function"};
	vector<string> functions = {"extern", "include", "vector", "from", "import"};
	vector<string> tags = {
		"html", "body", "footer",
		"title", "link", "head", "script",
		"a", "p", "div", "img", "center",
		"h1", "h2", "h3", "h4", "h5", "h6"
	};

	vector<string> tagLanguages = {
		"html",
		"php",
		"htm",
		"xhtml"
	};

	vector<vector<string>> comments = {
		{"lua", "--"},
		{"cpp", "//"},
		{"py", "#"},
		{"html", "<!--"}
	};

	bool isTagged = false;
	string commentString = "//"; // default to '//' if language not found

	// get comment mode //
	string fileExt = currentfile.substr(currentfile.find_last_of(".") + 1);

	// check if tag language //
	for (int i = 0; i < tagLanguages.size(); i++){
		if (fileExt == tagLanguages[i]){
			isTagged = true;
			break;
		}
	}

	for (int i = 0; i < comments.size(); i++){
		if (comments[i][0] == fileExt){
			commentString = comments[i][1];
		}
	}

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
			for (int b = 0; b < statements.size(); b++){
				string keyword = statements[b];

				if (text.substr(i, keyword.size()) == keyword && isTagged == false){
					string nextchar = text.substr(i + keyword.size(), 1);
					string prevchar = "";

					if (i > 0){
						prevchar = text.substr(i - 1, 1);
					}

					if (nextchar == " " || nextchar == "(" || nextchar == "" || nextchar == "{" || nextchar == ";"){
						if (prevchar == " " || prevchar == "" || i == 0 || prevchar == "}" || prevchar == ")"){
							string replacer = "\u001b[38;5;163m" + keyword + "\u001b[0m";
							text.replace(i, keyword.size(), replacer);
							i += replacer.size();
						}
					}
				}
			}

			for (int b = 0; b < variables.size(); b++){
				string keyword = variables[b];

				if (text.substr(i, keyword.size()) == keyword){
					string nextchar = text.substr(i + keyword.size(), 1);
					string prevchar = "";

					if (i > 0){
						prevchar = text.substr(i - 1, 1);
					}

					if (nextchar == " " || nextchar == "(" || nextchar == "{"){
						if (prevchar == " " || prevchar == "" || prevchar == "(" || i == 0){
							string replacer = "\u001b[38;5;32m" + keyword + "\u001b[0m";
							text.replace(i, keyword.size(), replacer);
							i += replacer.size();
						}
					}
				}
			}

			for (int b = 0; b < functions.size(); b++){
				string keyword = functions[b];

				if (text.substr(i, keyword.size()) == keyword){
					string prevchar = "";

					if (i > 0){
						prevchar = text.substr(i - 1, 1);
					}

					if (prevchar == " " || prevchar == "" || i == 0 || prevchar == "#"){
						string replacer = "\u001b[38;5;36m" + keyword + "\u001b[0m";
						text.replace(i, keyword.size(), replacer);
						i += replacer.size();
					}
				}
			}

			vector<string> numbers = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
			for (int b = 0; b < numbers.size(); b++){
				if (text.substr(i, 1) == numbers[b]){
					string replacer = "\u001b[38;5;81m" + numbers[b] + "\u001b[0m";
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
						if (text.substr(i + 1, tags[b].size()) == tags[b]){
							string replacer = "<\u001b[38;5;32m";
							replacer += text.substr(i + 1, tags[b].size());
							replacer += "\u001b[0m";
							
							text.replace(i, tags[b].size() + 1, replacer);
							i += replacer.size();
							//tagMode = true;
						}
					}
				}

				if (find_str(line, "#include") >= 1 || find_str(line, "vector<") >= 1 || find_str(line, "vector <") >= 1){
					string replacer = "<\u001b[38;5;32m";
					text.replace(i, 1, replacer);
					i += replacer.size();
					tagMode = true;
				}
			}

			if (text.substr(i, 1) == "'"){
				string replacer = "\u001b[38;5;135m'";
				text.replace(i, 1, replacer);
				i += replacer.size();
				charMode = true;
			}

			if (text.substr(i, commentString.size()) == commentString){
				string replacer = "\u001b[38;5;242m\u001b[3m";
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
					string replacer = "\u001b[38;5;214m\"\"\u001b[0m";
					text.replace(i, 2, replacer);
					i += replacer.size();
				}else{
					string replacer = "\u001b[38;5;214m\"";
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
