//
//  Typechecker.cpp
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "Typechecker.h"
#include <list>
#include "common.h"
#include <algorithm>
#include "ArrayType.h"

void Typechecker::typecheck(Visitable* v){
  env = new RootEnvironment();
  v->accept(this);
  if (!env->hasKey("main")){
    error("Must have main", 0);
  }
  
  FunctionType::args_t noargs;
  
  FunctionType* fun = static_cast<FunctionType*>(&*(*env)["main"]);
  if (*fun->getRetType() != *BasicType::getInt() || !fun->checkEq(noargs)){
    error("'main' incorrect", 0);
  }
  
  delete env;
}

void Typechecker::visitAProgram(AProgram *aprogram)
{
  /* Code For AProgram Goes Here */
  
  aprogram->listtopdef_->accept(this);
  while (!funBodies.empty()) {
    this->env = funBodies.top().first;
    funBodies.top().second->accept(this);
    if (*this->env->getRetType() != *BasicType::getVoid()){
      if (!returnchecker.returncheck(funBodies.top().second)){
        error("Function must return something", funBodies.top().second->line_number);
      }
    }else{
      if (returnchecker.returncheck(funBodies.top().second)){
        error("Function must not return anything", funBodies.top().second->line_number);
      }
    }
    this->env = this->env->pop();
    funBodies.pop();
  }
}

void Typechecker::visitFnDef(FnDef *fndef)
{
  /* Code For FnDef Goes Here */
  
  fndef->type_->accept(this);
  visitIdent(fndef->ident_);
  Ident fname = identS.front();
  identS.pop_front();
  
  Environment *lastEnv = env;
  
  LType::ptr typ = typeS.front();
  this->env = env->push(typ);
  typeS.pop_front();
  
  fndef->listarg_->accept(this);
  
  typeS.reverse();
  
  LType::ptr fun(new FunctionType(typ, typeS));
  fndef->funtyp = fun;
  typeS.clear();
  
  lastEnv->insert(fname, fun);
  
  funBodies.push(std::make_pair(this->env, fndef->block_));
  
  this->env = lastEnv;
}

void Typechecker::visitAnArg(AnArg *anarg)
{
  anarg->type_->accept(this);
  visitIdent(anarg->ident_);
  
  this->env->insert(identS.front(), typeS.front());
  identS.pop_front();
}

void Typechecker::visitABlock(ABlock *ablock)
{
  ablock->liststmt_->accept(this);
}

void Typechecker::visitEmpty(Empty *empty)
{
  /* Code For Empty Goes Here */
  
  
}

void Typechecker::visitBStmt(BStmt *bstmt)
{
  this->env = this->env->push();
  bstmt->block_->accept(this);
  this->env = this->env->pop();
}

void Typechecker::visitDecl(Decl *decl)
{
  decl->type_->accept(this);
  decl->listitem_->accept(this);
  
  while (!identS.empty()) {
    this->env->insert(identS.back(), typeS.front());
    identS.pop_back();
  }
  
  typeS.pop_back(); //Items should check for correctness
}

void Typechecker::visitNoInit(NoInit *noinit)
{
  /* Code For NoInit Goes Here */
  
  visitIdent(noinit->ident_);
  
}

void Typechecker::visitInit(Init *init)
{
  /* Code For Init Goes Here */
  
  visitIdent(init->ident_);
  init->expr_->accept(this);
  if (*typeS.front() != *typeS.back()){
    error("Incorrect initialization type", init->line_number);
  }
  init->expr_->typ = typeS.front();
  typeS.pop_front();
  
  assert(typeS.size() == 1);
}


void Typechecker::visitAss(Ass *ass)
{
  visitIdent(ass->ident_);
  Ident id = identS.front();
  identS.pop_front();
  
  ass->expr_->accept(this);
  if (*(*this->env)[id] != *typeS.front()){
    error("Variable assignment of another type", ass->line_number);
  }
  ass->expr_->typ = typeS.front();
  typeS.pop_front();
  
}

void Typechecker::visitIncr(Incr *incr)
{
  visitIdent(incr->ident_);
  if (*(*this->env)[identS.front()] != *BasicType::getInt()){
    error("Incrementing something else than int", incr->line_number);
  }
  identS.pop_front();
}

void Typechecker::visitDecr(Decr *decr)
{
  visitIdent(decr->ident_);
  if (*(*this->env)[identS.front()] != *BasicType::getInt()){
    error("Decrementing something else than int", decr->line_number);
  }
  identS.pop_front();
  
}

void Typechecker::visitRet(Ret *ret)
{
  ret->expr_->accept(this);
  if (*typeS.front() != *this->env->getRetType()){
    error("Function must return value of it's type", ret->line_number);
  }
  ret->expr_->typ = typeS.front();
  typeS.pop_front();
}

void Typechecker::visitVRet(VRet *vret)
{
  if (*this->env->getRetType() != *BasicType::getVoid()){
    error("Function must return something", vret->line_number);
  }
}

void Typechecker::visitCond(Cond *cond)
{
  cond->expr_->accept(this);
  if (*typeS.front() != *BasicType::getBool()){
    error("Condition must be boolean", cond->expr_->line_number);
  }
  typeS.pop_front();
  
  cond->stmt_->accept(this);
  
  Maybe val = interpreter.interpret(cond->expr_);
  if (val.hasValue()){
    bool v = val.bvalue();
    if (v){
      replaceStm = cond->stmt_->clone();
    }else{
      replaceStm = new Empty();
    }
  }
}

void Typechecker::visitCondElse(CondElse *condelse)
{
  condelse->expr_->accept(this);
  if (*typeS.front() != *BasicType::getBool()){
    error("Condition must be boolean", condelse->expr_->line_number);
  }
  typeS.pop_front();
  
  condelse->stmt_1->accept(this);
  condelse->stmt_2->accept(this);
  
  Maybe val = interpreter.interpret(condelse->expr_);
  if (val.hasValue()){
    bool v = val.bvalue();
    if (v){
      replaceStm = condelse->stmt_1->clone();
    }else{
      replaceStm = condelse->stmt_2->clone();
    }
  }
}

void Typechecker::visitWhile(While *aWhile)
{
  aWhile->expr_->accept(this);
  if (*typeS.front() != *BasicType::getBool()){
    error("Condition must be boolean", aWhile->expr_->line_number);
  }
  typeS.pop_front();
  
  aWhile->stmt_->accept(this);
}

void Typechecker::visitSExp(SExp *sexp)
{
  sexp->expr_->accept(this);
  typeS.pop_front();
}

void Typechecker::visitInt(Int *anInt)
{
  typeS.push_front(BasicType::getInt());
}

void Typechecker::visitStr(Str *str)
{
  typeS.push_front(BasicType::getString());
}

void Typechecker::visitBool(Bool *aBool)
{
  typeS.push_front(BasicType::getBool());
}

void Typechecker::visitVoid(Void *aVoid)
{
  typeS.push_front(BasicType::getVoid());
}

void Typechecker::visitEVar(EVar *evar)
{
  visitIdent(evar->ident_);
  typeS.push_front((*this->env)[identS.front()]);
  evar->typ = typeS.front();
  identS.pop_front();
}

void Typechecker::visitELitInt(ELitInt *elitint)
{
  //  visitInteger(elitint->integer_);
  typeS.push_front(BasicType::getInt());
  elitint->typ = typeS.front();
}

void Typechecker::visitELitTrue(ELitTrue *elittrue)
{
  typeS.push_front(BasicType::getBool());
  elittrue->typ = typeS.front();
}

void Typechecker::visitELitFalse(ELitFalse *elitfalse)
{
  typeS.push_front(BasicType::getBool());
  elitfalse->typ = typeS.front();
}

void Typechecker::visitEApp(EApp *eapp)
{
  visitIdent(eapp->ident_);
  Ident fun = identS.front();
  identS.pop_front();
  
  eapp->listexpr_->accept(this);
  
  FunctionType* funt = static_cast<FunctionType*>(&*(*this->env)[fun]);
  FunctionType::args_t args;
  for (int i = 0; i < eapp->listexpr_->size(); ++i){
    args.push_front(typeS.front());
    typeS.pop_front();
  }
  
  if (!funt->checkEq(args)){
    error("Function arguments mismatch", eapp->line_number);
  }
  typeS.push_front(funt->getRetType());
  eapp->typ = typeS.front();
}

void Typechecker::visitEString(EString *estring)
{
  typeS.push_front(BasicType::getString());
  estring->typ = typeS.front();
}

void Typechecker::visitNeg(Neg *neg)
{
  neg->expr_->accept(this);
  if (*typeS.front() != *BasicType::getInt()){
    error("Negating non-integer expression", neg->line_number);
  }
  neg->typ = typeS.front();
}

void Typechecker::visitNot(Not *aNot)
{
  /* Code For Not Goes Here */
  
  aNot->expr_->accept(this);
  if (*typeS.front() != *BasicType::getBool()){
    error("Flipping non-boolean expression", aNot->line_number);
  }
  aNot->typ = typeS.front();
}

void Typechecker::visitEMul(EMul *emul)
{
  /* Code For EMul Goes Here */
  
  emul->expr_1->accept(this);
  
  LType::ptr t = typeS.front();
  typeS.pop_front();
  
  emul->mulop_->accept(this);
  emul->expr_2->accept(this);
  
  if (*typeS.front() != *t || *t != *BasicType::getInt()){
    error("*, /, % only works for ints", emul->line_number);
  }
  emul->typ = typeS.front();
}

void Typechecker::visitEAdd(EAdd *eadd)
{
  /* Code For EAdd Goes Here */
  
  eadd->expr_1->accept(this);
  
  LType::ptr t = typeS.front();
  typeS.pop_front();
  
  eadd->addop_->accept(this);
  char myop = op;
  eadd->expr_2->accept(this);
  
  if (myop == '+'){
    if (*t != *typeS.front() || (*t != *BasicType::getInt() && *t != *BasicType::getString())){
      error("Invalid `+` operands", eadd->line_number);
    }
  }else if(myop == '-'){
    if (*t != *typeS.front() || *t != *BasicType::getInt()){
      error("Invalid `-` operands", eadd->line_number);
    }
  }
  eadd->typ = typeS.front();
}

void Typechecker::visitERel(ERel *erel)
{
  /* Code For ERel Goes Here */
  
  erel->expr_1->accept(this);
  
  LType::ptr t = typeS.front();
  typeS.pop_front();
  
  erel->relop_->accept(this);
  char myop = op;
  
  erel->expr_2->accept(this);
  
  if (myop == 'i'){
    if (*t != *typeS.front() || *t != *BasicType::getInt()){
      error("`<`, `<=`, `>`, `>=` only work with ints", erel->line_number);
    }
  }else if (myop == '=' || myop == '!'){
    if (*t != *typeS.front() || *t == *BasicType::getVoid()){
      error("Invalid `==` or `!=` parameters", erel->line_number);
    }
  }
  
  typeS.pop_front();
  typeS.push_front(BasicType::getBool());
  erel->typ = typeS.front();
}

void Typechecker::visitEAnd(EAnd *eand)
{
  /* Code For EAnd Goes Here */
  
  eand->expr_1->accept(this);
  
  LType::ptr t = typeS.front();
  typeS.pop_front();
  
  eand->expr_2->accept(this);
  
  if (*t != *typeS.front() || *t != *BasicType::getBool()){
    error("`&&` only works on boolean", eand->line_number);
  }
  eand->typ = typeS.front();
}

void Typechecker::visitEOr(EOr *eor)
{
  /* Code For EOr Goes Here */
  
  eor->expr_1->accept(this);
  
  LType::ptr t = typeS.front();
  typeS.pop_front();
  
  eor->expr_2->accept(this);
  
  if (*t != *typeS.front() || *t != *BasicType::getBool()){
    error("`||` only works on boolean", eor->line_number);
  }
  eor->typ = typeS.front();
}

void Typechecker::visitPlus(Plus *plus)
{
  op = '+';
}

void Typechecker::visitMinus(Minus *minus)
{
  op = '-';
}

void Typechecker::visitTimes(Times *times)
{
  op = 'i';
}

void Typechecker::visitDiv(Div *div)
{
  op = 'i';
}

void Typechecker::visitMod(Mod *mod)
{
  op = 'i';
}

void Typechecker::visitLTH(LTH *lth)
{
  op = 'i';
}

void Typechecker::visitLE(LE *le)
{
  op = 'i';
}

void Typechecker::visitGTH(GTH *gth)
{
  op = 'i';
}

void Typechecker::visitGE(GE *ge)
{
  op = 'i';
}

void Typechecker::visitEQU(EQU *equ)
{
  op = '=';
}

void Typechecker::visitNE(NE *ne)
{
  op = '!';
}


void Typechecker::visitListTopDef(ListTopDef* listtopdef)
{
  for (ListTopDef::iterator i = listtopdef->begin() ; i != listtopdef->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Typechecker::visitListArg(ListArg* listarg)
{
  for (ListArg::iterator i = listarg->begin() ; i != listarg->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Typechecker::visitListStmt(ListStmt* liststmt)
{
  bool broken = false;
  ListStmt::iterator i;
  
  for (i = liststmt->begin() ; i != liststmt->end() ; ++i)
  {
    (*i)->accept(this);
    if (returnchecker.returncheck(*i)){
      broken = true;
      break;
    }
    
    if (replaceStm != 0){
      delete *i;
      *i = replaceStm;
      replaceStm = 0;
    }
  }
  
  if (broken){
    ++i;
    liststmt->erase(i, liststmt->end());
  }
}

void Typechecker::visitListItem(ListItem* listitem)
{
  for (ListItem::iterator i = listitem->begin() ; i != listitem->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Typechecker::visitListExpr(ListExpr* listexpr)
{
  for (ListExpr::iterator i = listexpr->begin() ; i != listexpr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Typechecker::visitInteger(Integer x)
{
  throw "Not implemented";
}

void Typechecker::visitChar(Char x)
{
  throw "Not implemented";
}

void Typechecker::visitDouble(Double x)
{
  throw "Not implemented";
}

void Typechecker::visitString(String x)
{
  throw "Not implemented";
}

void Typechecker::visitIdent(Ident x)
{
  identS.push_front(x);
}

void Typechecker::visitAccess(Access *p){
  p->expr_->accept(this);
  
  if (*typeS.front() != *BasicType::getInt()){
    error("Array index must be integer", p->line_number);
  }
  p->expr_->typ = typeS.front();
  typeS.pop_front();
  
  LType::ptr typ = (*this->env)[p->ident_];
  if (*typ != *ArrayType::getAny()){
    error("tried to index access non-array variable", p->line_number);
  }
  LType::ptr inner = arrayTToInnerType(typ);
  arrayInner = inner;
}

void Typechecker::visitArrAss(ArrAss *p){
  p->acc_->accept(this);
  p->acc_->typ = arrayInner;
  
  p->expr_->accept(this);
  if (*p->acc_->typ != *typeS.front()){
    error("Array assignment of another type", p->line_number);
  }
  p->expr_->typ = typeS.front();
  typeS.pop_front();
}

void Typechecker::visitFor(For *p) {
  LType::ptr arrt = (*env)[p->ident_2];
  if (*arrt != *ArrayType::getAny()){
    error("Must iterate over an array", p->line_number);
  }
  
  p->type_->accept(this);
  if (*typeS.front() != *arrayTToInnerType(arrt)){
    error("Iterator of wrong type", p->line_number);
  }
  
  env = env->push();
  env->insert(p->ident_1, typeS.front());
  typeS.pop_front();
  
  p->stmt_->accept(this);
  
  env = env->pop();
}

void Typechecker::visitArray(Array *p){
  p->type_->accept(this);
  if (*typeS.front() == *BasicType::getVoid()){
    error("Cannot declare array of type void", p->line_number);
  }
  LType::ptr inner = typeS.front();
  if (*inner == *ArrayType::getAny()){
    error("cannot create multi dimensional array", p->line_number);
  }
  typeS.pop_front();
  typeS.push_front(ArrayType::getFromBasic(inner));
}

void Typechecker::visitELen(ELen *p){
  if (*(*env)[p->ident_] != *ArrayType::getAny()){
    error("Cannot get length of non-array", p->line_number);
  }
  typeS.push_front(BasicType::getInt());
  p->typ = typeS.front();
}

void Typechecker::visitEArrInit(EArrInit *p){
  p->type_->accept(this);
  
  p->expr_->accept(this);
  if(*typeS.front() != *BasicType::getInt()){
    error("Array length must be integer", p->line_number);
  }
  
  typeS.pop_front();
  
  LType::ptr inner = typeS.front();
  if (*inner == *ArrayType::getAny()){
    error("cannot create multi dimensional array", p->line_number);
  }
  
  typeS.pop_front();
  typeS.push_front(ArrayType::getFromBasic(inner));
}

void Typechecker::visitEGet(EGet *p){
  p->acc_->accept(this);
  p->acc_->typ = arrayInner;

  typeS.push_front(p->acc_->typ);
}
