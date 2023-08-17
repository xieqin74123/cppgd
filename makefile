all: main

main: main.cpp
	g++ main.cpp -o cppgd

clean:
	rm cppgd