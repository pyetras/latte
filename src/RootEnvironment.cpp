//
//  RootEnvironment.cpp
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "RootEnvironment.h"
#include "BasicType.h"
#include "FunctionType.h"
#include <sstream>

RootEnvironment::RootEnvironment(){
  this->env.insert(std::make_pair("printInt", LType::ptr(new FunctionType(BasicType::getVoid(), BasicType::getInt()))));
  this->env.insert(std::make_pair("printString", LType::ptr(new FunctionType(BasicType::getVoid(), BasicType::getString()))));
  this->env.insert(std::make_pair("error", LType::ptr(new FunctionType(BasicType::getVoid()))));
  this->env.insert(std::make_pair("readInt", LType::ptr(new FunctionType(BasicType::getInt()))));
  this->env.insert(std::make_pair("readString", LType::ptr(new FunctionType(BasicType::getString()))));
  
//  this->env.insert(std::make_pair("readString", LType::ptr(new FunctionType(BasicType::getVoid(), BasicType::getBool()))));
}

