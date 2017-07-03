//
//  main.cpp
//  latte
//
//  Created by Piotr Sokólski on 11.11.2012.
//  Copyright (c) 2012 Piotr Sokólski. All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Typechecker.h"
#include "Parser.H"
#include "LLVMCompiler.h"
#include <unistd.h>
#include <sys/wait.h>
#include <boost/filesystem.hpp>

int main(int argc, const char * argv[])
{
  FILE *input;
  std::string name;
  std::string dir;
  
  if (argc > 1)
  {
    input = fopen(argv[1], "r");
    if (!input)
    {
      fprintf(stderr, "ERROR\n");
      fprintf(stderr, "Error opening input file.\n");
      exit(1);
    }
    name = boost::filesystem::path(argv[1]).stem().string();
    dir = boost::filesystem::path(argv[1]).parent_path().string();
    if (dir == "") dir = ".";
  }else {
    input = stdin;
    name = "out";
    dir = ".";
  }
  std::string namej = name + ".ll";
  Program *parse_tree = pProgram(input);
  if (parse_tree)
  {
    Typechecker typ;
    typ.typecheck(parse_tree);
    
    LLVMCompiler compiler;
    std::ofstream jvmcode;
    std::string tmp (dir + "/" + namej);
    jvmcode.open(tmp.c_str());
    jvmcode << compiler.compile(parse_tree, name);
    jvmcode.close();

    std::string out(dir + "/" + "a.out");
    
    pid_t pid;
    switch(pid=fork()){
      case -1: 
        exit(-1);
      case 0 :
//        close(1);
//        close(2);
        execl("lib/llvm-compile.sh", "", tmp.c_str(), out.c_str(), NULL);
        break;
      default: wait(0);
    }
    
    fprintf(stderr, "OK\n");
    return 0;
  }
  return 1;
}

