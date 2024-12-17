run:
			./a.out < tinyC2_22CS30016_22CS30044_test.c > output_22CS30016_22CS30044.txt

term_run:
			./a.out < tinyC2_22CS30016_22CS30044_test.c 

all:
			flex tinyC2_22CS30016_22CS30044.l
			yacc -d tinyC2_22CS30016_22CS30044.y
			gcc lex.yy.c y.tab.c tinyC2_22CS30016_22CS30044.c 
clean:
			rm -f a.out
			rm -f lex.yy.c
			rm -f y.tab.c
			rm -f y.tab.h
			rm -f output_22CS30016_22CS30044.txt