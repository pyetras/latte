//
//  Returnchecker.h
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__Returnchecker__
#define __latte__Returnchecker__

#include <iostream>
/* You might want to change the above name. */

#include "Absyn.H"
#include "SpecialisedVisitor.h"
class Returnchecker : public SpecialisedVisitor
{

  bool ret;
  
public:
  Returnchecker(): ret(false) {};
  bool returncheck(Visitable* v);
  
  void visitABlock(ABlock* p);
  void visitEmpty(Empty* p);
  void visitBStmt(BStmt* p);
  void visitDecl(Decl* p);
  void visitAss(Ass* p);
  void visitIncr(Incr* p);
  void visitDecr(Decr* p);
  void visitRet(Ret* p);
  void visitVRet(VRet* p);
  void visitCond(Cond* p);
  void visitCondElse(CondElse* p);
  void visitWhile(While* p);
  void visitSExp(SExp* p);
  
  void visitArrAss(ArrAss *p);
  void visitFor(For *p);
};

#endif /* defined(__latte__Returnchecker__) */
