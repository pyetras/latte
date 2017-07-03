//
//  ArrayType.h
//  latte
//
//  Created by Piotr Sokólski on 17.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#ifndef __latte__ArrayType__
#define __latte__ArrayType__

#include <iostream>
#include "Type.h"
#include "BasicType.h"

class ArrayType : public LType {
public:
  LType::Type_t getType() const;
  bool operator==(const LType& other) const;
  operator llvm::Type*() const;
  
  std::string toJVMString(bool lowercase) const;
  static LType::ptr getBool();
  static LType::ptr getInt();
  static LType::ptr getString();
  static LType::ptr getAny();
  
  static LType::ptr getFromBasic(LType::ptr t);

  friend LType::ptr arrayTToInnerType(LType::ptr t);
private:
  friend class BasicType;
  
  ArrayType(BasicType::basictype_t typ): type(typ) {};

  BasicType::basictype_t type;
};

LType::ptr arrayTToInnerType(LType::ptr t);

#endif /* defined(__latte__BasicType__) */
