all:
	bison --yacc --defines --output=parser.cpp parser.y
	flex --outfile=scanner.cpp scanner.l
	g++ -g scanner.cpp parser.cpp symbol_table.cpp icode.cpp icode.h symbol_table.h target.hpp target.cpp alphavm.cpp alphavm.h

clean:
	rm a.out
	rm parser.cpp
	rm parser.hpp
	rm scanner.cpp
	rm symbol_table.h.gch
	rm icode.h.gch
	rm target.hpp.gch
	rm alphavm.h.gch