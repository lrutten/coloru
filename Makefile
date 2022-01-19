CPP_OPTIONS=-g -std=c++20

all: coloru

coloru: clotool.o textfile.o lex.o parser.o runner.o recurse.o easylogging++.o
	g++ -o coloru clotool.o textfile.o lex.o parser.o runner.o recurse.o easylogging++.o -lboost_coroutine -lboost_context -lboost_thread -lboost_system

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

easylogging++.o: easylogging++.cc easylogging++.h
	g++ -c ${CPP_OPTIONS} easylogging++.cc

run: coloru
	./coloru

install: coloru
	cp -v coloru ~/bin

valgrind: coloru
	valgrind --leak-check=full --show-leak-kinds=all ./coloru

check:
	cppcheck --enable=all .

clean:
	rm -vf *.log
	rm -vf *.o
	rm -vf coloru
	rm -vf core
	rm -vf *~


