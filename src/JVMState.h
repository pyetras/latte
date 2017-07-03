//
//  JVMState.h
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__JVMState__
#define __latte__JVMState__

#include <string>
#include <sstream>

#include "RootEnvironment.h"
#include <list>
#include "Type.h"
#include "Absyn.H"
#include "common.h"
#include "FunctionType.h"

class JVMState{
  int stack, maxStack, labelcnt;
  std::string name;
  void incStack(int i);
  void incStack();
  std::list<std::string> code;
  RootEnvironment *rootEnv;
  void addCodeLine(std::ostringstream& ss);
  void addCodeLine_(const std::string& str);
public:  
  JVMEnvironment *env;
  std::string signature;

  JVMState(RootEnvironment* root, LType::ptr retType, FunctionType* fun, const std::string& classname);
  void pushInt(int i);
  void pushBool(bool val);
  void pushString(const std::string& str);
  void pushVar(const std::string& var);
  void storeVar(const std::string& var);
  
  void add();
  void sub();
  void mul();
  void div();
  void rem();
  void addStrings(Expr* exp1, Expr* exp2, Visitor* compiler);
  void inc(const std::string& var, int val);
  void inc(const std::string& var);
  void dec(const std::string& var);
  void neg();
  void ret();
  void pop();
  void callFun(const std::string& fname);
  
  void noot();
  void whileLoop(Expr* expr, Stmt* stmt, Visitor* compiler);
  void condIf(Stmt* stmt, Visitor* compiler);
  void condIfElse(Stmt* block1, Stmt* block2, Visitor* compiler);
  
  void irel(OP::operator_t op);
  void eqStr();
  void neqStr();
  
  void bor(Expr* second, Visitor* compiler);
  void band(Expr* second, Visitor* compiler);
  
  void registerVar(const std::string& var, LType::ptr typ);
  void registerFun(const std::string& name, LType::ptr typ);
  void pushBlock();
  void popBlock();
    
  std::string getCode() const;
  
  ~JVMState();
};

#endif /* defined(__latte__JVMState__) */
