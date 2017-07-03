//
//  FunctionType.h
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__FunctionType__
#define __latte__FunctionType__

#include <iostream>
#include "Type.h"
#include <list>
#include "BasicType.h"

class FunctionType : public LType {
  typedef std::list<LType::ptr> arglist_t;
  typedef arglist_t::const_iterator arg_i;
  LType::ptr returnType;
public:
  typedef arglist_t args_t;
  arglist_t arguments;
  FunctionType(LType::ptr retType) : returnType(retType) {};
  FunctionType(LType::ptr retType, int count, ...);
  FunctionType(LType::ptr retType, LType::ptr arg1typ);
  FunctionType(LType::ptr retType, arglist_t args) : returnType(retType), arguments(args) {};
  
  bool operator==(const LType& other) const;
  operator llvm::Type*() const;
  operator llvm::FunctionType*() const;
  LType::Type_t getType() const;
  LType::ptr getRetType() const;
  bool checkEq(arglist_t args) const;
  
  std::string toJVMString(bool redundant) const;
  size_t nargs() const;
};

#endif /* defined(__latte__FunctionType__) */
