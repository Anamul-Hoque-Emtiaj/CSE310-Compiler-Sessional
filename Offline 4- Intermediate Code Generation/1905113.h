#include <iostream>
#include <sstream>
#include <vector>
using namespace std;




class SymbolInfo
{
    string name,type;
    SymbolInfo *nextInfo;

    string dataType; // data type of the variable or array or return type of the function
    int infoType; // variable or function (1=variable, 2=array, 3=function_declaration, 4=function_definition)
    string arraySize; // store array size if it is an array 
    
    
    vector<SymbolInfo*> param_list;  // parameter list for function declaration, definition and arg_list

    int sl,el;
    bool isleaf = false; 
    vector<SymbolInfo*> child_list; 
public:

    string varName = "";
    int stackOffset = 0;

    SymbolInfo()
    {
        this->nextInfo = NULL;
    }

    SymbolInfo(string name, string type)
    {
        this->name = name;
        this->type = type;
        this->nextInfo = NULL;
    }
    ~SymbolInfo()
    {
        delete nextInfo;
        param_list.clear();
        child_list.clear();
    }
    SymbolInfo(string name, string type, string dtype)
    {
        this->name = name;
        this->type = type;
        this->dataType = dtype;
        this->nextInfo = NULL;
    }

    SymbolInfo(string name, string type, int line)
    {
        this->name = name;
        this->type = type;
        this->nextInfo = NULL;
        sl = line;
        el = line;
        isleaf = true;
    }

    // copy constructor
    SymbolInfo(const SymbolInfo &ob)
    {
        this->name = ob.name;
        this->type = ob.type;
        this->nextInfo = ob.nextInfo;
    }

    // setters
    void setName(string name)
    {
        this->name = name;
    }

    void setType(string type)
    {
        this->type = type;
    }

    void setNextInfo(SymbolInfo *nextInfo)
    {
        this->nextInfo = nextInfo;
    }
    // getters
    string getName()
    {
        return name;
    }

    string getType()
    {
        return type;
    }

    SymbolInfo *getNextInfo()
    {
        return nextInfo;
    }

    void setDataType(string dataType)
    {
        this->dataType = dataType;
    }

    string getDataType()
    {
        return dataType;
    }

    void setInfoType(int infoType)
    {
        this->infoType = infoType;
    }

    int getInfoType()
    {
        return infoType;
    }

    void setArraySize(string arraySize)
    {
        this->arraySize = arraySize;
    }

    string getArraySize()
    {
        return arraySize;
    }
   
    int getParamSize() 
    {
        if(param_list.empty())
            return 0;
        return param_list.size();
    }

    void addParam(SymbolInfo* info) 
    {
        param_list.push_back(info);
    }

    void setParam(vector<SymbolInfo*> pList)
    {
        param_list = pList;
    }
    SymbolInfo* getParam(int index) 
    {
        return param_list[index];
    }

    vector<SymbolInfo*> getParam()
    {
        return param_list;
    }
    void setEl(int line)
    {
        el = line;
    }

    void setSl(int line)
    {
        sl = line;
    }
    void setIsleaf(bool _is)
    {
        isleaf = _is;
    }
    
    bool getIsleaf()
    {
        return isleaf;
    }

    int getEl()
    {
        return el;
    }

    int getSl()
    {
        return sl;
    }

    void addChild(SymbolInfo* sym) 
    {
        child_list.push_back(sym);
    }
    
    vector<SymbolInfo*> getChild()
    {
        return child_list;
    }

    void print()
    {
        if(this->type == "FUNCTION" || this->type == "ARRAY" )
            cout << "<" << this->name << ", " << this->type << ", "<<this->dataType<<"> ";
        else
            cout << "<" << this->name << ", " << this->dataType << "> "; 
    }
};

class ScopeTable
{
    int num_buckets;
    ScopeTable *parent_scope;
    SymbolInfo **hashTable;
    int scopeNum;

    unsigned long long int sdbm_hash(string str)
    {
        unsigned long long int hashVal = 0;
        int i = 0;
        int len = str.length();

        for (i = 0; i < len; i++)
        {
            hashVal = (str[i]) + (hashVal << 6) + (hashVal << 16) - hashVal;
        }

        return hashVal % num_buckets;
    }
public:
    int stackOffset = 0;
    ScopeTable(int num_buckets)
    {
        this->num_buckets = num_buckets;
        hashTable = new SymbolInfo*[num_buckets];
        for (int i = 0; i < num_buckets; i++)
            hashTable[i] = NULL;
    }
    ~ScopeTable()
    {
        for (int i=0; i<num_buckets; i++)
        {
            delete hashTable[i];
        }
        delete[] hashTable;
    }

    void setScopeNum(int n)
    {
        this->scopeNum = n;
    }

    void setParent_scope(ScopeTable *parent_scope)
    {
        this->parent_scope = parent_scope;
    }

    ScopeTable* getParent_scope()
    {
        return parent_scope;
    }

    int getScopeNum()
    {
        return scopeNum;
    }

    bool Insert(string name, string type)
    {
        bool f = true;
        unsigned long long int hashVal = sdbm_hash(name);
        int c = 1;
        if(hashTable[hashVal]!=NULL)
        {
            SymbolInfo* tmp = hashTable[hashVal];
            while(1)
            {
                c++;
                if(tmp->getName()==name)
                {
                    f = false;
                    break;
                }
                if(tmp->getNextInfo()==NULL)
                    break;
                tmp = tmp->getNextInfo();
            }
        }
        if(f)
        {
            if(hashTable[hashVal]==NULL)
            {
                SymbolInfo *nn = new SymbolInfo(name,type);
                if(scopeNum!=1)
                {
                    stackOffset-=2;
                    nn->stackOffset = stackOffset;
                }
                hashTable[hashVal] = nn;
            }
            else
            {
                SymbolInfo *nn = new SymbolInfo(name,type);
                if(scopeNum!=1)
                {
                    stackOffset-=2;
                    nn->stackOffset = stackOffset;
                }
                SymbolInfo *cur = hashTable[hashVal];
                while(1)
                {
                    if(cur->getNextInfo()==NULL)
                    {
                        cur->setNextInfo(nn);
                        break;
                    }
                    cur = cur->getNextInfo();
                }
            }
        }
        return f;
    }

    bool Insert(string name, string type, string dtype)
    {
        bool f = true;
        unsigned long long int hashVal = sdbm_hash(name);
        int c = 1;
        if(hashTable[hashVal]!=NULL)
        {
            SymbolInfo* tmp = hashTable[hashVal];
            while(1)
            {
                c++;
                if(tmp->getName()==name)
                {
                    f = false;
                    break;
                }
                if(tmp->getNextInfo()==NULL)
                    break;
                tmp = tmp->getNextInfo();
            }
        }
        if(f)
        {
            if(hashTable[hashVal]==NULL)
            {
                SymbolInfo *nn = new SymbolInfo(name,type,dtype);
                hashTable[hashVal] = nn;
            }
            else
            {
                SymbolInfo *nn = new SymbolInfo(name,type,dtype);
                SymbolInfo *cur = hashTable[hashVal];
                while(1)
                {
                    if(cur->getNextInfo()==NULL)
                    {
                        cur->setNextInfo(nn);
                        break;
                    }
                    cur = cur->getNextInfo();
                }
            }
        }
        return f;
    }

    SymbolInfo* LookUp(string name)
    {
        unsigned long long int hashVal = sdbm_hash(name);
        if(hashTable[hashVal]==NULL)
        {
            
            return NULL;
        }
        else
        {
            SymbolInfo* tmp = hashTable[hashVal];
            int c = 1;
            while(1)
            {
                if(tmp->getName()==name)
                {
                    return tmp;
                }
                if(tmp->getNextInfo()==NULL)
                    break;
                tmp = tmp->getNextInfo();
                c++;
            }
            return NULL;
        }
    }
    bool Delete(string name)
    {
        bool f = false;
        unsigned long long int hashVal = sdbm_hash(name);
        int c = 1;
        if(hashTable[hashVal]!=NULL)
        {
            SymbolInfo* tmp = hashTable[hashVal];
            while(1)
            {

                if(tmp->getName()==name)
                {
                    f = true;
                    break;
                }
                if(tmp->getNextInfo()==NULL)
                    break;
                tmp = tmp->getNextInfo();
                c++;
            }
        }
        if(f)
        {
            if(hashTable[hashVal]->getName()==name)
            {
                hashTable[hashVal] = hashTable[hashVal]->getNextInfo();
            }
            else
            {
                SymbolInfo *prev = hashTable[hashVal];
                while(1)
                {
                    if(prev->getNextInfo()->getName()==name)
                    {
                        prev->setNextInfo(prev->getNextInfo()->getNextInfo());
                        break;
                    }
                    prev = prev->getNextInfo();
                }
            }
        }
        return f;
    }

    void print()
    {
        cout<<"\t"<<"ScopeTable# "<<scopeNum<<"\n";
        for (int i = 0; i < num_buckets; i++)
        {
            SymbolInfo *cur = hashTable[i];
            if(cur!=NULL)
            {
                cout<<"\t"<<i+1<<"--> ";
                while(1)
                {
                    if(cur==NULL)
                    {
                        break;
                    }
                    cur->print();
                    cur = cur->getNextInfo();
                }
                cout<<"\n";
            }
        }
    }
};

class SymbolTable
{
    ScopeTable *currentScope;
    int scopeCount,num_buckets;
public:
    SymbolTable(int num_buckets)
    {
        scopeCount = 1;
        this->num_buckets = num_buckets;
        currentScope = new ScopeTable(num_buckets);
        currentScope->setScopeNum(scopeCount);
        currentScope->setParent_scope(NULL);
    }
    ~SymbolTable()
    {
        ScopeTable *tmp;
        while(currentScope!=NULL)
        {
            tmp = currentScope->getParent_scope();
            delete currentScope;
            currentScope = tmp;
        }
    }
    void enterScope()
    {
        scopeCount++;
        ScopeTable *tmp = new ScopeTable(num_buckets);
        tmp->setScopeNum(scopeCount);
        tmp->setParent_scope(currentScope);
        currentScope = tmp;
    }

    void exitScope()
    {
        if(currentScope->getParent_scope()!=NULL)
        {
            ScopeTable *tmp =  currentScope->getParent_scope();
            delete currentScope;
            currentScope = tmp;
        }
    }

    bool Insert(string name, string type)
    {
        if(currentScope->Insert(name,type))
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }

    bool Insert(string name, string type, string dtype)
    {
        if(currentScope->Insert(name,type, dtype))
        {
            return true;
        }
        else
        {
            return false;
        }
        
    }

    bool Delete(string name)
    {
        return currentScope->Delete(name);
    }

    void printCur()
    {
        currentScope->print();
    }

    void printAll()
    {
        ScopeTable *cur = currentScope;
        while(cur!=NULL)
        {
            cur->print();
            cur = cur->getParent_scope();
        }
    }

    SymbolInfo* LookUp(string name)
    {
        ScopeTable *cur = currentScope;
        SymbolInfo* inf = NULL;
        while(cur!=NULL && inf==NULL)
        {
            inf = cur->LookUp(name);
            cur = cur->getParent_scope();
        }
        return inf;
    }

    SymbolInfo* LookUp2(string name)
    {
        return  currentScope->LookUp(name);
    }

    int getCurScopeNum()
    {
        return currentScope->getScopeNum();
    }

    ScopeTable* getCurScope()
    {
        return currentScope;
    }
};