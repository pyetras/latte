//
//  Maybe.cpp
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "Maybe.h"

using namespace boost;

class eqVisitor : public static_visitor<bool>{
  Maybe::operators_t op;
public:
  eqVisitor(Maybe::operators_t anOp) : op(anOp) {};
  
  template <typename T>
  bool operator()(const T &left, const T &right){
    if (op == Maybe::EQ){
      return left == right;
    }else{
      return left != right;
    }
  }
  
  template <typename T, typename U>
  bool operator()(const T &left, const U &right){
    return false;
  }

};

Maybe Maybe::doNot() const{
  if (isNothing) return Maybe();
  else return Maybe(!get<bool>(val));
}

Maybe Maybe::doNegate() const{
  if (isNothing) return Maybe();
  else return Maybe(-get<int>(val));
}

Maybe Maybe::doOp(Maybe::operators_t op, const Maybe &other) const{
  if (isNothing || other.isNothing) return Maybe();
  else{
    if (op == EQ || op == NEQ){
      eqVisitor vis = eqVisitor(op);
      return Maybe(apply_visitor(vis, val, other.val));
    }else{
      switch (op) {
        case Plus:
          return Maybe(get<int>(val) + get<int>(other.val));
        case Minus:
          return Maybe(get<int>(val) - get<int>(other.val));
        case Mul:
          return Maybe(get<int>(val) * get<int>(other.val));
        case Div:
          return Maybe(get<int>(val) / get<int>(other.val));
        case Mod:
          return Maybe(get<int>(val) % get<int>(other.val));
        case LT:
          return Maybe(get<int>(val) < get<int>(other.val));
        case LTE:
          return Maybe(get<int>(val) <= get<int>(other.val));
        case GT:
          return Maybe(get<int>(val) > get<int>(other.val));
        case GTE:
          return Maybe(get<int>(val) >= get<int>(other.val));
        case Or:
          return Maybe(get<bool>(val) || get<bool>(other.val));
        case And:
          return Maybe(get<bool>(val) && get<bool>(other.val));
        default:
          break;
      }
    }
  }
  
  //should not happen
  return Maybe();
}

bool Maybe::hasValue() const{
  return !isNothing;
}

int Maybe::ivalue() const{
  return get<int>(val);
}

bool Maybe::bvalue() const{
  return get<bool>(val);
}

