#include "graf_macierz.hh"

MatrixGraph::MatrixGraph(int _numV, double _dense) 		//konstruktor
:Graph(_numV, static_cast<int>(_dense* _numV* (_numV - 1)), _dense), 
	
adjMartix(make_unique<unique_ptr<int[]>[]>(_numV)) 
{
	for (int _row = 0; _row < numV; ++_row) 
	{
		adjMartix[_row] = make_unique<int[]>(_numV); 	//tworzenie drugiego wymiaru tablicy
		for (int _column = 0; _column < numV; ++_column)
		{
			if (_row == _column) adjMartix[_row][_column] = 0; 	//zera na głównej przekątnej
			else adjMartix[_row][_column] = infNumber; 		//pozostałe pola na nieskończoność
		}
	}
}

void MatrixGraph::randFill(const bool loops) const 
{
	int randWeight;
	if (dense == 1) //przypadek dla grafu pełnego
	{
		for (int _rows = 0; _rows < numV; ++_rows) 		
		{ 
			for (int _columns = 0; _columns < numV; ++_columns) 
			{  
				if (_rows != _columns) //pomiń elementy na głównej przekątnej
				{
					randWeight = rand() % range  + offset;
					while (randWeight == 0) randWeight = rand() % range + offset;	//nie może wylosować się 0
					adjMartix[_rows][_columns] = randWeight;	//uzupełnianie macierzy losowymi wagami
				}
			}
		}
	}
	else 	//graf niepełny
	{
		int tmp = numE;
		while(tmp > 0)
		{
			int randRow = rand() % numV; //losowy wybór 2 wierzchołków pomiędzi którymi wklei się wagę
			int randCol = rand() % numV;

			if(adjMartix[randRow][randCol] == 0 || adjMartix[randRow][randCol] == infNumber) //pomija już zainicjalizowane krawędzie
			{
				randWeight = rand() % range + offset;
				while (randWeight == 0) randWeight = rand() % range + offset;	//nie może wylosować się 0

				if (randRow != randCol) //dla połączeń różnych wierzchołków
				{
					adjMartix[randRow][randCol] = randWeight;
					tmp--;
				}
				else if (loops)	//dla tego samego wierzchołka utwórz jeśli są dozwolone pętle
				{
					adjMartix[randRow][randCol] = randWeight;
					tmp--;
				}
			}
		}
	}
}

void MatrixGraph::printGraph() const 
{
	cout << "--Macierz sasiedztwa--" << endl;
	for(int _rows = 0; _rows < numV; ++_rows)
	{	
		for (int _columns = 0; _columns < numV; ++_columns) 
		{
			int tmp = adjMartix[_rows][_columns]; //pobierz wagę połączenia
			if (tmp >= infNumber) cout << "inf"; 
			else cout << tmp;					

			if(abs(tmp) < 10) 			cout << "    "; 	//liczby jednocyfrowe
			else if (abs(tmp) < 100) 	cout << "   ";	//liczby dwucyfrowe
			else 						cout << "  ";	//liczby trzycyfrowe lub inf

			if (tmp < 0) 				cout << '\b'; //zrób miejsce dla minusa
		}
		cout << endl;
	}
	cout << endl;
}

const int MatrixGraph::fileFill() //zwraca wierzchołek startowy
{
	ifstream file("graf.txt");  //poprawne otwieranie pliku
	if (!file.is_open()) 
	{
		cerr << "Blad otwierania pliku" << endl;
		return 1;
	}

	int startVert;	//wierzchołek startowy
	file >> numE >> numV >> startVert;	//uzupełnij: ilość krawędzi, wierzchołków i wierzchołek startowy
	
	adjMartix = make_unique<unique_ptr<int[]>[]>(numV);	//tworzy dynamiczną tablicę z łatwym usuwaniem (tak jak w konstruktorze)

	for (int _row = 0; _row < numV; ++_row)					
	{
		adjMartix[_row] = move(make_unique<int[]>(numV));
		for (int _column = 0; _column < numV; ++_column)
		{
			if (_row == _column) adjMartix[_row][_column] = 0;	//zera na głównej przekątnej
			else adjMartix[_row][_column] = infNumber;			//nieskończoność na innych
		}
	}
	
	int begVert, endVert, weight;
	for (int elem = 0; elem < numE; ++elem)	//w pliku jest tyle połączeń co krawędzi
	{
		file >> begVert >> endVert >> weight;
		adjMartix[begVert][endVert] = weight;	//uzupełnianie grafu z pliku
	}
	
	file.close();
	return startVert;
}

void MatrixGraph::exportGraph(const int startVert) const {
	
	ofstream file("Exported.txt");	//poprawne tworzenie pliku
	if(!file.is_open()) {
		cerr << "Nie udalo sie utworzyc pliku" << endl;
		return;
	}

	file << numE << " " << numV << " " << startVert << "\n";	//linia początkowa z info o grafie
	for (int _row = 0; _row < numV; ++_row)
	{
		for (int _column = 0; _column < numV; ++_column)
		{
			if (adjMartix[_row][_column] != infNumber && adjMartix[_row][_column] != 0)	//uzupełnianie zainicjalizowanymi połączeniami
			{
				file << _row << " ";
				file << _column << " ";
				file << adjMartix[_row][_column] << "\n";
			}
		}
	}
	file.close();
}
