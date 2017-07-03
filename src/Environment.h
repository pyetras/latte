//
//  Environment.h
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__Environment__
#define __latte__Environment__

#include <map>
#include <vector>
#include "Type.h"

class Environment {
  typedef std::map<std::string, LType::ptr> Map;

  Environment(LType::ptr retType) : returnType(retType), parent(0) {};
protected:
  Environment *parent;
  LType::ptr returnType;

  Map env;
  Environment() : parent(0) {};
public:
  LType::ptr operator[](const std::string& key) const;
  LType::ptr getRetType() const;
  bool hasKey(const std::string& key) const;
  virtual void insert(const std::string& key, LType::ptr typ);
  
  Environment *push(LType::ptr retType);
  Environment *push();
  
  Environment *pop();
  
  virtual ~Environment() {}
};

#endif /* defined(__latte__Environment__) */
