//
//  LLVMEnvironment.h
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__LLVMEnvironment__
#define __latte__LLVMEnvironment__

#include "Environment.h"
#include "Type.h"
#include <map>
#include "llvm/Instructions.h"
#include "llvm/Function.h"

class LLVMEnvironment : public Environment{
public:
  typedef std::pair<llvm::AllocaInst*, std::string> var_t;
  typedef std::map<std::string, int> CMap;

  var_t operator()(const std::string& key) const;
  void insert(const std::string& key, LType::ptr typ);
  
  LLVMEnvironment* pushBlock();
  LLVMEnvironment* pushFun(llvm::Function* func, LType::ptr typ);
  LLVMEnvironment* push(LType::ptr typ);
private:
  typedef std::map<std::string, var_t> AMap;
  
  static CMap nameCounter;
  AMap amap;
  llvm::Function *function;
  
  LLVMEnvironment(llvm::Function *func, LType::ptr retType) : function(func){
    this->parent = 0;
    this->returnType = retType;
  };
protected:
  LLVMEnvironment(): function(0){ this->parent = 0;};
};

#endif /* defined(__latte__LLVMEnvironment__) */
