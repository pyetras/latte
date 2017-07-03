//
//  JVMState.cpp
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//
#include <iostream>
#include <algorithm>
#include <math.h>

#include "JVMState.h"
#include "BasicType.h"

typedef std::ostringstream oss;

JVMState::JVMState(RootEnvironment *root, LType::ptr retType, FunctionType* fun, const std::string& classname) : env(root), stack(0), maxStack(0), rootEnv(root), labelcnt(0), name(classname) {
  env = env->pushFun(retType, (int)fun->nargs());
  
}

void JVMState::addCodeLine(oss& ss){
  code.push_back(ss.str());
  ss.str("");
}

void JVMState::addCodeLine_(const std::string& str){
  code.push_back(str);
}

void JVMState::incStack(int i){
  stack += i;
  if (i > 0 && stack > maxStack)
    maxStack = stack;
}

void JVMState::incStack(){
  incStack(1);
}

void JVMState::pushInt(int i){
  std::ostringstream ss;
  ss << "ldc ";
  ss << i;
  code.push_back(ss.str());
  incStack();
}

void JVMState::pushBool(bool val){
  if (val)
    code.push_back("iconst_1");
  else
    code.push_back("iconst_0");
  incStack();
}

void JVMState::pushString(const std::string &str){
  code.push_back("ldc \"" + str + "\"");
  incStack();
}

void JVMState::pushVar(const std::string &var){
  oss ss;
  LType::ptr typ = (*env)[var];
  if (*typ == *BasicType::getString()){
    ss << "a";
  }else{ //bool or int
    ss << "i";
  }
  
  ss << "load " << (*env)(var);
  code.push_back(ss.str());
  incStack();
}

void JVMState::storeVar(const std::string &var){
  oss ss;
  ss << (*env)[var]->toJVMString(true) << "store " << (*env)(var);
  code.push_back(ss.str());
  incStack(-1);
}

void JVMState::add(){
  code.push_back("iadd");
  incStack(-1);
}

void JVMState::sub(){
  code.push_back("isub");
  incStack(-1);
}

void JVMState::mul(){
  addCodeLine_("imul");
  incStack(-1);
}

void JVMState::div(){
  addCodeLine_("idiv");
  incStack(-1);
}

void JVMState::rem(){
  addCodeLine_("irem");
  incStack(-1);
}


void JVMState::addStrings(Expr* exp1, Expr* exp2, Visitor* compiler){
  oss ss;
  addCodeLine_("new java/lang/StringBuilder");
  addCodeLine_("dup");
  incStack(2);
  
  addCodeLine_("invokespecial java/lang/StringBuilder/<init>()V");
  incStack(-1);
  
  exp1->accept(compiler);
  
  addCodeLine_("invokevirtual java/lang/StringBuilder/append(Ljava/lang/String;)Ljava/lang/StringBuilder;");
  incStack(-1);
  
  exp2->accept(compiler);
  
  addCodeLine_("invokevirtual java/lang/StringBuilder/append(Ljava/lang/String;)Ljava/lang/StringBuilder;");
  incStack(-1);
  
  addCodeLine_("invokevirtual java/lang/StringBuilder/toString()Ljava/lang/String;");
  
  code.push_back(ss.str());
}

void JVMState::inc(const std::string &var, int val){
  pushVar(var);
  incStack();
  
  oss ss;
  ss << "iinc " << (*env)(var) << " " << val;
  code.push_back(ss.str());
  pop();
}

void JVMState::inc(const std::string &var){
  inc(var, 1);
}

void JVMState::dec(const std::string &var){
  inc(var, -1);
}

void JVMState::neg(){
  code.push_back("ineg");
}

void JVMState::ret(){
  if (*env->getRetType() == *BasicType::getVoid()){
    code.push_back("return");
  }else{
    code.push_back(env->getRetType()->toJVMString(true) + "return");
    incStack(-1);
  }
}

void JVMState::pop(){
  code.push_back("pop");
  incStack(-1);
}

void JVMState::callFun(const std::string &fname){
  oss ss;
  FunctionType fun = *dynamic_cast<FunctionType*>(&*(*env)[fname]);
  if (fname == "printInt"){
    ss << "getstatic java/lang/System/out Ljava/io/PrintStream;\n";
    incStack();
    ss << "swap\n";
    ss << "invokevirtual java/io/PrintStream/println(I)V";
    incStack(-1);
  }else if(fname == "printString"){
    ss << "getstatic java/lang/System/out Ljava/io/PrintStream;\n";
    incStack();
    ss << "swap\n";
    ss << "invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V";
    incStack(-1);
  }else if(fname == "error"){
    for (int i=0; i < stack; ++i)
      this->pop();
    ss << "ldc \"runtime error\"\n";
    incStack();
    ss << "invokevirtual java/io/PrintStream/println(Ljava/lang/String;)V\n";
    ss << "iconst_1\n";
    ss << "invokestatic java/lang/System/exit(I)V";
    incStack(-1);
  }else if(fname == "readInt"){
    ss << "invokestatic Runtime/readInt()I";
  }else if(fname == "readString"){
    ss << "invokestatic Runtime/readString()Ljava/lang/String;";
  }else{
    ss << "invokestatic " + name + "/latte_" +  fname + fun.toJVMString(true);
  }
  incStack(-(int)fun.nargs());
  if (*fun.getRetType() != *BasicType::getVoid()){
    incStack(1);
  }
  code.push_back(ss.str());
}

void JVMState::noot(){
  oss ss;
  std::string lab1 = env->genLabel(), lab2 = env->genLabel();
  ss << "ifne " << lab1;
  addCodeLine(ss);
  incStack(-1);
  pushBool(true);
  ss << "goto " << lab2;
  addCodeLine(ss);
  ss << lab1 << ":";
  addCodeLine(ss);
  incStack(-1);
  pushBool(false);
  ss << lab2 << ":";
  addCodeLine(ss);  
}

void JVMState::whileLoop(Expr* expr, Stmt* stmt, Visitor* compiler){
  oss ss;
  std::string lab1 = env->genLabel(), lab2 = env->genLabel();
  
  addCodeLine_(lab1 + ":");
  expr->accept(compiler);
  addCodeLine_("ifeq " + lab2);
  stmt->accept(compiler);
  addCodeLine_("goto " + lab1);
  addCodeLine_(lab2 + ":");
}

void JVMState::condIf(Stmt *stmt, Visitor *compiler){
  std::string lab1 = env->genLabel();
  addCodeLine_("ifeq " + lab1);
  stmt->accept(compiler);
  addCodeLine_(lab1 + ":");
}

void JVMState::condIfElse(Stmt *block1, Stmt *block2, Visitor *compiler){
  std::string lab1 = env->genLabel(), lab2 = env->genLabel();
  addCodeLine_("ifeq " + lab1);
  block1->accept(compiler);
  addCodeLine_("goto " + lab2);
  addCodeLine_(lab1 + ":");
  block2->accept(compiler);
  addCodeLine_(lab2 + ":");
}

void JVMState::irel(OP::operator_t op){
  oss ss;
  ss << "if_icmp";
  switch (op) {
    case OP::EQ:
      ss << "eq";
      break;
    case OP::NEQ:
      ss << "ne";
      break;
    case OP::LT:
      ss << "lt";
      break;
    case OP::LTE:
      ss << "le";
      break;
    case OP::GT:
      ss << "gt";
      break;
    case OP::GTE:
      ss << "ge";
      break;
      
    default:
      break;
  }
  std::string lab1 = env->genLabel();
  std::string lab2 = env->genLabel();
  ss << " " << lab1;
  addCodeLine(ss);
  incStack(-2);
  pushBool(false);
  addCodeLine_("goto " + lab2);
  addCodeLine_(lab1 + ":");
  incStack(-1);
  pushBool(true);
  addCodeLine_(lab2 + ":");
}

void JVMState::eqStr(){
  addCodeLine_("invokevirtual java/lang/String/equals(Ljava/lang/Object;)Z");
}

void JVMState::neqStr(){
  eqStr();
  noot();
}

void JVMState::bor(Expr *second, Visitor *compiler){
  addCodeLine_("dup"); incStack();
  
  pushBool(true);
  addCodeLine_("ixor"); incStack(-1);
  
  std::string lab = env->genLabel();
  addCodeLine_("ifeq " + lab); incStack(-1);
  pop();
  second->accept(compiler);
  addCodeLine_(lab + ":");
}

void JVMState::band(Expr *second, Visitor *compiler){
  addCodeLine_("dup"); incStack();
  
  pushBool(true);
  addCodeLine_("ixor"); incStack(-1);
  
  std::string lab = env->genLabel();
  addCodeLine_("ifne " + lab); incStack(-1);
  pop();
  second->accept(compiler);
  addCodeLine_(lab + ":");
}

void JVMState::registerVar(const std::string &var, LType::ptr typ){
  env->insert(var, typ);
}

void JVMState::registerFun(const std::string &name, LType::ptr typ){
  rootEnv->Environment::insert(name, typ);
}

void JVMState::pushBlock(){
  env = env->pushBlock();
}

void JVMState::popBlock(){
  env = env->pop();
}

std::string JVMState::getCode() const{
  bool isVoid = *env->getRetType() == *BasicType::getVoid();
  oss ss;
  ss << signature << '\n';
  ss << ".limit stack " << std::max(maxStack, (int)!isVoid) << '\n';
  ss << ".limit locals " << env->getCounter() << '\n';
  
  for (std::list<std::string>::const_iterator it = code.begin(); it != code.end(); ++it){
    ss << *it << '\n';
  }
  
  std::list<std::string>::const_iterator last = --code.end();
  if (isVoid && *last != "return"){
    ss << "return\n";
  }else if (!isVoid){
    if (*env->getRetType() == *BasicType::getString() && *last != "areturn"){
      ss << "ldc \"\"\n";
      ss << "areturn\n";
    }else if (*last != "ireturn"){
      ss << "iconst_0\n";
      ss << "ireturn\n";
    }
  }
  return ss.str();
}

JVMState::~JVMState(){
  delete env;
}