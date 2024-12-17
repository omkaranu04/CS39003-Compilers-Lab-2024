run: all
		./icgen < sample.txt > intcode.c 
		gcc -o ic intcode.c -lm
		./ic
all:
		flex expr.l
		yacc -d expr.y
		gcc -o icgen y.tab.c lex.yy.c intcodegen.c
clean:
		rm -f y.tab.c 
		rm -f y.tab.h
		rm -f lex.yy.c
		rm -f icgen
		rm -f intcode.c
		rm -f ic