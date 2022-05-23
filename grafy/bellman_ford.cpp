#include "bellman_ford.hh"

void exportSolution(int _pathArray[], int _costArray[], int _numV, int _startVert)
{
	ofstream file("wyniki.txt");	//poprawne tworzenie pliku
	if(!file.is_open()) 
	{
		cerr << "Nie udalo sie utworzyc pliku" << endl;
		return;
	}	
	file << "--Rozwiązanie algorytmem Bellmana Forda--" << endl;
	file << "Wierzchołek początkowy:  " << _startVert << endl << endl;

	for (int i = 0; i < _numV; ++i)	//iteruj po wszystkich wierzchołkach
	{
		if (i==_startVert) continue;
		
		if (_costArray[i] == neg_infNumber) //tekstowy zapis -nieskończoności
		{
			file << "--->" << i << ": koszt dojścia = -inf\n\n";
			continue; 
		}
		else if (_costArray[i] == infNumber) //tekstowy zapis nieskończoności
		{
			file << "--->" << i << ": koszt dojścia = inf\n\n";
			continue;
		}
		else file << "--->" << i << ": koszt dojścia = " << _costArray[i] << "\n";	//wpisz najkrótrzą odległość

		file << "Poprzez wierzchołki:	";
		int actual = i;
		while(_pathArray[actual] != _startVert)
		{
			file << _pathArray[actual] << " <-- ";
			actual = _pathArray[actual];
		}
		file << _startVert << "\n\n";
	}
	cout << endl;
	file.close();
	cout << "Pomyslnie wyeksportowano plik: wyniki.txt" << endl;
}

//wersja algorytmu dla grafu w postaci listy sąsiedztwa
double solveBellman(bool exp, shared_ptr<ListGraph>  _graph, int _startVert)
{
	int* costArray = new int[_graph->getV()]; 
	int* pathArray = new int[_graph->getV()];

	//inicjalizacja wszystkich elementów wartościami == nieskończoność
	for (int i = 0; i < _graph->getV(); ++i) costArray[i] = infNumber;
	//tylko waga dojścia do wierzchołka startowego jest równa 0
	costArray[_startVert] = 0; 

	//inicjalizacja wszystkich elementów wartościami == -1
	for (int i = 0; i < _graph->getV(); ++i) pathArray[i] = -1;

	auto time1 = chrono::high_resolution_clock::now(); //uruchom zegar

	int start, stop, currentWeight;
	for (int i = 1; i < _graph->getV(); ++i) //relaksacja krawędzi
	{
		for (int j = 0; j < _graph->getE(); ++j)
		{
			start = _graph->getConnections()[j].begVert;		//
			stop = _graph->getConnections()[j].endVert;			//zapisz informacje o połączeniu do zmiennych pomocniczych
			currentWeight = _graph->getConnections()[j].weight;	//

			if (costArray[start] + currentWeight < costArray[stop]) //jeśli koszt dojścia jest mniejszy
			{
				costArray[stop] = costArray[start] + currentWeight;	//nadpisz wartość drogi dla wierzchołka docelowego
				pathArray[stop] = start;	//nadpisz najkorzystniejszego poprzednika		
			}
		}
	}

	for (int i = 1; i < _graph->getV(); ++i) //teraz pętla zabezpieczająca przed ujemnym cyklem
	{
		for (int j = 0; j < _graph->getE(); ++j) 
		{
			start = _graph->getConnections()[j].begVert;		//
			stop = _graph->getConnections()[j].endVert;			//zapisz informacje o połączeniu do zmiennych pomocniczych
			currentWeight = _graph->getConnections()[j].weight;	//

			if (costArray[stop] > costArray[start] + currentWeight)	//jeśli jest to ujemny cykl
			{
				if (costArray[start] > infNumber/2) costArray[start] = infNumber;  
				else costArray[stop] = neg_infNumber;				
				pathArray[stop] = -1;
			}
			else if (costArray[start] > infNumber/2)
			{
				costArray[start] = infNumber;
				pathArray[stop] = -1;
			}
		}
	}
	auto time2 = chrono::high_resolution_clock::now(); //wyłącz zegar

	if(exp) exportSolution(move(pathArray), move(costArray), _graph->getV(), _startVert);
	delete[] costArray;
	delete[] pathArray;
	return chrono::duration<double, milli>(time2 - time1).count(); //return the time difference
}

//wersja algorytmu dla grafu w postaci macierzy sąsiedztwa
double solveBellman(bool exp, shared_ptr<MatrixGraph> _graph, int _startVert)
{
	auto t_start = chrono::high_resolution_clock::now();

	int* costArray = new int[_graph->getV()]; 
	int* pathArray = new int[_graph->getV()];

	//inicjalizacja wszystkich elementów wartościami == nieskończoność
	for (int i = 0; i < _graph->getV(); ++i) costArray[i] = infNumber;
	//tylko waga dojścia do wierzchołka startowego jest równa 0
	costArray[_startVert] = 0; 
	//inicjalizacja wszystkich elementów wartościami == -1
	for (int i = 0; i < _graph->getV(); ++i) pathArray[i] = -1;

	for (int i = 1; i < _graph->getV(); ++i)	//tyle przejść przez całą tablicę ile wierzchołków
	{
		for (int current = 0; current < _graph->getV(); ++current)	//iteracja po aktualnych
		{
			for (int checked = 0; checked < _graph->getV(); ++checked)	//iteracja po sprawdzanych
			{
				int weight = _graph->getElement(current, checked);	//waga połączenia
				if (costArray[current] + weight < costArray[checked])	//zrelaksuj jeśli warto
				{
					costArray[checked] = costArray[current] + weight;
					pathArray[checked] = current;
				}
			}
		}
	}
	for (int i = 1; i < _graph->getV(); ++i)	//tyle przejść przez całą tablicę ile wierzchołków
	{
		for (int current = 0; current < _graph->getV(); ++current)	//iteracja po aktualnych
		{
			for (int checked = 0; checked < _graph->getV(); ++checked)	//iteracja po sprawdzanych
			{
				int weight = _graph->getElement(current, checked);	//waga połączenia
				if (costArray[current] + weight < costArray[checked]) {
				
					if (costArray[current] > infNumber / 2) costArray[current] = infNumber;
					else if (weight == infNumber) continue; 
					else costArray[checked] = neg_infNumber;
				}
				else if (costArray[current] > infNumber/2) costArray[current] = infNumber;	
			}
		}
	}
	auto t_end = chrono::high_resolution_clock::now(); 

	if(exp) exportSolution(move(pathArray), move(costArray), _graph->getV(), _startVert);
	delete[] costArray;
	delete[] pathArray;
	return chrono::duration<double, milli>(t_end - t_start).count(); 
}
