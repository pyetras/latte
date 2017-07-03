//
//  LLVMCompiler.h
//  latte
//
//  Created by Piotr Sokólski on 13.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__LLVMCompiler__
#define __latte__LLVMCompiler__
#include "llvm/Support/IRBuilder.h"
#include "llvm/Module.h"
#include "llvm/Function.h"

#include "Absyn.H"
#include "SpecialisedVisitor.h"
#include "LLVMEnvironment.h"
#include "BasicType.h"
#include "FunctionType.h"
#include "Returnchecker.h"
#include "RootEnvironment.h"
#include "common.h"
#include <list>
#include <stack>

class LLVMCompiler : public SpecialisedVisitor
{  
  RootEnvironment rootEnv;
  OP::operator_t op;
  std::string code, name;
  
  llvm::Value* curValue;
  llvm::Module* module;
  llvm::IRBuilder<>* builder;
  
  LLVMEnvironment* env;
  
  LType::ptr declTyp;
  
  FunctionType::args_t::const_iterator funArgs;
  llvm::Function::arg_iterator fargIt;
  
  std::stack<std::pair<Block *, std::pair<llvm::IRBuilder<>*, LLVMEnvironment *> > > funBodies;
public:
  LLVMCompiler() {};
  std::string compile(Visitable* v, const std::string& name);
  
  void visitAProgram(AProgram* p);
  void visitFnDef(FnDef* p);
  void visitAnArg(AnArg *anarg);
  void visitABlock(ABlock* p);
  void visitEmpty(Empty* p);
  void visitBStmt(BStmt* p);
  void visitDecl(Decl* p);
  void visitNoInit(NoInit* p);
  void visitInit(Init* p);
  void visitAss(Ass* p);
  void visitIncr(Incr* p);
  void visitDecr(Decr* p);
  void visitRet(Ret* p);
  void visitVRet(VRet* p);
  void visitCond(Cond* p);
  void visitCondElse(CondElse* p);
  void visitWhile(While* p);
  void visitSExp(SExp* p);
  void visitInt(Int* p);
  void visitStr(Str* p);
  void visitBool(Bool* p);
  void visitVoid(Void* p);
  void visitEVar(EVar* p);
  void visitELitInt(ELitInt* p);
  void visitELitTrue(ELitTrue* p);
  void visitELitFalse(ELitFalse* p);
  void visitEApp(EApp* p);
  void visitEString(EString* p);
  void visitNeg(Neg* p);
  void visitNot(Not* p);
  void visitEMul(EMul* p);
  void visitEAdd(EAdd* p);
  void visitERel(ERel* p);
  void visitEAnd(EAnd* p);
  void visitEOr(EOr* p);
  void visitPlus(Plus* p);
  void visitMinus(Minus* p);
  void visitTimes(Times* p);
  void visitDiv(Div* p);
  void visitMod(Mod* p);
  void visitLTH(LTH* p);
  void visitLE(LE* p);
  void visitGTH(GTH* p);
  void visitGE(GE* p);
  void visitEQU(EQU* p);
  void visitNE(NE* p);
  
  void storeAtIndex(llvm::Value* ix, Ident ident_1, Ident ident_2);
  
  void visitAccess(Access *p);
  void visitArrAss(ArrAss *p);
  void visitFor(For *p);
  void visitArray(Array *p);
  void visitELen(ELen *p);
  void visitEArrInit(EArrInit *p);
  void visitEGet(EGet *p);


};


#endif /* defined(__latte__LLVMCompiler__) */
