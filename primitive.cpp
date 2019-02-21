#include <iostream>

#include "node.h"
#include "tree.h"
#include "common.h"
#include "derivative.h"
#include "parse.h"


void primitive(Node* node){
  if(node == NULL){return;}
  if(!containsX(node)){
    insertNode(node,"*",node,new Node("x"));
    return;
  }
  if(node->value == "+" || node->value == "-"){
    primitive(node->left);
    primitive(node->right);
    return;
  }
  if(node->value == "x"){
    insertNode(node,"*",new Node("0.5"),node);
    insertNode(node,"^",node,new Node("2"));
  }if(node->value == "^"){
    if(containsX(node->left)){
      if(containsX(node->right)){
        if(node->left->value == "x" && node->right->value == "x"){ // x^x
          std::cout << "error##Kan formule niet Primitiveren" << std::endl;
          exit(0);
          return;
        }
        //g(x)^g(x)
        std::cout << "error##Kan formule niet Primitiveren" << std::endl;
        exit(0);
        return;
      }
      Node* rc = NULL;
      copyTree(node->right,rc);
      insertNode(node,"/",node,new Node("+",rc,new Node("1")));
      Node* nn = NULL;
      copyTree(node->left,nn);
      insertNode(node,"*",node,nn);
      derivative(nn);
      Node* nl = NULL;//N
      copyTree(node->right,nl);//new Node(node->right->value);
      Node* nr = new Node("1");
      node->right->left = nl;
      node->right->right = nr;
      node->right->value = "+";
      return;
    }else if(containsX(node->right)){
      Node* mn = new Node("ln",new Node(node->left->value),NULL);
      insertNode(node,"/",node,mn);
      Node* nn = NULL;
      copyTree(node->right,nn);
      insertNode(mn,"*",nn,mn);
      derivative(nn);
      return;
    }
    return;
  }else if(node->value == "*"){
    if(!containsX(node->left)){
      primitive(node->right);
    }else if(!containsX(node->right)){
      primitive(node->left);
    }else{
      //f(x)*g(x)
      std::cout << "error##Kan formule niet Primitiveren" << std::endl;
      exit(0);
    }
    return;
  } if(node->value == "/"){
    if(!containsX(node->left)){
      if(node->right->value == "x"){
        insertNode(node,"abs",node,NULL);
        insertNode(getParent(root,node),"ln",getParent(root,node),NULL);
      }else{
        std::cout << "error##Kan formule niet Primitiveren" << std::endl;
        exit(0);
      }
    }else if(!containsX(node->right)){
      primitive(node->left);
    }else{
      //f(x)/g(x)
      std::cout << "error##Kan formule niet Primitiveren" << std::endl;
      exit(0);
    }
    return;
  }else if(node->value == "sqrt"){
    std::cout << "error##Kan formule niet Primitiveren" << std::endl;
    exit(0);
    return;
  }else if(node->value == "log"){
    std::cout << "error##Kan formule niet Primitiveren" << std::endl;
    exit(0);
    return;
  }else if(node->value == "ln"){
    std::cout << "error##Kan formule niet Primitiveren" << std::endl;
    exit(0);
    return;
  }else if(node->value == "sin"){
    node->value = "cos";
    Node* b = NULL;
    copyTree(node->left,b);
    insertNode(node,"/",node,b);
    derivative(b);
    insertNode(node,"-",new Node("0"),node);
    return;
  }else if(node->value == "cos"){
    node->value = "sin";
    Node* b = NULL;
    copyTree(node->left,b);
    insertNode(node,"/",node,b);
    derivative(b);
    return;
  }else if(node->value == "tan"){
    node->value = "cos";
    Node* b = NULL;
    copyTree(node->left,b);
    insertNode(node,"/",node,b);
    derivative(b);
    insertNode(node,"-",new Node("0"),node);
    insertNode(node,"ln",node,NULL);
    insertNode(node,"abs",node,NULL);
    return;
  }else if(node->value == "abs"){
    std::cout << "error##Kan formule niet Primitiveren" << std::endl;
    exit(0);
    return;
  }
}



std::string primitive(std::string formula){
  root = new Node("");
  constructTree(split(prepare(formula)),*root);
  print("", root, false);
  primitive(root);
  insertNode(root,"+",root,new Node("c"));
  print("", root, false);
  simplify(root);
  print("", root, false);

  return toLatex(root);
}
