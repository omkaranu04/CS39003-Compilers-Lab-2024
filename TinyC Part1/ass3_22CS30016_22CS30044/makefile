run: clean a.out
	./a.out < ass3_22CS30016_22CS30044_test.c
a.out: lex.yy.c
	gcc ass3_22CS30016_22CS30044.c
lex.yy.c:
	flex ass3_22CS30016_22CS30044.l
clean:
	rm -f a.out lex.yy.c