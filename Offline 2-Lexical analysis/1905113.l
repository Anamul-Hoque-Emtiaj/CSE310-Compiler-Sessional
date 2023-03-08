%option noyywrap
%x STRING 
%x SINGLE_LINE_COMMENT
%x MULTI_LINE_COMMENT 

%{
#include "1905113.h"
#define num_buckets 10

using namespace std;

int line_count=1;
int error_count = 0;
int startLine;
string tokenString, logString;

FILE *write_token;
SymbolTable *symbolTable = new SymbolTable(num_buckets);


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

void print(string lexeme, string token){
    fprintf(write_token, "<%s, %s>\n", const_cast<char*>(token.c_str()),const_cast<char*>(lexeme.c_str()));
	cout<<"Line# "<<line_count<<": Token <"<<token<<"> Lexeme "<<lexeme<<" found\n";
}

void error(string lexeme, string erorrMessage) {
	cout<<"Error at line# "<<line_count<<": "<<erorrMessage<<" "<<lexeme<<endl;
	error_count++;
}

%}

NEWLINE \r?\n
WHITESPACE [ \t\f\v\r]+
KEYWORD if|else|for|do|while|switch|case|default|int|float|double|char|void|break|continue|return

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
INCOP ("++"|"--")
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

{KEYWORD} {  
	int i =0;
	char *makeUpper=(char*)malloc(sizeof(char*)*(yyleng + 1));
	while(i<yyleng)
	{makeUpper[i]=yytext[i]-32;
	i++;
	}
    makeUpper[i] = '\0';
   	print(yytext,makeUpper);
	free(makeUpper);
}


{ADDOP} {print(yytext, "ADDOP");}

{MULOP} {print(yytext, "MULOP");}

{INCOP} {print(yytext, "INCOP");}

{RELOP} {print(yytext, "RELOP");}

{ASSIGNOP} {print(yytext, "ASSIGNOP");}

{LOGICOP} {print(yytext, "LOGICOP");}

{BITOP} {print(yytext, "BITOP");}

{NOT} {print(yytext, "NOT");}

{LPAREN} {print(yytext, "LPAREN");}

{RPAREN} {print(yytext, "RPAREN");}

{LCURL} {
	symbolTable->enterScope();
	print(yytext, "LCURL");
}


{RCURL} {
	symbolTable->exitScope();
	print(yytext, "RCURL");
}

{LSQUARE} {print(yytext, "LSQUARE");}

{RSQUARE} {print(yytext, "RSQUARE");}

{COMMA} {print(yytext, "COMMA");}

{SEMICOLON} {print(yytext, "SEMICOLON");}


{CONST_INT} {
    print(yytext,"CONST_INT");
}

{CONST_FLOAT} {
    print(yytext,"CONST_FLOAT");
}
{CONST_CHAR} {
    fprintf(write_token, "<CONST_CHAR, %c>\n",ConvertCharacter(yytext));
	cout<<"Line# "<<line_count<<": Token <CONST_CHAR> Lexeme "<<ConvertCharacter(yytext)<<" found\n";
}

{IDENTIFIERS}   {
	print(yytext,"ID");
	symbolTable->Insert(yytext,"ID");
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

	delete symbolTable;
	cout<<"Total lines: "<<line_count<<endl;
	cout<<"Total errors: "<<error_count<<endl;
    return 0;

}


<STRING>{SPECIAL_CHAR}   {
    logString += yytext;
	tokenString += ConvertCharacter(yytext,false);
}

<STRING>\" {
    logString += yytext;
	if(startLine==line_count) {
		fprintf(write_token, "<SINGLE LINE STRING, %s>\n", const_cast<char*>(tokenString.c_str()));
		cout<<"Line# "<<line_count<<": Token <SINGLE LINE STRING> Lexeme "<<logString<<" found\n";
	}else {
		fprintf(write_token, "<MULTI LINE STRING, %s>\n", const_cast<char*>(tokenString.c_str()));
		cout<<"Line# "<<startLine<<": Token <MULTI LINE STRING> Lexeme "<<logString<<" found\n";
	}
    
    BEGIN INITIAL;
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

    delete symbolTable;
	cout<<"Total lines: "<<line_count<<endl;
	cout<<"Total errors: "<<error_count<<endl;
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
	delete symbolTable;
	cout<<"Total lines: "<<line_count<<endl;
	cout<<"Total errors: "<<error_count<<endl;
    return 0;
}

%%

int main(int argc, char** argv) {
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	freopen("1905113_log.txt", "w", stdout);
    write_token = fopen("1905113_token.txt", "w");

	yyin= fin;
	yylex();


	fclose(yyin);
	fclose(write_token);
	return 0;
}
