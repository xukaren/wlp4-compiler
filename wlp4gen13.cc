#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <memory>
#include "wlp4gen.h"    // separated A8 into another file 
using namespace std;

/**** AX: WLP4 to MIPS Code Generation  *******/

//<------- MIPS Code snippets  -----> 
void push(int reg_num){
    cout << "sw $" << reg_num << ", -4($30)          ; push " << reg_num << endl;
    cout << "sub $30, $30, $4" << endl;
} 

void pop (int reg_num){
    cout<< "add $30, $30, $4         ; pop " << reg_num << endl;
    cout << "lw $" << reg_num << ", -4($30)" << endl;
}

int getOffset (string id){
    return symbolTable[currProc].second[id].second;
}

void code(string var){
    cout << "lw $3, " << getOffset(var) << "($29)" << endl;
}

void comment(string msg){
    cout << "\n; " << msg << endl; 
}

void done_comment(string msg){
    cout << "; (" << msg << ")" << endl; 
}

void set(string var){
    cout << "sw $3, " << getOffset(var) << "($29)" << endl;
}

// load "val" into "reg_num"
void load(int reg_num, string val){
    cout << "lis $" << reg_num << endl;
    cout << ".word " << val << endl;
}

void label(string name){
    cout << name << ":" << endl; 
}

void call(int reg_num, string val){
    load(reg_num, val); 
    cout << "jalr $" << reg_num<< endl; 
}

void ftitle(string val){
    cout << "\n;<------------------ " << val << " ---------------->" << endl;
}
void fcomponent(string val){
    cout << "\n;---- F" << currProc << " " << val << " -----" << endl;
}

void component(string val){
    cout << "\n;---- " << currProc << " " << val << " -----" << endl;
}

//<-------  Functions to convert WLP4 to MIPS code -----> 

void generateCode(shared_ptr<Node> &tree){

    // Q1: Main 
    if (tree->rule == "start BOF procedures EOF"){
        generateCode(tree->children.at(1));


    } else if(tree->rule == "procedures main"){
        generateCode(tree->children.at(0));


    } else if (tree->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
        currProc = "wain";
        ftitle(currProc); 

        component("prologue");
        
        cout << ".import print" << endl;
        cout << ".import init" << endl;
        cout << ".import new" << endl;
        cout << ".import delete" << endl;

        load(4, "4");
        load(10, "print");
        load(11, "1");
       
        cout << "sub $29, $30, $4" << endl;

        // comment("Pushing 2 params of main");
        push(1);  
        push(2);  

        // call init 
        if (tree->children.at(3)->getType() == "int" ){ 
            cout << "add $2, $0, $0" << endl; 
        }
        push(31);
        call(5, "init"); 
        pop(31);

        component("body");
        generateCode(tree->children.at(8));
        generateCode(tree->children.at(9));

        comment("Returning from main");
        generateCode(tree->children.at(11));

        comment("Restoring stack at end of main");
        cout << "add $30, $30, $4" << endl;
        cout << "add $30, $30, $4" << endl;

        component("epilogue");
        cout << "add $30, $29, $4" << endl;
        cout << "jr $31\n" << endl;

    // } else if (tree->rule == "dcl type ID"){
    //     generateCode(tree->children.at(0));
    // } else if (tree->rule == "statements"){

    } else if (tree->rule == "expr term"){
        generateCode(tree->children.at(0));

    } else if (tree->rule == "term factor"){
        generateCode(tree->children.at(0));

    } else if (tree->rule == "factor ID"){
        generateCode(tree->children.at(0));

    } else if (tree->tokens.at(0) == "ID"){
        code(tree->tokens.at(1));
       
    // Q2: Expr, with brackets
    } else if (tree->rule == "factor LPAREN expr RPAREN"){
        generateCode(tree->children.at(1)); 
    
    // Q3: Arithmetic: Integer constants and operators {+, -, *, /, %} 
    } else if(tree->rule == "expr expr PLUS term" || tree->rule == "expr expr MINUS term" || tree->rule == "term term STAR factor"|| tree->rule == "term term SLASH factor" || tree->rule== "term term PCT factor"){

        cout << "; (Evaluating expr with arithmetic)" << endl; 
        string op = tree->tokens.at(2); 
        string type1 = tree->children.at(0)->getType();
        string type2 = tree->children.at(2)->getType(); 

        if(op == "PLUS"){   //add pointers 
            generateCode(tree->children.at(0));
            
            if(type1 == "int" && type2 == "int*"){
                cout << "mult $3, $4" << endl;
                cout << "mflo $3" << endl;
            }

            push(3);
            generateCode(tree->children.at(2)); 

            if(type1 == "int*" && type2 == "int"){
                cout << "mult $3, $4" << endl;
                cout << "mflo $3" << endl;
            } 

            pop(5);
            cout << "add $3, $5, $3" << endl;

        } else if(op == "MINUS"){   //add pointers
            generateCode(tree->children.at(0));
            push(3);
            generateCode(tree->children.at(2)); 

            if(type1 == "int*" && type2 == "int"){
                cout << "mult $3, $4" << endl;
                cout << "mflo $3" << endl;
            } 

            pop(5);
            cout << "sub $3, $5, $3" << endl;
            
            if(type1 == "int*" && type2 == "int*"){
                cout << "div $3, $4" << endl;
                cout << "mflo $3" << endl;
            }

        } else if(op == "STAR"){
            generateCode(tree->children.at(0));
            push(3);
            generateCode(tree->children.at(2)); 
            pop(5);
            cout << "mult $5, $3" << endl;
            cout << "mflo $3" << endl;

        } else if(op == "SLASH"){
            generateCode(tree->children.at(0));
            push(3);
            generateCode(tree->children.at(2)); 
            pop(5);
            cout << "div $5, $3" << endl;
            cout << "mflo $3" << endl;    

        } else if(op == "PCT"){
            generateCode(tree->children.at(0));
            push(3);
            generateCode(tree->children.at(2)); 
            pop(5);
            cout << "div $5, $3" << endl;
            cout << "mfhi $3" << endl;
        }

        done_comment ("Done evaluating expr with arithmetic"); 

        
    } else if (tree->rule == "factor NUM"){
        generateCode(tree->children.at(0));

    } else if (tree->tokens.at(0) == "NUM"){
        load(3, tree->tokens.at(1));
    } 
    // Q4: Println statement
    else if(tree->rule == "statements statements statement"){
        generateCode(tree->children.at(0));
        generateCode(tree->children.at(1));

    } else if (tree->rule =="statement PRINTLN LPAREN expr RPAREN SEMI"){
        cout << "\n; Printing " << endl;
        push(1);
        generateCode(tree->children.at(2));
        cout << "add $1, $3, $0" << endl;
        push(31);
        cout << "jalr $10                ; Print " << endl;
        pop(31);
        pop(1); 
        cout << endl; 

    // Q5: Variable declarations and Assignment Statements
    } else if (tree->rule =="statement lvalue BECOMES expr SEMI"){
        cout << "\n; Setting " << tree->children.at(0)->children.at(0)->tokens.at(1) << " to 'expr'" << endl;

        generateCode(tree->children.at(2));
        
        if (tree->children.at(0)->rule == "lvalue ID"){
            set(tree->children.at(0)->children.at(0)->tokens.at(1));
        
        } else if (tree->children.at(0)->rule == "lvalue STAR factor"){
            // generateCode(tree->children.at(0));  //check

            push(3);
            //which? 
            generateCode(tree->children.at(0)->children.at(1)); 
            pop(5);
            cout << "sw $5, 0($3)" << endl;
        }
        done_comment("Done setting " +tree->children.at(0)->children.at(0)->tokens.at(1) + " to 'expr'");

    // check:
    } else if (tree->rule =="lvalue ID"){
        comment (" >>>>>>>> !!!!!! NOTICE ME I AM AN OUTER VAL lvalue ID >>>>>>> "); 
        // load(to_string(getOffset(tree->children.at(1)->children.at(0)->tokens.at(1))));

        load(3, to_string(getOffset(tree->children.at(0)->tokens.at(1))));
        cout << "add $3, $3, $29" << endl; 

    } else if (tree->rule =="lvalue LPAREN lvalue RPAREN"){
        generateCode(tree->children.at(1));

    } else if (tree->rule == "dcls dcls dcl BECOMES NUM SEMI"){
        generateCode(tree->children.at(0));
        
        cout << "\n; Initializing " << tree->children.at(1)->children.at(1)->tokens.at(1) << " to " << tree->children.at(3)->tokens.at(1) << endl;
        
        load(3, tree->children.at(3)->tokens.at(1));
        push(3); 

        done_comment("Done initializing " +tree->children.at(1)->children.at(1)->tokens.at(1) +" to "+ tree->children.at(3)->tokens.at(1));
    
    // Q6: While loops
    } else if (tree->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE"){
        int curr_counter = whilecounter; 
        whilecounter++;

        comment("While loop");

        label("loop" + to_string(curr_counter)); 
        generateCode(tree->children.at(2));
        cout << "beq $3, $0, endWhile" << curr_counter << endl;
        
        generateCode(tree->children.at(5));
        cout << "beq $0, $0, loop"<< curr_counter << endl;
        label("endWhile" +to_string(curr_counter));
        comment("End while loop");
    
    // Q7: Comparison Relations (test)
    } else if(tree->tokens.at(0) == "test"){
       
        generateCode(tree->children.at(0));
        push(3);
        generateCode(tree->children.at(2));
        pop(5);

        string func = "slt"; 
        if (tree->children.at(0)->getType() == "int*"){
            func = "sltu"; 
        }

        string comp = tree->tokens.at(2); 
        if (comp == "LT" || comp == "GE"){
            cout << func << " $3, $5, $3" << endl;
            if (comp == "GE"){
                cout << "sub $3, $11, $3" << endl;
            }
        } else if (comp == "GT" || comp == "LE"){
            cout << func << " $3, $3, $5" << endl;
    
            if (comp == "LE"){
                cout << "sub $3, $11, $3" << endl;
            }
        } else if (comp == "NE"){
            // store $6 and $7? 
            cout << func << " $6, $3, $5" << endl;
            cout << func << " $7, $5, $3" << endl;
            cout << "add $3, $6, $7" << endl;
        } else if (comp == "EQ"){
            // store $6 and $7? 
            cout << func << " $6, $3, $5" << endl;
            cout << func << " $7, $5, $3" << endl;
            cout << "add $3, $6, $7" << endl;
            cout << "sub $3, $11, $3" << endl;
        } else if (comp == "LE"){
            // store $6 and $7? 
            cout << func << " $6, $3, $5" << endl;
            cout << func << " $7, $5, $3" << endl;
            cout << "add $3, $6, $7" << endl;
            cout << "sub $3, $11, $3" << endl;
        } 

    // Q8: If/Else statements
    } else if (tree->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE"){

        int curr_counter = ifcounter; 

        ifcounter++; 
        comment("If statement");
        generateCode(tree->children.at(2));
        cout << "beq $3, $0, else" << curr_counter << endl;
        generateCode(tree->children.at(5));
        cout << "beq $0, $0, endif" << curr_counter << endl;
        
        comment("Else statement");
        label("else" +to_string(curr_counter));
        generateCode(tree->children.at(9));
        label("endif" +to_string(curr_counter));


    // Q9: Pointers 
    } else if (tree->rule == "dcls dcls dcl BECOMES NULL SEMI"){
        generateCode(tree->children.at(0));
        
        cout << "\n; Initializing *" << tree->children.at(1)->children.at(1)->tokens.at(1) << " to " << tree->children.at(3)->tokens.at(1) << endl;
        
        cout << "add $3, $11, $0" << endl; 
        push(3);

    } else if (tree->rule == "factor NULL"){
        cout << "add $3, $0, $11" << endl;

// check 
    } else if (tree->rule == "factor AMP lvalue"){  
        comment("Taking the address"); 
        if (tree->children.at(1)->rule == "lvalue ID"){
            cout << "\n; ID " << tree->children.at(1)->children.at(0)->tokens.at(1) << endl;

            load(3, to_string(getOffset(tree->children.at(1)->children.at(0)->tokens.at(1))));
            cout << "add $3, $3, $29" << endl;
            // generateCode(tree->children.at(1));
            // cout << "lw $3, 0($3)" << endl;
        
    // check 
        } else if (tree->children.at(1)->rule == "lvalue STAR factor"){
            comment("Pointer");

            generateCode(tree->children.at(1)->children.at(1));
        }

    } else if (tree->rule == "factor STAR factor"){
        comment("Dereferencing"); 
        
        generateCode(tree->children.at(1));
        cout << "lw $3, 0($3)" << endl;
        done_comment("Done dereferencing");

    } else if (tree->rule == "lvalue STAR factor"){
        generateCode(tree->children.at(1));

    // Q10: Dynamic memory allocation (new and delete)
    } else if (tree->rule == "factor NEW INT LBRACK expr RBRACK"){
        generateCode(tree->children.at(3)); 
        cout << "add $1, $3, $0" << endl; 

        //call new 
        push(31);
        call(5,"new");
        pop(31);

        cout << "bne $3, $0, 1" << endl; 
        cout << "add $3, $11, $0" << endl; 


    } else if (tree->rule == "statement DELETE LBRACK RBRACK expr SEMI"){
        int currcounter = deletecounter; 
        deletecounter++; 

        generateCode(tree->children.at(3)); 
        cout << "beq $3, $11, skipDelete" << currcounter << endl; 
        cout << "add $1, $3, $0" << endl;

        //call delete
        push(31);
        call(5, "delete"); 
        pop(31);

        label("skipDelete" + to_string(currcounter));

    // Q11: Pointer arithmetic (in "Q3: Arithmetic "section)
    // Q12: Pointer comparison (in "Q7: Comparison Relations "section)

    // Q13: Procedures with no arguments (other than wain)

    } else if (tree->rule == "procedures procedure procedures"){
        generateCode(tree->children.at(1));
        generateCode(tree->children.at(0));

    } else if (tree->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE"){
        currProc = tree->children.at(1)->tokens.at(1);  
        // offset = 0;
        ftitle("F"+currProc);
        fcomponent("prologue");

        
        label("F" +tree->children.at(1)->tokens.at(1));
        
        cout << "sub $29, $30, $4" << endl; 

        fcomponent("body");

        generateCode(tree->children.at(6));

        //push regs that current procedure will modify 
        push(1);
        push(2);
        push(5);
        // push(29);
        // push(31);
        //generateCode(tree->children.at(3));
        
        generateCode(tree->children.at(7));
        generateCode(tree->children.at(9));

    // // restore saved regs? 
        // pop(31);
        // pop(29);
        pop(5);
        pop(2);
        pop(1);

        comment("Returning from F" +currProc); 

        fcomponent("epilogue");

        cout << "add $30, $29, $4" << endl; //?  
        cout << "jr $31" << endl; 

    // }  else if (tree->rule == "params paramlist"){
    //     generateCode(tree->children.at(0));
    // } else if (tree->rule == "paramlist dcl"){

    // } else if (tree->rule == "paramlist dcl COMMA paramlist"){
    //     generateCode(tree->children.at(2));
   
    } else if (tree->rule == "factor ID LPAREN RPAREN"){
        comment("Calling procedure F"+tree->children.at(0)->tokens.at(1) +" (no args)");
        
        push(29);
        push(31);
        call(5, "F"+tree->children.at(0)->tokens.at(1));
        pop(31);
        pop(29);

        done_comment("Done calling procedure F"+tree->children.at(0)->tokens.at(1) +" (no args)");

    // Q14: Procedures with arguments 
 
    // } else if (tree->rule == "factor ID LPAREN arglist RPAREN"){
    //     comment("Calling procedure F"+tree->children.at(0)->tokens.at(1) +" (has args)");
    //     push(29);
    //     push(31);
    //     generateCode(tree->children.at(2));
    //     call(5, "F"+tree->children.at(0)->tokens.at(1));
    //     //pop n times ?????
    //     pop(31);
    //     pop(29);

    // } else if (tree->rule == "arglist expr"){
    //     generateCode(tree->children.at(0)); 
    //     push(3);
    
    // } else if (tree->rule == "arglist expr COMMA arglist"){
    //     generateCode(tree->children.at(0)); 
    //     push(3);
    //     generateCode(tree->children.at(2));
    }
}
    

int main(){

    // Build parse tree 
    string line;
    getline(cin, line); 
    shared_ptr<Node> parseTree = createTree(line);

    // Add procedures/variables to symbol table 
    try{
        addToSymTable(parseTree);
    } catch(int i){
        return 0;
    }

    // Print symbol table to cerr
    printSymTable();

    // Type check (or terminate with ERROR)

    // try{
    //     computeTypes(parseTree); 
    // } catch(int i){  
    //     return 0; 
    // }

    // Print parse tree to cerr 
    cerr << parseTree->rule << endl;
    printTree(parseTree); 
    cerr << endl; 

    // Generate MIPS code 
   
    generateCode(parseTree); 

    return 1;
}


