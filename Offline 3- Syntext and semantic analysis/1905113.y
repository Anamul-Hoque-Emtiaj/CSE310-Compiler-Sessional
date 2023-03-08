%{
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<cmath>
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
	for(SymbolInfo* info: paramList){

		declareFuncParam(info->getDataType(), info->getName(), info->getInfoType());
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

		genTree($$,0);
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

				if(info->getInfoType()==2)
					newVar->setArraySize(info->getArraySize());
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

		if(!symbolTable->LookUp($3->getName())){
			errFile<<"Line# "<<line_count<<" : Undeclared variable '"<<$3->getName()<<"'\n";
			error_count++;
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
	}
	| logic_expression{
		$$ = new SymbolInfo("logic_expression", "arguments");
		cout<<$$->getType()<<" : "<<$$->getName()<<" "<<endl;
		$$->setSl($1->getSl());
		$$->setEl($1->getEl());
		$$->addChild($1);

		$$->addParam($1);
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
	

	yyin=fp;
	yyparse();
	

	fclose(yyin);
    errFile.close();
    treeFile.close();
	
	return 0;
}

