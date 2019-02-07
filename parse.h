#ifndef PARSE_H
#define PARSE_H
#include <string>

bool isNumber(std::string);
bool isOperand(std::string);
bool isFunction(std::string);
bool isOperator(std::string);
int comparePrecedence(std::string,std::string);
bool isLeftAssociative(std::string);
std::vector<std::string> splitOnTokens(std::string);
std::vector<std::string> split(std::string);
std::string prepare(std::string);
#endif
