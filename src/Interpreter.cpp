//
//  Interpreter.cpp
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "Interpreter.h"
#include <list>

Maybe Interpreter::interpret(Visitable* v){
  v->accept(this);
  Maybe ret = operands.top();
  operands.pop();
  assert(operands.size()==0);
  
  return ret;
}

void Interpreter::visitEVar(EVar *evar)
{
  operands.push(Maybe());
}

void Interpreter::visitELitInt(ELitInt *elitint)
{  
  visitInteger(elitint->integer_);
}

void Interpreter::visitELitTrue(ELitTrue *elittrue)
{
  operands.push(Maybe(true));
}

void Interpreter::visitELitFalse(ELitFalse *elitfalse)
{
  operands.push(Maybe(false));
}

void Interpreter::visitEApp(EApp *eapp)
{
  operands.push(Maybe());
}

void Interpreter::visitEString(EString *estring)
{  
  //visitString(estring->string_);
  operands.push(Maybe());
}

void Interpreter::visitNeg(Neg *neg)
{
  neg->expr_->accept(this);
  Maybe m = operands.top();
  operands.pop();
  operands.push(m.doNegate());
}

void Interpreter::visitNot(Not *aNot)
{
  aNot->expr_->accept(this);
  Maybe m = operands.top();
  operands.pop();
  operands.push(m.doNot());
}

void Interpreter::visitEMul(EMul *emul)
{
  emul->expr_1->accept(this);
  
  Maybe left = operands.top();
  operands.pop();
  
  emul->mulop_->accept(this);
  Maybe::operators_t myop = op;
  
  emul->expr_2->accept(this);
  
  Maybe right = operands.top();
  operands.pop();

  operands.push(left.doOp(myop, right));
}

void Interpreter::visitEAdd(EAdd *eadd)
{
  eadd->expr_1->accept(this);
  
  Maybe left = operands.top();
  operands.pop();

  eadd->addop_->accept(this);
  Maybe::operators_t myop = op;
  
  eadd->expr_2->accept(this);
  
  Maybe right = operands.top();
  operands.pop();
  
  operands.push(left.doOp(myop, right));
}

void Interpreter::visitERel(ERel *erel)
{
  /* Code For ERel Goes Here */
  
  erel->expr_1->accept(this);
  
  Maybe left = operands.top();
  operands.pop();

  erel->relop_->accept(this);
  Maybe::operators_t myop = op;
  
  erel->expr_2->accept(this);
  
  Maybe right = operands.top();
  operands.pop();
  
  operands.push(left.doOp(myop, right));
}

void Interpreter::visitEAnd(EAnd *eand)
{
  /* Code For EAnd Goes Here */
  
  eand->expr_1->accept(this);
  
  Maybe left = operands.top();
  operands.pop();

  eand->expr_2->accept(this);
  
  Maybe right = operands.top();
  operands.pop();
  
  operands.push(left.doOp(Maybe::And, right));
  
}

void Interpreter::visitEOr(EOr *eor)
{
  /* Code For EOr Goes Here */
  
  eor->expr_1->accept(this);
  
  Maybe left = operands.top();
  operands.pop();

  eor->expr_2->accept(this);
  
  Maybe right = operands.top();
  operands.pop();
  
  operands.push(left.doOp(Maybe::Or, right));
  
}

void Interpreter::visitPlus(Plus *plus)
{
  op = Maybe::Plus;
}

void Interpreter::visitMinus(Minus *minus)
{
  op = Maybe::Minus;
}

void Interpreter::visitTimes(Times *times)
{
  op = Maybe::Mul;
}

void Interpreter::visitDiv(Div *div)
{
  op = Maybe::Div;
}

void Interpreter::visitMod(Mod *mod)
{
  op = Maybe::Mod;
}

void Interpreter::visitLTH(LTH *lth)
{
  op = Maybe::LT;
}

void Interpreter::visitLE(LE *le)
{
  op = Maybe::LTE;
}

void Interpreter::visitGTH(GTH *gth)
{
  op = Maybe::GT;
}

void Interpreter::visitGE(GE *ge)
{
  op = Maybe::GTE;
}

void Interpreter::visitEQU(EQU *equ)
{
  op = Maybe::EQ;
}

void Interpreter::visitNE(NE *ne)
{
  op = Maybe::NEQ;
}

void Interpreter::visitInteger(Integer x)
{
  operands.push(Maybe(x));
}

void Interpreter::visitELen(ELen *p)
{
  operands.push(Maybe());
}

void Interpreter::visitEArrInit(EArrInit *p)
{
  operands.push(Maybe());
}

void Interpreter::visitEGet(EGet *p)
{
  operands.push(Maybe());
}
