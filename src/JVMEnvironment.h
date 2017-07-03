//
//  JVMEnvironment.h
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__JVMEnvironment__
#define __latte__JVMEnvironment__

#include "Environment.h"
#include "Type.h"
#include <map>

class JVMEnvironment : public Environment{
  typedef std::map<std::string, int> CMap;
  int varCounter;
  bool isolated;
  CMap cenv;
  
  JVMEnvironment(int initialCounter, LType::ptr retType) : varCounter(initialCounter), isolated(false) {
    this->parent = 0;
    this->returnType = retType;
  };
public:
  virtual std::string genLabel();

  JVMEnvironment(): varCounter(0) {
    this->parent = 0;
  };
  int incCounter();
  int getCounter() const;
  int operator()(const std::string& key) const;
  void insert(const std::string& key, LType::ptr typ);
  
  JVMEnvironment* pushBlock();
  JVMEnvironment* pushFun(LType::ptr typ, int nargs);
  JVMEnvironment* push(LType::ptr typ, bool isolated);
  JVMEnvironment* pop();
};

#endif /* defined(__latte__JVMEnvironment__) */
