#ifndef GRAF_MACIERZ_HH
#define GRAF_MACIERZ_HH

#include "graf.hh" 

#define infNumber 99999	//wartość oznaczająca nieskończoność

class MatrixGraph : public Graph
{
private:
	unique_ptr <unique_ptr <int[]> [] > adjMartix; //macierz sąsiedztwa
														
public:
	void randFill(const bool loops) const override;
	void printGraph() const override;
	const int fileFill() override;
	void exportGraph(const int startVert) const override;

	const int& getElement(int _row, int _column) const {return adjMartix[_row][_column];}

	explicit MatrixGraph(int _numV, double _dense);
	MatrixGraph() : Graph() {};
};
#endif