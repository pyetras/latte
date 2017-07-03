//
//  runtime.c
//  latte
//
//  Created by Piotr Sokólski on 19.01.2013.
//  Copyright (c) 2013 Piotr Sokólski. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t _strcmp(char* s1, char* s2){
  return strcmp(s1, s2) == 0;
}

char* _concat(char* s1,char* s2) {
  char* t = (char*)malloc(strlen(s1)+strlen(s2)+1);
  return strcat(strcpy(t,s1),s2);
}

void printInt(int i){
  printf("%d\n", i);
}

void printString(char *c){
  printf("%s\n", c);
}

void error(){
  printf("runtime error\n");
  exit(1);
}

int readInt(){
  int i;
  scanf("%d\n", &i);
  return i;
}

char* readString(){
  char* s = 0;
  size_t n;
  getline(&s, &n, stdin);
  if (s[strlen(s) - 1] == '\n') s[strlen(s) - 1] = 0;
  return s;
}