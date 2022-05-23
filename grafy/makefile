a.out: main.o bellman_ford.o graf_macierz.o graf_lista.o
	g++ main.o bellman_ford.o graf_macierz.o graf_lista.o

main.o: main.cpp bellman_ford.hh
	g++ -c -Wall main.cpp

bellman_ford.o: bellman_ford.cpp bellman_ford.hh graf_macierz.hh graf_lista.hh
	g++ -c -Wall bellman_ford.cpp

graf_macierz.o: graf_macierz.cpp graf_macierz.hh graf.hh
	g++ -c -Wall graf_macierz.cpp

graf_lista.o: graf_lista.cpp graf_lista.hh graf.hh
	g++ -c -Wall graf_lista.cpp