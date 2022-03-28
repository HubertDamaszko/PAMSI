#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

void checkSort(int *sorted, int _tableSize)
{
    int errors = 0;
    for(int i=0; i<_tableSize-1; i++)
    {
        if(sorted[i]>sorted[i+1])
        {
            errors++;
        }
    }
    if(errors!=0) cout<<"Zle posortowano " << errors << " elementow" << endl;
}

void mergeSort(int *_randArray, int _begIndex, int _endIndex)
{
    if(_endIndex > _begIndex)
    {
        int _midIndex = (_endIndex - _begIndex)/2 + _begIndex;  //środkowy element
        mergeSort(_randArray,_begIndex, _midIndex);             //dzielenie aż do uzyskania pojedyńczych tablic
        mergeSort(_randArray,_midIndex+1, _endIndex);
        int len1 = _midIndex-_begIndex+1;      
        int len2 = _endIndex-_midIndex;
        int *tab1 = new int[len1];
        int *tab2 = new int[len2];

        for (int i = 0; i < len1; i++) tab1[i] = _randArray[_begIndex+i];       //
                                                                                //kopiowanie do tablic pomocniczych
        for (int j = 0; j < len2; j++) tab2[j] = _randArray[_midIndex+j+1];     //

        int m=0, n=0, o=_begIndex;

        while (m < len1 && n < len2)    //scalanie tablic
        {
            if (tab1[m] <= tab2[n])
            {
                _randArray[o] = tab1[m];
                m++;
            } 
            else 
            {
                _randArray[o] = tab2[n];
                n++;
            }
            o++;
        }

        while (m < len1)     //przepisz resztę jeśli jedna z tablic się wyczerpie
        {
            _randArray[o] = tab1[m];
            m++;
            o++;
        }

        while (n < len2)
        {
            _randArray[o] = tab2[n];
            n++;
            o++;
        }
        delete [] tab1;
        delete [] tab2;
    } 
}

void sortSubheap(int *_randArray, int current_size, int i)
{
    int maxed = i;
    int current = i;
    int l_child = 2*i+1;
    int r_child = 2*i+2;

    while (true) 
    {
        maxed = current;
        l_child = 2 * maxed+1, r_child = l_child+1;
        if (l_child < current_size && _randArray[l_child] > _randArray[current])    //sprawdzanie czy dziecko jest większe od ojca
            current = l_child;                                                     //jeśli tak to zapamiętaj to
        if (r_child < current_size && _randArray[r_child] > _randArray[current])    
            current = r_child;
        if (current == maxed)   //jeśli nie ma już żadnego większego dziecka to zakończ sortowanie
        break;
        swap(_randArray[current], _randArray[maxed]);   //zamień znalezione większe dziecko z ojcem
    }
}

void heapSort(int *_randArray, int _tableSize)
{
    for(int i = _tableSize/2-1; i >= 0; --i)    sortSubheap (_randArray, _tableSize, i); //sortuj kopiec od ostatniego ojca
    for(int i = _tableSize-1; i > 0; --i)
    {
        swap (_randArray [0] , _randArray[i]);  //zamień korzeń z ostatnim elementem
        sortSubheap(_randArray, i, 0);          //napraw strukturę kopca
    }
}

void quickSort(int *_randArray, int left, int right)
{
	if(left >= right) return;
	int l_iter = left-1;
    int r_iter = right+1; 
	int pivot = _randArray[(right+left)/2]; //piwot to środkowy element
	
	while(1)
	{
		while(pivot>_randArray[++l_iter]){}; //znajduje pierwszy >= element od piwota od lewej strony
		
		while(pivot<_randArray[--r_iter]){}; //znajduje pierwszy <= element od piwota od prawej strony
		
		if( l_iter <= r_iter) swap(_randArray[l_iter],_randArray[r_iter]); //jeśli liczniki się nie minęły to zamienia miejscami elementy
		else  break;
	}

	if(left < r_iter) quickSort(_randArray, left, r_iter); //powtórz procedurę na lewej podtablicy
	if(right > l_iter)  quickSort(_randArray, l_iter, right); //powtórz procedurę na prawej podtablicy
}

void print_times(double *times)
{
    cout << "Czasy sortowania:" << endl;
    cout << "Elementy nieposortowane: " << times[0] << "ms" << endl;
    cout << "Elementy posortowane w 25%: " << times[1] << "ms" << endl;
    cout << "Elementy posortowane w 50%: " << times[2] << "ms" << endl;
    cout << "Elementy posortowane w 75%: " << times[3] << "ms" << endl;
    cout << "Elementy posortowane w 95%: " << times[4] << "ms" << endl;
    cout << "Elementy posortowane w 99%: " << times[5] << "ms" << endl;
    cout << "Elementy posortowane w 99,7%: " << times[6] << "ms" << endl;
    cout << "Elementy posortowane odwrotnie: " << times[7] << "ms" << endl << endl;
}

void test(int _algorytm, int *_randArray, int _tableSize, double *times)
{
    //cout << "uruchomiono test" << endl;
    int begIndex = 0;
    int endIndex = _tableSize-1;
    int *unsorted = new int[_tableSize];
    int tmp;
    
    double percValue[] = {0, 0.25, 0.5, 0.75, 0.95, 0.99, 0.997};

    switch(_algorytm)
    {
    case 1:
    {
        for(int i=0; i<(sizeof(percValue)/sizeof(double)); i++)
        {
            for (int u = 0; u < _tableSize; u++)    unsorted[u]=_randArray[u];
            mergeSort(unsorted, begIndex, int(endIndex*percValue[i]));

            auto start = std::chrono::high_resolution_clock::now();
            mergeSort(unsorted, begIndex, endIndex);
            auto end = std::chrono::high_resolution_clock::now();

            checkSort(unsorted,endIndex);
            std::chrono::duration<double, std::milli> float_ms = end - start;
            times[i] += float_ms.count(); 
        }

        for (int u = 0; u < _tableSize; u++)  unsorted[u]=_randArray[u];
        mergeSort(unsorted, begIndex, endIndex);
        for (int i=0; i<_tableSize/2; i++) swap(unsorted[i],unsorted[_tableSize-i-1]);

        auto start = std::chrono::high_resolution_clock::now();
        mergeSort(unsorted, begIndex, endIndex);
        auto end = std::chrono::high_resolution_clock::now();

        checkSort(unsorted,endIndex);
        std::chrono::duration<double, std::milli> float_ms = end - start;
        times[7] += float_ms.count();
        break;
    }
    
    case 2:
    {
        for(int i=0; i<(sizeof(percValue)/sizeof(double)); i++)
        {
            for (int u = 0; u < _tableSize; u++)    unsorted[u]=_randArray[u];
            quickSort(unsorted, begIndex, int(endIndex*percValue[i]));

            auto start = std::chrono::high_resolution_clock::now();
            quickSort(unsorted, begIndex, endIndex);
            auto end = std::chrono::high_resolution_clock::now();

            checkSort(unsorted,endIndex);
            std::chrono::duration<double, std::milli> float_ms = end - start;
            times[i] += float_ms.count();
        }

        for (int u = 0; u < _tableSize; u++)    unsorted[u]=_randArray[u];
        quickSort(unsorted, begIndex, endIndex);
        for (int i=0; i<_tableSize/2; i++) swap(unsorted[i],unsorted[_tableSize-i-1]);

        auto start = std::chrono::high_resolution_clock::now();
        quickSort(unsorted, begIndex, endIndex);
        auto end = std::chrono::high_resolution_clock::now();

        checkSort(unsorted,endIndex);
        std::chrono::duration<double, std::milli> float_ms = end - start;
        times[7] += float_ms.count();
        break;
    }

    case 3:
    {
        for(int i=0; i<(sizeof(percValue)/sizeof(double)); i++)
        {
            for (int u = 0; u < _tableSize; u++)    unsorted[u]=_randArray[u];
            heapSort(unsorted, int(_tableSize*percValue[i]));

            auto start = std::chrono::high_resolution_clock::now();
            heapSort(unsorted, _tableSize);
            auto end = std::chrono::high_resolution_clock::now();

            checkSort(unsorted,endIndex);
            std::chrono::duration<double, std::milli> float_ms = end - start;
            times[i] += float_ms.count();
        }  


    for (int u = 0; u < _tableSize; u++)    unsorted[u]=_randArray[u];
    heapSort(unsorted, _tableSize);
    for (int i=0; i<_tableSize/2; i++) swap(unsorted[i],unsorted[_tableSize-i-1]);

    auto start = std::chrono::high_resolution_clock::now();
    heapSort(unsorted, _tableSize);
    auto end = std::chrono::high_resolution_clock::now();

    checkSort(unsorted,endIndex);
    std::chrono::duration<double, std::milli> float_ms = end - start;
    times[7] += float_ms.count();
    break;
    }
       
    default:
        cout << "Zła wartość" << endl;
        break;
    }

    delete [] unsorted;
}

int main()
{
    int tableSize;
    cout << "Podaj rozmiar tablicy: ";
    cin >> tableSize;
    cout << endl;
    
    int algorytm;
    cout << "Podaj algorytm sortowania:" << endl;
    cout << "1. merge sort" << endl;
    cout << "2. quick sort" << endl;
    cout << "3. heap sort" << endl;
    cin >> algorytm;
    cout << endl;

    double times[] = {0,0,0,0,0,0,0,0};

    int *randArray = new int[tableSize];
    for(int i=0;i<100;i++)
    {
        for(int j=0;j<tableSize;j++)
        {
            randArray[j]=rand()%10000;  //Generate number between 0 to 100000
        }
        test(algorytm, randArray, tableSize, times);
    }
    print_times(times);
    delete [] randArray;
    return 0;
}