//
//  LLVMCompiler.cpp
//  latte
//
//  Created by Piotr Sokólski on 13.12.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "LLVMCompiler.h"
#include "FunctionType.h"
#include "RootEnvironment.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <stack>
#include "llvm/Support/raw_ostream.h"
#include "llvm/Linker.h"
#include "llvm/Support/PathV1.h"
#include "Returnchecker.h"
#include "llvm/PassManager.h"
#include "llvm/LLVMContext.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/DerivedTypes.h"
#include "ArrayType.h"
#include "llvm/Support/TypeBuilder.h"
#include "llvm/Instruction.h"

static void trimToTerminator(llvm::BasicBlock *block){
  std::stack<llvm::Instruction*> toDelete;
  bool hasTerminator = false;
  for (llvm::BasicBlock::iterator it = block->begin(); it != block->end(); ++it) {
    llvm::Instruction *i = it;
    if (hasTerminator){
      toDelete.push(i);
    }else{
      hasTerminator = i->isTerminator();
    }
  }
  
  while(!toDelete.empty()){
    toDelete.top()->eraseFromParent();
    toDelete.pop();
  }  
}

std::string LLVMCompiler::compile(Visitable* v, const std::string& name){
  this ->name = name;
  
  module = new llvm::Module(name, llvm::getGlobalContext());
  env = new RootEnvironment();
  
  v->accept(this);

  llvm::verifyModule(*module);
  
  llvm::FunctionPassManager fpm(module);
  fpm.add(llvm::createBasicAliasAnalysisPass());
  fpm.add(llvm::createInstructionCombiningPass());
  fpm.add(llvm::createReassociatePass());
  fpm.add(llvm::createGVNPass());
  fpm.add(llvm::createCFGSimplificationPass());
  fpm.add(llvm::createPromoteMemoryToRegisterPass());
  
  fpm.doInitialization();

  for(llvm::Module::iterator it = module->begin(); it != module->end(); ++it){
    fpm.run(*it);
  }
  
//  llvm::Linker linker(name, module);
//  llvm::sys::Path path("/Users/Pietras/Documents/studia/12:13/mrj/compiler/lib/runtime.bc");
//  bool isNative = true;
//  linker.LinkInFile(path, isNative);
//  
//  module = linker.getModule();
  
  std::string s;
  llvm::raw_string_ostream ss(s);
  module->print(ss, 0);
  delete env;
  delete module;
  
  return ss.str();
}

void LLVMCompiler::visitAProgram(AProgram *aprogram)
{
  /* Code For AProgram Goes Here */
  
  aprogram->listtopdef_->accept(this);
  while (!funBodies.empty()) {
    builder = funBodies.top().second.first;
    env = funBodies.top().second.second;
    
    funBodies.top().first->accept(this);
        
    llvm::Function *fun = builder->GetInsertBlock()->getParent();
    for (llvm::Function::iterator it = fun->begin(); it != fun->end(); ++it){
      trimToTerminator(it);
    }
    
    if (!builder->GetInsertBlock()->getTerminator() && *env->getRetType() != *BasicType::getVoid()){
      builder->GetInsertBlock()->eraseFromParent();
    }else if(!builder->GetInsertBlock()->getTerminator()){
      builder->CreateRetVoid();
    }
    
    delete builder;
    env = static_cast<LLVMEnvironment*>(env->pop());
    
    funBodies.pop();
  }
}

void LLVMCompiler::visitFnDef(FnDef *fndef)
{
  /* Code For FnDef Goes Here */
  
  fndef->type_->accept(this);
  Ident fname = fndef->ident_;

  FunctionType* ftyp = static_cast<FunctionType*>(&*fndef->funtyp);
  
  builder = new llvm::IRBuilder<>(llvm::getGlobalContext());
  
  llvm::Function* func = llvm::Function::Create(*ftyp, llvm::Function::ExternalLinkage, fname, module);
  llvm::BasicBlock* entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
  builder->SetInsertPoint(entry);
  
  funArgs = ftyp->arguments.begin();
  fargIt = func->arg_begin();
  LLVMEnvironment* oldenv = env;
  
  env = env->pushFun(func, ftyp->getRetType());
  
  fndef->listarg_->accept(this);
    
  funBodies.push(std::make_pair(fndef->block_, std::make_pair(builder, env)));
  env = oldenv;
  env->Environment::insert(fname, fndef->funtyp);
}

void LLVMCompiler::visitAnArg(AnArg *anarg)
{
  env->insert(anarg->ident_, *funArgs);
  builder->CreateStore(fargIt, (*env)(anarg->ident_).first);
  funArgs++;
  fargIt++;
}

void LLVMCompiler::visitABlock(ABlock *ablock)
{
  ablock->liststmt_->accept(this);
}

void LLVMCompiler::visitEmpty(Empty *empty)
{

}

void LLVMCompiler::visitBStmt(BStmt *bstmt)
{
  env = static_cast<LLVMEnvironment*>(env->pushBlock());
  bstmt->block_->accept(this);
  env = static_cast<LLVMEnvironment*>(env->pop());
}

void LLVMCompiler::visitDecl(Decl *decl)
{
  decl->type_->accept(this);
  //declTyp
  decl->listitem_->accept(this);
}

void LLVMCompiler::visitNoInit(NoInit *noinit)
{
  env->insert(noinit->ident_, declTyp);
  llvm::AllocaInst* alloca = (*env)(noinit->ident_).first;
  
  if (*declTyp == *BasicType::getString()){
    builder->CreateStore(builder->CreateGlobalStringPtr(""), alloca);
  }else if(*declTyp == *BasicType::getInt()){
    builder->CreateStore(llvm::ConstantInt::get(*BasicType::getInt(), 0), alloca);
  }else if(*declTyp == *BasicType::getBool()){
    builder->CreateStore(llvm::ConstantInt::getFalse(llvm::getGlobalContext()), alloca);
  }
}

void LLVMCompiler::visitInit(Init *init)
{
  init->expr_->accept(this);
  env->insert(init->ident_, declTyp);
  builder->CreateStore(curValue, (*env)(init->ident_).first);
}

void LLVMCompiler::visitAss(Ass *ass)
{
  Ident id = ass->ident_;
  
  ass->expr_->accept(this);
  builder->CreateStore(curValue, (*env)(id).first);
}

void LLVMCompiler::visitIncr(Incr *incr)
{
  llvm::AllocaInst* alloca = (*env)(incr->ident_).first;
  llvm::Value* var = builder->CreateLoad(alloca);
  llvm::Value* inc = builder->CreateAdd(var, llvm::ConstantInt::get(*BasicType::getInt(), 1));
  builder->CreateStore(inc, alloca);
}

void LLVMCompiler::visitDecr(Decr *decr)
{
  llvm::AllocaInst* alloca = (*env)(decr->ident_).first;
  llvm::Value* var = builder->CreateLoad(alloca);
  llvm::Value* dec = builder->CreateSub(var, llvm::ConstantInt::get(*BasicType::getInt(), 1));
  builder->CreateStore(dec, alloca);
}

void LLVMCompiler::visitRet(Ret *ret)
{
  ret->expr_->accept(this);
  builder->CreateRet(curValue);
}

void LLVMCompiler::visitVRet(VRet *vret)
{
  builder->CreateRetVoid();
}

void LLVMCompiler::visitCond(Cond *cond)
{
  cond->expr_->accept(this);
  
  llvm::Function *function = builder->GetInsertBlock()->getParent();
  
  llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", function);
  llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merge");
  
  builder->CreateCondBr(curValue, thenBlock, mergeBlock);
  builder->SetInsertPoint(thenBlock);
    
  cond->stmt_->accept(this);
    
  builder->CreateBr(mergeBlock);
  
  function->getBasicBlockList().push_back(mergeBlock);
  builder->SetInsertPoint(mergeBlock);
}

void LLVMCompiler::visitCondElse(CondElse *condelse)
{
  condelse->expr_->accept(this);
  llvm::Function *function = builder->GetInsertBlock()->getParent();
  
  llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "then", function);
  llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "else");
  llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merge");
  
  builder->CreateCondBr(curValue, thenBlock, elseBlock);
  builder->SetInsertPoint(thenBlock);
  
  condelse->stmt_1->accept(this);
  
  Returnchecker retchk;
  
  bool lret, rret;
  if (!(lret = retchk.returncheck(condelse->stmt_1)))
    builder->CreateBr(mergeBlock);
      
  function->getBasicBlockList().push_back(elseBlock);
  builder->SetInsertPoint(elseBlock);
  
  condelse->stmt_2->accept(this);
  
  if (!(rret = retchk.returncheck(condelse->stmt_2)))
    builder->CreateBr(mergeBlock);
  
  if (!lret || !rret){
    function->getBasicBlockList().push_back(mergeBlock);
    builder->SetInsertPoint(mergeBlock);
  }
}

void LLVMCompiler::visitWhile(While *aWhile)
{
  llvm::Function *function = builder->GetInsertBlock()->getParent();
  
  llvm::BasicBlock *condBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "startwhile", function);
  
  builder->CreateBr(condBlock);
  builder->SetInsertPoint(condBlock);
  
  aWhile->expr_->accept(this);
  
  llvm::BasicBlock *bodyBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "while");
  
  llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(llvm::getGlobalContext(), "endwhile");
  
  builder->CreateCondBr(curValue, bodyBlock, mergeBlock);
  
  function->getBasicBlockList().push_back(bodyBlock);
  builder->SetInsertPoint(bodyBlock);
  
  aWhile->stmt_->accept(this);
  
  builder->CreateBr(condBlock);
  
  function->getBasicBlockList().push_back(mergeBlock);
  builder->SetInsertPoint(mergeBlock);
}

void LLVMCompiler::visitSExp(SExp *sexp)
{
  sexp->expr_->accept(this);
  curValue = 0;
}

void LLVMCompiler::visitInt(Int *anInt)
{
  declTyp = BasicType::getInt();
}

void LLVMCompiler::visitStr(Str *str)
{
  declTyp = BasicType::getString();
}

void LLVMCompiler::visitBool(Bool *aBool)
{
  declTyp = BasicType::getBool();
}

void LLVMCompiler::visitVoid(Void *aVoid)
{
  declTyp = BasicType::getVoid();
}

void LLVMCompiler::visitEVar(EVar *evar)
{  
  curValue = builder->CreateLoad((*env)(evar->ident_).first);
}

void LLVMCompiler::visitELitInt(ELitInt *elitint)
{
  curValue = llvm::ConstantInt::get(*BasicType::getInt(), elitint->integer_);
}

void LLVMCompiler::visitELitTrue(ELitTrue *elittrue)
{
  curValue = llvm::ConstantInt::getTrue(llvm::getGlobalContext());
}

void LLVMCompiler::visitELitFalse(ELitFalse *elitfalse)
{
  curValue = llvm::ConstantInt::getFalse(llvm::getGlobalContext());
}

void LLVMCompiler::visitEApp(EApp *eapp)
{
  Ident fun = eapp->ident_;
  llvm::FunctionType *typ = *static_cast<FunctionType*>(&*(*env)[fun]);
  
  llvm::Constant *function = module->getOrInsertFunction(fun, typ);
  std::vector<llvm::Value*> args;
  for(ListExpr::iterator it = eapp->listexpr_->begin(); it != eapp->listexpr_->end(); ++it){
    (*it)->accept(this);
    args.push_back(curValue);
  }
  
  curValue = builder->CreateCall(function, args);
}

void LLVMCompiler::visitEString(EString *estring)
{
  curValue = builder->CreateGlobalStringPtr(estring->string_);
}

void LLVMCompiler::visitNeg(Neg *neg)
{
  neg->expr_->accept(this);
  curValue = builder->CreateNeg(curValue);
}

void LLVMCompiler::visitNot(Not *aNot)
{
  aNot->expr_->accept(this);
  curValue = builder->CreateNot(curValue);
}

void LLVMCompiler::visitEMul(EMul *emul)
{
  emul->expr_1->accept(this);
  llvm::Value* val1 = curValue;
  
  emul->expr_2->accept(this);
  llvm::Value* val2 = curValue;
  
  emul->mulop_->accept(this);

  switch (op) {
    case OP::Mul:
      curValue = builder->CreateMul(val1, val2);
      break;
    case OP::Div:
      curValue = builder->CreateSDiv(val1, val2);
      break;
    case OP::Mod:
      curValue = builder->CreateSRem(val1, val2);
      break;
    default: ;
      break;
  }
}

void LLVMCompiler::visitEAdd(EAdd *eadd)
{
  eadd->addop_->accept(this);
  OP::operator_t myop = op;
  
  eadd->expr_1->accept(this);
  llvm::Value* val1 = curValue;

  eadd->expr_2->accept(this);
  llvm::Value* val2 = curValue;

  if (myop == OP::Plus){
    if (*eadd->expr_1->typ == *BasicType::getInt()){
      curValue = builder->CreateAdd(val1, val2);
    }else{
      std::vector<llvm::Type*> args(2, *BasicType::getString());
      llvm::FunctionType *concatType = llvm::FunctionType::get(*BasicType::getString(), args, false);
      llvm::Constant *concat = module->getOrInsertFunction("_concat", concatType);
      curValue = builder->CreateCall2(concat, val1, val2);
    }
  }else if(myop == OP::Minus){
    curValue = builder->CreateSub(val1, val2);
  }
  
}

void LLVMCompiler::visitERel(ERel *erel)
{
  erel->expr_1->accept(this);
  llvm::Value* val1 = curValue;

  erel->expr_2->accept(this);
  llvm::Value* val2 = curValue;

  erel->relop_->accept(this);
  OP::operator_t myop = op;
  if ((myop == OP::EQ || myop == OP::NEQ) && *erel->expr_1->typ == *BasicType::getString()){
    std::vector<llvm::Type*> args(2, *BasicType::getString());
    llvm::FunctionType *concatType = llvm::FunctionType::get(builder->getInt32Ty(), args, false);
    llvm::Constant *strcmp = module->getOrInsertFunction("_strcmp", concatType);
    curValue = builder->CreateCall2(strcmp, val1, val2);
    if (myop == OP::EQ){
      curValue = builder->CreateICmpEQ(curValue, builder->getInt32(1));
    }else{
      curValue = builder->CreateICmpEQ(curValue, builder->getInt32(0));
    }
  }else{
    switch (op) {
      case OP::EQ:
        curValue = builder->CreateICmpEQ(val1, val2);
        break;
      case OP::NEQ:
        curValue = builder->CreateICmpNE(val1, val2);
        break;
      case OP::LT:
        curValue = builder->CreateICmpSLT(val1, val2);
        break;
      case OP::LTE:
        curValue = builder->CreateICmpSLE(val1, val2);
        break;
      case OP::GT:
        curValue = builder->CreateICmpSGT(val1, val2);
        break;
      case OP::GTE:
        curValue = builder->CreateICmpSGE(val1, val2);
        break;
      default:
        break;
    }
  }    
}

void LLVMCompiler::visitEAnd(EAnd *eand)
{
  eand->expr_1->accept(this);
  llvm::Value* val1 = curValue;
    
  llvm::BasicBlock* isTrueB =llvm::BasicBlock::Create(llvm::getGlobalContext(), "isTrue", builder->GetInsertBlock()->getParent());
  llvm::BasicBlock* isFalseB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "isFalse");
  llvm::BasicBlock* mergeB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merge");
  
  builder->CreateCondBr(val1, isTrueB, isFalseB);
  
  builder->SetInsertPoint(isTrueB);

  eand->expr_2->accept(this);
  llvm::Value* val2 = curValue;

  llvm::Value * trueVal = val2;
  
  builder->CreateBr(mergeB);
  
  isTrueB = builder->GetInsertBlock();
  
  builder->GetInsertBlock()->getParent()->getBasicBlockList().push_back(isFalseB);
  builder->SetInsertPoint(isFalseB);
  
  llvm::Value* falseVal = builder->getInt1(false);
  
  builder->CreateBr(mergeB);
  
  builder->GetInsertBlock()->getParent()->getBasicBlockList().push_back(mergeB);
  builder->SetInsertPoint(mergeB);
  
  llvm::PHINode *phi = builder->CreatePHI(*BasicType::getBool(), 2);
  phi->addIncoming(trueVal, isTrueB);
  phi->addIncoming(falseVal, isFalseB);
  
  curValue = phi;
}

void LLVMCompiler::visitEOr(EOr *eor)
{
  eor->expr_1->accept(this);
  llvm::Value* val1 = curValue;
  
  llvm::BasicBlock* isTrueB =llvm::BasicBlock::Create(llvm::getGlobalContext(), "isTrue", builder->GetInsertBlock()->getParent());
  llvm::BasicBlock* isFalseB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "isFalse");
  llvm::BasicBlock* mergeB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merge");
  
  builder->CreateCondBr(val1, isTrueB, isFalseB);
  
  builder->SetInsertPoint(isTrueB);
  
  llvm::Value * trueVal = builder->getInt1(true);
  
  builder->CreateBr(mergeB);
  
  builder->GetInsertBlock()->getParent()->getBasicBlockList().push_back(isFalseB);
  builder->SetInsertPoint(isFalseB);
  
  eor->expr_2->accept(this);
  llvm::Value* val2 = curValue;

  llvm::Value* falseVal = val2;
  
  builder->CreateBr(mergeB);
  
  isFalseB = builder->GetInsertBlock();
  
  builder->GetInsertBlock()->getParent()->getBasicBlockList().push_back(mergeB);
  builder->SetInsertPoint(mergeB);
  
  llvm::PHINode *phi = builder->CreatePHI(*BasicType::getBool(), 2);
  phi->addIncoming(trueVal, isTrueB);
  phi->addIncoming(falseVal, isFalseB);
  
  curValue = phi;
}

void LLVMCompiler::visitPlus(Plus *plus)
{
  op = OP::Plus;
}

void LLVMCompiler::visitMinus(Minus *minus)
{
  op = OP::Minus;
}

void LLVMCompiler::visitTimes(Times *times)
{
  op = OP::Mul;
}

void LLVMCompiler::visitDiv(Div *div)
{
  op = OP::Div;
}

void LLVMCompiler::visitMod(Mod *mod)
{
  op = OP::Mod;
}

void LLVMCompiler::visitLTH(LTH *lth)
{
  op = OP::LT;
}

void LLVMCompiler::visitLE(LE *le)
{
  op = OP::LTE;
}

void LLVMCompiler::visitGTH(GTH *gth)
{
  op = OP::GT;
}

void LLVMCompiler::visitGE(GE *ge)
{
  op = OP::GTE;
}

void LLVMCompiler::visitEQU(EQU *equ)
{
  op = OP::EQ;
}

void LLVMCompiler::visitNE(NE *ne)
{
  op = OP::NEQ;
}

void LLVMCompiler::visitArray(Array *p)
{
  p->type_->accept(this);
  
  declTyp = ArrayType::getFromBasic(declTyp);
}

void LLVMCompiler::visitAccess(Access *p){
  p->expr_->accept(this);
  
  llvm::Value *ix = curValue;
  std::vector<llvm::Value *> idx;
  idx.push_back(builder->getInt32(0));
  idx.push_back(builder->getInt32(1));
  
  llvm::Value* arrPtr = builder->CreateInBoundsGEP((*env)(p->ident_).first, idx);
  
  std::vector<llvm::Value*> idx2;
  idx2.push_back(builder->getInt32(0));
  idx2.push_back(ix);
  
  llvm::Value* elemPtr = builder->CreateInBoundsGEP(builder->CreateLoad(arrPtr), ix);
  
  curValue = elemPtr;
}

void LLVMCompiler::storeAtIndex(llvm::Value *ix, Ident ident_1, Ident ident_2){
  std::vector<llvm::Value *> idx;
  idx.push_back(builder->getInt32(0));
  idx.push_back(builder->getInt32(1));
  
  llvm::Value* arrPtr = builder->CreateInBoundsGEP((*env)(ident_2).first, idx);
  
  llvm::Value* elemPtr = builder->CreateInBoundsGEP(builder->CreateLoad(arrPtr), ix);
  
  
  builder->CreateStore(builder->CreateLoad(elemPtr), (*env)(ident_1).first);

}

void LLVMCompiler::visitArrAss(ArrAss *p){
  p->acc_->accept(this);
  llvm::Value* elemPtr = curValue;
  
  p->expr_->accept(this);
  builder->CreateStore(curValue, elemPtr);
}

void LLVMCompiler::visitFor(For *p) {
  llvm::Function *function = builder->GetInsertBlock()->getParent();
  LType::ptr arrT = (*env)[p->ident_2];
  LType::ptr inner = arrayTToInnerType(arrT);
  
  std::vector<llvm::Value*> idx;
  idx.push_back(builder->getInt32(0));
  idx.push_back(builder->getInt32(0));
  llvm::Value* lenptr = builder->CreateInBoundsGEP((*env)(p->ident_2).first, idx);
  llvm::Value* length = builder->CreateLoad(lenptr);
  
  llvm::IRBuilder<> bldr(&function->getEntryBlock(), function->getEntryBlock().begin());
  llvm::AllocaInst *iterator = bldr.CreateAlloca(*BasicType::getInt(), 0);
  builder->CreateStore(builder->getInt64(0), iterator);
  
  env = static_cast<LLVMEnvironment*>(env->pushBlock());
  env->insert(p->ident_1, inner);
  llvm::BasicBlock *preLoopB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "preFor", function);
  builder->CreateBr(preLoopB);
  
  llvm::BasicBlock *loopB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "for");
  llvm::BasicBlock *mergeB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "merge");
  
  builder->SetInsertPoint(preLoopB);
  
  builder->CreateCondBr(builder->CreateICmpSLT(builder->CreateLoad(iterator), length), loopB, mergeB);

  function->getBasicBlockList().push_back(loopB);
  builder->SetInsertPoint(loopB);
  
  storeAtIndex(builder->CreateLoad(iterator), p->ident_1, p->ident_2);
  
  p->stmt_->accept(this);
  
  builder->CreateStore(builder->CreateAdd(builder->CreateLoad(iterator), builder->getInt64(1)), iterator);
  
  builder->CreateBr(preLoopB);
  
  function->getBasicBlockList().push_back(mergeB);
  builder->SetInsertPoint(mergeB);
  
  env = static_cast<LLVMEnvironment*>(env->pop());
//  LType::ptr arrt = (*env)[p->ident_2];
//  if (*arrt != *ArrayType::getAny()){
//    error("Must iterate over an array", p->line_number);
//  }
//  
//  p->type_->accept(this);
//  if (*typeS.front() != *arrayTToInnerType(arrt)){
//    error("Iterator of wrong type", p->line_number);
//  }
//  
//  env = env->push();
//  env->insert(p->ident_1, typeS.front());
//  typeS.pop_front();
//  
//  p->stmt_->accept(this);
//  
//  env = env->pop();
}

void LLVMCompiler::visitELen(ELen *p){
  std::vector<llvm::Value*> idx;
  idx.push_back(builder->getInt32(0));
  idx.push_back(builder->getInt32(0));
  llvm::Value* lenptr = builder->CreateInBoundsGEP((*env)(p->ident_).first, idx);
  curValue = builder->CreateLoad(lenptr);
}

void LLVMCompiler::visitEArrInit(EArrInit *p){
  p->expr_->accept(this);
  llvm::Value *length = curValue;
  
  std::vector<llvm::Value*> idx;
  idx.push_back(builder->getInt32(0));
  idx.push_back(builder->getInt32(0));
  
  llvm::Value* struc = builder->CreateAlloca(*p->typ);

  llvm::Value* lenPtr = builder->CreateInBoundsGEP(struc, idx);
  builder->CreateStore(length, lenPtr);
  
  std::vector<llvm::Value*> idx2;
  idx2.push_back(builder->getInt32(0));
  idx2.push_back(builder->getInt32(1));
  llvm::Value* arrPtr = builder->CreateInBoundsGEP(struc, idx2);
  
  std::vector<llvm::Value*> idx3;
  idx3.push_back(builder->getInt32(0));
  idx3.push_back(length);
  LType::ptr inner = arrayTToInnerType(p->typ);

  llvm::Value* size = builder->CreateInBoundsGEP(llvm::Constant::getNullValue(llvm::ArrayType::get(*inner, 0)->getPointerTo()), idx3);
  
  llvm::Value* sizeU = builder->CreateCast(llvm::Instruction::PtrToInt, size, builder->getInt64Ty());
  
  std::vector<llvm::Type*> args;
  args.push_back(builder->getInt64Ty());
  llvm::FunctionType *mallocT = llvm::FunctionType::get(builder->getInt8PtrTy(), args, false);
  llvm::Constant *malloc = module->getOrInsertFunction("malloc", mallocT);
  llvm::Value* arrayByte = builder->CreateCall(malloc, sizeU);
  llvm::Value* array = builder->CreateCast(llvm::Instruction::BitCast, arrayByte, arrPtr->getType()->getPointerElementType());

  builder->CreateStore(array, arrPtr);
  
  curValue = builder->CreateLoad(struc);
}

void LLVMCompiler::visitEGet(EGet *p){
  p->acc_->accept(this);
  curValue = builder->CreateLoad(curValue);
}

