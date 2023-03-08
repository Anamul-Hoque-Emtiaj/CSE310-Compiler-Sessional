%option noyywrap
%x STRING 
%x SINGLE_LINE_COMMENT
%x MULTI_LINE_COMMENT 

%{
#include "1905113.h"
#include "y.tab.h"


using namespace std;

int line_count=1;
int error_count = 0;
int startLine;
string tokenString, logString;

extern ofstream errFile;
extern YYSTYPE yylval;


char ConvertCharacter(char *ch, bool f=true){
	int in = 2;
	if(!f)
		in = 1;
	if(ch[in-1] != '\\')
	{
		return ch[1];
	} 
	else if(ch[in-1] == '\\') 
	{
		if(ch[in] == 'n')
			return (char)10;
		else if(ch[in] == 't')
			return (char)9;
		else if(ch[in] == '\\')
			return (char)92;
		else if(ch[in] == '\'')
			return (char)39;
		else if(ch[in] == 'a')
			return (char)7;
		else if(ch[in] == 'f')
			return (char)12;
		else if(ch[in] == 'r')
			return (char)13;
		else if(ch[in] == 'b')
			return (char)8;
		else if(ch[in] == 'v')
			return (char)11;
		else if(ch[in] == '0')
			return (char)0;
		else if(ch[in] == '\"')
			return (char)34;    
		
	}
	return (char)0;
}


void error(string lexeme, string erorrMessage) {
	//errFile<<"line# "<<line_count<<": "<<erorrMessage<<" "<<lexeme<<endl;
	error_count++;
}

%}

NEWLINE \r?\n
WHITESPACE [ \t\f\v\r]+

DIGIT [0-9]
DIGITS {DIGIT}+
CHAR ([ -~]{-}['\\])
SPECIAL_CHAR (\\[ntafrbv0\"\'\\])


CONST_INT {DIGITS}
CONST_FLOAT {DIGIT}*(\.{DIGITS})?([Ee][+-]?{DIGITS})?
CONST_CHAR '({SPECIAL_CHAR}|{CHAR})'

IDENTIFIERS [a-zA-Z_][a-zA-Z0-9_]*



ADDOP [+-]
MULOP [*/%]
INCOP "++"
DECOP "--"
RELOP ("<"|"<="|">"|">="|"=="|"!=")
ASSIGNOP [=]
LOGICOP ("&&"|"||")
BITOP ("&"|"|"|"^"|"<<"|">>")
NOT [!]
LPAREN [(]
RPAREN [)]
LCURL [{]
RCURL [}]
LSQUARE ("[")
RSQUARE ("]")
COMMA [,]
SEMICOLON [;]


SINGLELINE_COMMENT_START \/\/
MULTILINE_COMMENT_START \/\*
STRING_START \"

DECIMAL_POINT_ERROR {DIGIT}*(\.{DIGIT}*)+(\.{DIGIT}*)+([Ee][+-]?{DIGITS})?
ILL_FORMATION_ERROR ({DIGIT}*([Ee][+-]?{DIGITS})+(\.{DIGITS})+)
INVALID_SUFFIX_PREFIX_ERROR ({DIGITS}[a-zA-Z_]+)
MULTI_CHAR_CONST_ERROR '(({CHAR}|{SPECIAL_CHAR})({CHAR}|{SPECIAL_CHAR})+)'
UNFINISHED_CHAR_ERROR ('([_a-zA-Z0-9]|{SPECIAL_CHAR})*)
%%

{WHITESPACE}    { }
{NEWLINE} {line_count++;}

if {
	cout<<"Line# "<<line_count<<": Token <IF> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"IF",line_count);
	return IF;
}
else {
	cout<<"Line# "<<line_count<<": Token <ELSE> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"ELSE",line_count);
	return ELSE;
}
for {
	cout<<"Line# "<<line_count<<": Token <FOR> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"FOR",line_count);
	return FOR;
}
do {
	cout<<"Line# "<<line_count<<": Token <DO> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"DO",line_count);
	return DO;
}
while {
	cout<<"Line# "<<line_count<<": Token <WHILE> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"WHILE",line_count);
	return WHILE;
}
switch {
	cout<<"Line# "<<line_count<<": Token <SWITCH> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"SWITCH",line_count);
	return SWITCH;
}
case {
	cout<<"Line# "<<line_count<<": Token <CASE> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"CASE",line_count);
	return CASE;
}
default {
	cout<<"Line# "<<line_count<<": Token <DEFAULT> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"DEFAULT",line_count);
	return DEFAULT;
}
int {
	cout<<"Line# "<<line_count<<": Token <INT> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"INT",line_count);
	return INT;
}
float {
	cout<<"Line# "<<line_count<<": Token <FLOAT> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"FLOAT",line_count);
	return FLOAT;
}
double {
	cout<<"Line# "<<line_count<<": Token <DOUBLE> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"DOUBLE",line_count);
	return DOUBLE;
}
char {
	cout<<"Line# "<<line_count<<": Token <CHAR> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"CHAR",line_count);
	return CHAR;
}
void {
	cout<<"Line# "<<line_count<<": Token <VOID> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"VOID",line_count);
	return VOID;
}
break {
	cout<<"Line# "<<line_count<<": Token <BREAK> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"BREAK",line_count);
	return BREAK;
}
continue {
	cout<<"Line# "<<line_count<<": Token <CONTINUE> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"CONTINUE",line_count);
	return CONTINUE;
}
return {
	cout<<"Line# "<<line_count<<": Token <RETURN> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"RETURN",line_count);
	return RETURN;
}
println {
	cout<<"Line# "<<line_count<<": Token <PRINTLN> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"PRINTLN",line_count);
	return PRINTLN;
}
{ADDOP} {
	cout<<"Line# "<<line_count<<": Token <ADDOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"ADDOP",line_count);
	return ADDOP;
	}

{MULOP} {
	cout<<"Line# "<<line_count<<": Token <MULOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext, "MULOP",line_count);
	return MULOP;
}

{INCOP} {
	cout<<"Line# "<<line_count<<": Token <INCOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext, "INCOP",line_count);
	return INCOP;
	}

{DECOP} {
	cout<<"Line# "<<line_count<<": Token <DECOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext, "DECOP",line_count);
	return DECOP;
	}

{RELOP} {
	cout<<"Line# "<<line_count<<": Token <RELOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext, "RELOP",line_count);
	return RELOP;
}

{ASSIGNOP} {
	cout<<"Line# "<<line_count<<": Token <ASSIGNOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext, "ASSIGNOP",line_count);
	return ASSIGNOP;
}

{LOGICOP} {
	cout<<"Line# "<<line_count<<": Token <LOGICOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext, "LOGICOP",line_count);
	return LOGICOP;
}

{BITOP} {
	cout<<"Line# "<<line_count<<": Token <BITOP> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext, "BITOP",line_count);
	return BITOP;
}

{NOT} {
	cout<<"Line# "<<line_count<<": Token <NOT> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"NOT",line_count);
	return NOT;}

{LPAREN} {
	cout<<"Line# "<<line_count<<": Token <LPAREN> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"LPAREN",line_count);
	return LPAREN;}

{RPAREN} {
	cout<<"Line# "<<line_count<<": Token <RPAREN> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"RPAREN",line_count);
	return RPAREN;}

{LCURL} {
	cout<<"Line# "<<line_count<<": Token <LCURL> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"LCURL",line_count);
	return LCURL;
}


{RCURL} {
	cout<<"Line# "<<line_count<<": Token <RCURL> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"RCURL",line_count);
	return RCURL;
}

{LSQUARE} {
	cout<<"Line# "<<line_count<<": Token <LSQUARE> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"LSQUARE",line_count);
	return LSQUARE;
}

{RSQUARE} {
	cout<<"Line# "<<line_count<<": Token <RSQUARE> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"RSQUARE",line_count);
	return RSQUARE;
}

{COMMA} {
	cout<<"Line# "<<line_count<<": Token <COMMA> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"COMMA",line_count);
	return COMMA;}

{SEMICOLON} {
	cout<<"Line# "<<line_count<<": Token <SEMICOLON> Lexeme "<<yytext<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"SEMICOLON",line_count);
	return SEMICOLON;
}


{CONST_INT} {
	cout<<"Line# "<<line_count<<": Token <CONST_INT> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext,"CONST_INT",line_count);
	return CONST_INT;
}

{CONST_FLOAT} {
	cout<<"Line# "<<line_count<<": Token <CONST_FLOAT> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext,"CONST_FLOAT",line_count);
	return CONST_FLOAT;
}
{CONST_CHAR} {
	cout<<"Line# "<<line_count<<": Token <CONST_CHAR> Lexeme "<<ConvertCharacter(yytext)<<" found\n";
	yylval.symbolInfo = new SymbolInfo(yytext,"CONST_CHAR",line_count);
	return CONST_CHAR;
}

{IDENTIFIERS}   {
	cout<<"Line# "<<line_count<<": Token <ID> Lexeme "<<yytext<<" found\n";
    yylval.symbolInfo = new SymbolInfo(yytext,"ID",line_count);
	return ID;
}


{STRING_START} {
	BEGIN STRING;
	startLine = line_count;
	logString = yytext;
	tokenString = "";
}

<STRING>\\{NEWLINE} {
    line_count++;
	logString += yytext;
}

<STRING>[^\"\n\r] {
    tokenString += yytext;
    logString += yytext;
}

<STRING>{NEWLINE} {
	error(logString,"UNFINISHED_STRING");

	line_count++;
	tokenString = "";
	logString = "";
	BEGIN INITIAL;
	
}

<STRING><<EOF>> {
	error(logString,"UNFINISHED_STRING");

	tokenString = "";
	logString = "";
	BEGIN INITIAL;

    return 0;

}


<STRING>{SPECIAL_CHAR}   {
    logString += yytext;
	tokenString += ConvertCharacter(yytext,false);
}

<STRING>\" {
    logString += yytext;
    BEGIN INITIAL;
	cout<<"Line# "<<line_count<<": Token <STRING> Lexeme "<<logString<<" found\n";
	yylval.symbolInfo = new SymbolInfo(logString,"STRING",line_count);
	return STRING;
}


{SINGLELINE_COMMENT_START}  {
	BEGIN SINGLE_LINE_COMMENT;
	startLine = line_count;
	logString = yytext;
}

<SINGLE_LINE_COMMENT>. {
    logString+=yytext;
}

<SINGLE_LINE_COMMENT>\\{NEWLINE} {
    logString+=yytext;
    line_count++;
}

<SINGLE_LINE_COMMENT>{NEWLINE} {

	cout<<"Line# "<<startLine<<": Token <SINGLE LINE COMMENT> Lexeme "<<logString<<" found\n";
    line_count++;
    BEGIN INITIAL;
}

{MULTILINE_COMMENT_START} {
	BEGIN MULTI_LINE_COMMENT;
	startLine = line_count;
	logString = yytext;
}

<MULTI_LINE_COMMENT>. {
    logString+=yytext;
}


<MULTI_LINE_COMMENT>{NEWLINE} {
    logString += yytext;
    line_count++;
}


<MULTI_LINE_COMMENT>"*/" {
    logString += yytext;
	cout<<"Line# "<<startLine<<": Token <MULTI LINE COMMENT> Lexeme "<<logString<<" found\n";
    BEGIN INITIAL;
} 

    
<MULTI_LINE_COMMENT><<EOF>> {
    
	error(logString,"UNFINISHED_COMMENT");
    return 0;
}

{ILL_FORMATION_ERROR}   {
	error(yytext,"ILLFORMED_NUMBER");
}

{DECIMAL_POINT_ERROR}   {
    error(yytext,"TOO_MANY_DECIMAL_POINTS");
}

{INVALID_SUFFIX_PREFIX_ERROR}   {
    error(yytext,"INVALID_ID_SUFFIX_NUM_PREFIX");
}

{MULTI_CHAR_CONST_ERROR}    {
    error(yytext,"MULTICHAR_CONST_CHAR");
}


{UNFINISHED_CHAR_ERROR} {
    error(yytext,"UNFINISHED_CONST_CHAR");
}

''  {
    error(yytext,"EMPTY_CONST_CHAR");
}

.   {
    error(yytext,"UNRECOGNIZED_CHAR");
}

<<EOF>> {
    return 0;
}

%%
