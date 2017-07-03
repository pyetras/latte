//
//  BasicType.cpp
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "BasicType.h"

LType::Type_t BasicType::getType() const{
  return LType::TBasicType;
}

bool BasicType::operator==(const LType& other) const{
  if (other.getType() == LType::TBasicType){
    const BasicType *t = static_cast<const BasicType*>(&other);
    return t->type == this->type;
  }else{
    return false;
  }
}

LType::ptr BasicType::getBool(){
  return LType::ptr(new BasicType(TBool));
}

LType::ptr BasicType::getInt(){
  return LType::ptr(new BasicType(TInt));
}

LType::ptr BasicType::getString(){
  return LType::ptr(new BasicType(TString));
}

LType::ptr BasicType::getVoid(){
  return LType::ptr(new BasicType(TVoid));
}

BasicType::operator llvm::Type*() const{
  switch (type) {
    case TBool:
      return llvm::Type::getInt1Ty(llvm::getGlobalContext());
    case TInt:
      return llvm::Type::getInt64Ty(llvm::getGlobalContext());
    case TString:
      return llvm::Type::getInt8PtrTy(llvm::getGlobalContext());
    case TVoid:
      return llvm::Type::getVoidTy(llvm::getGlobalContext());
  }
}

std::string BasicType::toJVMString(bool lowercase) const{
  std::string c;
  switch (type) {
    case TBool:
    case TInt:
      c = "i";
      break;
      
    case TString:
      if (lowercase) return "a";
      else return "Ljava/lang/String;";
      break;
    case TVoid:
      c = "v";
      break;
  }
  
  if (!lowercase) *c.begin() = toupper(*c.begin());
  return c;
}