//
//  Interpreter.h
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__Interpreter__
#define __latte__Interpreter__

#include <iostream>
/* You might want to change the above name. */

#include "Absyn.H"
#include "RootEnvironment.h"
#include "Maybe.h"
#include <stack>
#include "SpecialisedVisitor.h"

class Interpreter : public SpecialisedVisitor
{
  std::stack<Maybe> operands;
  
  Maybe::operators_t op;
  
public:
  Maybe interpret(Visitable* v);
  
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
  
  void visitInteger(Integer x);
  
  void visitELen(ELen *p);
  void visitEArrInit(EArrInit *p);
  void visitEGet(EGet *p);
};

#endif /* defined(__latte__Interpreter__) */
