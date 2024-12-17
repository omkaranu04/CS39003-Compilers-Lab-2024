all:
			flex expr.l
			yacc -d expr.y 
			gcc -o eval lex.yy.c y.tab.c eval.c 
run: 
			./eval < input.txt
clean:
			rm -f ./eval
			rm -f y.tab.c 
			rm -f lex.yy.c
			rm -f y.tab.h
