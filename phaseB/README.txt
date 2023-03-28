COMPILE
bison --yacc --defines --output=parser.cpp parser.y
flex --outfile=scanner.cpp scanner.l
g++ -o calc scanner.cpp parser.cpp symbol_table.cpp symbol_table.h
