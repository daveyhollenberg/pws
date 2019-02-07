#include "node.h"
#include "tree.h"
#include "common.h"
#include "parse.h"


void derivative(Node* node){
  if(node == NULL){return;}
  if(!containsX(node)){
    node->value = "0";
    node->left = NULL;
    node->right = NULL;
    return;
  }
  if(node->value == "+" || node->value == "-"){
    derivative(node->left);
    derivative(node->right);
    return;
  }if(node->value == "x"){
    node->value = "1";
  }else if(node->value == "^"){
    Node* f1 = NULL;
    Node* g1 = NULL;
    copyTree(node->left,f1);
    copyTree(node->right,g1);
    Node* a1 = new Node("^",f1,g1);
    Node* a2 = NULL;
    copyTree(node->right,a2);
    Node* f2 = NULL;
    copyTree(node->left,f2);
    Node* a3 = new Node("ln",f2,NULL);
    Node* a4 = new Node("*",a1,a2);
    Node* a = new Node("*",a4,a3);

    Node* f3 = NULL;
    Node* g2 = NULL;
    copyTree(node->left,f3);
    copyTree(node->right,g2);
    Node* gm1 = new Node("-",g2,new Node("1"));
    Node* b1 = new Node("^",f3,gm1);
    Node* b2 = NULL;
    copyTree(node->right,b2);
    Node* b3 = NULL;
    copyTree(node->left,b3);
    Node* b4 = new Node("*",b1,b2);
    Node* b = new Node("*",b4,b3);

    node->left = a;
    node->right = b;
    node->value = "+";

    derivative(a2);
    derivative(b3);

    return;
  }else if(node->value == "*"){
    Node* cl = NULL;
    copyTree(node->left,cl);
    Node* cr = NULL;
    copyTree(node->right,cr);
    node->value = "+";
    insertNode(node->left,"*",cr,node->left);
    insertNode(node->right,"*",cl,node->right);
    derivative(node->right->right);
    derivative(node->left->right);
    return;
  }else if(node->value == "/"){
    Node* dl = NULL;
    copyTree(node->left,dl);
    Node* dr = NULL;
    copyTree(node->right,dr);
    Node* topLeft = new Node("*",node->right,dl);
    Node* topRight = new Node("*",node->left,dr);
    Node* top = new Node("-",topLeft,topRight);
    Node* nrc = NULL;
    copyTree(node->right,nrc);
    Node* bottom = new Node("^",nrc,new Node("2"));
    node->left = top;
    node->right = bottom;
    derivative(dl);
    derivative(dr);
    return;
  }else if(node->value == "sqrt"){
    Node* top = NULL;
    copyTree(node->left,top);
    Node* copy = NULL;
    copyTree(node,copy);
    Node* bottom = new Node("*",new Node("2"),copy);
    node->value = "/";
    node->left = top;
    node->right = bottom;
    derivative(top);
    return;
  }else if(node->value == "log"){
    Node* top = NULL;
    copyTree(node->left,top);
    Node* copy = NULL;
    copyTree(node->left,copy);
    Node* bottom = new Node("*",copy,new Node("ln",new Node("10"),NULL));
    node->value = "/";
    node->left = top;
    node->right = bottom;
    derivative(top);
    return;
  }else if(node->value == "ln"){
    Node* top = NULL;
    copyTree(node->left,top);
    Node* bottom = NULL;
    copyTree(node->left,bottom);
    node->value = "/";
    node->left = top;
    node->right = bottom;
    derivative(top);
    return;
  }else if(node->value == "sin"){
    node->value = "cos";
    Node* left = NULL;
    copyTree(node->left,left);
    insertNode(node,"*",left,node);
    derivative(left);
    return;
  }else if(node->value == "cos"){
    node->value = "sin";
    Node* left = NULL;
    copyTree(node->left,left);
    Node* neg = new Node("-",new Node("0"),left);
    insertNode(node,"*",neg,node);
    derivative(left);
    return;
  }else if(node->value == "tan"){
    Node* top = NULL;
    copyTree(node->left,top);
    Node* copy = NULL;
    copyTree(node->left,copy);
    Node* cc = new Node("cos",copy,NULL);
    Node* bottom = new Node("^",cc,new Node("2"));
    node->value = "/";
    node->left = top;
    node->right = bottom;
    derivative(top);
    return;
  }else if(node->value == "abs"){
    Node* c = NULL;
    Node* d = NULL;
    copyTree(node->left,c);
    copyTree(node->left,d);
    Node* top = new Node("*",c,d);
    Node* bottom = NULL;
    copyTree(node,bottom);
    node->value = "/";
    node->left = top;
    node->right = bottom;
    derivative(d);
    return;
  }
}

std::string derivative(std::string formula){
  root = new Node("");
  constructTree(split(prepare(formula)),*root);
  print("", root, false);
  derivative(root);
  print("", root, false);
  simplify(root);
  print("", root, false);

  return toLatex(root);
}
