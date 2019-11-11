#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

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

// add support for lexer file later on //
string syntaxLine(string line){
	string text = "\u001b[0m" + line;

	bool commentMode = false;
	bool stringMode = false;

	vector<string> statements = {"if", "else", "return", "for"};
	vector<string> variables = {"bool", "string", "int"};
	vector<string> functions = {"extern", "include"};

	for (int i = 0; i < text.size(); i ++){
		if (commentMode == true){
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
					string replacer = "\u001b[38;5;163m" + keyword + "\u001b[0m";
					text.replace(i, keyword.size(), replacer);
					i += replacer.size();
				}
			}

			for (int b = 0; b < variables.size(); b++){
				string keyword = variables[b];

				if (text.substr(i, keyword.size()) == keyword){
					string replacer = "\u001b[38;5;32m" + keyword + "\u001b[0m";
					text.replace(i, keyword.size(), replacer);
					i += replacer.size();
				}
			}

			for (int b = 0; b < functions.size(); b++){
				string keyword = functions[b];

				if (text.substr(i, keyword.size()) == keyword){
					string replacer = "\u001b[38;5;36m" + keyword + "\u001b[0m";
					text.replace(i, keyword.size(), replacer);
					i += replacer.size();
				}
			}

			if (text.substr(i, 2) == "//"){
				string replacer = "\u001b[38;5;242m\u001b[3m//";
				text.replace(i, 2, replacer);
				i += replacer.size();
				commentMode = true;
			}

			if (text.substr(i, 1) == "\""){
				string replacer = "\u001b[38;5;214m\"";
				text.replace(i, 1, replacer);
				i += replacer.size();
				stringMode = true;
			}
		}
	}

	// standard key words //
	/*vector<string> keywords = {"if", "else", "return", "for"};

	for(int i=0; i < keywords.size(); i++){
		string keyword = keywords[i];
		text = replace_all(text, keyword, "\u001b[38;5;163m" + keyword + "\u001b[0m");
	}

	// variable types //
	vector<string> variables = {"bool", "string", "int"};
	for (int i=0; i < variables.size(); i++){
		string keyword = variables[i];
		text = replace_all(text, keyword, "\u001b[38;5;32m" + keyword + "\u001b[0m");
	}

	// function names //
	vector<string> functions = {"include", "extern"};
	for (int i = 0; i < functions.size(); i++){
		string keyword = functions[i];
		text = replace_all(text, keyword, "\u001b[38;5;36m" + keyword + "\u001b[0m");
	}

	// comments //
	string keyword = "/";
	keyword += "/";
	text = replace_all(text, keyword, "\u001b[0m\u001b[38;5;242m\u001b[3m" + keyword);
	*/

	return text;
}
