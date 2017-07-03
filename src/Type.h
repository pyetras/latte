//
//  Type.h
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__Type__
#define __latte__Type__

#include <iostream>
#include <boost/smart_ptr.hpp>
#include <boost/operators.hpp>

#include "llvm/DerivedTypes.h"
#include "llvm/LLVMContext.h"

class LType : boost::equality_comparable<LType>{
public:
  typedef boost::shared_ptr<LType> ptr;
  
  enum Type_t{
    TBasicType,
    TFunction,
    TArray
  };

  virtual bool operator==(const LType& other) const = 0;
  virtual Type_t getType() const = 0;
  virtual ~LType() {};
  virtual std::string toJVMString(bool lowercase) const { return ""; };
  
  virtual operator llvm::Type*() const { return llvm::Type::getVoidTy(llvm::getGlobalContext()); };
};

#endif /* defined(__latte__Type__) */
