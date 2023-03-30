all:
	bison --yacc --defines --output=parser.cpp parser.y
	flex --outfile=scanner.cpp scanner.l
	g++  scanner.cpp parser.cpp symbol_table.cpp symbol_table.h

clean:
	rm a.out
	rm parser.cpp
	rm parser.hpp
	rm scanner.cpp
	rm symbol_table.h.gch