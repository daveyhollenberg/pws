#ifndef NODE_H
#define NODE_H
#include <string>

struct Node{
  std::string value;
  Node* left;
  Node* right;
  Node(std::string v){
    value = v;
    left = NULL;
    right = NULL;
  }
  Node(std::string v, Node* l,Node* r){
    value = v;left = l;right = r;
  }
};
#endif
