
DATA = fibonacci

matrix: DATA = matrix
markdown: DATA = markdown

08: 
	@flex -o $@.yy.c $@.l
	@gcc $@.yy.c -lfl -o $@.run
	@./$@.run

%: 
	@flex -o $*.yy.c $*.l
	@gcc $*.yy.c -lfl -o $*.run
	@./$*.run < $(DATA).txt

clean:
	@rm -f *.run
	@rm -f *.yy.c
