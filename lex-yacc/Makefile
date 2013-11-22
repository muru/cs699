all: count-lines count-lwc count-num calc text-cleanup

optional: ex1 ex2 ex3 ex4

calc: calc.l calc.y
	yacc -d calc.y
	lex calc.l
	gcc -o $@ lex.yy.c y.tab.c

%: %.l
	lex $^
	gcc -o $@ -ll lex.yy.c

.PHONY: clean

clean:
	rm -f count-lines count-lwc count-num calc text-cleanup lex.yy.c y.tab.c
