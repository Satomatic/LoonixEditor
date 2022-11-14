#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <algorithm>
#include "lexerdata.cpp"
extern string currentfile;

// will load the theme from ~/.ledit/theme by theme name
void loadTheme(std::string name){
	std::string homepath = "/home/"; // this changes on mac
	std::string configDir = homepath + getenv("USER") + "/.ledit/";
	
	// check if user is root
	if (configDir == "/home/root/.ledit/"){
		configDir = "/root/.ledit/";
	}
	
	if (FileExists(configDir + "/themes/" + name)){
		std::ifstream file(configDir + "/themes/" + name);
		std::string line;
		while (getline(file, line)){
			std::vector<std::string> linesplit = split(line, ':');
			
			for (int i = 0; i < colors.size(); i++){
				if (colors[i][0] == linesplit[0]){
					colors[i][1] = "\033[" + linesplit[1];
					break;
				}
			}
		}
		file.close();
	}
}

// gets the file extension and gets the comment string assigned
std::string getCommentString(){
	std::string fileExt = currentfile.substr(currentfile.find_last_of(".") + 1);
	for (int i = 0; i < comments.size(); i++){
		if (comments[i][0] == fileExt){
			return comments[i][1];
		}
	}
	return "//";
}

std::string getColor(std::string color){
	for (int i = 0; i < colors.size(); i++){
		if (colors[i][0] == color){
			return colors[i][1];
		}
	}
	return "\033[0m";
}

bool charContains(string character){
	bool found = false;
	for (int i = 0; i < extraChars.size(); i++){
		if (extraChars[i] == character){
			found = true;
			break;
		}
	}
	return found;
}

std::string syntaxLine(std::string line){
	std::string fileExt = currentfile.substr(currentfile.find_last_of(".") + 1);
	std::string commentString = getCommentString();
	std::string text = line;
	
	bool commentMode = false;
	bool stringMode = false;
	bool charMode = false;
	bool tagMode = false;
	
	for (int i = 0; i < text.size(); i++){
		if (commentMode == true) continue;
		
		if (tagMode == true){
			if (text.substr(i, 1) == ">"){
				std::string replacer = "\033[0m>";
				text.replace(i, 1, replacer);
				i += replacer.size();
				tagMode = false;
			}
			
		}else if (charMode == true){
			if (text.substr(i, 1) == "'"){
				std::string replacer = "'\033[0m";
				text.replace(i, 1, replacer);
				i += replacer.size();
				charMode = false;
			}
		
		}else if (stringMode == true){
			if (text.substr(i ,1) == "\"" && text.substr(i - 1, 1) != "\\"){
				std::string replacer = "\"\u001b[0m";
				text.replace(i, 1, replacer);
				i += replacer.size();
				stringMode = false;
			}
		}else{
			if (
				to_lower(fileExt) == "asm" ||
				to_lower(fileExt) == "s"
			){
				//
				// Assembly Syntaxing
				//
				for (int y = 0; y < assembly_keywords.size(); y++){
					// for keyword in section //
					for (int x = 0; x < assembly_keywords[y].size(); x++){
						std::string keyword = assembly_keywords[y][x];
						std::string current = text.substr(i, keyword.size());
						std::string upper = to_upper(keyword);
						
						if (current == keyword || current == upper){
							std::string nchar = text.substr(i + keyword.size(), 1);
							std::string pchar;
							
							if (i > 0) pchar = text.substr(i - 1, 1);
							
							bool p_found = charContains(pchar);
							bool n_found = charContains(nchar);
							
							if (p_found && n_found){
								std::string color = getColor(assembly_colors[y]);
								std::string replacer = color + current + "\033[0m";
								text.replace(i, keyword.size(), replacer);
								i += replacer.size();
							}
						}
					}
				}
			}else{
				//
				// Standard Syntaxing
				//
				for (int y = 0; y < generic_keywords.size(); y++){
					for (int x = 0; x < generic_keywords[y].size(); x++){
						std::string keyword = generic_keywords[y][x];
						std::string current = text.substr(i, keyword.size());
						std::string upper = to_upper(keyword);
						
						if (current == keyword || current == upper || line.size() >= 4 && current.substr(0, 4) == "uint"){
							std::string nchar = text.substr(i + keyword.size(), 1);
							std::string pchar;
							
							if (i > 0) pchar = text.substr(i - 1, 1);
							
							bool p_found = charContains(pchar);
							bool n_found = charContains(nchar);
							
							if (p_found && n_found){
								std::string color = colors[y][1];
								std::string replacer = color + current + "\033[0m";
								text.replace(i, keyword.size(), replacer);
								i += replacer.size();
							}
						}
					}
				}
			}
			
			//
			// Boolean Syntaxing
			//
			bool bool_type = 0;
			if (to_lower(text.substr(i, 5)) == "false"){
				bool_type = 1;
			}
			
			std::string keyword = text.substr(i, 4 + bool_type);
			if (to_lower(keyword) == "true" || to_lower(keyword) == "false"){
				std::string nchar = text.substr(i + keyword.size(), 1);
				std::string pchar;
				if (i > 0) pchar = text.substr(i - 1, 1);
				
				bool p_found = charContains(pchar);
				bool n_found = charContains(nchar);
				
				if (p_found && n_found){
					std::string color = getColor("numbers");
					std::string replacer = color + keyword + "\033[0m";
					text.replace(i, keyword.size(), replacer);
					i += replacer.size();
				}
			}
			
			//
			// Number Syntaxing
			//
			std::string numbers = "1234567890";
			std::string cchar = text.substr(i, 1);
			if (numbers.find(cchar) != string::npos) {
				std::string color = getColor("numbers");
				std::string replacer = color + cchar + "\033[0m";
				text.replace(i, 1, replacer);
				i += replacer.size() - 1;
			}
			
			//
			// Comment Syntaxing
			//
			if (text.substr(i, commentString.size()) == commentString){
				std::string replacer = getColor("comments");
				replacer += commentString;
				text.replace(i, commentString.size(), replacer);
				i += replacer.size();
				commentMode = true;
			}
			
			//
			// Tag Syntaxing
			//
			if (cchar == "<"){
				if (
					// i know this looks kinda ugly, sorry //
					line.find("#include") != string::npos ||
					line.find("vector<")  != string::npos ||
					line.find("vector <") != string::npos ||
					line.find("pair<")    != string::npos ||
					line.find("pair <")   != string::npos
				){
					std::string replacer = "<" + getColor("tagged");
					text.replace(i, 1, replacer);
					i += replacer.size();
					tagMode = true;
				}
			}
			
			//
			// String Syntaxing
			//
			if (cchar == "\""){
				if (text.substr(i, 2) == "\"\""){
					std::string replacer = getColor("strings") + "\"\"\033[0m";
					text.replace(i, 2, replacer);
					i += replacer.size();
				}else{
					std::string replacer = getColor("strings") + "\"";
					text.replace(i, 1, replacer);
					i += replacer.size();
					stringMode = true;
				}
			}
			
			//
			// Char Syntaxing
			//
			if (cchar == "'"){
				std::string replacer = getColor("chars") + "'";
				text.replace(i, 1, replacer);
				i += replacer.size();
				charMode = true;
			}
		}
	}

	return "\u001b[0m" + text;
}
