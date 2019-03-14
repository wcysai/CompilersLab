run: lexical.l 
	flex lexical.l
	g++ lex.yy.c -o run
	./run


