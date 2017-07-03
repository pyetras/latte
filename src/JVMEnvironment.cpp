//
//  JVMEnvironment.cpp
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "JVMEnvironment.h"

JVMEnvironment *JVMEnvironment::pushBlock(){
  return this->push(this->getRetType(), false);
}

JVMEnvironment *JVMEnvironment::pushFun(LType::ptr typ, int nargs){
  varCounter += nargs;
  return this->push(typ, true);
}

JVMEnvironment* JVMEnvironment::push(LType::ptr typ, bool isolated){
  JVMEnvironment *e = new JVMEnvironment((isolated)?0:varCounter, typ);
  e->parent = this;
  e->isolated = isolated;
  return e;
}

JVMEnvironment* JVMEnvironment::pop(){
  JVMEnvironment* e = static_cast<JVMEnvironment*>(this->parent);
  if (!isolated) e->varCounter = this->varCounter;
  delete this;
  return e;
}

int JVMEnvironment::incCounter(){
  return varCounter++;
}

int JVMEnvironment::getCounter() const{
  return varCounter;
}

void JVMEnvironment::insert(const std::string &key, LType::ptr typ){
  Environment::insert(key, typ);
  cenv.insert(std::make_pair(key, varCounter));
  incCounter();
}

int JVMEnvironment::operator()(const std::string &key) const{
  CMap::const_iterator it;
  if ((it = this->cenv.find(key)) != this->cenv.end()){
    return (it->second);
  }else if (this->parent != 0){
    return (*static_cast<JVMEnvironment*>(this->parent))(key);
  }else{
    return -1;
  }
}

std::string JVMEnvironment::genLabel(){
  return static_cast<JVMEnvironment*>(this->parent)->genLabel();
}