#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// funkcja ta jest odpowiedzialna za tworzenie talicy wypelnionej liczbami 1-len
void generating_positive(int *tab, int len);

// funkcja ta jest odpowiedzialna za tworzenie talicy wypelnionej liczbami len-1
void generating_negative(int *tab, int len);

/* do tych funkcji przekazujemy jako parametr funkcje odpoweidzialna za sortowanie
 * zwracaja nam one czas wykonania sortowania ze wzgledu na to ze funkcje bubble + insertion przyjmuja
 * inne parametry niz quick i merge podzielilem to na dwie podobne funkcje ale z innymi parametrami zeby
 * sobie ulatwic
 */
double time_evaluation(void (*sortFunction)(int*, int), int *tab, int len);
double time_evaluation2(void (*sortFunction)(int*, int,int), int *tab, int start2 , int end2 );

// zamiana elementow w tablicy miejscami, wykorzsytywana w niektorych funkcjach sortowania
void swap(int &a, int &b);

//wszystkie metody sortowania
void bubbleSort(int arr[], int n);
void insertionSort(int arr[], int n);
void merge(int *arr, int start, int mid, int end);
void mergeSort(int *arr, int start, int end);
int partition(int arr[], int start, int end);
void quickSort(int arr[], int start, int end);

//odczytywanie konkretnej linii z pliku tekstowego i zwrocenie go jako stringa
string odczyt(int line);

// prosta wersja split z pythona (oddzielamy elementy tablicy podzielone przcinkiem i "pchamy na koniec vektora"
vector<int> split(string x);




int main() {

    //generujemy losowe zbiory liczb dla podanych wielkosci i zapisujemy je do pliku gdzie wartosci
    // sa rozdzielone przecinkami
    int wielkosci_tablic[6] = {50, 100, 200, 500, 1000, 2000};


    ofstream zapis("C:\\Users\\dawst\\CLionProjects\\Algorytmy2\\losowe.txt");
    zapis.close(); // za kazdym odpaleniem pliku czyscimy stary istniejacy plik
    srand(time(NULL));

    for (int i: wielkosci_tablic) {
        zapis.open("C:\\Users\\dawst\\CLionProjects\\Algorytmy2\\losowe.txt", ios_base::app);
        for (int x = 0; x < i; x++) {
            zapis << rand() % 200 + 1 << ',';

        }
        zapis << '\n';
        zapis.close();
    }



    //wywolanie wszystkich sortowan dla optymistycznej wersji
    cout << "POSITIVE:" << endl << endl;
    for (int i: wielkosci_tablic) {
        int tab[i];

        generating_positive(tab, i);
        double time = time_evaluation(insertionSort, tab, i);
        cout << "Insertion positive for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl;

        generating_positive(tab, i);
        time = time_evaluation(bubbleSort, tab, i);
        cout << "Bubble positive for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl;

        generating_positive(tab, i);
        time = time_evaluation2(quickSort, tab, 0, sizeof(tab) / sizeof(tab[0]) - 1);
        cout << "Quick positive for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl ;

        generating_positive(tab, i);
        time = time_evaluation2(mergeSort, tab, 0, sizeof(tab) / sizeof(tab[0]) - 1);
        cout << "Merge positive for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl << endl;

    }

    //wywolanie wszystkich sortowan dla losowej wersji
    cout << "RANDOM:" << endl << endl;
    for (int i = 0; i < sizeof(wielkosci_tablic) / sizeof(wielkosci_tablic[0]); i++) {
        vector<int> t = split(odczyt(i)); // przyjmujemy wektor z konkretnej linii



        // Insertion Sort
        /* dla kazdego sortowania zmieniamy wektor na zwykla
        * tablice takie parametry przyjmuje time_evaluation
        */
        int *tab1 = t.data();
        double time1 = time_evaluation(insertionSort, tab1, t.size());
        cout << "Insertion random for: " << fixed << wielkosci_tablic[i] << " elems and time: " << time1 * 1000 << " ms" << endl;

        // Bubble Sort
        int *tab2 = t.data();
        double time2 = time_evaluation(bubbleSort, tab2, t.size());
        cout << "Bubble random for: " << fixed << wielkosci_tablic[i] << " elems and time: " << time2 * 1000 << " ms" << endl;

        // Quick Sort
        int *tab3 = t.data();
        double time3 = time_evaluation2(quickSort, tab3, 0, t.size() - 1);
        cout << "Quick random for: " << fixed << wielkosci_tablic[i] << " elems and time: " << time3 * 1000 << " ms" << endl;

        // Merge Sort
        int *tab4 = t.data();
        double time4 = time_evaluation2(mergeSort, tab4, 0, t.size() - 1);
        cout << "Merge random for: " << fixed << wielkosci_tablic[i] << " elems and time: " << time4 * 1000 << " ms" << endl << endl;
    }




    //wywoalnie wszystkich sortowan dla negatywnej wersji
    cout << "NEGATIVE:" << endl;
    for (int i: wielkosci_tablic) {
        int tab[i];
        generating_negative(tab, i);
        double time = time_evaluation(insertionSort, tab, i);
        cout << "Insertion negative for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl;

        generating_negative(tab, i);
        time = time_evaluation(bubbleSort, tab, i);
        cout << "Bubble negative for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl;

        generating_negative(tab, i);
        time = time_evaluation2(quickSort, tab, 0, sizeof(tab) / sizeof(tab[0]) - 1);
        cout << "Quick negative for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl;

        generating_negative(tab, i);
        time = time_evaluation2(mergeSort, tab, 0, sizeof(tab) / sizeof(tab[0]) - 1);
        cout << "Merge negative for:  " <<fixed  << i << " elems and time: " << time*1000 << " ms" << endl << endl;

    }


}

//generowanie tablicy z zakresu 1 - (zakres liczb ktory zostal podany)
void generating_positive(int *tab, int len) {
    for (int i = 0; i < len; i++) {
        tab[i] = i + 1;
    }
}
//generowanie tablicy liczb z zakresu 1-(zakres podany) w odwrotnej kolejnosci
void generating_negative(int *tab, int len) {
    for (int i = len - 1; i >= 0; i--) {
        tab[i] = len - i;
    }
}

double time_evaluation(void (*sortFunction)(int*, int), int *tab, int len ) {
    auto start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false);
    sortFunction(tab, len);
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    setprecision(9);
    return  time_taken;

}

double time_evaluation2(void (*sortFunction)(int*, int,int), int *tab, int start2 , int end2 ) {
    auto start = chrono::high_resolution_clock::now();
    ios_base::sync_with_stdio(false);
    sortFunction(tab, start2 , end2);
    auto end = chrono::high_resolution_clock::now();
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    time_taken *= 1e-9;
    setprecision(9);
    return  time_taken;

}

void swap(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

//bubble sort
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // Przesuwanie elementów większych od klucza do przodu
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void merge(int *arr, int start, int mid, int end) {
    // Obliczenie rozmiarów dwóch połówek
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    // Utworzenie tymczasowych tablic dla lewej i prawej połówki
    int left[leftSize], right[rightSize];

    // Skopi owanie danych do tymczasowych tablic left i right
    for (int i = 0; i < leftSize; i++)
        left[i] = arr[start + i];
    for (int i = 0; i < rightSize; i++)
        right[i] = arr[mid + 1 + i];

    // Scalanie dwóch połówek z powrotem do oryginalnej tablicy arr
    int leftIndex = 0, rightIndex = 0, scalingIndex = start;

    // Porównywanie elementów z tablic left i right i scalanie ich w posortowanej kolejności
    while (leftIndex < leftSize && rightIndex < rightSize) {
        if (left[leftIndex] <= right[rightIndex]){
            arr[scalingIndex] = left[leftIndex];
            scalingIndex+=1;
            leftIndex+=1;

        }
        else{
            arr[scalingIndex] = right[rightIndex];
            scalingIndex+=1;
            rightIndex+=1;
        }

    }

    // Skopiowanie pozostałych elementów z lewej
    while (leftIndex < leftSize){
        arr[scalingIndex] = left[leftIndex];
        scalingIndex+=1;
        leftIndex+=1;
    }



    // Skopiowanie pozostałych elementów z prawej
    while (rightIndex < rightSize){
        arr[scalingIndex] = right[rightIndex];
        scalingIndex+=1;
        rightIndex+=1;
    }

}


void mergeSort(int *arr, int start, int end) {
    if (start < end) {
        //Podział tablicy na dwie części
        int middle = start + (end - start) / 2;
        mergeSort(arr, start, middle);
        mergeSort(arr, middle + 1, end);

        //Scalanie dwóch powstałych tablic w jedną.
        merge(arr, start, middle, end);
    }
}


int partition(int arr[], int start, int end)
{

    int pivot = arr[start];

    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }

    // Giving pivot element its correct position
    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);

    // Sorting left and right parts of the pivot element
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i] <= pivot) {
            i++;
        }

        while (arr[j] > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }

    return pivotIndex;
}

void quickSort(int arr[], int start, int end)
{

    // base case
    if (start >= end)
        return;

    // partitioning the array
    int p = partition(arr, start, end);

    // Sorting the left part
    quickSort(arr, start, p - 1);

    // Sorting the right part
    quickSort(arr, p + 1, end);
}



string odczyt(int liczba)
{
    ifstream file; // Declare file stream
    file.open("C:\\Users\\dawst\\CLionProjects\\Algorytmy2\\losowe.txt", ios::in);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        // You might want to handle the error or return an error value here
        return "";
    }

    int line = 0;
    string ans;

    while (line != liczba) {
        if (!getline(file, ans)) {
            cerr << "Error reading line " << liczba << " from file." << endl;
            // You might want to handle the error or return an error value here
            return "";
        }
        line++;
    }

    file.close(); // Close the file

    return ans;
}


vector<int> split(string x){
    char separator = ',';
    int i = 0;
    vector <int > vect = {};
    string s;
    while (x[i] != '\0') {
        if (x[i] != separator) {
            s += x[i];
        } else {
            vect.push_back(stoi(s));
            s.clear();
        }
        i++;

    }
    return vect;
}