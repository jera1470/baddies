build: buildWin32 buildWin64
	rm -f baddies
	g++ -g -std=c++11 -Wall main.cpp -o baddies

buildWin32:
	rm -f baddies32.exe
	i686-w64-mingw32-g++ -o baddies32.exe main.cpp -static -static-libgcc -static-libstdc++

buildWin64:
	rm -f baddies64.exe
	i686-w64-mingw32-g++ -o baddies64.exe main.cpp -static -static-libgcc -static-libstdc++

run:
	./baddies

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./game.exe

run_solution:
	chmod a+x solution.exe
	./solution.exe
