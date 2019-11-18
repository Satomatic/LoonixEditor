#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

bool publicCommentMode = false;

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

// add support for lexer file later on //
string syntaxLine(string line){
	string text = line;

	bool commentMode = false;
	bool stringMode = false;
	bool tagMode = false;
	bool charMode = false;

	vector<string> statements = {"if", "else", "return", "for", "while"};
	vector<string> variables = {"bool", "string", "int", "void", "class"};
	vector<string> functions = {"extern", "include", "vector"};

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

				if (text.substr(i, keyword.size()) == keyword){
					string nextchar = text.substr(i + keyword.size(), 1);
					string prevchar = "";

                    if (i > 0){
                        prevchar = text.substr(i - 1, 1);
					}

					if (nextchar == " " || nextchar == "(" || nextchar == "" || nextchar == "{"){
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
						if (prevchar == " " || prevchar == "" || i == 0){
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
					i += replacer.size();
				}
			}

			if (text.substr(i, 1) == "<"){
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

			if (text.substr(i, 2) == "//"){
				string replacer = "\u001b[38;5;242m\u001b[3m//";
				text.replace(i, 2, replacer);
				i += replacer.size();
				commentMode = true;
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
