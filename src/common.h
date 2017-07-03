//
//  common.h
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__common__
#define __latte__common__
#include <string>

void error(const std::string& message, int line);
namespace OP {
  enum operator_t {
    Plus, Minus, Mul, Div, Mod, LT, LTE, EQ, NEQ, GT, GTE, Or, And
  };
}

#endif /* defined(__latte__common__) */
