CPP_OPTIONS=-g -std=c++17

all: clotool

clotool: clotool.o textfile.o lex.o parser.o runner.o
	g++ -o clotool clotool.o textfile.o lex.o parser.o runner.o

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

run: clotool
	./clotool

install: clotool
	cp -v clotool ~/bin

valgrind: clotool
	valgrind --leak-check=full --show-leak-kinds=all ./clotool

check:
	cppcheck --enable=all .

clean:
	rm -vf *.o
	rm -vf clotool

