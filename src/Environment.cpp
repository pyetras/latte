//
//  Environment.cpp
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "Environment.h"
#include "common.h"

bool Environment::hasKey(const std::string &key) const{
  if (this->env.find(key) == this->env.end()){
    if (this->parent == 0) {
      return false;
    }else{
      return this->parent->hasKey(key);
    }
  }else{
    return true;
  }
}

LType::ptr Environment::getRetType() const{
  return this->returnType;
}

LType::ptr Environment::operator[](const std::string &key) const{
  Map::const_iterator it;
  if ((it = this->env.find(key)) != this->env.end()){
    return (it->second);
  }else if (this->parent != 0){
    return (*this->parent)[key];
  }else{
    error(key + " does not exist", 0);
  }
}

void Environment::insert(const std::string &key, LType::ptr typ){
  if (this->env.find(key) != this->env.end()){
    error(key + " already exists", 0);
  }else{
    this->env.insert(std::make_pair(key, typ));
  }
}

Environment *Environment::push(LType::ptr retType){
  Environment *e = new Environment(retType);
  e->parent = this;
  return e;
}

Environment *Environment::push(){
  return this->push(this->returnType);
}

Environment *Environment::pop(){
  Environment *e = this->parent;
  delete this;
  return e;
}