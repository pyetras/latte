//
//  Typechecker.h
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__Typechecker__
#define __latte__Typechecker__

#include <iostream>
/* You might want to change the above name. */

#include "Absyn.H"
#include "RootEnvironment.h"
#include "BasicType.h"
#include "FunctionType.h"
#include "Returnchecker.h"
#include "Interpreter.h"
#include "SpecialisedVisitor.h"
#include <list>
#include <stack>

class Typechecker : public SpecialisedVisitor
{
  Environment *env;
  std::list<LType::ptr> typeS;
  std::list<Ident> identS;
  
  std::stack<std::pair<Environment *, Block *> > funBodies;
  
  char op;
  
  Returnchecker returnchecker;
  Interpreter interpreter;
  
  Stmt* replaceStm;
  
  LType::ptr arrayInner;
public:
  Typechecker() : replaceStm(0) {};
  void typecheck(Visitable* v);
  
  void visitAProgram(AProgram* p);
  void visitFnDef(FnDef* p);
  void visitAnArg(AnArg* p);
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
  void visitListTopDef(ListTopDef* p);
  void visitListArg(ListArg* p);
  void visitListStmt(ListStmt* p);
  void visitListItem(ListItem* p);
  void visitListExpr(ListExpr* p);
  
  void visitInteger(Integer x);
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);
  
  void visitAccess(Access *p);
  void visitArrAss(ArrAss *p);  
  void visitFor(For *p);
  void visitArray(Array *p);
  void visitELen(ELen *p);
  void visitEArrInit(EArrInit *p);
  void visitEGet(EGet *p);
};

#endif /* defined(__latte__Typechecker__) */
