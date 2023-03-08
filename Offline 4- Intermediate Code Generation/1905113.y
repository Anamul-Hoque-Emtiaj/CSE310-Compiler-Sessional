%{
#include<iostream>
#include<fstream>
#include <bits/stdc++.h>
#include "1905113.h"
#define num_buckets 11


using namespace std;

int yyparse(void);
int yylex(void);

extern FILE *yyin;
extern int line_count;
extern int error_count;

SymbolTable *symbolTable = new SymbolTable(num_buckets);
vector<SymbolInfo*> paramList;
ofstream errFile;
ofstream treeFile;
ofstream codeFile;
ifstream codeFile2;
ofstream optCodeFile;


string typeCasting(string x, string y){
		if(x == y)
			return x;
		if(x == "INT" && y == "FLOAT"){
			return "FLOAT";
		}else if(x == "FLOAT" && y == "INT"){
			return "FLOAT";
		}
		return "VOID";
	}


void declareFuncParam(string dataType, string name, int it){
		if(dataType == "VOID"){
			errFile<<"Line# "<<line_count<<": Function parameter cannot be void\n";
			error_count++;
			return;
		}
		if(!symbolTable->Insert(name, "ID",dataType) && name!=""){
			errFile<<"Line# "<<line_count<<": Redefinition of parameter '"<<name<<"'\n";
			error_count++;
			return;
		}

		SymbolInfo* info = symbolTable->LookUp(name);
		info->setInfoType(it);
	}

void declareFuncParamList(){
	if(paramList.empty()){
		return;
	}
	int stackOffset = 2;
	for(int i = paramList.size()-1; i>=0; i--){
		
		//declareFuncParam(info->getDataType(), info->getName(), info->getInfoType());
		SymbolInfo* info = paramList[i];
		if(info!=NULL)
		{
			if(info->getDataType() == "VOID"){
			errFile<<"Line# "<<line_count<<": Function parameter cannot be void\n";
			error_count++;
			}
			else if(!symbolTable->Insert(info->getName(), "ID",info->getDataType()) && info->getName()!=""){
				errFile<<"Line# "<<line_count<<": Redefinition of parameter '"<<info->getName()<<"'\n";
				error_count++;
			}
			else
			{
				SymbolInfo* sinfo = symbolTable->LookUp(info->getName());
				sinfo->setInfoType(info->getInfoType());
				stackOffset+=2;
				sinfo->stackOffset = stackOffset;
			}
		}
	}
	paramList.clear();
}

void deleteInfo(SymbolInfo* info)
{
	for(int i=0; i<info->getChild().size(); i++){
		deleteInfo(info->getChild()[i]);
	}
	delete info;
}

string getOpJMP(string op){
	string opcode = "";
	if(op == "<") opcode ="JL";
	else if(op == "<=") opcode ="JLE";
	else if(op == ">") opcode ="JG";
	else if(op == ">=") opcode ="JGE";
	else if(op == "!=") opcode ="JNE";
	else if(op == "==") opcode ="JE";
	return opcode;
}

void genTree(SymbolInfo* symInfo, int sc){
	for(int i=1; i<=sc; i++)
		treeFile<<" ";
	
	if(symInfo->getIsleaf())
	{
		if(symInfo->getType()=="ARRAY")
			treeFile<<"ID : "<<symInfo->getName()<<"\t<Line: "<<symInfo->getSl()<<">\n";
		else
			treeFile<<symInfo->getType()<<" : "<<symInfo->getName()<<"\t<Line: "<<symInfo->getSl()<<">\n";
	}
	else
		treeFile<<symInfo->getType()<<" : "<<symInfo->getName()<<" \t<Line: "<<symInfo->getSl()<<"-"<<symInfo->getEl()<<">\n";

	for(int i=0; i<symInfo->getChild().size(); i++){
		genTree(symInfo->getChild()[i], sc+1);
	}

}

string findLeafVal(SymbolInfo* info)
{
	if(info->getIsleaf())
		return info->getName();
	return findLeafVal(info->getChild()[0]);
}




int labelCounter = 0;

int getLabelCounter(){
	
	return labelCounter;
}
string newLabelCounter(){
	labelCounter++;
	return "L"+to_string(labelCounter);
}

SymbolInfo* curFunc;
vector<string> globalVariables;
bool hasMain = false;


void genCode(SymbolInfo* info)
{
	if(info->getType()=="start")
	{
		codeFile << ".MODEL SMALL\n.STACK 100h\n.DATA\n";
		codeFile << "\tCR EQU 0DH\n\tLF EQU 0AH\n\tnumber DB \"00000$\"\n";
		for(string variable: globalVariables)
		{
			codeFile<<'\t'<<variable<<" DW 0\n";
		}
		codeFile<<".CODE\n";
	}
	else if(info->getType()=="func_definition")
	{
		codeFile<<"\n"<<info->getChild()[1]->getName()<<"  PROC\n";
		curFunc = info;
		if(info->getChild()[1]->getName()=="main")
		{
			codeFile<<"\tMOV AX,@DATA\n\tMOV DS,AX\n\n";
		}
		codeFile<<"\tPUSH BP\n\tMOV BP,SP\n";
	}

	if(info->getType()=="statement")
	{
		if(info->getName()=="var_declaration" || info->getName()=="expression_statement" || info->getName()=="compound_statement")
			genCode(info->getChild()[0]);
		else if(info->getName()== "FOR LPAREN expression_statement expression_statement expression RPAREN statement" )
		{
			codeFile<<"\t;======for loop start======\n";
			genCode(info->getChild()[2]);
			string label_loop = newLabelCounter();
			codeFile<<label_loop<<":\n";
			genCode(info->getChild()[3]);
			string label_end = newLabelCounter();
			codeFile<<"\tJCXZ "<<label_end<<" ;break for loop"<<endl;
			genCode(info->getChild()[6]);
			genCode(info->getChild()[4]);
			codeFile<<"\tJMP "<<label_loop<<endl;
			codeFile<<label_end<<":\n";
			codeFile<<"\t;======for loop end======\n";
		}
		else if(info->getName()== "IF LPAREN expression RPAREN statement" )
		{
			codeFile<<"\t;======IF statement start======\n";
			genCode(info->getChild()[2]);
			string label_false = newLabelCounter();
			codeFile<<"\tJCXZ "<<label_false<<" ;jump for false"<<endl;
			genCode(info->getChild()[4]);
			string label_true = newLabelCounter();
			codeFile<<"\tJMP "<<label_true<<" ;jump for true"<<endl;
			codeFile<<label_false<<":\n";
			codeFile<<label_true<<":\n";
			codeFile<<"\t;======IF statement end======\n";
		}
		else if(info->getName()== "IF LPAREN expression RPAREN statement ELSE statement" )
		{
			codeFile<<"\t;======IF ELSE statement start======\n";
			genCode(info->getChild()[2]);
			string label_false = newLabelCounter();
			codeFile<<"\tJCXZ "<<label_false<<" ;jump for false"<<endl;
			genCode(info->getChild()[4]);
			string label_true = newLabelCounter();
			codeFile<<"\tJMP "<<label_true<<" ;jump for true"<<endl;
			codeFile<<label_false<<":\n";
			genCode(info->getChild()[6]);
			codeFile<<label_true<<":\n";
			codeFile<<"\t;======IF ELSE statement end======\n";
		}
		else if(info->getName()== "WHILE LPAREN expression RPAREN statement" )
		{
			codeFile<<"\t;======WHILE loop start======\n";
			string label_loop =newLabelCounter();
			codeFile<<label_loop<<":\n";
			genCode(info->getChild()[2]);
			string label_end = newLabelCounter();
			codeFile<<"\tJCXZ "<<label_end<<" ;break while loop"<<endl;
			genCode(info->getChild()[4]);
			codeFile<<"\tJMP "<<label_loop<<endl;
			codeFile<<label_end<<":\n";
			codeFile<<"\t;======WHILE loop end======\n";
		}
		else if(info->getName()== "PRINTLN LPAREN ID RPAREN SEMICOLON" )
		{
			codeFile<<"\tMOV AX, "<<info->varName<<endl;
			codeFile<<"\tCALL print_output\n\tCALL new_line\n";

		}
		else if(info->getName()== "RETURN expression SEMICOLON" )
		{
			codeFile<<"\t;======RETURN STATEMENT start======\n";
			genCode(info->getChild()[1]);
			codeFile<<"\tMOV DX,CX\n\tJMP "<<curFunc->getChild()[1]->getName()<<"_end\n";
			codeFile<<"\t;======RETURN STATEMENT end======\n";
		}
	}
	else if(info->getType()=="logic_expression" && info->getName()=="rel_expression LOGICOP rel_expression")
	{
		genCode(info->getChild()[0]);
		codeFile<<"\tCMP CX,0\n";
		string labelEnd = newLabelCounter();
		if(info->getChild()[1]->getName() == "&&")
			codeFile<<"\tJCXZ "<<labelEnd<<endl;
		else
			codeFile<<"\tJNZ "<<labelEnd<<endl;
		genCode(info->getChild()[2]);
		codeFile<<labelEnd<<":\n";
	}
	else if(info->getType()=="rel_expression" && info->getName()=="simple_expression RELOP simple_expression")
	{
		string opcode = getOpJMP(info->getChild()[1]->getName());
		genCode(info->getChild()[0]);
		codeFile<<"\tPUSH CX\n";
		genCode(info->getChild()[2]);
		string labelTrue = newLabelCounter();
		string labelFalse = newLabelCounter();
		codeFile<<"\tPOP AX\n\tCMP AX,CX\n\t";
		codeFile<<opcode<<" "<<labelTrue<<"\n\tMOV CX,0\n\tJMP "<<labelFalse<<endl<<labelTrue<<":\n";
		codeFile<<"\tMOV CX,1\n"<<labelFalse<<":\n";
	}
	else if(info->getType()=="simple_expression" && info->getName()=="simple_expression ADDOP term")
	{
		genCode(info->getChild()[0]);
		codeFile<<"\tPUSH CX\n";
		genCode(info->getChild()[2]);
		if(info->getChild()[1]->getName()=="+")
			codeFile<<"\tPOP AX\n\tADD AX,CX\n\tMOV CX,AX\n";
		else
			codeFile<<"\tPOP AX\n\tSUB AX,CX\n\tMOV CX,AX\n";
	}
	else if(info->getType()=="term" && info->getName()=="term MULOP unary_expression")
	{
		genCode(info->getChild()[0]);
		codeFile<<"\tPUSH CX\n";
		genCode(info->getChild()[2]);
		if(info->getChild()[1]->getName()=="*")
			codeFile<<"\tPOP AX\n\tIMUL CX\n\tMOV CX,AX\n";
		else if(info->getChild()[1]->getName()=="/")
			codeFile<<"\tPOP AX\n\tCWD\n\tIDIV CX\n\tMOV CX,AX\n";
		else if(info->getChild()[1]->getName()=="%")
			codeFile<<"\tPOP AX\n\tCWD\n\tIDIV CX\n\tMOV CX,DX\n";
	}
	else
	{
		for(int i=0; i<info->getChild().size(); i++)
		{
			genCode(info->getChild()[i]);
		}
	}
	

	if(info->getType()=="start")
	{
		codeFile<<"new_line proc\n\tpush ax\n\tpush dx\n\tmov ah,2\n\tmov dl,cr\n\tint 21h\n\tmov ah,2\n\tmov dl,lf\n\tint 21h\n\tpop dx\n\tpop ax\n\tret\nnew_line endp\n";
		codeFile<<"print_output proc  ;print what is in ax";
		codeFile<<"\n\tpush ax\n\tpush bx\n\tpush cx\n\tpush dx\n\tpush si\n\tlea si,number\n\tmov bx,10\n\tadd si,4\n\tcmp ax,0\n\tjnge negate\n\tprint:";
		codeFile<<"\n\txor dx,dx\n\tdiv bx\n\tmov [si],dl\n\tadd [si],'0'\n\tdec si\n\tcmp ax,0\n\tjne print\n\tinc si\n\tlea dx,si\n\tmov ah,9\n\tint 21h";
		codeFile<<"\n\tpop si\n\tpop dx\n\tpop cx\n\tpop bx\n\tpop ax\n\tret\n\tnegate:\n\tpush ax\n\tmov ah,2\n\tmov dl,'-'\n\tint 21h\n\tpop ax\n\tneg ax\n\tjmp print\nprint_output endp\nEND main";
	}
	else if(info->getType()=="func_definition")
	{
		codeFile<<info->getChild()[1]->getName()<<"_end:";
		codeFile<<"\n\tADD SP,"<<(info->stackOffset*-1)<<"\n\tPOP BP\n";
		if(info->getChild()[1]->getName()=="main")
		{
			codeFile<<"\tMOV AX,4CH\n\tINT 21H\n";
		}
		else{
			if(info->getChild()[3]->getParamSize()>0)
				codeFile<<"\tRET "<<(info->getChild()[3]->getParamSize())*2<<endl;
			else
				codeFile<<"\tRET\n";
		}
		codeFile<<info->getChild()[1]->getName()<<" ENDP\n";
	}
	else if(info->getType()=="var_declaration")
	{
		if(info->stackOffset==1)
		{
			for(auto sinfo: info->getChild()[1]->getParam())
			{
				if(sinfo->getInfoType()==2)
					codeFile<<"\tSUB SP,"<<(2*stoi(sinfo->getArraySize()))<<endl;
				else
					codeFile<<"\tSUB SP,2\n";
			}
		}
	
	}
	else if(info->getType()=="variable" && info->getName()=="ID LTHIRD expression RTHIRD")
	{
		codeFile<<"\tMOV BX,CX\n\tSHL BX,1\n\tNEG BX\n\tADD BX,"<<info->stackOffset<<"\n\tADD BX,BP\n";
	}
	else if(info->getType()=="expression" && info->getName()=="variable ASSIGNOP logic_expression")
	{
		codeFile<<"\tMOV "<<info->getChild()[0]->varName<<",CX\n";
	}
	else if(info->getType()=="unary_expression")
	{
		if(info->getName()=="ADDOP unary_expression" && info->getChild()[0]->getName()=="-")
			codeFile<<"NEG CX\n";
		else if(info->getName()=="NOT unary_expression" )
		{
			string labelEnd =newLabelCounter();
			string labelOne = newLabelCounter();
			codeFile<<"\tJCXZ "<<labelOne<<"\n\tMOV CX,0\n";
			codeFile<<labelOne<<":\n";
			codeFile<<"\tMOV CX,1\n";
			codeFile<<labelEnd<<":\n";
		}
	}
	else if(info->getType()=="factor")
	{
		if(info->getName()=="variable")
			codeFile<<"\tMOV CX,"<<info->getChild()[0]->varName<<endl;
		else if(info->getName()=="ID LPAREN argument_list RPAREN")
		{
			codeFile<<"\tCALL "<<info->getChild()[0]->getName()<<"\n\tMOV CX,DX\n\tADD SP,"<<info->stackOffset<<endl;
		}
		else if(info->getName()=="CONST_INT" || info->getName()=="CONST_FLOAT")
			codeFile<<"\tMOV CX,"<<info->getChild()[0]->getName()<<endl;
		else if(info->getName()=="variable INCOP")
			codeFile<<"\tMOV CX,"<<info->getChild()[0]->varName<<"\n\tINC "<<info->getChild()[0]->varName<<"\n";
		else if(info->getName()=="variable DECOP")
			codeFile<<"\tMOV CX,"<<info->getChild()[0]->varName<<"\n\tMOV AX,CX\n\tSUB AX,1\n\tMOV "<<info->getChild()[0]->varName<<",AX\n";
	}
	else if(info->getType()=="arguments")
		codeFile<<"\tPUSH CX\n";
	
}

vector<string> splitLine(string line)
{
	string code = line;
	while(code[code.size()-1] == ' ' or code[code.size()-1] == '\t' ) 
		code.pop_back();
	reverse(code.begin(),code.end());
	while(code[code.size()-1] == ' ' or code[code.size()-1] == '\t' ) 
		code.pop_back();
	reverse(code.begin(),code.end());

	stringstream ss(code);
	vector<string> ret, codes,codes2;
	
 
    while (ss.good()) {
        string str;
        getline(ss, str, ' ');
        codes.push_back(str);
    }

	if( codes.size()>0 ) 
		ret.push_back(codes[0]);
	else 
		ret.push_back("");

	if( codes.size()>1 )
	{
		stringstream ss(codes[1]);
		while (ss.good()) 
		{
			string str;
			getline(ss, str, ',');
			codes2.push_back(str);
		}	
	}

	if( codes2.size() == 0 ) 
	{
		ret.push_back("");
		ret.push_back("");
		ret.push_back("1");
	}
	else if( codes2.size() == 1 )
	{
		ret.push_back(codes2[0]);
		ret.push_back("");
		ret.push_back("1");
	}
	else
	{
		ret.push_back(codes2[0]);
		ret.push_back(codes2[1]);
		if((ret[0]=="ADD" && ret[2]=="0") || (ret[0]=="MUL" && ret[2]=="1"))
			ret.push_back("0");
		else
			ret.push_back("1");
	}
	ret.push_back(line);
	return ret;
}

void genOptCode()
{
	vector<string> codes;
	vector<vector<string>> optCode;
	string line;
	codeFile.close();
	codeFile2.open("code.asm");
	while (getline(codeFile2, line)) 
	{
		codes.push_back(line);
	}
	
	for(int i=0; i<codes.size(); i++)
	{
		if( codes[i][0] == ';' )
		{
			optCode.push_back(splitLine(codes[i]));
			continue;
		}

		
		vector<string> cur3add = splitLine(codes[i]);
		if( optCode.size() == 0 )
		{
			if(cur3add[3]=="1")
				optCode.push_back(cur3add);
			continue;
		}

		auto prev3add = optCode.back();
		if( cur3add[0] == "MOV" && prev3add[0] == "MOV" )
		{
			if(!((cur3add[1] == prev3add[1] and cur3add[2] == prev3add[2]) || (cur3add[1] == prev3add[2] and cur3add[2] == prev3add[1])))
				optCode.push_back(cur3add);
		}
		else if( cur3add[0] == "POP" )
		{
			if( prev3add[0] == "PUSH" )
			{
				optCode.pop_back();
			}
			else
			{
				
				optCode.push_back(cur3add);
			}
		}
		else
		{	if(cur3add[3]=="1")
				optCode.push_back(cur3add);
		}

	}
	for(auto cur3add : optCode)
	{
		optCodeFile<<cur3add[4]<<endl;
	}
	codeFile2.close();
}

void yyerror(char *s)
{
	//write your code
}


%}

%token<symbolInfo> IF ELSE FOR DO WHILE SWITCH CASE DEFAULT INT FLOAT DOUBLE CHAR VOID BREAK CONTINUE RETURN PRINTLN
%token<symbolInfo> LPAREN RPAREN LCURL RCURL LSQUARE RSQUARE COMMA SEMICOLON NOT

%token<symbolInfo> ADDOP MULOP INCOP DECOP RELOP ASSIGNOP LOGICOP BITOP
%token<symbolInfo> CONST_INT CONST_FLOAT CONST_CHAR ID 

%type<symbolInfo> start program unit func_declaration func_definition parameter_list compound_statement var_declaration type_specifier declaration_list statements _lcurl
%type<symbolInfo> statement expression_statement variable expression logic_expression rel_expression simple_expression term unary_expression factor argument_list arguments

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%union{
	SymbolInfo* symbolInfo; 
}

%%

start : program
	{
		$$ = new SymbolInfo("program", "start");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		cout<<"Total Lines: "<<line_count<<endl;
		cout<<"Total Errors: "<<error_count<<endl;

		if(!hasMain){
			errFile<<"Line# "<<$$->getSl()<<": undefined reference to main\n";
			error_count++;
		}
		if(error_count==0)
		{
			genCode($$);
			genOptCode();
			genTree($$,0);
		}

		deleteInfo($$);
		delete symbolTable;
	}
	;

program : program unit {
		$$ = new SymbolInfo("program unit", "program");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);
	}
	| unit{
		$$ = new SymbolInfo("unit", "program");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	;
	
unit : var_declaration{
		$$ = new SymbolInfo("var_declaration", "unit");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

	 }
     | func_declaration{
		$$ = new SymbolInfo("func_declaration", "unit");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

	 }
     | func_definition{
		$$ = new SymbolInfo("func_definition", "unit");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	 }
     ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON{
		string name = "type_specifier ID LPAREN parameter_list RPAREN SEMICOLON";
		$$ = new SymbolInfo(name, "func_declaration");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($6->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);
		$$->addChild($6);


		bool inserted = symbolTable->Insert($2->getName(),"FUNCTION",$1->getName());
		if(!inserted){
			errFile<<"Line# "<<$$->getSl()<<": Redeclaration of function '"<<$2->getName()<<"'\n";
			error_count++;
		}
		else{
			SymbolInfo* info = symbolTable->LookUp($2->getName());
			info->setInfoType(3);
			
			info->setParam($4->getParam());
		}
	 }
	| type_specifier ID LPAREN RPAREN SEMICOLON{
		string name = "type_specifier ID LPAREN RPAREN SEMICOLON";
		$$ = new SymbolInfo(name, "func_declaration");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($5->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);

		bool inserted = symbolTable->Insert($2->getName(),"FUNCTION",$1->getName());
		if(!inserted){
			errFile<<"Line# "<<$$->getSl()<<": Redeclaration of function '"<<$2->getName()<<"'\n";
			error_count++;
		}
		else{
			SymbolInfo* info = symbolTable->LookUp($2->getName());
			info->setInfoType(3);
		}
	}
	;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN {

	SymbolInfo* info = symbolTable->LookUp($2->getName());

		if(info!=NULL)
		{
			if(info->getInfoType()==4)
			{
				errFile<<"Line# "<<$1->getSl()<<": Redefinition of function '"<<$2->getName()<<"'\n";
				error_count++;
			}
			else if(info->getInfoType()==3)
			{
				if(info->getDataType()!= $1->getName()){
					errFile<<"Line# "<<$1->getSl()<<": Conflicting types for '"<<$2->getName()<<"'\n";
					error_count++;
				}
				else if(info->getParamSize()!= $4->getParamSize()){
					errFile<<"Line# "<<$1->getSl()<<": Conflicting types for '"<<$2->getName()<<"'\n";
					error_count++;
				}
				else if(info->getParamSize()>0)
				{
					vector<SymbolInfo*> paramList1 = info->getParam();
					vector<SymbolInfo*> paramList2 = $4->getParam();

					for(int i=0; i<info->getParamSize(); i++)
					{
						if(paramList1[i]->getDataType() != paramList2[i]->getDataType())
						{
							errFile<<"Line# "<<$1->getSl()<<": conflicting types for '"<<$2->getName()<<"'\n";
							error_count++;
							break;
						}
					}
				}
				
			}
			else{
				errFile<<"Line# "<<$1->getSl()<<": '"<<$2->getName()<<"' redeclared as different kind of symbol\n";
				error_count++;
			}

			info->setInfoType(4);
		}
		else
		{
			symbolTable->Insert($2->getName(),"FUNCTION",$1->getName());
			SymbolInfo* info = symbolTable->LookUp($2->getName());
			info->setInfoType(4);
			info->setParam($4->getParam());
		}
}
compound_statement{
		string name = "type_specifier ID LPAREN parameter_list RPAREN compound_statement";
		$$ = new SymbolInfo(name, "func_definition");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($7->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);
		$$->addChild($7);

		if($2->getName()=="main")
		{
			hasMain = true;
		}
		$$->stackOffset = $7->stackOffset;
	}
	| type_specifier ID LPAREN RPAREN {

		SymbolInfo* info = symbolTable->LookUp($2->getName());
		
		if(info!=NULL)
		{
			if(info->getInfoType()==4)
			{
				errFile<<"Line# "<<$1->getSl()<<": Redefinition of function '"<<$2->getName()<<"'\n";
				error_count++;
			}
			else if(info->getInfoType()==3)
			{
				if(info->getDataType()!= $1->getName()){
					errFile<<"Line# "<<$1->getSl()<<": Conflicting types for '"<<$2->getName()<<"'\n";
					error_count++;
				}
				else if(info->getParamSize()!= 0){
					errFile<<"Line# "<<$1->getSl()<<": Conflicting types for '"<<$2->getName()<<"'\n";
					error_count++;
				}
				
			}
			else{
				errFile<<"Line# "<<$1->getSl()<<": '"<<$2->getName()<<"' redeclared as different kind of symbol\n";
				error_count++;
			}

			info->setInfoType(4);
		}
		else
		{
			symbolTable->Insert($2->getName(),"FUNCTION",$1->getName());
			SymbolInfo* info = symbolTable->LookUp($2->getName());
			info->setInfoType(4);
		}
	}
	compound_statement{
		string name = "type_specifier ID LPAREN RPAREN compound_statement";
		$$ = new SymbolInfo(name, "func_definition");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($6->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($6);

		if($2->getName()=="main")
		{
			hasMain = true;
		}
		$$->stackOffset = $6->stackOffset;
	}
 	;				


parameter_list  : parameter_list COMMA type_specifier ID{
		string name = "parameter_list COMMA type_specifier ID";
		$$ = new SymbolInfo(name, "parameter_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($4->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);

		$$->setParam($1->getParam());

		$4->setDataType($3->getName());
		$4->setInfoType(1);

		$$->addParam($4);

		paramList = $$->getParam();
	}
	| parameter_list COMMA type_specifier{  
		string name = "parameter_list COMMA type_specifier";
		$$ = new SymbolInfo(name, "parameter_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		$$->setParam($1->getParam());

		$$->addParam(new SymbolInfo("","ID",$3->getName()));

		paramList = $$->getParam();
	}
	| type_specifier ID{
		string name = "type_specifier ID";
		$$ = new SymbolInfo(name, "parameter_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);

		$2->setDataType($1->getName());
		$2->setInfoType(1);
		$$->addParam($2);

		paramList = $$->getParam();
	}
	| type_specifier{ 
		string name = "type_specifier";
		$$ = new SymbolInfo(name, "parameter_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);


		$$->addParam(new SymbolInfo("","ID",$1->getName()));

		paramList = $$->getParam();
	}
	;

 		
compound_statement : _lcurl statements RCURL{
		string name = "LCURL statements RCURL";
		$$ = new SymbolInfo(name, "compound_statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		$$->stackOffset = symbolTable->getCurScope()->stackOffset; 

		symbolTable->printAll();
		symbolTable->exitScope();
	}
	| _lcurl RCURL{
		string name = "LCURL RCURL";
		$$ = new SymbolInfo(name, "compound_statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);

		symbolTable->printAll();
		symbolTable->exitScope();
	}
	;
 		    
var_declaration : type_specifier declaration_list SEMICOLON{
		string name = "type_specifier declaration_list SEMICOLON";
		$$ = new SymbolInfo(name, "var_declaration");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		if(symbolTable->getCurScopeNum() == 1)
		{
			$$->stackOffset = 0;
		}
		else
			$$->stackOffset = 1;

		for(SymbolInfo* info : $2->getParam())
		{
			if($1->getName() == "VOID"){
				errFile<<"Line# "<<line_count<<": Variable or field '"<<info->getName()<<"' declared void\n";
				error_count++;
				break;
			}
			bool success = symbolTable->Insert(info->getName(), info->getType());
			if(success)
			{
				SymbolInfo* newVar = symbolTable->LookUp(info->getName());
				newVar->setDataType($1->getName());
				newVar->setInfoType(info->getInfoType());

				if(symbolTable->getCurScopeNum() == 1)
				{
					info->stackOffset = 0;
					globalVariables.push_back(info->getName());
				}
				if(info->getInfoType()==2)
				{
					newVar->setArraySize(info->getArraySize());
					int stackOffset = symbolTable->getCurScope()->stackOffset;
					stackOffset +=2;
					stackOffset-=(2*stoi(info->getArraySize()));
					symbolTable->getCurScope()->stackOffset = stackOffset;
				}
			}
			else
			{
				errFile<<"Line# "<<line_count<<": Conflicting types for'"<<info->getName()<<"'\n";
				error_count++;
			}
		}
	}
	;
 		 
type_specifier	: INT{
		$$ = new SymbolInfo("INT", "type_specifier");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	| FLOAT{
		$$ = new SymbolInfo("FLOAT", "type_specifier");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	| VOID{
		$$ = new SymbolInfo("VOID", "type_specifier");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	;
 		
declaration_list : declaration_list COMMA ID{
		string name = "declaration_list COMMA ID";
		$$ = new SymbolInfo(name, "declaration_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		$$->setParam($1->getParam());

		$3->setInfoType(1);
		$$->addParam($3);
	}
	| declaration_list COMMA ID LSQUARE CONST_INT RSQUARE{
		string name = "declaration_list COMMA ID LSQUARE CONST_INT RSQUARE";
		$$ = new SymbolInfo(name, "declaration_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($6->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);
		$$->addChild($6);

		$$->setParam($1->getParam());

		$3->setInfoType(2);
		$3->setType("ARRAY");
		$3->setArraySize($5->getName());
		$$->addParam($3);
	}
	| ID{
		$$ = new SymbolInfo("ID", "declaration_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$1->setInfoType(1);
		$$->addParam($1);
	}
	| ID LSQUARE CONST_INT RSQUARE{
		string name = "ID LSQUARE CONST_INT RSQUARE";
		$$ = new SymbolInfo(name, "declaration_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($4->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);

		$1->setInfoType(2);
		$1->setType("ARRAY");
		$1->setArraySize($3->getName());
		$$->addParam($1);
	}
	;
 		  
statements : statement{
		$$ = new SymbolInfo("statement", "statements");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	| statements statement{
		$$ = new SymbolInfo("statements statement", "statements");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);
	}
	;
	   
statement : var_declaration{
		$$ = new SymbolInfo("var_declaration", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	  | expression_statement{
		$$ = new SymbolInfo("expression_statement", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	  | compound_statement{
		$$ = new SymbolInfo("compound_statement", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement{
		$$ = new SymbolInfo("FOR LPAREN expression_statement expression_statement expression RPAREN statement", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($7->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);
		$$->addChild($6);
		$$->addChild($7);

	}
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE{
		$$ = new SymbolInfo("IF LPAREN expression RPAREN statement", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($5->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);
	}
	  | IF LPAREN expression RPAREN statement ELSE statement{
		$$ = new SymbolInfo("IF LPAREN expression RPAREN statement ELSE statement", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($7->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);
		$$->addChild($6);
		$$->addChild($7);
	}
	  | WHILE LPAREN expression RPAREN statement{
		$$ = new SymbolInfo("WHILE LPAREN expression RPAREN statement", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($5->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);
	}
	  | PRINTLN LPAREN ID RPAREN SEMICOLON{
		$$ = new SymbolInfo("PRINTLN LPAREN ID RPAREN SEMICOLON", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($5->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		$$->addChild($5);

		SymbolInfo* info = symbolTable->LookUp($3->getName());
		if(info==NULL){
			errFile<<"Line# "<<line_count<<" : Undeclared variable '"<<$3->getName()<<"'\n";
			error_count++;
		}
		else
		{
			$$->stackOffset =  info->stackOffset;
			if( $$->stackOffset != 0 ) 
				$$->varName = to_string($$->stackOffset)+"[BP]"; 
			else 
				$$->varName = $3->getName(); // global variable
		}
	}
	  | RETURN expression SEMICOLON{
		$$ = new SymbolInfo("RETURN expression SEMICOLON", "statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
	}
	  ;
	  
expression_statement 	: SEMICOLON{
		$$ = new SymbolInfo("SEMICOLON", "expression_statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);
	}

	| expression SEMICOLON{
		$$ = new SymbolInfo("expression SEMICOLON", "expression_statement");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);
	}
	;
	  
variable : ID{
		$$ = new SymbolInfo("ID", "variable");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		SymbolInfo *info = symbolTable->LookUp($1->getName());
		if(info!=NULL){
			if(info->getInfoType() ==2){
				//errFile<<"Line# "<<line_count<<" :Type mismatch'"<<info->getName()<<" is an array\n";
			}
			$$->setDataType(info->getDataType());
			$$->stackOffset =  info->stackOffset;
			if( $$->stackOffset != 0 ) 
				$$->varName = to_string($$->stackOffset)+"[BP]";
            else 
				$$->varName = $1->getName(); // global variable
		}else{
			errFile<<"Line# "<<line_count<<": Undeclared variable '"<<$1->getName()<<"'\n";
			error_count++;
		}
	} 		
	| ID LSQUARE expression RSQUARE{
		$$ = new SymbolInfo("ID LSQUARE expression RSQUARE", "variable");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($4->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);

		SymbolInfo *info = symbolTable->LookUp($1->getName());
		if(info!=NULL){
			if(info->getInfoType()!=2){
				errFile<<"Line# "<<line_count<<": '"<<info->getName()<<"' is not an array\n";
				error_count++;
			}
			else if($3->getDataType()!="INT"){
				errFile<<"Line# "<<line_count<<": "<<"Array subscript is not an integer\n";
				error_count++;
			}
			$$->setDataType(info->getDataType());
			$$->stackOffset = info->stackOffset;
			$$->varName = "[BX]";
			
		}else{
			errFile<<"Line# "<<line_count<<": Undeclared variable '"<<$1->getName()<<"'\n";
			error_count++;
		}
	} 	
	;
	 
 expression : logic_expression{
		$$ = new SymbolInfo("logic_expression", "expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType($1->getDataType());
	}	
	| variable ASSIGNOP logic_expression{
		$$ = new SymbolInfo("variable ASSIGNOP logic_expression", "expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);



		if($1->getDataType()== "VOID" || $3->getDataType()== "VOID")
		{
			errFile<<"Line# "<<line_count<<": Void cannot be used in expression\n";
			error_count++;
		}
		else if($1->getDataType()== "INT" && $3->getDataType()=="FLOAT"){
			errFile<<"Line# "<<line_count<<": Warning: possible loss of data in assignment of FLOAT to INT\n";
			error_count++;
		}
		$$->setDataType($1->getDataType());
	}	
	;
			
logic_expression : rel_expression{
		$$ = new SymbolInfo("rel_expression", "logic_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType($1->getDataType());
	}	
	| rel_expression LOGICOP rel_expression{
		$$ = new SymbolInfo("rel_expression LOGICOP rel_expression", "logic_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		$$->setDataType("INT");
	}	 	
	;
	
rel_expression	: simple_expression{
		$$ = new SymbolInfo("simple_expression", "rel_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType($1->getDataType());
	} 
	| simple_expression RELOP simple_expression{
		$$ = new SymbolInfo("simple_expression RELOP simple_expression", "rel_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		$$->setDataType("INT");
	} 
	;
				
simple_expression : term{
		$$ = new SymbolInfo("term", "simple_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType($1->getDataType());
	} 
	| simple_expression ADDOP term{
		$$ = new SymbolInfo("simple_expression ADDOP term", "simple_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		$$->setDataType(typeCasting($1->getDataType(),$3->getDataType()));
	} 
	;
					
term :	unary_expression{
		$$ = new SymbolInfo("unary_expression", "term");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType($1->getDataType());
	}
	|  term MULOP unary_expression{
		$$ = new SymbolInfo("term MULOP unary_expression", "term");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		if($2->getName()=="%")
		{
			$$->setDataType("INT");
			if(!($1->getDataType()=="INT" && $3->getDataType()=="INT")){
				errFile<<"Line# "<<line_count<<": Operands of modulus must be integers\n";
				error_count++;
			}
			if(findLeafVal($3)=="0"){
				errFile<<"Line# "<<line_count<<": Warning: division by zero\n";
				error_count++;
			}
		}
		else if($2->getName()=="/")
		{
			if(findLeafVal($3)=="0"){
				errFile<<"Line# "<<line_count<<": Warning: division by zero\n";
				error_count++;
			}
			$$->setDataType(typeCasting($1->getDataType(),$3->getDataType()));
		}
		else
			$$->setDataType(typeCasting($1->getDataType(),$3->getDataType()));
	}
	;

unary_expression : ADDOP unary_expression{
		$$ = new SymbolInfo("ADDOP unary_expression", "unary_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);

		$$->setDataType($2->getDataType());
	}  
	| NOT unary_expression{
		$$ = new SymbolInfo("NOT unary_expression", "unary_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);

		$$->setDataType("INT");
	}  
	| factor{
		$$ = new SymbolInfo("factor", "unary_expression");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType($1->getDataType());
	}
	;
	
factor	: variable{
		$$ = new SymbolInfo("variable", "factor");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType($1->getDataType());
	} 
	| ID LPAREN argument_list RPAREN{
		$$ = new SymbolInfo("ID LPAREN argument_list RPAREN", "factor");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($4->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);
		$$->addChild($4);
		SymbolInfo* info = symbolTable->LookUp($1->getName());
		
		if(info==NULL)
		{
			errFile<<"Line# "<<line_count<<": "<<"Undeclared function '"<<$1->getName()<<"'\n";
			error_count++;
		}
		else if(info->getInfoType() != 4)
		{
			if(info->getInfoType() == 3){
				//errFile<<"Line# "<<line_count<<": "<<"Function '"<<$1->getName()<<"' not defined\n";
				//error_count++;
			}
			else{
				errFile<<"Line# "<<line_count<<": '"<<$1->getName()<<"' is not a function\n";
				error_count++;
			}

			$$->setDataType(info->getDataType());	
		}
		else
		{
			if(info->getParamSize() != $3->getParamSize())
			{
				if($3->getParamSize() < info->getParamSize()){
					errFile<<"Line# "<<line_count<<": Too few arguments to function '"<<$1->getName()<<"'\n";
					error_count++;
				}
				else{
					errFile<<"Line# "<<line_count<<": Too many arguments to function '"<<$1->getName()<<"'\n";
					error_count++;
				}

				$$->setDataType(info->getDataType());	
			}
			else
			{
				vector<SymbolInfo*> paramList1 = info->getParam();
				vector<SymbolInfo*> paramList2 = $3->getParam();
				for(int i=0; i<$3->getParamSize(); i++)
				{
					if(paramList1[i]->getDataType() != paramList2[i]->getDataType())
					{
						errFile<<"Line# "<<line_count<<": Type mismatch for argument "<<i+1<<" of '"<<$1->getName()<<"'\n";
						error_count++;
					}
				}
				$$->setDataType(info->getDataType());	
			}
		}
		//*/
		
	}
	| LPAREN expression RPAREN{
		$$ = new SymbolInfo("LPAREN expression RPAREN", "factor");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		$$->setDataType($2->getDataType());
	}
	| CONST_INT{
		$$ = new SymbolInfo("CONST_INT", "factor");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType("INT");
	} 
	| CONST_FLOAT{
		$$ = new SymbolInfo("CONST_FLOAT", "factor");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setDataType("FLOAT");
	}
	| variable INCOP{
		$$ = new SymbolInfo("variable INCOP", "factor");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->setDataType($1->getDataType());
	} 
	| variable DECOP{
		$$ = new SymbolInfo("variable DECOP", "factor");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($2->getEl());
		$$->addChild($1);
		$$->addChild($2);

		$$->setDataType($1->getDataType());
	} 
	;
	
argument_list : arguments{
		$$ = new SymbolInfo("arguments", "argument_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->setParam($1->getParam());
		$$->stackOffset = $1->stackOffset;
	}
	| 
	{
		$$ = new SymbolInfo("", "argument_list");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl(line_count);
		$$->setEl(line_count);
	}
	;
	
arguments : arguments COMMA logic_expression{
		$$ = new SymbolInfo("arguments COMMA logic_expression", "arguments");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($3->getEl());
		$$->addChild($1);
		$$->addChild($2);
		$$->addChild($3);

		
		$$->setParam($1->getParam());
		$$->addParam($3);
		$$->stackOffset = $1->stackOffset + 2;
	}
	| logic_expression{
		$$ = new SymbolInfo("logic_expression", "arguments");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->addParam($1);
		$$->stackOffset = 2;
	}
	;

_lcurl : LCURL{
		symbolTable->enterScope();
		if(!paramList.empty())
			declareFuncParamList();
		$$ = $1;
	}
 

%%
int main(int argc,char *argv[])
{
    FILE *fp;
	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	freopen("1905113_log.txt", "w", stdout);

    errFile.open("1905113_error.txt");
    treeFile.open("1905113_ parsetree.txt");
	codeFile.open("code.asm");
	optCodeFile.open("optcode.asm");

	yyin=fp;
	yyparse();
	

	fclose(yyin);
    errFile.close();
    treeFile.close();
	codeFile.close();
	optCodeFile.close();
	return 0;
}

