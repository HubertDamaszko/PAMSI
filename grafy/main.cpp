#include "bellman_ford.hh"
#include <iomanip>

int main()
{
	int mode;
	cout << "Wybierz zadanie:" << endl;
	cout << "1 - Testy wydajności" << endl;
	cout << "2 - Rozwiązanie własnego grafu" << endl;
	cin >> mode;
	cout << endl;
	
	if(mode == 1)
	{
		cout << "Rozpoczęto testy wydajności, może to zająć kilka minut" << endl;
		int instances = 100;
		int verts[5] = 		{10, 25, 50, 75, 100};
		double denses[4] = 	{0.25, 0.5, 0.75, 1};
		
		ofstream file("czasy.txt");
		if (!file.is_open())
		{
			cerr << "Nie można utworzyć pliku" << endl;
			return 1;
		}
	//--------------------------------------------------------------------------------------	
		file << "Czasy działania algorytmu dla 100 instancji: \n\n";
	//--------------------------------------------------------------------------------------
		file << "Implementacja grafu jako lista: \n";
		file << "     ";
		for (int i = 0; i < (int)(sizeof(verts)/sizeof(*verts)); i++)
		{
			if (verts[i]<10) 	file << verts[i] << "      ";
			else 				file << verts[i] << "     ";
		}
		file << "\n";
		file << fixed;
		for(int i = 0; i < (int)(sizeof(denses)/sizeof(*denses)); i++)
		{
			file << setprecision(0) << denses[i]*100 << "% ";
			if(denses[i]<1) file << " ";
			
			for(int j = 0; j < (int)(sizeof(verts)/sizeof(*verts)); j++)
			{
				//cout << "aktualna ilość wierzch: " << verts[j] << endl;
				double sum = 0;
				for(int loop = 0; loop < instances; loop++)
				{
					//tworzenie grafu o zdanych parametrach
					shared_ptr<ListGraph> graph = make_shared<ListGraph>(verts[j], denses[i]); 
					
					int startNode = rand() % verts[j]; 	//losowy wierzchołek startowy
					
					graph->randFill(true);				//losowe wypełnianie grafu, możliwe pętle

					sum += solveBellman(false, move(graph), startNode); //sumowanie czasu działania algorytmu 
				}
				file << setprecision(4) << sum / instances << " ";
			}
			file << "\n";
		}
		file << "\n";
	//--------------------------------------------------------------------------------------
		file << "Implementacja grafu jako macierz: \n";
		file << "     ";
		for (int i = 0; i < (int)(sizeof(verts)/sizeof(*verts)); i++)
		{
			if (verts[i]<10) 	file << verts[i] << "      ";
			else 				file << verts[i] << "     ";
		}
		file << "\n";
		file << fixed;
		for(int i = 0; i < (int)(sizeof(denses)/sizeof(*denses)); i++)
		{
			file << setprecision(0) << denses[i]*100 << "% ";
			if(denses[i]<1) file << " ";
			
			for(int j = 0; j < (int)(sizeof(verts)/sizeof(*verts)); j++)
			{
				double sum = 0;
				for(int loop = 0; loop < instances; loop++)
				{
					//tworzenie grafu o zdanych parametrach
					shared_ptr<MatrixGraph> graph = make_shared<MatrixGraph>(verts[j], denses[i]); 

					int startNode = rand() % verts[j]; 	//losowy wierzchołek startowy

					graph->randFill(true);				//losowe wypełnianie grafu, możliwe pętle

					sum += solveBellman(false, move(graph), startNode); //sumowanie czasu działania algorytmu
				}
				file << setprecision(4) << sum / instances << " ";
			}
			file << "\n";
		}
		file << "\n";
		file.close();
		cout << "Pomyślnie utworzono plik: czasy.txt" << endl;
		return 0;
	}

	else if(mode == 2)	//rozwiąż problem wczytując graf z pliku
	{
		typedef	MatrixGraph ActualGraph; //wybrany typ implementacji to macierz
		shared_ptr<ActualGraph> graph;	
		shared_ptr<ActualGraph> tmp = make_shared<ActualGraph>();
		graph = tmp;
		int startVertex = graph->fileFill();	//wypełnij graf z pliku, zwróć wierzchołek początkowy
		//graph->printGraph();
		solveBellman(true, move(graph), startVertex); //rozwiąż problem, tworząc plik wyjściowy z rozwiązaniem
	}
	else cout << "nie ma takiej opcji" << endl;
	return 0;
}