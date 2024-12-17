run : lex.yy.o TinyC3_22CS30016_22CS30044.tab.o TinyC3_22CS30016_22CS30044_translator.o
		g++ lex.yy.o TinyC3_22CS30016_22CS30044.tab.o TinyC3_22CS30016_22CS30044_translator.o

TinyC3_22CS30016_22CS30044_translator.o : TinyC3_22CS30016_22CS30044_translator.h TinyC3_22CS30016_22CS30044_translator.cxx
		g++ -c TinyC3_22CS30016_22CS30044_translator.h
		g++ -c TinyC3_22CS30016_22CS30044_translator.cxx

lex.yy.o : lex.yy.c
		g++ -c lex.yy.c 

TinyC3_22CS30016_22CS30044.tab.o : TinyC3_22CS30016_22CS30044.tab.c 
		g++ -c TinyC3_22CS30016_22CS30044.tab.c 

lex.yy.c : TinyC3_22CS30016_22CS30044.l TinyC3_22CS30016_22CS30044.tab.h TinyC3_22CS30016_22CS30044_translator.h
		flex TinyC3_22CS30016_22CS30044.l 

TinyC3_22CS30016_22CS30044.tab.c TinyC3_22CS30016_22CS30044.tab.h : TinyC3_22CS30016_22CS30044.y 
		bison -dtv TinyC3_22CS30016_22CS30044.y

onfiles : a.out
		mkdir -p ./TestOutputs
		@echo "\nTestFiles contains the Test Cases.\n"
		@echo "\nRunning on Test Cases ... "
		./a.out < TestFiles/TinyC3_22CS30016_22CS30044_test1.c > TestOutputs/TinyC3_22CS30016_22CS30044_quads1.out
		./a.out < TestFiles/TinyC3_22CS30016_22CS30044_test2.c > TestOutputs/TinyC3_22CS30016_22CS30044_quads2.out
		./a.out < TestFiles/TinyC3_22CS30016_22CS30044_test3.c > TestOutputs/TinyC3_22CS30016_22CS30044_quads3.out
		./a.out < TestFiles/TinyC3_22CS30016_22CS30044_test4.c > TestOutputs/TinyC3_22CS30016_22CS30044_quads4.out
		./a.out < TestFiles/TinyC3_22CS30016_22CS30044_test5.c > TestOutputs/TinyC3_22CS30016_22CS30044_quads5.out
		@echo "\nTest Cases run successfully.\n"
		@echo "\nOutputs stored in TestOutputs directory.\n"

clean :
		rm -f lex.yy.c
		rm -f TinyC3_22CS30016_22CS30044.tab.c
		rm -f TinyC3_22CS30016_22CS30044.tab.h
		rm -f *.output 
		rm -f *.o 
		rm -f *.gch
		rm -f a.out
		rm -rf ./TestOutputs