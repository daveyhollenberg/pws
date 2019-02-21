#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <cmath>
#include "node.h"
#include "parse.h"
#include "common.h"
#include "rules.h"

void constructTree(std::vector<std::string> el, Node &root) {
  for(int i=0;i<el.size();i++){
    if(el[i] == ""){
      el.erase(el.begin()+i);
      i--;
    }
  }
  std::stack<Node*> stack;
  for(int i = 0; i < el.size(); i++) {
    if(isFunction(el[i])){
      Node* a = stack.top();
      stack.pop();
      stack.push(new Node(el[i],a,NULL));
  }else if( el[i] == "+" || el[i] == "-" || el[i] == "*" || el[i] == "/" || el[i] == "^") {
      Node* b = stack.top();
      stack.pop();
      Node* a = stack.top();
      stack.pop();
      stack.push(new Node(el[i],a,b));
    }else if(isNumber(el[i]) || isOperand(el[i]) || el[i].find("$") != std::string::npos){
      stack.push(new Node(el[i]));
    }
  }
  root = *stack.top();
}
Node* getParent(Node* current,Node* node){
  if(current == NULL){return NULL;}
  if(current->left == node){return current;}
  if(current->right == node){return current;}
  if(getParent(current->left,node) != NULL){return getParent(current->left,node);}
  if(getParent(current->right,node) != NULL){return getParent(current->right,node);}
  return NULL;
}
Node* getFirstParentOperand(Node* current,Node* node){
  if(current == NULL){return new Node("");}
  if(current->left == node){return current;}
  if(current->right == node){return current;}
  if(getFirstParentOperand(current->left,node)->value != ""){
    if(isOperator(getFirstParentOperand(current->left,node)->value)){
      return getFirstParentOperand(current->left,node);
    }else{
      return current;
    }
  }
  if(getFirstParentOperand(current->right,node)->value != ""){
    if(isOperator(getFirstParentOperand(current->right,node)->value)){
      return getFirstParentOperand(current->right,node);
    }else{
      return current;
    }
  }
  return new Node("");
}

bool hasLeftX(Node* node){
  if(node->value == "x"){return true;}
  if(node->value == "^" && node->left->value == "x"){return true;}
  return false;
}
void getLatex(Node* node, std::string& res){
  if(node == NULL){return;}
  bool par = false;
  if(isOperator(node->value) && (comparePrecedence(node->value,getFirstParentOperand(root,node)->value) == -1 || (comparePrecedence(node->value,getFirstParentOperand(root,node)->value) == 0 && getFirstParentOperand(root,node)->value == "-" && getFirstParentOperand(root,node)->right->value.find_first_not_of("+-") == std::string::npos)) && getFirstParentOperand(root,node)->value.find_first_not_of("/^") != std::string::npos){
    par = true;
  }
  if(node->value == "*" && (hasLeftX(node->left) || hasLeftX(node->right))){
    if(hasLeftX(node->left)){
      std::string a = "";
      std::string b = "";
      getLatex(node->right, a);
      getLatex(node->left, b);
      res += a + b;
    }else{
      std::string a = "";
      std::string b = "";
      getLatex(node->left, a);
      getLatex(node->right, b);
      res += a + b;
    }
  }else if(node->value == "*" && node->left->value == "-1"){
    if(node->right->value.substr(0,1) == "-"){
      node->right->value = node->right->value.substr(1,node->right->value.length()-1);
    }else{
      res += "-";
    }
    getLatex(node->right,res);
  }else if(node->value == "/"){
    res += "\\frac{";
    getLatex(node->left,res);
    res += "}{";
    getLatex(node->right,res);
    res += "}";
  }else if(node->value == "^"){
    bool a = false;
    bool b = false;
    if(node->left->value.find_first_not_of("+-*/^") == std::string::npos){a = true;}
    if(!isNumber(node->right->value) || node->right->value.length() > 1){b = true;}
    if(a){res += "\\left (";}
    getLatex(node->left,res);
    if(a){res += "\\right )";}
    res += node->value;
    if(b){res += "{";}
    getLatex(node->right,res);
    if(b){res += "}";}
  }else if(node->value =="sqrt"){
    res += "\\sqrt{";
    getLatex(node->left,res);
    res += "}";
  }else if(node->value =="abs"){
    res += "\\left |";
    getLatex(node->left,res);
    res += "\\right |";
  }else if(isFunction(node->value)){
    res += node->value;
    bool p = false;
    if(node->value == "abs" && isFunction(getParent(root,node)->value)){p = true;}
    if(p){
      res += "\\left (";
    }
    getLatex(node->left,res);
    if(p){
      res += "\\right )";
    }
  }else{
    if(par){res += "\\left (";}
    getLatex(node->left,res);
    res += node->value;
    getLatex(node->right,res);
    if(par){res += "\\right )";}
  }
  replace(res,"*","\\cdot");
}
std::string toLatex(Node* node){
  std::string result = "";
  getLatex(root,result);
  return result;
}
void print(const std::string& prefix, const Node* node, bool left){
  if(node == NULL){return;}
  std::cout << prefix;
  std::cout << (left ? "├──" : "└──");
  std::cout << node->value << std::endl;
  print(prefix + (left ? "│   " : "    "), node->left, (node->right == NULL)?false:true);
  print(prefix + (left ? "│   " : "    "), node->right, false);
}


bool containsX(Node* node){
  if(node == NULL){return false;}
  if(node->value.find("x") != std::string::npos){return true;}
  if(containsX(node->left)){return true;}
  if(containsX(node->right)){return true;}
  return false;
}
void copyTree(Node* old,Node*& copy){
  if(old == NULL){return;}
  copy = new Node(old->value);
  copyTree(old->left,copy->left);
  copyTree(old->right,copy->right);
}
void insertNode(Node* child, std::string val,Node* a,Node* b){
  Node* node = new Node(val,a,b);
  if(child == root){
    root = node;
    return;
  }
  Node* parent = getParent(root,child);
  if(parent->left == child){
    parent->left = node;
  }else if(parent->right == child){
    parent->right = node;
  }
}
void setNode(Node* a,Node* b){
  a->value = b->value;
  a->left = b->left;
  a->right = b->right;
}
void removeNode(Node* node){
  Node* parent = getParent(root,node);
  if(parent->left == node){
    setNode(parent,parent->right);
  }else if(parent->right == node){
    setNode(parent,parent->left);
  }
}

int decimals = 2;

std::string formatNumber(std::string num){
  int p = num.find(".");
  if(p == std::string::npos){return num;}
  int a = p+decimals+1;
  int b = num.length();
  num = num.substr(0,a<b?a:b);
  for(int i = num.length()-1;i>=p;i--){
    if(num[i] == '0' || num[i] == '.'){
      num = num.substr(0,i);
    }else{
      break;
    }
  }
  return num;
}
std::string toString(double string){
  return formatNumber(std::to_string(string));
}
void collectNodesWithOperator(Node* node,std::string op,std::vector<Node*>* list){
  if(node == NULL){return;}
  if(node->value == op){
    if(isNumber(node->left->value)){
      list->push_back(node->left);
    }
    if(isNumber(node->right->value)){
      list->push_back(node->right);
    }
    collectNodesWithOperator(node->left,op,list);
    collectNodesWithOperator(node->right,op,list);
  }
}
void collectTokenNodesWithOperators(Node* node,std::string ops,std::vector<Node*>* list,std::string token){
  if(node == NULL){return;}
  if(node->value.find_first_not_of(ops) == std::string::npos){
    if(node->left->value == token){
      list->push_back(node->left);
    }
    if(node->right->value == token){
      list->push_back(node->right);
    }
    collectTokenNodesWithOperators(node->left,ops,list,token);
    collectTokenNodesWithOperators(node->right,ops,list,token);
  }
}
int getXValue(Node* node,int n, int sign){
  bool is = false;
  if(node == NULL){return n;}
  if(isFunction(node->value)){return -999;}
  if(node->value != "*" && node->value != "/" && node->value != "x" && node->value != "^" && !isNumber(node->value)){return n;}
  if(node->left != NULL && node->right != NULL){//new
    if(node->left->value == "0" || node->right->value == "0"){
      return -999;
    }
  }
  if(node->value == "/"){is = true;}
  if(node->value == "x"){n+=sign;}
  if(node->value == "^"){
    if(node->left->value == "x"){
      if(isNumber(node->right->value)){
        n += sign*stod(node->right->value);
      }
      return n;
    }
  }
  if(node->value == "+" && node->value == "-"){
    return n;
  }
  n = getXValue(node->left,n,sign);
  if(is){sign*=-1;}
  if(n == -999){return -999;}
  n = getXValue(node->right,n,sign);
  return n;
}
int getXValue(Node* node){
  int v = getXValue(node,0,1);
  if(v == 0 || v == -999){
    v = rand();
  }
  return v;
}
void simplify1(Node* node){
  if(node == NULL){return;}
  simplify1(node->left);
  simplify1(node->right);
        Rule rules[] = {
          Rule("0*$1","0"),
          Rule("$1*0","0"),
          Rule("0+$1","$1"),
          Rule("$1+0","$1"),
          Rule("1*$1","$1"),
          Rule("$1*1","$1"),
          Rule("$1-0","$1"),
          Rule("$1/1","$1"),
          Rule("$1^0","1"),
          Rule("$1^1","$1"),
          Rule("0^$1","1"),
          Rule("1^$1","1"),
          Rule("0/$1","0"),
        };
    for(int i=0;i<sizeof(rules)/sizeof(rules[0]);i++){
      rules[i].checkRule(node);
    }
    std::vector<Node*> list;
    collectNodesWithOperator(node,"*",&list);
    while(list.size() > 1){
      Node* a = list[0];
      Node* b = list[1];
      a->value = toString(stod(a->value)*stod(b->value));
      b->value = "1";
      list.erase(list.begin());
    }
    list.clear();
    collectTokenNodesWithOperators(node,"*",&list,"x");//error ^?
    if(list.size() > 1){
      int n = 0;
      for(int i=0;i<list.size();i++){
        Node* parent = getParent(root,list[i]);
        if(parent->value == "^" && parent->left->value == "x"){
          if(!isNumber(parent->right->value)){
            list.erase(list.begin() + i);
            i--;
          }else{
            n += stod(parent->right->value);
          }
        }else{
          n++;
        }
      }
      Node* a = list[0];
      Node* nn = new Node("^");
      nn->left = new Node("x");
      nn->right = new Node(std::to_string(n));
      setNode(a,nn);
      for(int i=1;i<list.size();i++){
        list[i]->value = "1";
      }
    }
    if(node->value == "+" || node->value == "-"){
      if(getXValue(node->left) == getXValue(node->right)){
        int xv = getXValue(node->left);
        print("",root,false);
        std::vector<Node*> list;
        collectNodesWithOperator(node->left,"*",&list);
        collectNodesWithOperator(node->right,"*",&list);
        while(list.size() > 1){
          Node* a = list[0];
          Node* b = list[1];
          if(getParent(root,a) != getParent(root,b) && a->value != "1" && b->value != "1"){//common before x
          if(node->value == "+"){
            a->value = toString(stod(a->value)+stod(b->value));
          }else{
            a->value = toString(stod(a->value)-stod(b->value));
          }
          b->value = "0";
        }
          list.erase(list.begin());
        }
      }
    }
  }
void simplify2(Node* node){
  if(node == NULL || node->left == NULL || node->right == NULL){return;}
  simplify2(node->left);
  simplify2(node->right);
    if(node->value == "+"){
        if(isNumber(node->left->value) && isNumber(node->right->value)){
          node->value = toString(stod(node->left->value)+stod(node->right->value));
          node->left = NULL;
          node->right = NULL;
        }
      }else if(node->value == "-"){
        if(isNumber(node->left->value) && isNumber(node->right->value)){
          node->value = toString(stod(node->left->value)-stod(node->right->value));
          node->left = NULL;
          node->right = NULL;
        }else if(node->left->value == "0"){
          setNode(node,new Node("*",new Node("-1"),node->right));
        }
      }else if(node->value == "*"){
        if(isNumber(node->left->value) && isNumber(node->right->value)){
          node->value = toString(stod(node->left->value)*stod(node->right->value));
          node->left = NULL;
          node->right = NULL;
        }else{
          if(isNumber(node->left->value) && node->right->value == "/" && isNumber(node->right->right->value)){
            std::string v = node->left->value;
            node->left = new Node("/");
            node->left->left = new Node(v);
            node->left->right = new Node(node->right->right->value);
            setNode(node->right,node->right->left);
          }
        }
      }else if(node->value == "/"){
        if(isNumber(node->left->value) && isNumber(node->right->value)){
          if(node->right->value == "0"){
            std::cout << "error##Delen door 0" << std::endl;
            exit(0);
            return;
          }
          double val = stod(node->left->value)/stod(node->right->value);
          if(std::to_string(val) == toString(val)){
            node->value = toString(val);
            node->left = NULL;
            node->right = NULL;
          }
        }
      }

}
void simplify(Node* node){
  for(int i=0;i<5;i++){
    simplify2(node);
    simplify1(node);
  }
}
