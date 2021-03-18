#include <iostream>
#include <list>
#include <math.h>
#include <numeric>
#include <omp.h>
#include <fstream>
#include <string>


using namespace std;


list <int> separate_digits(int number){
    list <int> separated;

    while(number > 0){
        int digit = number % 10;
        number /= 10;
        separated.push_front(digit);
    }

    return separated;
}

int disarium_number(int number){
    list<int> digits = separate_digits(number);

    int digit = 0;
    int counter = 0;
    int i = 0;
    list <int>::iterator it = digits.begin();

    for(i = 0; i < digits.size(); i++) {
        digit++;
        counter = counter + pow(*it, digit);
        it++;
    }

    return counter;
}

void save_arrays_to_file(int checked[], int numbers[], int array_size){
    ofstream out_myfile ("./disarium.txt");

    for(int i=0; i < array_size; ++i){
        out_myfile << numbers[i] << " " << checked[i];
        out_myfile << '\n' ;
    }
    out_myfile.close();
}

int main(int argc, char* argv[]) {
    int thread_num;

    if(argc > 2){
        std::cout << "Usage: " << argv[0] << " thread_number";
        return 1;
    }
    if(argc == 2){
        thread_num = stoi(argv[1]);
    }
    double start, end;

    int array_size = 1000000;
    int checked[1000000] = {};
    int numbers[array_size];

    int i , st = 0;
    std::iota(numbers, numbers+array_size, st);

    start = omp_get_wtime();
    #pragma omp parallel for shared(numbers, checked) private(i) \
	num_threads(thread_num)
    for(i = 0; i < array_size; i++){
        //std::cout << omp_get_num_threads() << '\n' ;

        int disarium = disarium_number(numbers[i]);

        if ( disarium == i){
            checked[disarium] = 1;
        }
    }
    end = omp_get_wtime();
    std::cout<<"Time: " << end-start << endl;

    save_arrays_to_file(checked, numbers, array_size);

    return 0;
}