CPP_OPTIONS=-g -std=c++17

all: coloru

coloru: clotool.o textfile.o lex.o parser.o runner.o recurse.o
	g++ -o coloru clotool.o textfile.o lex.o parser.o runner.o recurse.o

clotool.o: clotool.cpp textfile.h lex.h parser.h runner.h
	g++ -c ${CPP_OPTIONS} clotool.cpp

textfile.o: textfile.cpp textfile.h
	g++ -c ${CPP_OPTIONS} textfile.cpp

lex.o: lex.cpp lex.h textfile.h
	g++ -c ${CPP_OPTIONS} lex.cpp

parser.o: parser.cpp lex.h textfile.h parser.h
	g++ -c ${CPP_OPTIONS} parser.cpp

runner.o: runner.cpp lex.h textfile.h parser.h runner.h
	g++ -c ${CPP_OPTIONS} runner.cpp

recurse.o: recurse.cpp lex.h textfile.h parser.h runner.h
	g++ -c ${CPP_OPTIONS} recurse.cpp

run: coloru
	./coloru

install: coloru
	cp -v coloru ~/bin

valgrind: coloru
	valgrind --leak-check=full --show-leak-kinds=all ./coloru

check:
	cppcheck --enable=all .

clean:
	rm -vf *.o
	rm -vf coloru
	rm -vf core

