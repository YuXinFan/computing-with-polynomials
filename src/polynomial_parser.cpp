#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "polynomial_parser.hpp"

Polynomial PolynomialParser::compute_polynomial( const std::string & filename,
                                                 std::map<std::string, Polynomial> & polys ) {
  std::ifstream file(filename);
  std::istreambuf_iterator<char> it(file),end;
  std::vector<std::string> stack_expr_str = std::vector<std::string>();
  std::string pol_str = std::string();
  while(it != end) {
    if ( *it == '(' ||
        * it == ')' ||
        * it == '+' ||
        * it == '-' ||
        * it == '*') {
      if ( pol_str.size() != 0 ) {
        stack_expr_str.push_back( pol_str );
        pol_str.clear();
      }
      stack_expr_str.push_back(std::string(1, *it));
    }else {
      pol_str.append(1, *it);
    }
    it++;
  }

  std::cout <<"QBQ ";
  for ( auto i : stack_expr_str ) {
    std::cout << i << " ";
  }
  std::cout<<std::endl;
  std::reverse(stack_expr_str.begin(), stack_expr_str.end());
  std::cout <<"QBQ ";
  for ( auto i : stack_expr_str ) {
    std::cout << i << " ";
  }
  std::cout<<std::endl;
  //exit(0);
  std::vector<std::string> stack_stack = std::vector<std::string>();
  while ( !stack_expr_str.empty()) {
    if (stack_expr_str.back() == ")" ) {
      std::vector<std::string>::reverse_iterator it = stack_stack.rbegin();
      it++;
      // Case (p) with stack: ...(p
      //     with expr_stack: ...)
      if ( *it == "(" ) {
        // Pop ) from expr
        stack_expr_str.pop_back();

        std::string p2_str = stack_stack.back();
        // Pop p from stack
        stack_stack.pop_back();
        // Pop ( from stack
        stack_stack.pop_back();
        // Case p1(p2) with stack: ...p1 
        if ( stack_stack.empty() ) {
          stack_stack.push_back(p2_str);
        }else if ( stack_stack.back().size() > 1 ) {
          std::string p1_str = stack_stack.back();
          Polynomial p1 = polys[p1_str];
          Polynomial p2 = polys[p2_str];
          std::string p3_str = p1_str + p2_str;
          Polynomial p3 = p1.compose(p2);
          polys[p3_str] = p3;
          // Pop p1 from stack
          stack_stack.pop_back();
          // Push p3 into stack
          stack_stack.push_back(p3_str);
        }else {
          stack_stack.push_back(p2_str);
        }
      }
    }else if (stack_expr_str.back().size() > 1){
      // Case p1 op p2 with stack: ...p1 op
      //                     expr: ...p2
      if ( stack_stack.back() == "+" ) {
        // Pop + from stack
        stack_stack.pop_back();
        std::string p1_str = stack_stack.back();
        Polynomial p1 = polys[p1_str];
        // Pop p1 from stack
        stack_stack.pop_back();
        std::string p2_str = stack_expr_str.back();
        Polynomial p2 = polys[p2_str];
        // Pop p2 from expr
        stack_expr_str.pop_back();
        std::string p3_str = p1_str + "+" + p2_str;
        Polynomial p3 = p1 + p2;
        polys[p3_str] = p3;
        // Push p3 into stack
        stack_stack.push_back(p3_str);
      }else if ( stack_stack.back() == "-" ) {
        // Pop + from stack
        stack_stack.pop_back();
        std::string p1_str = stack_stack.back();
        Polynomial p1 = polys[p1_str];
        // Pop p1 from stack
        stack_stack.pop_back();
        std::string p2_str = stack_expr_str.back();
        Polynomial p2 = polys[p2_str];
        // Pop p2 from expr
        stack_expr_str.pop_back();
        std::string p3_str = p1_str + "-" + p2_str;
        Polynomial p3 = p1 - p2;
        polys[p3_str] = p3;
        // Push p3 into stack
        stack_stack.push_back(p3_str);
      }else if ( stack_stack.back() == "*" ) {
        // Pop + from stack
        stack_stack.pop_back();
        std::string p1_str = stack_stack.back();
        Polynomial p1 = polys[p1_str];
        // Pop p1 from stack
        stack_stack.pop_back();
        std::string p2_str = stack_expr_str.back();
        Polynomial p2 = polys[p2_str];
        // Pop p2 from expr
        stack_expr_str.pop_back();
        std::string p3_str = p1_str + "*" + p2_str;
        Polynomial p3 = p1 * p2;
        polys[p3_str] = p3;
        // Push p3 into stack
        stack_stack.push_back(p3_str);
      }else {
        stack_stack.push_back( stack_expr_str.back());
        stack_expr_str.pop_back();
      }      
    }else {
      stack_stack.push_back( stack_expr_str.back());
      stack_expr_str.pop_back();
    }
    
  }

//   exit(0);
  
  Polynomial ret = polys[stack_stack.front()];

  return ret;

}

std::function<float(float)> PolynomialParser::compute_lambda( const std::string & filename, std::map<std::string, Polynomial> & polys) {
  scalarFct s;
  s = [&, filename](float x)->float{
  // Load expression string from path
  std::cout<< filename <<std::endl;
  std::ifstream file(filename);
  std::istreambuf_iterator<char> it(file),end;
  std::vector<std::string> stack_expr_str = std::vector<std::string>();
  std::string pol_str = std::string();
  while(it != end) {
    if ( *it == '(' ||
        * it == ')' ||
        * it == '+' ||
        * it == '-' ||
        * it == '*') {
      if ( pol_str.size() != 0 ) {
        stack_expr_str.push_back( pol_str );
        pol_str.clear();
      }
      stack_expr_str.push_back(std::string(1, *it));
    }else {
      pol_str.append(1, *it);
    }
    it++;
  }
  
  // Calculate polynomial p with paramater x
  std::function<float(Polynomial &, float)> calculateFct = [&](Polynomial p, float x){
    float result = 0.f;
    for (int i = 0; i < p.size(); i++ ) {
      result += (p[i]* pow(x, i));
    }
    return result;
  };
  std::function<Polynomial(Polynomial &, Polynomial &, float)> plusFct = [&](Polynomial p1, Polynomial p2, float x)->Polynomial{
    float result = calculateFct(p1, x) + calculateFct(p2, x);
    return Polynomial(std::vector<float>{result});
  };
  std::function<Polynomial(Polynomial &, Polynomial &, float)> minusFct = [&](Polynomial p1, Polynomial p2, float x)->Polynomial{
    float result = calculateFct(p1, x) - calculateFct(p2, x);
    return Polynomial(std::vector<float>{result});
  };
  std::function<Polynomial(Polynomial &, Polynomial &, float)> mulFct = [&](Polynomial p1, Polynomial p2, float x)->Polynomial{
    float result = calculateFct(p1, x) * calculateFct(p2, x);
    return Polynomial(std::vector<float>{result});
  }; 
  #define show(x)   std::cout << "SHOW "; for(int i = 0; i < x.size(); i++) {std::cout << x[i] << " ";}std::cout<<std::endl;

  std::function<Polynomial(Polynomial &, Polynomial &, float)> composeFct = [&](Polynomial p1, Polynomial p2, float x)->Polynomial{
    show(p2);
    show(p1);
    float var = calculateFct(p2, x);
    float result = calculateFct(p1, var);
    std::cout<<var<<" "<< result<<std::endl;
    return Polynomial(std::vector<float>{result});
  }; 
  std::reverse(stack_expr_str.begin(), stack_expr_str.end());
  std::vector<std::string> stack_stack = std::vector<std::string>();
  
  //exit(0);
    while ( !stack_expr_str.empty()) {   
      std::cout<<"GGG ";show(stack_expr_str);
      std::cout<<"BBB ";show(stack_stack);
      if (stack_expr_str.back() == ")" ) {
        std::vector<std::string>::reverse_iterator it = stack_stack.rbegin();
        it++;  
        // Case (p) with stack: ...(p
        //     with expr_stack: ...)
        if ( *it == "(" ) {
          // Pop ) from expr
          stack_expr_str.pop_back();
          std::string p2_str = stack_stack.back();
          // Pop p from stack
          stack_stack.pop_back();
          // Pop ( from stack
          stack_stack.pop_back();
          // Case p1(p2) with stack: ...p1 
          if ( stack_stack.empty() ) {
            stack_stack.push_back(p2_str);
          }else if ( stack_stack.back().size() > 1 ) {
            std::string p1_str = stack_stack.back();
            Polynomial p1 = polys[p1_str];
            Polynomial p2 = polys[p2_str];
            std::string p3_str = p1_str + p2_str;
            Polynomial p3 = composeFct(p1, p2, x);
            polys[p3_str] = p3;
            // Pop p1 from stack
            stack_stack.pop_back();
            // Push p3 into stack
            stack_stack.push_back(p3_str);
          }else {
            stack_stack.push_back(p2_str);
          }
        }
      }else if (stack_expr_str.back().size() > 1){

        // Case (p1-p2)(p3) with stack: ...(p1-p2
        //                        expr: ...)p3()
        // change stacks to stack: ...(p1(p3
        //                   expr: ...))p3(p2-)
        if ( stack_expr_str.size() >= 5 && stack_stack.size() >=3 ) {
          if ( stack_expr_str[stack_expr_str.size()-2] == ")" && stack_expr_str[stack_expr_str.size()-3]== "(") {
            std::string p1_str = stack_stack[stack_stack.size()-2];
            std::string p2_str = stack_expr_str.back();
            std::string op = stack_stack.back();
            std::string p3_str = stack_expr_str[stack_expr_str.size()-4];
            
            std::string p13_str = p1_str + p3_str;
            Polynomial p13 = composeFct(polys[p1_str], polys[p3_str], x);
            polys[p13_str] = p13;
            std::string p23_str = p2_str + p3_str;
            Polynomial p23 = composeFct(polys[p2_str], polys[p3_str], x);
            polys[p23_str] = p23;
            stack_stack[stack_stack.size()-2] = p13_str;
            
            for ( int i : {1,2,3,4} ) {
              stack_expr_str.pop_back();
            }
            stack_expr_str.push_back(p23_str);
            continue;
          }
        }
        // Case p1 op p2 with stack: ...p1 op
        //                     expr: ...p2
        if ( stack_stack.back() == "+" ) {
          // Pop + from stack
          stack_stack.pop_back();
          std::string p1_str = stack_stack.back();
          Polynomial p1 = polys[p1_str];
          // Pop p1 from stack
          stack_stack.pop_back();
          std::string p2_str = stack_expr_str.back();
          Polynomial p2 = polys[p2_str];
          // Pop p2 from expr
          stack_expr_str.pop_back();
          std::string p3_str = p1_str + "+" + p2_str;
          Polynomial p3 = plusFct(p1, p2, x);
          polys[p3_str] = p3;
          // Push p3 into stack
          stack_stack.push_back(p3_str);
        }else if ( stack_stack.back() == "-" ) {
          // Pop + from stack
          stack_stack.pop_back();
          std::string p1_str = stack_stack.back();
          Polynomial p1 = polys[p1_str];
          // Pop p1 from stack
          stack_stack.pop_back();
          std::string p2_str = stack_expr_str.back();
          Polynomial p2 = polys[p2_str];
          // Pop p2 from expr
          stack_expr_str.pop_back();
          std::string p3_str = p1_str + "-" + p2_str;
          Polynomial p3 = minusFct(p1, p2, x);
          polys[p3_str] = p3;
          // Push p3 into stack
          stack_stack.push_back(p3_str);
        }else if ( stack_stack.back() == "*" ) {
          // Pop + from stack
          
          stack_stack.pop_back();
          std::string p1_str = stack_stack.back();
          Polynomial p1 = polys[p1_str];
          // Pop p1 from stack
          stack_stack.pop_back();
          std::string p2_str = stack_expr_str.back();
          Polynomial p2 = polys[p2_str];
          // Pop p2 from expr
          stack_expr_str.pop_back();
          std::string p3_str = p1_str + "*" + p2_str;
          
          Polynomial p3 = mulFct(p1, p2, x);
          //exit(0);
          polys[p3_str] = p3;
          // Push p3 into stack
          stack_stack.push_back(p3_str);
        }else {
          stack_stack.push_back( stack_expr_str.back());
          stack_expr_str.pop_back();
        }      
      }else {
        stack_stack.push_back( stack_expr_str.back());
        stack_expr_str.pop_back();
      }
      
    }
    
    return polys[stack_stack.back()][0];
  };
  return s;
}
//TODO: implement this file