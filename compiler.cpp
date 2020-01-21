#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <regex>        
using namespace std;
//Chun Dong, Teng Yan, Xuanqi Xu
//File is tested with 
//{
//int a ;
//a = 1 ;
//while ( a < 10 ) {
//  a = a + 1 ; 
//}
//}
//it prints out a html tree as intended.
//The hardest part is finding all the Follow sets. 
//It doesn't get easier after practicing. 

/*
  Grammar modified for recursive descent parsing, 
  e is epsilon

   <program>  ::= <block>
   <block>    ::= { <decls> <stmts> }
   <decls>    ::= e 
                | <decl> <decls>
   <decl>     ::= <type> ID ;
   <type>     ::= BASIC <typeclosure>
   <typeclosure>   ::= e 
                    | [ NUM ] <typeclosure>
   <stmts>    ::= e 
                | <stmt> <stmts>
   <stmt>     ::= <loc> = <bool> ;
                | IF ( <bool> ) <stmt>
                | IF ( <bool> ) <stmt> ELSE <stmt>
                | WHILE ( <bool> ) <stmt>
                | <block>
   <loc>      ::= ID <locclosure>
   <locclosure>    ::= e 
                    | [ <bool> ] <locclosure>
   <bool>     ::= <join> <boolclosure>
   <boolclosure>   ::= e 
                    | || <join> <boolclosure>
   <join>     ::= <equality> <joinclosure>
   <joinclosure>   ::= e 
                    | && <equality> <joinclosure>
   <equality> ::= <rel> <equalclosure>
   <equalclosure>  ::= e 
                    | == <rel> <equalclosure> 
                    | != <rel> <equalclosure>
   <rel>      ::= <expr> <reltail>
   <reltail>  ::= e 
                | <= <expr>
                | >= <expr>
                | > <expr>
                | < <expr>
   <expr>     ::= <term> <exprclosure>
   <exprclosure>   ::= e
                  | + <term> <exprclosure>
                  | - <term> <exprclosure>
   <term>     ::= <unary> <termclosure>
   <termclosure>   ::= e
                  | * <unary> <termclosure>
                  | / <unary> <termclosure>
   <unary>    ::= ! <unary>
                | - <unary>
                | <factor>
   <factor>   ::= ( <bool> )
                | <loc>
                | NUM
                | REAL
                | TRUE
                | FALSE
 */
 

//	nonterminals. translated from figure 1.
enum nonterminal {
    TERMINAL,
    PROGRAM,
    BLOCK,
    DECLS,
    DECL,
    TYPE,
    TYPECLOSURE,
    STMTS,
    STMT,
    LOC,
    LOCCLOSURE,
    BOOL,
    BOOLCLOSURE,
    JOIN,
    JOINCLOSURE,
    EQUALITY,
    EQUALCLOSURE,
    REL,
    RELTAIL,
    EXPR,
    EXPRCLOSURE,
    TERM,
    TERMCLOSURE,
    UNARY,
    FACTOR
};

/*
 * Terminal types:
 */
enum vocab
{
  EOS,				//0
  OPEN_PAREN,		//1
  CLS_PAREN,		//2
  OPEN_BRACE,		//3
  CLS_BRACE,		//4
  OPEN_SQPAR,		//5
  CLS_SQPAR,		//6
  IF,				//7
  ELSE,				//8
  WHILE,			//9
  ID,				//10
  AND,				//11
  OR,				//12
  ASSIGN,			//13
  EQ,
  NEQ,
  LTEQ,
  GTEQ,
  LT,
  GT,
  PLUS,
  MINUS,
  NUM,
  REAL,
  TRUE,
  FALSE,
  NOT,
  MUL,
  DIV,
  BASIC,
  SEMICOLON		//30
};

// Token Structure
// a Token must contain the whole lexeme, and the type of the lexeme.
// Non-terminals never show up in input files. 
// Thus,only vocab type is needed.
class Token{
	public:
	  string str;     //Lexeme
	  vocab type;     //Type of Lexeme
	Token(string str, vocab voc)
	  {
	    this->str = str;
	    this->type = voc;
	  }
	Token()
	  {
	    this->type = EOS;
	  }
 };
 
//Use regular expression. 
bool isDouble(string s) {
    regex regex_double("(\\+|\\-)?[0-9]+.[0-9]+");
    bool test = regex_match(s,regex_double);
    return test;
}
bool isInteger(string s) {
    regex integer("(\\+|\\-)?[0-9]+");
    bool test = regex_match(s,integer);
	return test;
}

 //process which type of Token is it.
 Token getToken() {
    string str;

    if ( cin.eof() ) {
        return Token();  // EOS on end of the file
    }
    cin >> str;         // if it's not eos, then take next lexeme.
    if ( str == "") {
        return Token();  // if it's nothing, it's also EOS
    }

    if ( str == "(" )
        return Token(str, OPEN_PAREN);
    if ( str == ")" )
        return Token(str, CLS_PAREN);
    if ( str == "{" )
        return Token(str, OPEN_BRACE);
    if ( str == "}" )
        return Token(str, CLS_BRACE);
    if ( str == "[" )
        return Token(str, OPEN_SQPAR);
    if ( str == "]" )
        return Token(str, CLS_SQPAR);
    if ( str == "if" )
        return Token(str, IF);
    if ( str == "while" )
        return Token(str, WHILE);
    if ( str == "else" )
        return Token(str, ELSE);
    if ( str == "&&" )
        return Token(str, AND);
    if ( str == "||" )
        return Token(str, OR);
    if ( str == "=" )
        return Token(str, ASSIGN);
    if ( str == "==" )
        return Token(str, EQ);
    if ( str == "!=" )
        return Token(str, NEQ);
    if ( str == "<=" )
        return Token(str, LTEQ);
    if ( str == ">=" )
        return Token(str, GTEQ);
    if ( str == "<" )
        return Token(str, LT);
    if ( str == ">" )
        return Token(str, GT);
    if ( str == "+" )
        return Token(str, PLUS);
    if ( str == "-" )
        return Token(str, MINUS);
    if ( str == "true" )
        return Token(str, TRUE);
    if ( str == "false" )
        return Token(str, FALSE);
    if ( str == "!" )
        return Token(str, NOT);
    if ( str == "*" )
        return Token(str, MUL);
    if ( str == "/" )
        return Token(str, DIV);
    if ( str == ";" )
        return Token(str, SEMICOLON);
    if ( str == "int" )
        return Token(str, BASIC);
    if ( str == "char" )
        return Token(str, BASIC);
    if ( str == "bool" )
        return Token(str, BASIC);
    if ( str == "double" )
        return Token(str, BASIC);
    //NUM: only integers
    if (isInteger(str)) {
        return Token(str, NUM);
    }
    //REAL: other numbers excluding integer.
    if (isDouble(str)) {
        return Token(str, REAL);
    }
    //Everything else from declaration is an ID
    return Token(str, ID);
}

/****************************\
*   Data Structure for Node. *
*   Build an AST.            *
*\***************************/
class Node{
  public:
    Token token;				//if nt, token= default EOS
    nonterminal nt;				//if token, nt = null = 0 = TERMINAL
    list <Node*> children;
    Node(nonterminal ntmn){
    	this->nt = ntmn;
	}
	Node(Token t){
		this->token = t;
		this->nt    = TERMINAL;
	}
  //Add child to children node at the end.
  void addChild(Node* n){
    this->children.push_back(n);
  }
  //Used for printing. 
  bool isT(){
  	return this->nt==TERMINAL;
  }
  bool isNT(){
  	return this->nt!=TERMINAL;
  }
  //Print function. from Stack Overflow
  void print(int a = 0){
  	string info;
  	string blanks (a ,32) ;
	if(isT()){
		info = token.str;
		cout<<blanks<<info<<endl;
	}else{
		info = getNTinfo(nt);
		cout<<blanks<<"<"<<info<<">"<<endl;
	}
	for (list<Node*>::const_iterator i = children.begin(); i != children.end(); ++i) {
        Node* child = (*i);
        child->print(a+1);
    }
	//after everything, we need /html.
	if(isNT()){
		cout<<blanks<<"</"<<info<<">"<<endl;
	}
  }
  
  string getNTinfo(nonterminal nt){
    switch (nt) {
    case PROGRAM:
        return "program";
    case BLOCK:
        return "block";
    case DECLS:
        return "decls";
    case DECL:
        return "decl";
    case TYPE:
        return "type";
    case TYPECLOSURE:
        return "typeclosure";
    case STMTS:
        return "stmts";
    case STMT:
        return "stmt";
    case LOC:
        return "loc";
    case LOCCLOSURE:
        return "locclosure";
    case BOOL:
        return "bool";
    case BOOLCLOSURE:
        return "boolclosure";
    case JOIN:
        return "join";
    case JOINCLOSURE:
        return "joinclosure";
    case EQUALITY:
        return "equality";
    case EQUALCLOSURE:
        return "equalityclosure";
    case REL:
        return "rel";
    case RELTAIL:
        return "reltail";
    case EXPR:
        return "expr";
    case EXPRCLOSURE:
        return "exprclosure";
    case TERM:
        return "term";
    case TERMCLOSURE:
        return "termclosure";
    case UNARY:
        return "unary";
    case FACTOR:
        return "factor";
    default:
        return "Error:unknown NT";  
    }
}

};






/*construct a function for each non-terminal.
  Decide which function to call based on the next input Token.
  functions:
  Each function creates a current Node
  children are built by recursive calls
  return current Node.
*/

//get next token first. Begin the program. called in Main.
Token token;	

/*********\
* mustBe *
\*********/
void mustBe(vocab v){
	//If it's not the expected token, give error message and quit.
	if(v!=token.type){
		cerr<<"Wrong token: "<<token.str<<" Expected: Vocab@"<<v<<"\n";			//unbuffered error message. Display immediately.
		exit(1);									    //exit with not completed.
	}else{
		token = getToken();						        // else, get next token.
	}
}


/****************************\
* Functions for nonterminals
\****************************/
Node* Block();
Node* Bool();
Node* Loc();  

// <factor>   ->( <bool> )
//              | <loc>
//              | NUM
//              | REAL
//              | TRUE
//              | FALSE
Node* Factor() {
    Node* node = new Node (FACTOR);
    switch(token.type) {
    case NUM:
    case REAL:
    case TRUE:
    case FALSE:	//for NUM/REAQL/TRUE/FALSe, just add it to tree.
        node -> addChild(new Node (token));
        token = getToken();
        break;
    case ID:	//See Loc. first(Loc) = ID.
        node -> addChild(Loc());
        break;
    default:
        mustBe(OPEN_PAREN);
        node -> addChild(Bool());
        mustBe(CLS_PAREN);
    }
    return node;
}

// <unary>    -> ! <unary>
//              | - <unary>
//              | <factor>
Node* Unary() {
    Node* node = new Node (UNARY);
    if(token.type==NOT || token.type==MINUS) {
        node -> addChild(new Node (token));	//We need the "!" and "-". then the Unary
        token = getToken();
        node -> addChild(Unary());
	}else{
        node -> addChild(Factor());
    }
    return node;
}

// <termclosure>-> e
//              | * <unary> <termcl>
//              | / <unary> <termcl>
Node* Termclosure() {
    Node* node = new Node (TERMCLOSURE);
    if(token.type==MUL || token.type==DIV) {
        node -> addChild(new Node(token));
        token = getToken();
        node -> addChild(Unary());
        node -> addChild(Termclosure());
    }
    return node;
}

// <term> -> <unary> <termcl>
Node* Term() {
    Node* node = new Node (TERM);
    node -> addChild(Unary());
    node -> addChild(Termclosure());
    return node;
}

// <exprcl> -> e
//         | + <term> <exprclosure>
//         | - <term> <exprclosure>
Node* Exprclosure() {
    Node* node = new Node (EXPRCLOSURE);
    if(token.type==PLUS || token.type==MINUS) {
        node -> addChild(new Node(token));
        token = getToken();
        node -> addChild(Term());
        node -> addChild(Exprclosure());
    }
    return node;
}

// <expr> -><term> <exprclosure>
Node* Expr() {
    Node* node = new Node (EXPR);
    node -> addChild(Term());
    node -> addChild(Exprclosure());
    return node;
}

// <reltail>  -> e 
//              | <= <expr>
//              | >= <expr>
//              | > <expr>
//              | < <expr>
Node* Reltail() {
    Node* node = new Node (RELTAIL);
    switch(token.type) {
    case LTEQ:
    case GTEQ:
    case LT:
    case GT:
        node -> addChild(new Node(token));
        token = getToken();
        node -> addChild(Expr());
        break;
    default:
        break;
    }
    return node;
}

// <rel> -><expr> <reltail>
Node* Rel() {
    Node* node = new Node (REL);
    node -> addChild(Expr());
    node -> addChild(Reltail());
    return node;
}

// <equalclosure> -> e 
//             		| == <rel> <equalclosure> 
//             		| != <rel> <equalclosure>
Node* Equalclosure() {
    Node* node = new Node (EQUALCLOSURE);
    if(token.type==EQ || token.type==NEQ) {
        node -> addChild(new Node(token));
        token = getToken();
        node -> addChild(Rel());
        node -> addChild(Equalclosure());
    }
    return node;
}

// <equality> -> <rel> <equalclosure>
Node* Equality() {
    Node* node = new Node (EQUALITY);
    node -> addChild(Rel());
    node -> addChild(Equalclosure());
    return node;
}

// <joinclosure>-> e 
//              | && <equality> <joinclosure>
Node* Joinclosure() {
    Node* node = new Node (JOINCLOSURE);
    if(token.type == AND) {
        node -> addChild(new Node(token));
        token = getToken();
        node -> addChild(Equality());
        node -> addChild(Joinclosure());
    }
    return node;
}

// <join>     -> <equality> <joinclosure>
Node* Join() {
    Node* node = new Node (JOIN);
    node -> addChild(Equality());
    node -> addChild(Joinclosure());
    return node;
}

// <boolclosure> -> e 
//           | || <join> <boolcl>
Node* Boolclosure() {
    Node* node = new Node (BOOLCLOSURE);
    if(token.type==OR){	//if boolclosure != empty
        node -> addChild(new Node(token));
        token = getToken();
        node -> addChild(Join());
        node -> addChild(Boolclosure());
    }
    return node;
}

// <bool> -> <join> <boolclosure>
Node* Bool() {
    Node* node = new Node (BOOL);
    node -> addChild(Join());
    node -> addChild(Boolclosure());
    return node;
}


// <locclosure>  -> e 
//              | [ <bool> ] <locclosure>
Node* Locclosure() {
    Node* node = new Node (LOCCLOSURE);
    if(token.type==OPEN_SQPAR){
        token = getToken();
        node -> addChild(Bool());
        mustBe(CLS_SQPAR);
        node -> addChild(Locclosure());
    }
    return node;
}

// <loc>      -> ID <locclosure>
Node* Loc() {
    Node* node = new Node (LOC);
    node -> addChild(new Node(token));
    mustBe(ID);
    node -> addChild(Locclosure());
    return node;
}

//<stmt>     -> <loc> = <bool> ;
//            | IF ( <bool> ) <stmt>
//            | IF ( <bool> ) <stmt> ELSE <stmt>
//            | WHILE ( <bool> ) <stmt>
//            | <block>
Node* Stmt(){
	Node* node = new Node(STMT);
	//first(stmt): IF, WHILE, first(loc),first(block)
	//           : IF, WHILE, ID, OPEN_BRACE
	switch(token.type) {
    case IF:
        node -> addChild(new Node(token));
        token = getToken();
        mustBe(OPEN_PAREN);
        node -> addChild(Bool());
        mustBe(CLS_PAREN);
        node -> addChild(Stmt());
        if (token.type == ELSE) {
            node -> addChild(new Node(token));
            token = getToken();
            node -> addChild(Stmt());
        }
        break;
    case WHILE:
        node -> addChild(new Node(token));
        token = getToken();
        mustBe(OPEN_PAREN);
        node -> addChild(Bool());
        mustBe(CLS_PAREN);
        node -> addChild(Stmt());
        break;
    case OPEN_BRACE:
        node -> addChild(Block());
        break;
    default:	//Otherwise, it's loc.it must be ID.
        node -> addChild(Loc());
        node -> addChild(new Node(token));
        mustBe(ASSIGN);
        node -> addChild(Bool());
        mustBe(SEMICOLON);
        break;
    }
    return node;

}



//<stmts>    ::= e 
//             | <stmt> <stmts>
Node* Stmts(){
	Node* node = new Node(STMTS);
	//stmts can be empty. check if it's ")".
	//if so, do nothing. there's no statement.
	//else, add to child.
	if(token.type!=CLS_BRACE){
		node->addChild(Stmt());
		node->addChild(Stmts());
	}
	return node;
}
//<typeclosure> -> e 
//              | [NUM] <typeclosure>
Node* Typeclosure(){
	Node* node = new Node(TYPECLOSURE);
	if(token.type == OPEN_SQPAR){
		token = getToken();
		mustBe(NUM);
		node->addChild(new Node(token));
		mustBe(CLS_SQPAR);
		node->addChild(Typeclosure());
	}else{
		//else, typeclosure is empty. we do nothing.
	}
	return node;
}
//<type>     -> BASIC <typeclosure>
Node* Type(){
	Node* node = new Node(TYPE);
	node->addChild(new Node(token));	//Basic: current token must be int/double/... converted to Basic by getToken.
	mustBe(BASIC);
	node->addChild(Typeclosure());
	//Not checking follow set. Already checked in Decl.
	return node;
}
//	<decl>-> <type> ID ;
//	follow(type) = ID,Semicolon
Node* Decl(){
	Node* node = new Node(DECL);
	node->addChild(Type());
	mustBe(ID);
	node->addChild(new Node(token));	//After type, this must be ID (token)
	mustBe(SEMICOLON);
	return node;
}
// <decls>    -> e 
//            	|<decl> <decls>
Node* Decls()
{
	Node* node = new Node(DECLS);
	/*
	since decls can be empty, we need to check follow(decls).
    what can show up here:
    follow(decls) = first(stmts)
	first(stmts)  = first(stmt)+ follow(stmts)
	first(stmt) = IF,WHILE,first(Loc),first(block)
		        = IF,WHILE,ID,{
	follow(stmts) = }
	Ignore If,while,{},because our AST don't need them.
	ID will be processed in Decl.
	if IF,WHILE...shows up , it means our decls is empty.
    */
    switch(token.type) {
    case IF:
    case WHILE:
    case ID:
    case OPEN_BRACE:
    case CLS_BRACE:
        break;
    default:
        node -> addChild(Decl());
        node -> addChild(Decls());
        break;
    }
    return node;
}
// <block> -> { <decls> <stmts> }
Node* Block() {
    Node* node = new Node (BLOCK);
    mustBe(OPEN_BRACE);
    node -> addChild(Decls());
    node -> addChild(Stmts());
    mustBe(CLS_BRACE);
    return node;
}

// <program>  -> <block>
Node* Program(){
  Node* node = new Node(PROGRAM);
  node->addChild(Block());
  return node;
}

int main(){
  token = getToken();        // get first lexeme.
  Node* root = Program();    // Program returns a root node of the whole tree.
  mustBe(EOS);  
  cout<<"printing tree in HTML format"<<endl;            
  if(root){
    root->print();
  }else{
    cout<<"No Input Detected"<<endl;
  }
  return 0;
}

