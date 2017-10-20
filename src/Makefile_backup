bcc:	y.tab.c lex.yy.c
	g++ y.tab.c lex.yy.c -lfl -o bcc
y.tab.c: parser.y 
	yacc -d --debug --verbose parser.y 
y.tab.h: parser.y
	yacc -d  --debug --verbose parser.y
lex.yy.c: scanner.l y.tab.h
	lex scanner.l

.PHONY: clean 
clean:
	-@rm -rf y.tab.c y.tab.h lex.yy.c y.output bcc 2>/dev/null || true
