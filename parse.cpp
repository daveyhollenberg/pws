#include <string>
#include <vector>
#include <stack>
#include <iostream>

#define ASSOCIATIVITY_LEFT 0
#define ASSOCIATIVITY_RIGHT 1

struct Operator{
  std::string op;
  int precedence;
  int associativity;
  Operator(std::string a, int b, int c){
    op = a;precedence = b;associativity = c;
  }
};

Operator operators[] = {
  Operator("log",5,ASSOCIATIVITY_LEFT),
  Operator("ln",5,ASSOCIATIVITY_LEFT),
  Operator("sqrt",5,ASSOCIATIVITY_LEFT),
  Operator("abs",5,ASSOCIATIVITY_LEFT),
  Operator("cos",5,ASSOCIATIVITY_LEFT),
  Operator("sin",5,ASSOCIATIVITY_LEFT),
  Operator("tan",5,ASSOCIATIVITY_LEFT),

  Operator("^",4,ASSOCIATIVITY_RIGHT),
  Operator("*",3,ASSOCIATIVITY_LEFT),
  Operator("/",3,ASSOCIATIVITY_LEFT),
  Operator("+",2,ASSOCIATIVITY_LEFT),
  Operator("-",2,ASSOCIATIVITY_LEFT),
};

std::string functions[] = {"log","ln","sqrt","abs","cos","sin","tan"};
std::string constants[] = {"e","pi","$1","$2","$3","$4"};

bool isNumber(std::string token){
  if(token[0] == '-' && token.length() > 1){return token.substr(1,token.length()-1).find_first_not_of("0123456789.") == std::string::npos;}
  return token.find_first_not_of("0123456789.") == std::string::npos;
}
bool isOperand(std::string token){
 return token == "x" || token == "e" || token == "pi" || token == "a" || token == "$1" || token == "$2" || token == "$3" || token == "$4";
}
bool isFunction(std::string token){
  for(int i=0;i<sizeof(functions)/sizeof(functions[0]);i++){
    if(functions[i] == token){
      return true;
    }
  }
  return false;
}
bool isOperator(std::string token){
  for(int i=0;i<sizeof(operators)/sizeof(operators[0]);i++){
    if(operators[i].op == token){
      return true;
    }
  }
  return false;
}
int comparePrecedence(std::string a,std::string token){// -1 = lower, 0 = equal, 1 = higher
  int pa = 0;
  int pb = 0;
  for(int i=0;i<sizeof(operators)/sizeof(operators[0]);i++){
    if(operators[i].op == a){
      pa = operators[i].precedence;
    }
    if(operators[i].op == token){
      pb = operators[i].precedence;
    }
  }
  if(pa < pb){return -1;}
  if(pa > pb){return 1;}
  return 0;
}
bool isLeftAssociative(std::string token){
  for(int i=0;i<sizeof(operators)/sizeof(operators[0]);i++){
    if(operators[i].op == token){
      return operators[i].associativity == ASSOCIATIVITY_LEFT;
    }
  }
  return false;
}
std::vector<std::string> splitOnTokens(std::string formula){
  std::vector<std::string> result;
  int ptr = 0;
  while(ptr < formula.length()){
    if(isOperator(formula.substr(ptr,1)) || formula.substr(ptr,1) == "(" || formula.substr(ptr,1) == ")"){
      result.push_back(formula.substr(0,ptr));
      result.push_back(formula.substr(ptr,1));
      formula = formula.substr(ptr+1,formula.length()-ptr-1);
      ptr = 0;
      continue;
    }
    ptr++;
  }
  result.push_back(formula);
  return result;
}
std::vector<std::string> split(std::string formula){
  std::vector<std::string> tokens = splitOnTokens(formula);
  std::vector<std::string> outputQueue;
  std::stack<std::string> operatorStack;
  for(int i=0;i<tokens.size();i++){
    std::string token = tokens[i];
    if(isOperand(token)){
      outputQueue.push_back(token);
    }if(isNumber(token)){
      outputQueue.push_back(token);
    }else if(isOperator(token)){
      if(operatorStack.size() > 0){
      std::string a = operatorStack.top();
      while((isFunction(a) || (isOperator(a) && comparePrecedence(a,token) == 1) || (isOperator(a) && comparePrecedence(a,token) == 0 && isLeftAssociative(a))) && a != "("){
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
        if(operatorStack.size() == 0){
          break;
        }
        a = operatorStack.top();
      }
    }
     operatorStack.push(token);
    }else if(token == "("){
      operatorStack.push(token);
    }else if(token == ")"){
      std::string a = operatorStack.top();
      while(a != "("){
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
        a = operatorStack.top();
      }
      operatorStack.pop();
    }
  }
  while(operatorStack.size() > 0){
    outputQueue.push_back(operatorStack.top());
    operatorStack.pop();
  }
  return outputQueue;
}
std::string prepare(std::string formula){
  int c = 0;
  int ptr = 0;
  while(ptr < formula.length()){
    if(formula.substr(ptr,1) == ","){
      formula = formula.substr(0,ptr) + "." + formula.substr(ptr + 1, formula.length()-ptr-1);
    }
    if(formula.substr(ptr,1) == "x" || formula.substr(ptr,1) == "a"){
      if(ptr - 1 >= 0){
        if(isNumber(formula.substr(ptr-1,1)) || formula.substr(ptr-1,1).find_first_not_of("a)") == std::string::npos){
          formula = formula.substr(0,ptr) + "*" + formula.substr(ptr,formula.length()-ptr);
          continue;
        }
      }
      if(ptr + 1 < formula.length()){
        if(isNumber(formula.substr(ptr + 1,1)) || formula.substr(ptr+1,1).find_first_not_of("a(") == std::string::npos){
          formula = formula.substr(0,ptr+1) + "*" + formula.substr(ptr+1,formula.length()-ptr-1);
          continue;
        }
      }
    }
    if(formula.substr(ptr,1) == "-"){
        if(ptr == 0 || formula.substr(ptr - 1,1).find_first_not_of("( */+-^") == std::string::npos){
          int p = ptr+1;
          while(p < formula.length()){
            if(!isNumber(formula.substr(p,1)) && formula.substr(p,1) != "x"){
              break;
            }
            p++;
          }
          formula = formula.substr(0,ptr) + "(0" + formula.substr(ptr,p) + ")" + formula.substr(p+1,formula.length()-p-1);
          continue;
        }
    }
    ptr++;
  }
  return formula;
}
