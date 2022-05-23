#include "graf_lista.hh"

ListGraph::ListGraph(int _numV, double _dense)	//konstruktor z listą inicjalizacyjną
	:Graph(_numV, static_cast<int>(_dense* _numV* (_numV - 1)), _dense),
	arr(new connection[static_cast<int>(_dense* _numV* (_numV - 1))]){}

void ListGraph::addConnection(int _begVert, int _endVert, int _weight, int t_flag) const
{
	arr[t_flag].endVert = _endVert;		//
	arr[t_flag].begVert = _begVert;		//wpisz nowe połączenie
	arr[t_flag].weight = _weight;		//
}

bool ListGraph::checkConnection(int _begVert, int _endVert) const 	
{
	for (int i = 0; i < numE; ++i)
	{
		if (arr[i].endVert == _endVert && arr[i].begVert == _begVert) return true; 
	}
	return false;
}

void ListGraph::randFill(const bool loops) const	//uzupełnianie losowymi wagami tak jak w przypadku macierzy
{
	int randWeight;
	if(dense == 1)  //przypadek dla grafu pełnego
	{
		int tmp = 0;
		for (int _begVert = 0; _begVert < numV; ++_begVert) 
		{
			for (int _endVert = 0; _endVert < numV; ++_endVert)
			{
				if (_begVert!= _endVert) 
				{
					randWeight = rand() % range  + offset;
					while (randWeight == 0) randWeight = rand() % range  + offset; //nie może wylosować się 0
					addConnection(_begVert, _endVert, randWeight, tmp++);		//dodaje nowe połączenie z losową wagą
				}
			}
		}
	}
	else 	//graf niepełny
	{
		int tmp = 0;
		while (tmp < numE)
		{
			int randBeg = rand() % numV; 	//losowy wybór 2 wierzchołków pomiędzi którymi wklei się wagę
			int randEnd = rand() % numV;
			if (!checkConnection(randBeg, randEnd))	//jeśli połączenie nie istnieje
			{
				randWeight = rand() % range  + offset;
				while (randWeight == 0) randWeight = rand() % range  + offset;	//generuj wagę != 0

				if (randBeg != randEnd) addConnection(randBeg, randEnd, randWeight, tmp++); //jeśli są to 2 różne wierzchołki to wklej wagę
				else if (loops) addConnection(randBeg, randEnd, randWeight, tmp++);		//jeśli to ten sam wierzchołek to wklej jeśli wolno tworzyć pętle
			}
		}
	}
}

void ListGraph::printGraph() const
{
	cout << "--Lista sasiedztwa--" << endl;
	for (int _begVert = 0; _begVert < numV; ++_begVert)
	{
		cout << _begVert;
		for (int i = 0; i < numE; ++i)
		{
			if (arr[i].begVert == _begVert)	//jeśli połączenie istnieje
			cout << "---" << arr[i].endVert << ": " << arr[i].weight << endl; //vert1 --- vert 2: waga
		}
	}
	cout << endl;
}

const int ListGraph::fileFill()	//prawie identycznie jak w przypadku macierzy
{
	ifstream file("graf.txt");  //poprawne otwieranie pliku
	if (!file.is_open()) 
	{
		cerr << "Blad otwierania pliku" << endl;
		return 1;
	}

	int startVert;	//wierzchołek startowy
	file >> numE >> numV >> startVert; //uzupełnij: ilość krawędzi, wierzchołków i wierzchołek startowy
	arr = new connection[numE]; //nowa lista połączeń

	int _begVert, _endVert, _weight;
	for (int i = 0; i < numE; ++i) {

		file >> _begVert >> _endVert >> _weight;
		addConnection(_begVert, _endVert, _weight, i);	//uzupełnianie grafu z pliku
	}
	file.close();
	return startVert; 
}

void ListGraph::exportGraph(const int startVert) const
{
	ofstream file("Exported.txt");	//poprawne tworzenie pliku
	if(!file.is_open()) {
		cerr << "Nie udalo sie utworzyc pliku" << endl;
		return;
	}

	file << numE << " " << numV << " " << startVert << "\n"; //linia początkowa z info o grafie
	for (int i = 0; i < numE; ++i) {

		file << arr[i].begVert << " ";
		file << arr[i].endVert << " ";
		file << arr[i].weight << "\n";		
	}
	file.close();
}
