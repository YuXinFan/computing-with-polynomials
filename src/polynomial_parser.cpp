#include <fstream>
#include <iostream>
#include <algorithm>

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
    std::cout << "QAQ ";for ( auto i : stack_expr_str) { std::cout << i << " ";} std::cout<<std::endl;
    std::cout << "QZQ ";for ( auto i : stack_stack) { std::cout << i << " ";}std::cout<<std::endl;
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
  return s;
}
//TODO: implement this file