CC = g++

LLVMCC = $(shell llvm-config --cxxflags)
CCFLAGS = -g  $(filter-out -fno-rtti -fno-exceptions,$(LLVMCC))

LLVMLD = $(shell llvm-config --ldflags --libs)
LDFLAGS = -g -lboost_system -lboost_filesystem $(filter-out -fno-rtti -fno-exceptions,$(LLVMLD))

FLEX = flex
BISON = bison
LATEX = latex
DVIPS = dvips
COMMONOBJ = Absyn.o Lexer.o Parser.o Printer.o Interpreter.o Returnchecker.o Typechecker.o Environment.o FunctionType.o RootEnvironment.o Type.o BasicType.o Maybe.o common.o SpecialisedVisitor.o LLVMCompiler.o LLVMEnvironment.o ArrayType.o

all: latc runtime

test: testLatte runtime

clean:
	rm -f *.o latc testLatte src/Lexer.C src/Parser.C lib/Runtime.class lib/runtime.bc

runtime: lib/runtime.bc

latc:	main.o ${COMMONOBJ}
		${CC} ${LDFLAGS} -L/usr/local/lib $^ -o latc

testLatte: Test.o ${COMMONOBJ}
	${CC} ${LDFLAGS} $^ -o testLatte
   
lib/Runtime.class: src/Runtime.java
	javac -d lib $<
     
lib/runtime.bc: src/runtime.c
	clang -O3 -emit-llvm src/runtime.c -c -o lib/runtime.bc

Absyn.o: src/Absyn.C src/Absyn.H
	${CC} ${CCFLAGS} -c src/Absyn.C

src/Lexer.C: src/Latte.l
	${FLEX} -osrc/Lexer.C src/Latte.l

src/Parser.C: src/Latte.y
	${BISON} src/Latte.y -o src/Parser.C

Lexer.o: src/Lexer.C src/Parser.H
	${CC} ${CCFLAGS} -c src/Lexer.C 

Parser.o: src/Parser.C src/Absyn.H
	${CC} ${CCFLAGS} -c src/Parser.C

Printer.o: src/Printer.C src/Printer.H src/Absyn.H
	${CC} ${CCFLAGS} -c src/Printer.C

%.o: src/%.cpp
	${CC} -c ${CCFLAGS} -o $@ $<
