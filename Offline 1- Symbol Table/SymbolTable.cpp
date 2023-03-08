#include<iostream>
#include <sstream>
#include <vector>
using namespace std;

class SymbolInfo
{
    string name,type;
    SymbolInfo *nextInfo;
public:

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

    void print()
    {
        cout << "<" << this->name << "," << this->type << "> ";
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

    ScopeTable(int num_buckets)
    {
        this->num_buckets = num_buckets;
        hashTable = new SymbolInfo*[num_buckets];
        for (int i = 0; i < num_buckets; i++)
            hashTable[i] = NULL;
    }
    ~ScopeTable()
    {
        cout<<"\t"<<"ScopeTable# "<<scopeNum<<" removed\n";
        for (int i=0; i<num_buckets; i++)
        {
            delete hashTable[i];
        }
        delete[] hashTable;
    }

    void setScopeNum(int n)
    {
        this->scopeNum = n;
        cout<<"\t"<<"ScopeTable# "<<scopeNum<<" created\n";
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
                hashTable[hashVal] = nn;
            }
            else
            {
                SymbolInfo *nn = new SymbolInfo(name,type);
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
            cout<<"\t"<<"Inserted in ScopeTable# "<<scopeNum<<" at position "<<hashVal+1<<", "<<c<<"\n";
        }
        else
        {
            cout<<"\t"<<"'"<<name<<"'"<<" already exists in the current ScopeTable\n";
        }
        return f;
    }
    SymbolInfo* LookUp(string name)
    {
        unsigned long long int hashVal = sdbm_hash(name);
        if(hashTable[hashVal]==NULL)
        {
            if(parent_scope==NULL)
                cout<<"\t"<<"'"<<name<<"'"<<" not found in any of the ScopeTables\n";
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
                    cout<<"\t"<<"'"<<name<<"'"<<" found in ScopeTable# "<<scopeNum<<" at position "<<hashVal+1<<", "<<c<<"\n";
                    return tmp;
                }
                if(tmp->getNextInfo()==NULL)
                    break;
                tmp = tmp->getNextInfo();
                c++;
            }
            if(parent_scope==NULL)
                cout<<"\t"<<"'"<<name<<"'"<<" not found in any of the ScopeTables\n";
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
            cout<<"\t"<<"Deleted '"<<name<<"' from ScopeTable# "<<scopeNum<<" at position "<<hashVal+1<<", "<<c<<"\n";
        }
        else
        {
            cout<<"\t"<<"Not found in the current ScopeTable\n";
        }
        return f;
    }

    void print()
    {
        cout<<"\t"<<"ScopeTable# "<<scopeNum<<"\n";
        for (int i = 0; i < num_buckets; i++)
        {
            SymbolInfo *cur = hashTable[i];
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
        else
        {
            cout<<"\t"<<"ScopeTable# "<<currentScope->getScopeNum()<<" cannot be removed\n";
        }
    }

    bool Insert(string name, string type)
    {
        return currentScope->Insert(name,type);
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
};

int split (const string &s, string str[])
{
    stringstream ss (s);
    string item;
    int c=0;
    while (getline (ss, item, ' '))
    {
        str[c] = item;
        c++;
    }
    return c;
}

int main()
{
    freopen("testin.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string line;
    getline(cin,line);
    int num_buckets,c=1;
    num_buckets = stoi(line);
    SymbolTable *st = new SymbolTable(num_buckets);
    while(1)
    {
        getline(cin,line);
        cout<<"Cmd "<<c++<<": "<<line<<"\n";
        string r[100];
        int c = split(line, r);
        string cmd,name,type;
        cmd = r[0];

        if(cmd=="I")
        {
            if(c==3)
            {
                name = r[1];
                type = r[2];
                st->Insert(name,type);
            }
            else
                cout<<"\t"<<"Number of parameters mismatch for the command I\n";
        }
        else if(cmd=="L")
        {
            if(c==2)
            {
                name = r[1];
                st->LookUp(name);
            }
            else
                cout<<"\t"<<"Number of parameters mismatch for the command L\n";
        }
        else if(cmd=="D")
        {
            if(c==2)
            {
                name = r[1];
                st->Delete(name);
            }
            else
                cout<<"\t"<<"Number of parameters mismatch for the  command D\n";

        }
        else if(cmd=="P")
        {
            if(c==2)
            {
                type = r[1];
                if(type=="A")
                    st->printAll();
                else if(type=="C")
                    st->printCur();
                else
                {
                    cout<<"\t"<<"Number of parameters mismatch for the command P\n";
                }
            }
            else
                cout<<"\t"<<"Number of parameters mismatch for the command P\n";
        }
        else if(cmd=="S")
        {
            if(c==1)
                st->enterScope();
            else
                cout<<"\t"<<"Number of parameters mismatch for the command S\n";

        }
        else if(cmd=="E")
        {
            if(c==1)
                st->exitScope();
            else
                cout<<"\t"<<"Number of parameters mismatch for the command E\n";
        }
        else if(cmd=="Q")
        {
            if(c==1)
            {
                delete st;
                break;
            }
            else
                cout<<"\t"<<"Number of parameters mismatch for the command S\n";
        }
        else
        {
            cout<<"\t"<<"Wrong command given!\n";
        }
    }

}
