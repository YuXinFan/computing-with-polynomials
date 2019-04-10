#include <string>
#include <fstream>
#include <iostream>

#include <cmath>
#include "polynomial.hpp"

Polynomial::Polynomial() {

}

Polynomial::Polynomial( const std::vector<float> & coeffs ) {
  _coeffs = coeffs;
}

Polynomial::Polynomial( std::vector<float> && coeffs ) {
  _coeffs = std::move( coeffs );
}

Polynomial::Polynomial( std::initializer_list<float> coeffs ) {
  _coeffs = std::vector<float>(coeffs.begin(), coeffs.end());
}

Polynomial::Polynomial( const std::string & path ) {
  std::ifstream file(path);
  _coeffs = std::vector<float>();
  std::string coeff_str;
  //std::string vector_str = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  
  while( getline(file, coeff_str, ' ') ) {
    float coeff = stof(coeff_str);
    _coeffs.push_back(coeff);
  }
  //std::cout<<"QAQ "<<vector_str<<std::endl;
}

Polynomial::Polynomial( const Polynomial & p) {
  *this = p;
}

Polynomial::Polynomial( Polynomial && p) {
  *this = std::move(p);
}

Polynomial & Polynomial::operator=( const Polynomial & p ) {
  _coeffs = p._coeffs;
  return *this;
}

Polynomial & Polynomial::operator=( Polynomial && p ) {
  *this = std::move(p);
  return *this;
}

Polynomial::~Polynomial(void) {

}

float & Polynomial::operator[]( int index ) {
  return _coeffs[ index ];
}

const float & Polynomial::operator[]( int index ) const {
  return _coeffs[ index ];
}

int Polynomial::size() const {
  return _coeffs.size();
}

void Polynomial::print() const {
  for( auto i : _coeffs ) {
    std::cout<< i <<" "; 
  }
  std::cout<<std::endl;
}

Polynomial Polynomial::operator+( const Polynomial & p ) const {
  std::vector<float> ret = std::vector<float>();
  if ( size() >= p.size() ) {
    for ( int i = 0; i < p.size(); i++ ) {
      ret.push_back( _coeffs[i] + p._coeffs[i]);
    }
    ret.insert(ret.end(), _coeffs.begin()+p.size(), _coeffs.end());
  }else {
    for ( int i = 0; i < size(); i++ ) {
      ret.push_back( _coeffs[i] + p._coeffs[i]);
    }
    ret.insert(ret.end(), p._coeffs.begin()+size(), p._coeffs.end());
  }
  return Polynomial(ret);
}

Polynomial & Polynomial::operator+=( const Polynomial & p ) {
  if ( size() >= p.size() ) {
    for ( int i = 0; i < p.size(); i++ ) {
      _coeffs[i] += p._coeffs[i];
    }
  }else {
    for ( int i = 0; i < size(); i++ ) {
      _coeffs[i] += p._coeffs[i];
    }
    for ( int i = size(); i < p.size(); i++ ) {
      _coeffs.push_back(p._coeffs[i]);
    }
  }
  return *this;
}

Polynomial Polynomial::operator-( const Polynomial & p ) const {
  std::vector<float> ret = std::vector<float>();
  if ( size() >= p.size() ) {
    for ( int i = 0; i < p.size(); i++ ) {
      ret.push_back( _coeffs[i] - p._coeffs[i]);
    }
    ret.insert(ret.end(), _coeffs.begin()+p.size(), _coeffs.end());
  }else {
    for ( int i = 0; i < size(); i++ ) {
      ret.push_back( _coeffs[i] - p._coeffs[i]);
    }
    for ( int i = size(); i < p.size(); i++ ) {
      ret.push_back( -p._coeffs[i]);
    }
  }
  return Polynomial(ret);
}

Polynomial & Polynomial::operator-=( const Polynomial & p ) {
  if ( size() >= p.size() ) {
    for ( int i = 0; i < p.size(); i++ ) {
      _coeffs[i] -= p._coeffs[i];
    }
  }else {
    for ( int i = 0; i < size(); i++ ) {
      _coeffs[i] -= p._coeffs[i];
    }
    for ( int i = size(); i < p.size(); i++ ) {
      _coeffs.push_back(-p._coeffs[i]);
    }
  }
  return *this;
}

Polynomial Polynomial::operator*( const Polynomial & p ) const {
  std::vector<float> ret = std::vector<float>(size()+p.size()-1, 0.0);
  for ( int i = 0; i < size(); i ++ ) {
    for ( int j = 0; j < p.size(); j++ ) {
      ret[i+j] += _coeffs[i]*p._coeffs[j];
    }
  }
  return Polynomial(ret);
}

Polynomial & Polynomial::operator*=( const Polynomial & p ) {
  std::vector<float> ret = std::vector<float>(size()+p.size()-1, 0.0);
  for ( int i = 0; i < size(); i ++ ) {
    for ( int j = 0; j < p.size(); j++ ) {
      ret[i+j] += _coeffs[i]*p._coeffs[j];
    }
  }
  _coeffs = std::move(ret);
  return *this;
}

Polynomial Polynomial::operator*( float factor ) const {
  std::vector<float> ret = std::vector<float>();
  for ( auto i : _coeffs ) {
    ret.push_back( i * factor );
  }
  return Polynomial(ret);
}

Polynomial Polynomial::compose( const Polynomial & p ) const {
  std::vector<float> ret;
  //exit(0);
  Polynomial p_pow_i = Polynomial(p);
  Polynomial p_r = Polynomial(std::vector<float>{_coeffs[0]});
  for ( int i =1; i < _coeffs.size(); i++ ) {
    p_r += (p_pow_i * _coeffs[i]) ;
    p_pow_i *= p;
  }
  // std::cout << "COM ";
  // for ( auto i : p_r._coeffs){
  //   std::cout << i << " ";
  // }
  // std::cout << std::endl;

  return p_r;
}

float Polynomial::operator()( float x ) const {
  float result = 0.0;
  for ( int i = 0; i < size(); i++ ) {
    result += _coeffs[i]*(pow(x,i));
  }
  //std::cout<< result<<std::endl;
  return result;
}


//TODO: implement this file