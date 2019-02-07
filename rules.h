#ifndef RULES_H
#define RULES_H
#include <string>

#include "tree.h"
#include "parse.h"
#include "node.h"

struct Rule{
  Node* rule;
  Node* replacement;
  Node* placeholders[16];

  Rule(std::string ru,std::string re){
    rule = new Node("");
    replacement = new Node("");
    constructTree(split(prepare(ru)),*rule);
    constructTree(split(prepare(re)),*replacement);
    for(int i=0;i<16;i++){
      placeholders[i] = NULL;
    }
  }
  void getPlaceholderValues(Node* node,Node* rule){
    if(node == NULL || rule == NULL){return;}
    getPlaceholderValues(node->left,rule->left);
    getPlaceholderValues(node->right,rule->right);
    if(rule->value.find("$") != std::string::npos){
      std::string num = rule->value.substr(1,rule->value.length()-1);
      if(isNumber(num)){
        int n = std::stoi(num);
        placeholders[n] = node;
      }
    }
  }
  void checkRule(Node* node){
    Node* result = NULL;
    if(compareRule(node,rule)){
      getPlaceholderValues(node,rule);
      copyTree(replacement,result);
      replaceRule(result);
      setNode(node,result);
    }
  }
  bool compareRule(Node* node,Node* node2){
    if(node == NULL || node2 == NULL){return true;}
    if(!compareRule(node->left,node2->left)){return false;}
    if(!compareRule(node->right,node2->right)){return false;}
    if(node2->value.find("$") == std::string::npos && node->value != node2->value){return false;}
    return true;
  }
  void replaceRule(Node* node){
    if(node == NULL){return;}
    if(node->value.find("$") != std::string::npos){
      std::string num = node->value.substr(1,node->value.length()-1);
      if(isNumber(num)){
        int n = std::stoi(num);
        setNode(node,placeholders[n]);
        return;
      }
    }
    replaceRule(node->left);
    replaceRule(node->right);
  }
};

#endif
