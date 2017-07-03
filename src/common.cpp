//
//  common.cpp
//  latte
//
//  Created by Piotr Sokólski on 18.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//

#include "common.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void error(const std::string& message, int line){
  fprintf(stderr, "ERROR\n");
  if (line > 0)
    std::cout << "line " << line << std::endl ;
  fprintf(stderr,"error: %s\n",message.c_str());
  exit(1);
}