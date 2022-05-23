#ifndef GRAF_LISTA_HH
#define GRAF_LISTA_HH

#include "graf.hh"

struct connection	//typ danych zawierający informacje o połączeniu dwóch wierzchołków
{	
	int begVert;	
	int endVert;
	int weight; 
};

class ListGraph : public Graph 
{
private:
	connection* arr; //lista wszystkich połączeń w grafie

public:
 	explicit ListGraph(int _numV, double _dense);

	ListGraph() : Graph() {};
	~ListGraph() { delete[] arr;}
	void randFill(const bool loops) const override; 
	void printGraph() const override;
	const int fileFill() override;
	void exportGraph(const int startVert) const override;

	const connection* getConnections() const {return arr;} 	
	void addConnection(int _begVert, int _endVert, int _weight, int flag) const; //dodaje nowe połączenie
	bool checkConnection(int _begVert, int _endVert) const; 	//zwraca true jeśli połączenie istnieje
	
	
};
#endif