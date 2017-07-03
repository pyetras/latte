//
//  Returnchecker.cpp
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "Returnchecker.h"
#include <list>

bool Returnchecker::returncheck(Visitable* v){
  ret = false;
  v->accept(this);
  return ret;
}

void Returnchecker::visitABlock(ABlock *ablock)
{
  ablock->liststmt_->accept(this);
}

void Returnchecker::visitEmpty(Empty *empty)
{
}

void Returnchecker::visitBStmt(BStmt *bstmt)
{
  bstmt->block_->accept(this);
}

void Returnchecker::visitDecl(Decl *decl)
{

}

void Returnchecker::visitAss(Ass *ass)
{
  
}

void Returnchecker::visitIncr(Incr *incr)
{  

}

void Returnchecker::visitDecr(Decr *decr)
{

}

void Returnchecker::visitRet(Ret *aret)
{
  ret = true;
}

void Returnchecker::visitVRet(VRet *vret)
{
  
}

void Returnchecker::visitCond(Cond *cond)
{
  //cond->stmt_->accept(this);
}

void Returnchecker::visitCondElse(CondElse *condelse)
{
  if (!ret){
    condelse->stmt_1->accept(this);
    bool tmp = ret;
    ret = false;
    condelse->stmt_2->accept(this);
    ret = ret && tmp;
  }
}

void Returnchecker::visitWhile(While *aWhile)
{
}

void Returnchecker::visitSExp(SExp *sexp)
{
}

void Returnchecker::visitArrAss(ArrAss *p){
  
}

void Returnchecker::visitFor(For *p){
  
}

