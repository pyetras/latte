//
//  ArrayType.cpp
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "ArrayType.h"
#include <vector>
#include "llvm/ADT/ArrayRef.h"

LType::Type_t ArrayType::getType() const{
  return LType::TArray;
}

bool ArrayType::operator==(const LType& other) const{
  if (other.getType() == LType::TArray){
    const ArrayType *t = static_cast<const ArrayType*>(&other);
    return t->type == this->type || t->type == BasicType::TVoid || this->type == BasicType::TVoid;
  }else{
    return false;
  }
}

LType::ptr ArrayType::getBool(){
  return LType::ptr(new ArrayType(BasicType::TBool));
}

LType::ptr ArrayType::getInt(){
  return LType::ptr(new ArrayType(BasicType::TInt));
}

LType::ptr ArrayType::getString(){
  return LType::ptr(new ArrayType(BasicType::TString));
}

LType::ptr ArrayType::getAny(){
  return LType::ptr(new ArrayType(BasicType::TVoid));
}

LType::ptr ArrayType::getFromBasic(LType::ptr typ){
  BasicType *t = static_cast<BasicType*>(&*typ);
  return LType::ptr(new ArrayType(t->type));
}

ArrayType::operator llvm::Type*() const{
  std::vector<llvm::Type*> members;
  members.push_back(llvm::Type::getInt64Ty(llvm::getGlobalContext()));
  llvm::Type *typ;
  switch (type) {
    case BasicType::TBool:
      typ = llvm::Type::getInt1Ty(llvm::getGlobalContext());
      break;
    case BasicType::TInt:
      typ = llvm::Type::getInt64Ty(llvm::getGlobalContext());
      break;
    case BasicType::TString:
      typ = llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
      break;
    case BasicType::TVoid:
    default:
      throw "not implemented";
  }
  members.push_back(typ->getPointerTo());
  
  return llvm::StructType::get(llvm::getGlobalContext(), members);
}

std::string ArrayType::toJVMString(bool lowercase) const{
  std::string c;
  switch (type) {
    case BasicType::TBool:
    case BasicType::TInt:
      c = "i";
      break;
      
    case BasicType::TString:
      if (lowercase) return "a";
      else return "Ljava/lang/String;";
      break;
    case BasicType::TVoid:
      c = "v";
      break;
  }
  
  if (!lowercase) *c.begin() = toupper(*c.begin());
  return c;
}

LType::ptr arrayTToInnerType(LType::ptr t){
  ArrayType *arr = static_cast<ArrayType*>(&*t);
  return LType::ptr(new BasicType(arr->type));
}
