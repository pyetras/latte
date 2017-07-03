//
//  JVMCompiler.cpp
//  latte
//
//  Created by Piotr Sokólski on 13.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "JVMCompiler.h"
#include "FunctionType.h"

std::string JVMCompiler::compile(Visitable* v, const std::string& name){
  this ->name = name;
  code += ".class public " +name+ "\n";
  code +=".super java/lang/Object\n\n";
  code +=".method public static main([Ljava/lang/String;)V\n";
  code +=".limit stack 1\n";
  code +=".limit locals 1\n";
    
  code +="invokestatic " +name + "/latte_main()I\n";
  code +="invokestatic java/lang/System/exit(I)V\n";
  code +="return\n";
  code +=".end method\n\n";
  v->accept(this);
  return code;
}

void JVMCompiler::visitAProgram(AProgram *aprogram)
{
  /* Code For AProgram Goes Here */
  
  aprogram->listtopdef_->accept(this);
  while (!funBodies.empty()) {
    state = funBodies.top().first;
    funBodies.top().second->accept(this);
    code += state->getCode();
    code += ".end method\n\n";
    delete state;
    funBodies.pop();
  }
}

void JVMCompiler::visitFnDef(FnDef *fndef)
{
  /* Code For FnDef Goes Here */
  
  fndef->type_->accept(this);
  Ident fname = fndef->ident_;

  FunctionType* ftyp = static_cast<FunctionType*>(&*fndef->funtyp);
  JVMState *s = new JVMState(&rootEnv, ftyp->getRetType(), ftyp, name);
  JVMState *oldstate = state;
  state = s;

  funArgs = ftyp->arguments;
  fndef->listarg_->accept(this);
  
  std::string code = ".method public static latte_"  + fname;
  std::string sig = ftyp->toJVMString(true);
  code += sig;
  s->signature = code;
  
  state->registerFun(fname, fndef->funtyp);
  funBodies.push(std::make_pair(s, fndef->block_));
      
  state = oldstate;
}

void JVMCompiler::visitAnArg(AnArg *anarg)
{
  this->state->registerVar(anarg->ident_, funArgs.front());
  funArgs.pop_front();
}

void JVMCompiler::visitABlock(ABlock *ablock)
{
  ablock->liststmt_->accept(this);
}

void JVMCompiler::visitEmpty(Empty *empty)
{

}

void JVMCompiler::visitBStmt(BStmt *bstmt)
{
  this->state->pushBlock();
  bstmt->block_->accept(this);
  this->state->popBlock();
}

void JVMCompiler::visitDecl(Decl *decl)
{
  decl->type_->accept(this);
  //declTyp
  decl->listitem_->accept(this);
}

void JVMCompiler::visitNoInit(NoInit *noinit)
{
  state->registerVar(noinit->ident_, declTyp);
  if (*declTyp == *BasicType::getString()){
    state->pushString("");
  }else{
    state->pushInt(0);
  }
  state->storeVar(noinit->ident_);
}

void JVMCompiler::visitInit(Init *init)
{
  init->expr_->accept(this);
  state->registerVar(init->ident_, declTyp);
  state->storeVar(init->ident_);  
}


void JVMCompiler::visitAss(Ass *ass)
{
  Ident id = ass->ident_;
  
  ass->expr_->accept(this);
  state->storeVar(id);
}

void JVMCompiler::visitIncr(Incr *incr)
{
  state->inc(incr->ident_);
}

void JVMCompiler::visitDecr(Decr *decr)
{
  state->dec(decr->ident_);
}

void JVMCompiler::visitRet(Ret *ret)
{
  ret->expr_->accept(this);
  state->ret();
}

void JVMCompiler::visitVRet(VRet *vret)
{
  state->ret();
}

void JVMCompiler::visitCond(Cond *cond)
{
  cond->expr_->accept(this);
  state->condIf(cond->stmt_, this);
}

void JVMCompiler::visitCondElse(CondElse *condelse)
{
  condelse->expr_->accept(this);
  state->condIfElse(condelse->stmt_1, condelse->stmt_2, this);
}

void JVMCompiler::visitWhile(While *aWhile)
{
  state->whileLoop(aWhile->expr_, aWhile->stmt_, this);
}

void JVMCompiler::visitSExp(SExp *sexp)
{
  sexp->expr_->accept(this);
  if (*sexp->expr_->typ != *BasicType::getVoid())
    state->pop();
}

void JVMCompiler::visitInt(Int *anInt)
{
  declTyp = BasicType::getInt();
}

void JVMCompiler::visitStr(Str *str)
{
  declTyp = BasicType::getString();
}

void JVMCompiler::visitBool(Bool *aBool)
{
  declTyp = BasicType::getBool();
}

void JVMCompiler::visitVoid(Void *aVoid)
{
  declTyp = BasicType::getVoid();
}

void JVMCompiler::visitFun(Fun *fun)
{
  throw "Not implemented";
}

void JVMCompiler::visitEVar(EVar *evar)
{  
  state->pushVar(evar->ident_);  
}

void JVMCompiler::visitELitInt(ELitInt *elitint)
{
  state->pushInt(elitint->integer_);
}

void JVMCompiler::visitELitTrue(ELitTrue *elittrue)
{
  state->pushBool(true);
}

void JVMCompiler::visitELitFalse(ELitFalse *elitfalse)
{
  state->pushBool(false);
}

void JVMCompiler::visitEApp(EApp *eapp)
{
  Ident fun = eapp->ident_;
  
  eapp->listexpr_->accept(this);
  
  state->callFun(fun);
}

void JVMCompiler::visitEString(EString *estring)
{
  state->pushString(estring->string_);
}

void JVMCompiler::visitNeg(Neg *neg)
{
  neg->expr_->accept(this);
  state->neg();  
}

void JVMCompiler::visitNot(Not *aNot)
{
  aNot->expr_->accept(this);
  state->noot();
}

void JVMCompiler::visitEMul(EMul *emul)
{
  emul->expr_1->accept(this);
  emul->expr_2->accept(this);
  
  emul->mulop_->accept(this);

  switch (op) {
    case OP::Mul:
      state->mul();
      break;
    case OP::Div:
      state->div();
      break;
    case OP::Mod:
      state->rem();
      break;
    default: ;
      break;
  }
}

void JVMCompiler::visitEAdd(EAdd *eadd)
{
  eadd->addop_->accept(this);
  OP::operator_t myop = op;
  
  if (myop == OP::Plus){
    if (*eadd->expr_1->typ == *BasicType::getInt()){
      eadd->expr_1->accept(this);
      eadd->expr_2->accept(this);
      state->add();
    }else{
      state->addStrings(eadd->expr_1, eadd->expr_2, this);
    }
  }else if(myop == OP::Minus){
    eadd->expr_1->accept(this);
    eadd->expr_2->accept(this);
    state->sub();
  }
  
}

void JVMCompiler::visitERel(ERel *erel)
{
  erel->expr_1->accept(this);
  erel->expr_2->accept(this);
  
  erel->relop_->accept(this);
  OP::operator_t myop = op;
  if ((myop == OP::EQ || myop == OP::NEQ) && *erel->expr_1->typ == *BasicType::getString()){
    if (myop == OP::EQ){
      state->eqStr();
    }else{
      state->neqStr();
    }
  }else{
    state->irel(op);
  }    
}

void JVMCompiler::visitEAnd(EAnd *eand)
{
  eand->expr_1->accept(this);
  state->band(eand->expr_2, this);
}

void JVMCompiler::visitEOr(EOr *eor)
{
  eor->expr_1->accept(this);
  state->bor(eor->expr_2, this);
}

void JVMCompiler::visitPlus(Plus *plus)
{
  op = OP::Plus;
}

void JVMCompiler::visitMinus(Minus *minus)
{
  op = OP::Minus;
}

void JVMCompiler::visitTimes(Times *times)
{
  op = OP::Mul;
}

void JVMCompiler::visitDiv(Div *div)
{
  op = OP::Div;
}

void JVMCompiler::visitMod(Mod *mod)
{
  op = OP::Mod;
}

void JVMCompiler::visitLTH(LTH *lth)
{
  op = OP::LT;
}

void JVMCompiler::visitLE(LE *le)
{
  op = OP::LTE;
}

void JVMCompiler::visitGTH(GTH *gth)
{
  op = OP::GT;
}

void JVMCompiler::visitGE(GE *ge)
{
  op = OP::GTE;
}

void JVMCompiler::visitEQU(EQU *equ)
{
  op = OP::EQ;
}

void JVMCompiler::visitNE(NE *ne)
{
  op = OP::NEQ;
}
