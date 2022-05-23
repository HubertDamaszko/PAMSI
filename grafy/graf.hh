#ifndef GRAF_HH
#define GRAF_HH

#include <iostream>
#include <fstream>
#include <chrono>
#include <string> 
#include <time.h>
#include <cstdlib>
#include <memory>

using namespace std;

#define range 30	//zakres wag krawędzi
#define offset -5	//przesunięcie wag do wylosowania (dla offset==-5 będzie to od -5 do 25)

//abstrakcyjna klasa reprezentująca graf
class Graph
{
protected:
	int numV;			//ilość vierzchołków
	int numE; 			//ilość krawędzi
	double dense; 		//gęstość grafu

public:
	explicit Graph(int _numV, int _numE, double _dense)	//lista inicjalizacyjna
	: 	numV(_numV), 
		numE(_numE), 
		dense(_dense){}
	Graph(){}; 	//konstruktor bezparametryczny
	virtual ~Graph() {};

	const int& getV() const {return numV;}	//getter wierzchołków					
	const int& getE() const {return numE;}	//getter krawędzi

	virtual void randFill(const bool loops) const = 0; //funkcja wirt. do randomowego tworzenia grafu
	virtual void printGraph() const =  0;  				//wypisanie zawartości grafu na konsoli
	virtual const int fileFill() = 0; 					//uzupełnia graf danymi z pliku
	virtual void exportGraph(const int startVert) const = 0; //eksportuje graf do pliku
};	
#endif