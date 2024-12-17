LEX_FILE = poly.l
YACC_FILE = poly.y
UTILS_FILE = polyutils.c
LEX_GEN = lex.yy.c
YACC_GEN_C = y.tab.c
YACC_GEN_H = y.tab.h
EXEC = a.out
INPUT = sample.txt

default: run

$(EXEC): $(LEX_GEN) $(YACC_GEN_C) $(UTILS_FILE)
	gcc $(LEX_GEN) $(YACC_GEN_C) $(UTILS_FILE) -lm -o $(EXEC)

$(LEX_GEN): $(LEX_FILE)
	flex $(LEX_FILE)

$(YACC_GEN_C) $(YACC_GEN_H): $(YACC_FILE)
	yacc -d $(YACC_FILE)

run: $(EXEC)
	./$(EXEC) < $(INPUT)

clean:
	rm -f $(EXEC) $(LEX_GEN) $(YACC_GEN_C) $(YACC_GEN_H)
