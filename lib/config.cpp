#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class ConfigManager{
	public:
		vector<vector<string>> config;
		string configDir;
		string mainFile;
		string user;

	void checkConfig(){
		configDir = "/Users/X/.ledit";
		configDir = replace_all(configDir, "X", getenv("USER"));
	
		if (configDir == "/home/root/.ledit"){
			configDir = "/root/.ledit";
		}
		
		// check config dir //
		if (!DirectoryExists(configDir.c_str())){
			string command = "mkdir ";
			command += configDir;
			system(command.c_str());
			
			command = "mkdir ";
			command += configDir;
			command += "/themes";
			system(command.c_str());
		}
		
		// check main config file //
		mainFile = configDir + "/main.config";
		
		if (!FileExists(mainFile)){
			ofstream file(mainFile);
			
			// write default values
			file << "diff_enabled:1" << endl;
			file << "line_enabled:1" << endl;
			file << "sess_enabled:0";
			
			file.close();
		}
		
		// write default theme //
		string defaultFile = configDir + "/themes/default";
		if (!FileExists(defaultFile)){
			ofstream file(defaultFile);
			file << "variables:38;5;32m" << endl;
			file << "statements:38;5;163m" << endl;
			file << "functions:38;5;36m" << endl;
			file << "comments:38;5;242;3m" << endl;
			file << "numbers:38;5;81m" << endl;
			file << "strings:38;5;214m" << endl;
			file << "chars:38;5;135m" << endl;
			file.close();
		}
	}
	
	void loadConfig(){
		vector<string> datasplit;
		string line;
		
		ifstream file(mainFile);
		while (getline(file, line)){
			datasplit = split(line, ':');
			
			string key = datasplit[0];
			string value;
			
			for (int i = 1; i < datasplit.size(); i++){
				value += datasplit[i];
			}
			
			config.push_back({key, value});
		}
	}
	
	void saveConfig(){
		ofstream file(mainFile);
		for (int i = 0; i < config.size(); i++){
			file << config[i][0] << ":" << config[i][1] << endl;
		}
		file.close();
	}

	string getValue(string key){
		for (int i = 0; i < config.size(); i++){
			if (config[i][0] == key){
				return config[i][1];
			}
		}
		
		return "";
	}
	
	void putValue(string key, string value){
		// check if key already exists //
		for (int i = 0; i < config.size(); i++){
			if (config[i][0] == key){
				config[i][1] = value;
				saveConfig();
				return;
			}
		}
		
		// insert new value //
		config.push_back({key, value});
	
		// save changes //
		saveConfig();
	}
	
	string getSetting(string key, string value){
		if (getValue(key) != ""){
			return getValue(key);
		}
		
		return value;
	}
};
