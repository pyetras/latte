//
//  FunctionType.cpp
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "FunctionType.h"
#include "llvm/ADT/ArrayRef.h"
#include <cstdarg>
#include <boost/foreach.hpp>
#include <vector>

LType::Type_t FunctionType::getType() const{
  return TFunction;
}

bool FunctionType::operator==(const LType &other) const{
  if (other.getType() == TFunction){
    const FunctionType* t = static_cast<const FunctionType*>(&other);
    if (t->returnType == this->returnType){
      return this->checkEq(t->arguments);
    }
  }
  return false;
}

FunctionType::FunctionType(LType::ptr retType, int count, ...) : returnType(retType) {
  va_list arguments;
  va_start(arguments, count);
  for (int i = 0; i< count; ++i){
    LType *t = va_arg(arguments, LType*);
    this->arguments.push_back(LType::ptr(t));
  }
}

FunctionType::FunctionType(LType::ptr retType, LType::ptr arg1typ) : returnType(retType){
  this->arguments.push_back(arg1typ);
}

LType::ptr FunctionType::getRetType() const{
  return this->returnType;
}

bool FunctionType::checkEq(arglist_t args) const{
  arg_i one, two;
  for(one = this->arguments.begin(), two = args.begin();
      one != this->arguments.end() && two != args.end();
      ++one, ++two){
    if (**one != **two){
      return false;
    }
  }
  if (one != this->arguments.end() || two != args.end()){
    return false;
  }else{
    return true;
  }
}

std::string FunctionType::toJVMString(bool redundant) const{
  std::string code = "(";
  BOOST_FOREACH(LType::ptr typ, arguments){
    code += typ->toJVMString(false);
  }
  code += ")" + returnType->toJVMString(false);
  return code;
}

size_t FunctionType::nargs() const{
  return arguments.size();
}

FunctionType::operator llvm::Type*() const{
  return static_cast<llvm::FunctionType*>(*this);
}

FunctionType::operator llvm::FunctionType*() const{
  std::vector<llvm::Type*> args;
  BOOST_FOREACH(LType::ptr arg, arguments){
    args.push_back(*arg);
  }
  
  return llvm::FunctionType::get(static_cast<llvm::Type*>(*returnType), args, false);
}
