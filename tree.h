#ifndef TREE_H
#define TREE_H
#include <string>
#include <vector>
#include "node.h"


void constructTree(std::vector<std::string>,Node&);
Node* getParent(Node*,Node*);
Node* getFirstParentOperand(Node*,Node*);
void getLatex(Node*,std::string&);
std::string toLatex(Node*);
void print(const std::string&, const Node*, bool);
bool containsX(Node*);
void copyTree(Node*,Node*&);
void insertNode(Node*,std::string,Node*,Node*);
void setNode(Node*,Node*);
void simplify(Node*);
#endif
