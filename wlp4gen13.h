#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <memory>
using namespace std;        // Know this is bad, but didn't want to have to add "std::"s in front of everything

int offset = 0;
int ifcounter=0; 
int whilecounter=0; 
int deletecounter=0;

/****** A8 (Parse tree creation, symbol table creation, semantic error checking) *****/

// Global variables
const vector<string> terminals = {"BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID", "IF", "INT", "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS", "PRINTLN", "RBRACE", "RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", "RBRACK", "NEW", "DELETE", "NULL"};
string currProc = ""; 
string sigCountProc = "";   // the name of the procedure for which we are counting the # signatures 
int sigLength = 0;           // length of signature of the current procedure
typedef map<string, pair<string, int>> VariableTable; // key is variable name, value is type and offset 
typedef vector<string> Signature; // procedure signature is a vector of strings (types).
typedef map<string, pair<Signature, VariableTable>> ProcedureTable;
ProcedureTable symbolTable;


// Print error messages and throw  
void printExitErrMsg(string msg = ""){
  cerr << "ERROR: " << msg << endl;
  throw 100;    // chose error # to be 100
}

// Prints the symbol table 
void printSymTable (){
    for(ProcedureTable::iterator it = symbolTable.begin(); it != symbolTable.end(); it++){
        
        // print procedure name 
        string proc = it->first; 
        cerr << proc; 

        // print signature (if exist) 
        for (int i = 0; i < it->second.first.size(); i++){
            cerr << " " << it->second.first.at(i);
        }
        cerr << endl; 

        // print variables (if exist)
        for (VariableTable::iterator it2 = it->second.second.begin(); it2 != it->second.second.end(); it2++){
            cerr << it2->first << " "  << it2->second.first << " " <<  it2->second.second << endl;
        }
        cerr << endl; 
    }
}

// Parse Tree structure 
struct Node{
    string rule; 
    vector<string> tokens;
    vector<shared_ptr<Node>> children;  
    string type=""; 
    
    string getType(){ 

        if (this->type != ""){
            return this->type; 
        }

        string lhs = this->tokens.at(0); 

        // terminals 
        if (lhs == "ID"){
            string rhs = this->tokens.at(1);
            this->type = symbolTable[currProc].second[rhs].first;
            
        } else if (lhs == "NUM"){
            this->type = "int"; 

        } else if (lhs == "NULL" ){
            this->type = "int*"; 

        } else if (this->rule == "dcl type ID" ){
            this->type = this->children.at(0)->getType();

        } else if (this->rule == "type INT" ){
            this->type = "int";

        } else if (this->rule == "type INT STAR" ){
            this->type = "int*";

        } else if (this->rule == "factor ID" || this->rule == "factor NUM" || this->rule == "factor NULL" || this->rule == "lvalue ID") { 
            this->type = this->children.at(0)->getType();

        } else if (this->rule == "factor LPAREN expr RPAREN" || this->rule == "lvalue LPAREN expr RPAREN") { 
            this->type = this->children.at(1)->getType();

        } else if (this->rule == "factor AMP lvalue") { 
            if(this->children.at(1)->getType() != "int") {
                printExitErrMsg("operator & expects int operand"); 
            }
            this->type = "int*";

        } else if (this->rule == "factor STAR factor" || this->rule == "lvalue STAR factor") { 

            if(this->children.at(1)->getType() != "int*") {
                printExitErrMsg("cannot dereference int, only int* allowed"); 
            }
            this->type = "int";

        } else if (this->rule == "factor NEW INT LBRACK expr RBRACK") { 
            if(this->children.at(3)->getType() != "int") {
                printExitErrMsg("operator & expects int operand"); 
            }
            this->type = "int*";

        } else if (this->rule == "factor ID LPAREN RPAREN") { 
            
            string proc_name = this->children.at(0)->tokens.at(1); 

            if (symbolTable[proc_name].first.size() > 0){
                printExitErrMsg("expects no signature for " +(this->children.at(0)->tokens.at(1))); 
            }
            this->type = "int";

        } else if (this->rule == "factor ID LPAREN arglist RPAREN"){

            sigCountProc = this->children.at(0)->tokens.at(1); 
            sigLength = 0; 
            this->type = this->children.at(2)->getType(); 

        } else if(this->rule =="arglist expr COMMA arglist"){
            sigLength++; 

            int correctLength = symbolTable[sigCountProc].first.size(); 
            if(sigLength > correctLength) {
                printExitErrMsg("too many params for " +sigCountProc); 
            } else {
                // check if type of expr and the one in symbolTable matches
                if (this->children.at(0)->getType() != symbolTable[sigCountProc].first.at(sigLength-1)){
                    printExitErrMsg("incorrect type for param " +this->children.at(0)->children.at(0)->children.at(0)->children.at(0)->rule+ " vs " + symbolTable[sigCountProc].first.at(sigLength)+ " in signature of " +sigCountProc); 
                }
                // keep recursing 
                this->type = this->children.at(2)->getType();     
            }
            
        } else if(this->rule =="arglist expr"){
            sigLength++; 

            int correctLength = symbolTable[sigCountProc].first.size(); 

            if(sigLength != correctLength) {
                printExitErrMsg("incorrect # of params for " +sigCountProc); 
            } else {
                // check if type of expr and the one in symbolTable matches
                if (this->children.at(0)->getType() != symbolTable[sigCountProc].first.at(sigLength-1)){
                    printExitErrMsg("incorrect type for param " +this->children.at(0)->getType()+ " in signature of " +sigCountProc); 
                }
            }
            //base case for procedure call with params
            this->type = "int";
            
        } else if (this->rule == "term factor" ){
            this->type = this->children.at(0)->getType(); 

         // all other cases of prod rules with term on the lhs
        } else if (lhs == "term"){
             if(this->children.at(0)->getType() != "int" || this->children.at(2)->getType() != "int") {
                printExitErrMsg("term and factor must be type int"); 
            }
            this->type = "int"; 

        } else if (this->rule == "expr term" ){
            this->type = this->children.at(0)->getType(); 

        } else if (this->rule == "expr expr PLUS term"){
            if(this->children.at(0)->getType() == "int" && this->children.at(2)->getType() == "int") {
                this->type = "int"; 
            } else if (this->children.at(0)->getType() == "int*" && this->children.at(2)->getType() == "int") {
                this->type = "int*"; 
            } else if (this->children.at(0)->getType() == "int" && this->children.at(2)->getType() == "int*") {
                this->type = "int*"; 
            } else{
                printExitErrMsg("rhs of 'expr -> expr PLUS term' have wrong types"); 
            }
        
        } else if (this->rule == "expr expr MINUS term" ){
            if(this->children.at(0)->getType() == "int" && this->children.at(2)->getType() == "int") {
                this->type = "int"; 
            } else if (this->children.at(0)->getType() == "int*" && this->children.at(2)->getType() == "int") {
                this->type = "int*"; 
            } else if (this->children.at(0)->getType() == "int*" && this->children.at(2)->getType() == "int*") {
                this->type = "int"; 
            } else{
                printExitErrMsg("rhs of 'expr -> expr MINUS term' have wrong types"); 
            }
        
        } else if (this->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
            
            if(this->children.at(5)->getType() != "int"){
                printExitErrMsg("second param of wain must be int"); 
            }
            if(this->children.at(11)->getType() != "int"){
                printExitErrMsg("expr from wain must be int"); 
            }  

        } else if (this->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
            
            if(this->children.at(9)->getType() != "int"){    
                printExitErrMsg("procedure expr must be int"); 
            } 

        } 
        // statement / test rules
        else if (this->rule == "statement lvalue BECOMES expr SEMI"){
            if(this->children.at(0)->getType() != this->children.at(2)->getType()){
                printExitErrMsg( this->children.at(0)->getType() + " = "  +this->children.at(2)->getType() +" invalid. needs same type"); 
            } 

        } else if (this->rule == "statement PRINTLN LPAREN expr RPAREN SEMI"){
            if(this->children.at(2)->getType() != "int"){
                printExitErrMsg("println needs type int"); 
            } 

        } else if (this->rule == "statement DELETE LBRACK RBRACK expr SEMI"){
            if(this->children.at(3)->getType() != "int*"){
                printExitErrMsg("delete[] needs type int*"); 
            } 

        } else if (lhs == "test"){
            if(this->children.at(0)->getType() != this->children.at(2)->getType()){
                printExitErrMsg("test needs 2 same types"); 
            } 

        } else if (this->rule == "dcls dcls dcl BECOMES NUM SEMI"){
            if(this->children.at(1)->getType() != "int"){
                printExitErrMsg("dcls -> dcls dcl BECOMES NUM SEMI needs int"); 
            } 

        } else if (this->rule == "dcls dcls dcl BECOMES NULL SEMI"){
            if(this->children.at(1)->getType() != "int*"){
                printExitErrMsg("dcls -> dcls dcl BECOMES NULL SEMI needs int*"); 
            } 
        } else {
            this->type = "none"; 
        }
        return this->type;
    }
};


// helper function to determine whether "lhs" is a terminal or non-terminal 
bool isTerminal(string lhs){

   
    for (int i = 0; i < terminals.size(); i++){
        if (lhs == terminals.at(i)){
            return true;
        }
    }
    return false;

}

// returns root node of the tree, wtih rule="rule", tokens, and children
shared_ptr<Node> createTree (string rule){

    shared_ptr<Node> root = shared_ptr<Node>(new Node());

    root->rule = rule;
    istringstream iss(rule);
    string tok;
    while(iss >> tok){
        root->tokens.emplace_back(tok);
    }
    // if non-terminal
    if(!isTerminal(root->tokens.at(0))){
        for (int i = 1; i < root->tokens.size(); i++){
            string line;
            getline(cin, line);
            root->children.emplace_back(createTree(line)); 
        }
    }
    return root;
}



// traverse the parse tree and for each variable declaration, 
// print out the name and type. The rule "dcl type ID" 
// corresponds to a variable declaration.
void printTree (shared_ptr<Node> &tree){
    for (int i = 0; i < tree->children.size(); i++){
        if (!isTerminal(tree->children.at(i)->tokens.at(0))){  
            cerr << tree->children.at(i)->rule << endl;
        }
        printTree(tree->children.at(i));
    }
}


void addToSymTable (shared_ptr<Node> &tree){
    for (int i = 0; i < tree->children.size(); i++){
       
        shared_ptr<Node> currChild = tree->children.at(i); 
        
        // Procedure Declaration 
        if(currChild->tokens.at(0)== "main"){
             
            // addProcedure(currChild): 
                // extract name and signature of procedure 
                string name = "wain"; 
                vector<string> signature;
                
                //add to table 
            if(symbolTable.find("wain") == symbolTable.end()){
                currProc = "wain"; 
                offset = 0; 
                symbolTable.insert(make_pair(currProc, make_pair(Signature(), VariableTable())));
                // cerr << currProc << endl;

                string id1 = currChild->children.at(3)->children.at(0)->rule;
                string id2 = currChild->children.at(5)->children.at(0)->rule;

                if(id1 == "type INT STAR"){
                    symbolTable[currProc].first.emplace_back("int*");
                } else {
                    symbolTable[currProc].first.emplace_back("int");
                }

                if(id2 == "type INT STAR"){
                    symbolTable[currProc].first.emplace_back("int*");

                    // symbolTable[currProc].first.emplace_back(currChild->children.at(5)->children.at(0)->tokens.at(1) + currChild->children.at(3)->children.at(0)->tokens.at(2));
                } else {
                    symbolTable[currProc].first.emplace_back("int");
                }

            } else {
                // cerr << "ERROR: wain already declared" << endl; 
                // throw "error";
                printExitErrMsg("wain already declared");
            }
            // end of addProcedure(currChild)
        }

        // Procedure declaratoion
        else if (currChild->tokens.at(0) == "procedure") {
            // addProcedure(currChild):
             // extract name and signature of procedure 
                vector<string> signature; 
                string name = currChild->children.at(1)->tokens.at(1); 
            
            if(symbolTable.find(name) == symbolTable.end()){

                currProc = name;
                offset = 0; 

                symbolTable.insert(make_pair(currProc,  make_pair(signature, VariableTable())));

            } else {
                printExitErrMsg(name+" already declared");
            }
            // end of addProcedure(currChild)
        }
        // Procedure Signature 
        else if (currChild->tokens.at(0) == "paramlist"){ 
            //cerr << "params" << endl;
            // if there are parameters 
            if (currChild->children.size() > 0) {
                // paramlist's 
                //  -> children[0] = dcl's
                //   -> children[0] = type's 
                //    -> rule = type INT or type INT STAR 
                
                string type_rule = currChild->children.at(0)->children.at(0)->rule; 
                if (type_rule == "type INT STAR"){
                    symbolTable[currProc].first.emplace_back("int*");
                } else {
                    symbolTable[currProc].first.emplace_back("int");
                }
            }
                
        }
        
        // Variable Declaration 
        else if (currChild->rule == "dcl type ID"){  
         
            // extract 
            string id = currChild->children.at(1)->tokens.at(1);
            
            VariableTable varTable = symbolTable[currProc].second;
        

            // if variable not in variableTable yet 
            if(varTable.find(id) == varTable.end()){

                string type = ""; 
                if(currChild->children.at(0)->rule == "type INT STAR"){
                    type = "int*";
                } else{
                    type = "int";
                }

                symbolTable[currProc].second.insert(make_pair(id, make_pair(type, offset))); 
                // cerr << id << " " << symbolTable[currProc].second[id] << endl; // signature? 
                offset -= 4; 
            }
            else{
                printExitErrMsg(id +" already exists in "+ currProc );

            }
            // end of addVariable(currChild) 
        } 
        
        // Variable use
        else if (currChild->rule == "factor ID" || currChild->rule == "lvalue ID"){
           
            string id = currChild->children.at(0)->tokens.at(1);
            // isAccessibleVariable(id)
            VariableTable varTable = symbolTable[currProc].second;
            
            if(varTable.find(id) != varTable.end()){
                // true 
            } 
            else {
                printExitErrMsg(id +" undeclared in "+ currProc);
            }
            // end isAccessibleVariable(id)

        } 

        // Procedure Call
        else if (currChild->rule == "factor ID LPAREN RPAREN" || currChild->rule == "factor ID LPAREN arglist RPAREN"){
            string proc_name = currChild->children.at(0)->tokens.at(1);
            // isAccessibleProcedure(id)

            // if a local variable has same name as procedure being called, ERROR 
            VariableTable varTable = symbolTable[currProc].second;
           
            if (varTable.find(proc_name) != varTable.end()){
                printExitErrMsg("the procedure " + proc_name+ " being called is a local variable in " +currProc);
            }

            // if the procedure being called has not been declared yet 
            if(symbolTable.find(proc_name) == symbolTable.end()){
                printExitErrMsg(proc_name +" undeclared in "+ currProc);
            } else {
                // success
            }
            // end of isAccessibleProcedure(id)

        }
        addToSymTable(tree->children.at(i));
    }
}


// traverses the tree and calls getType 
// on every node that needs a type computed.
// update currentProcedure and offset as traverse tree
void computeTypes(shared_ptr<Node> &tree){
    for (int i = 0; i < tree->children.size(); i++){
        string lhs = tree->children.at(i)->tokens.at(0); 

        if(lhs == "procedure"){
            currProc = tree->children.at(1)->tokens.at(1);
            // offset = 0;

        } else if (lhs == "main"){
            currProc = "wain"; 
            // offset = 0;

        } 
        tree->children.at(i)->type = tree->children.at(i)->getType();       //redundant
        computeTypes(tree->children.at(i));
    }
}
