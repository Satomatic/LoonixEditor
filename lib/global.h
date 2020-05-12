#ifndef GLOBALH
#define GLOBALH

// main extern variables //
extern int screenWidth;
extern int screenHeight;

extern int curx;
extern int cury;
extern int index;
extern int XOffset;

extern int prex;
extern int prey;

extern int startx;
extern int starty;
extern int endx;
extern int endy;

extern vector<string> raw;
extern vector<string> lines;
extern vector<string> viewport;
extern vector<string> rawViewport;

extern vector<vector<string>> openFiles;
extern vector<vector<string>> fileMemory;
extern int fileIndex;

extern string currentfile;
extern bool insertMode;
extern bool hasEdited;

#endif 
 
