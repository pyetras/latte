//
//  SpecialisedVisitor.cpp
//  latte
//
//  Created by Piotr Sokólski on 12.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "SpecialisedVisitor.h"

void SpecialisedVisitor::visitProgram(Program* t) {} //abstract class
void SpecialisedVisitor::visitTopDef(TopDef* t) {} //abstract class
void SpecialisedVisitor::visitArg(Arg* t) {} //abstract class
void SpecialisedVisitor::visitAcc(Acc* t) {} //abstract class
void SpecialisedVisitor::visitBlock(Block* t) {} //abstract class
void SpecialisedVisitor::visitStmt(Stmt* t) {} //abstract class
void SpecialisedVisitor::visitItem(Item* t) {} //abstract class
void SpecialisedVisitor::visitType(Type* t) {} //abstract class
void SpecialisedVisitor::visitExpr(Expr* t) {} //abstract class
void SpecialisedVisitor::visitAddOp(AddOp* t) {} //abstract class
void SpecialisedVisitor::visitMulOp(MulOp* t) {} //abstract class
void SpecialisedVisitor::visitRelOp(RelOp* t) {} //abstract class

void SpecialisedVisitor::visitAProgram(AProgram *aprogram)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitFnDef(FnDef *fndef)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitAnArg(AnArg *anarg)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitAccess(Access *evar)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitABlock(ABlock *ablock)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEmpty(Empty *empty)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitBStmt(BStmt *bstmt)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitDecl(Decl *decl)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitNoInit(NoInit *noinit)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitInit(Init *init)
{
  throw "Not implemented";
}


void SpecialisedVisitor::visitAss(Ass *ass)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitArrAss(ArrAss *evar)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitIncr(Incr *incr)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitDecr(Decr *decr)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitRet(Ret *ret)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitVRet(VRet *vret)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitCond(Cond *cond)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitCondElse(CondElse *condelse)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitWhile(While *aWhile)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitSExp(SExp *sexp)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitFor(For *sexp)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitInt(Int *anInt)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitStr(Str *str)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitBool(Bool *aBool)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitVoid(Void *aVoid)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitArray(Array *evar)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitELen(ELen *evar)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEVar(EVar *evar)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitELitInt(ELitInt *elitint)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitELitTrue(ELitTrue *elittrue)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitELitFalse(ELitFalse *elitfalse)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEApp(EApp *eapp)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEString(EString *estring)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitNeg(Neg *neg)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitNot(Not *aNot)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEArrInit(EArrInit *emul)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEGet(EGet *emul)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEMul(EMul *emul)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEAdd(EAdd *eadd)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitERel(ERel *erel)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEAnd(EAnd *eand)
{
  throw "Not implemented";
  
}

void SpecialisedVisitor::visitEOr(EOr *eor)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitPlus(Plus *plus)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitMinus(Minus *minus)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitTimes(Times *times)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitDiv(Div *div)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitMod(Mod *mod)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitLTH(LTH *lth)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitLE(LE *le)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitGTH(GTH *gth)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitGE(GE *ge)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitEQU(EQU *equ)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitNE(NE *ne)
{
  throw "Not implemented";
}


void SpecialisedVisitor::visitListTopDef(ListTopDef* listtopdef)
{
  for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void SpecialisedVisitor::visitListArg(ListArg* listarg)
{
  for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void SpecialisedVisitor::visitListStmt(ListStmt* liststmt)
{
  for (ListStmt::iterator i = liststmt->begin() ; i != liststmt->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void SpecialisedVisitor::visitListItem(ListItem* listitem)
{
  for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void SpecialisedVisitor::visitListExpr(ListExpr* listexpr)
{
  for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void SpecialisedVisitor::visitInteger(Integer x)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitChar(Char x)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitDouble(Double x)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitString(String x)
{
  throw "Not implemented";
}

void SpecialisedVisitor::visitIdent(Ident x)
{
  throw "Not implemented";
}
