//
//  Maybe.h
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__Maybe__
#define __latte__Maybe__

#include <iostream>
#include <boost/variant.hpp>

class Maybe {  
  boost::variant<int, bool> val;
  bool isNothing;
public:
  enum operators_t {
    Plus, Minus, Mul, Div, Mod, LT, LTE, EQ, NEQ, GT, GTE, Or, And
  };
  
  Maybe() : isNothing(true) {};
  Maybe(int i) : isNothing(false), val(i) {};
  Maybe(bool b) : isNothing(false), val(b) {};
  Maybe(boost::variant<int, bool> &v): isNothing(false), val(v) {};
  
  Maybe doNot() const;
  Maybe doNegate() const;
  Maybe doOp(operators_t op, const Maybe& other) const;
  
  bool hasValue() const;
  int ivalue() const;
  bool bvalue() const;
};
#endif /* defined(__latte__Maybe__) */
