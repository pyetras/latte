//
//  BasicType.h
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__BasicType__
#define __latte__BasicType__

#include <iostream>
#include "Type.h"

class ArrayType;

class BasicType : public LType {
public:
  LType::Type_t getType() const;
  bool operator==(const LType& other) const;
  operator llvm::Type*() const;
  
  std::string toJVMString(bool lowercase) const;
  static LType::ptr getBool();
  static LType::ptr getInt();
  static LType::ptr getString();
  static LType::ptr getVoid();

private:
  enum basictype_t{
    TBool,
    TInt,
    TString,
    TVoid
  };

  BasicType(basictype_t typ): type(typ) {};
  
  friend class ArrayType;
  friend LType::ptr arrayTToInnerType(LType::ptr t);

  basictype_t type;
};

#endif /* defined(__latte__BasicType__) */
