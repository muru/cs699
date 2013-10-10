all: count-lines count-lwc count-num calc text-cleanup

optional: ex1 ex2 ex3 ex4

%: %.l
	lex $^
	gcc -o $@ -ll lex.yy.c

.PHONY: clean

clean:
	rm -f count-lines count-lwc count-num calc text-cleanup lex.yy.c
