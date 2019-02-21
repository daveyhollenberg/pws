#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <algorithm>

#include "parse.h"
#include "tree.h"
#include "primitive.h"
#include "derivative.h"
#include "common.h"

Node* root;

void replaceOne(std::string& s, std::string a, std::string b){
  int p = s.find(a);
  if(p == std::string::npos){return;}
  if(b.find("\\") != std::string::npos){
    b = b + " ";
  }
  s = s.substr(0,p) + b + s.substr(p + a.length(),s.length() - p - a.length());
}
void replace(std::string& s, std::string a, std::string b){
  std::string old;
  do{
    old = s;
    replaceOne(s,a,b);
  }while(s != old);
}

int main(int argc,char** argv){
  std::string formula = "";
  std::string dp = "d";
  if(argc > 1){
    formula = argv[1];
    if(formula == ""){
      std::cout << "error##Geen formule ingevoerd" << std::endl;
      return 0;
    }
    if(argc > 2){
      dp = argv[2];
    }
  }


//3*x^2-5*x+8+4^(3x)   (4x)^(2x)
  //5x^2-6x p
  //-1*XX -> -XX
  // 3*x^2-5*x+8  //5x^2+8*4x-7+log(5x)  e^(x+4)*sin((x+ln(2))/6)

  if(dp == "d"){
    std::string output = derivative(formula);
    std::cout << output << std::endl;
  }else if(dp == "p"){
    std::string output = primitive(formula);
    std::cout << output << std::endl;
  }
  return 0;
}
