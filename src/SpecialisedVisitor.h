//
//  SpecialisedVisitor.h
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__SpecialisedVisitor__
#define __latte__SpecialisedVisitor__

#include "Absyn.H"

class SpecialisedVisitor : public Visitor
{
  
public:
  virtual void visitProgram(Program *p);
  virtual void visitTopDef(TopDef *p);
  virtual void visitArg(Arg *p);
  virtual void visitAcc(Acc *p);
  virtual void visitBlock(Block *p);
  virtual void visitStmt(Stmt *p);
  virtual void visitItem(Item *p);
  virtual void visitType(Type *p);
  virtual void visitExpr(Expr *p);
  virtual void visitAddOp(AddOp *p);
  virtual void visitMulOp(MulOp *p);
  virtual void visitRelOp(RelOp *p);
  virtual void visitAProgram(AProgram *p);
  virtual void visitFnDef(FnDef *p);
  virtual void visitAnArg(AnArg *p);
  virtual void visitAccess(Access *p);
  virtual void visitABlock(ABlock *p);
  virtual void visitEmpty(Empty *p);
  virtual void visitBStmt(BStmt *p);
  virtual void visitDecl(Decl *p);
  virtual void visitNoInit(NoInit *p);
  virtual void visitInit(Init *p);
  virtual void visitAss(Ass *p);
  virtual void visitArrAss(ArrAss *p);
  virtual void visitIncr(Incr *p);
  virtual void visitDecr(Decr *p);
  virtual void visitRet(Ret *p);
  virtual void visitVRet(VRet *p);
  virtual void visitCond(Cond *p);
  virtual void visitCondElse(CondElse *p);
  virtual void visitWhile(While *p);
  virtual void visitSExp(SExp *p);
  virtual void visitFor(For *p);
  virtual void visitInt(Int *p);
  virtual void visitStr(Str *p);
  virtual void visitBool(Bool *p);
  virtual void visitVoid(Void *p);
  virtual void visitArray(Array *p);
  virtual void visitELen(ELen *p);
  virtual void visitEVar(EVar *p);
  virtual void visitELitInt(ELitInt *p);
  virtual void visitELitTrue(ELitTrue *p);
  virtual void visitELitFalse(ELitFalse *p);
  virtual void visitEApp(EApp *p);
  virtual void visitEString(EString *p);
  virtual void visitNeg(Neg *p);
  virtual void visitNot(Not *p);
  virtual void visitEArrInit(EArrInit *p);
  virtual void visitEGet(EGet *p);
  virtual void visitEMul(EMul *p);
  virtual void visitEAdd(EAdd *p);
  virtual void visitERel(ERel *p);
  virtual void visitEAnd(EAnd *p);
  virtual void visitEOr(EOr *p);
  virtual void visitPlus(Plus *p);
  virtual void visitMinus(Minus *p);
  virtual void visitTimes(Times *p);
  virtual void visitDiv(Div *p);
  virtual void visitMod(Mod *p);
  virtual void visitLTH(LTH *p);
  virtual void visitLE(LE *p);
  virtual void visitGTH(GTH *p);
  virtual void visitGE(GE *p);
  virtual void visitEQU(EQU *p);
  virtual void visitNE(NE *p);
  virtual void visitListTopDef(ListTopDef *p);
  virtual void visitListArg(ListArg *p);
  virtual void visitListStmt(ListStmt *p);
  virtual void visitListItem(ListItem *p);
  virtual void visitListExpr(ListExpr *p);
  
  
  virtual void visitInteger(Integer x);
  virtual void visitChar(Char x);
  virtual void visitDouble(Double x);
  virtual void visitString(String x);
  virtual void visitIdent(Ident x);
  
};
#endif /* defined(__latte__SpecialisedVisitor__) */
