#include <vector>
#include <string>

// default colors used by lexer
// updated from theme file
std::vector<std::vector<std::string>> colors = {
	{"statements", "\033[38;5;163m"},
	{"variables",  "\033[38;5;32m"},
	{"functions",  "\033[38;5;36m"},
	{"comments",   "\033[38;5;242;3m"},
	{"numbers",    "\033[38;5;81m"},
	{"strings",    "\033[38;5;214m"},
	{"chars",      "\033[38;5;135m"},
	{"tagged",     "\033[38;5;32m"},
};

// keywords used when syntaxing assembly files
std::vector<std::vector<std::string>> assembly_keywords = {
	{
		"mov", "add", "sub", "inc", 
		"call", "int", "cmp", "je", 
		"jmp", "ret"
	},
	{
		"ax", "bx", "cx", "dx", 
		"al", "bl", "cl", "dl", 
		"ah", "bh", "ch", "dh",
		"si", "es", "ds", "ss", "sp", "fs", "gs"
	},
	{
		"pusha", "push", "pop", "popa",
		"dd", "dw", "db", "times", "equ",
		"cli", "sti", "cld", "align"
	}
};

std::vector<std::string> assembly_colors = {
	"variables",
	"statements",
	"functions"    
};

// general keywords used when syntaxing generic code files
// should detect language later on
std::vector<std::vector<std::string>> generic_keywords = {
	// statements
	{
		"if", "else", "return", "for", "while",
		"elif", "then", "end", "and", "break",
		"in", "or", "elseif", "public", "static", "private"
	},
	// variables
	{
		"bool", "string", "int", "void", "class",
		"def", "function", "char", "do", "define",
		"ifndef", "endif", "try", "except", "catch",
		"const", "auto", "unsigned"
	},
	// functions
	{
		"extern", "extrn", "include", "vector",
		"from", "import", "export", "pair", "as"
	}
};

// comments used by different languages
// defaults to "//"
std::vector<std::vector<std::string>> comments = {
	{"lua",  "--"},
	{"cpp",  "//"},
	{"py",   "#"},
	{"html", "<!--"},
	{"asm",  ";"},
	{"sh",   "#"}
};

// chars that can come before or after keywords
std::vector<std::string> extraChars = {
	"{", "(", "[", "<",
	"}", ")", "]", ">",
	",", ".", ";", ":",
	"+", "-", "#", "%",
	"@", "=", "", " "
};

std::string fileExtension;
std::string commentString;
