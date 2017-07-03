//
//  LLVMEnvironment.cpp
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "LLVMEnvironment.h"
#include <sstream>
#include "llvm/Support/IRBuilder.h"

LLVMEnvironment *LLVMEnvironment::pushBlock(){
  return this->push(this->getRetType());
}

LLVMEnvironment *LLVMEnvironment::pushFun(llvm::Function* func, LType::ptr typ){
  LLVMEnvironment *e = new LLVMEnvironment(func, typ);
  e->parent = this;
  return e;
}

LLVMEnvironment* LLVMEnvironment::push(LType::ptr typ){
  LLVMEnvironment *e = new LLVMEnvironment(function, typ);
  e->parent = this;
  return e;
}

LLVMEnvironment::CMap LLVMEnvironment::nameCounter;

void LLVMEnvironment::insert(const std::string &key, LType::ptr typ){
  Environment::insert(key, typ);
  
  if (function){
    CMap::const_iterator it = nameCounter.find(key);
    
    int nameI = -1;
    if (it != nameCounter.end()){
      nameI = it->second;
    }
    nameI++;
    nameCounter[key] = nameI;
    
    std::ostringstream ss;
    ss << key << "." << nameI;
    std::string llvmKey = ss.str();
    
    llvm::IRBuilder<> builder(&function->getEntryBlock(), function->getEntryBlock().begin());
    llvm::AllocaInst *alloca = builder.CreateAlloca(*typ, 0, llvmKey);
    amap.insert(std::make_pair(key, std::make_pair(alloca, llvmKey)));
  }
}

LLVMEnvironment::var_t LLVMEnvironment::operator()(const std::string &key) const{
  AMap::const_iterator it;
  if ((it = this->amap.find(key)) != this->amap.end()){
    return (it->second);
  }else if (this->parent != 0){
    return (*static_cast<LLVMEnvironment*>(this->parent))(key);
  }else{
    throw "not found";
  }
}

