#include <iostream>
#include <list>
#include <math.h>
#include <numeric>
#include <fstream>
#include <string>
#include <mpi.h>


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

//-----------------------------------------------------------
    int pid, np, count;

    MPI_Init(&argc, &argv);
    MPI_Status status;

    // find out process ID, and number of process
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Request *request = new MPI_Request[np];
    MPI_Request req;

    //int *localArray = (int *)malloc(count * sizeof(int));
    int *localArray;


    count = array_size / np;
    start = MPI_Wtime();

    int i , st = 0;
    std::iota(numbers, numbers+array_size, st);

    if (pid == 0) {

        for(int dest = 0; dest <= np; ++dest) {

            if(np == dest){
                MPI_Isend((numbers + ((np - 1) * count)), (count + (array_size % np)), MPI_INT, np - 1, 0, MPI_COMM_WORLD, &request[np-1]);
            }
            else{
                MPI_Isend(numbers + (dest * count), count, MPI_INT, dest, 0, MPI_COMM_WORLD, &request[dest]);
                printf("P0 sent a %d elements to P%d.\n", count, dest);
            }
        }

    }

    int loc;
    if (pid < np){
        loc = count;
        localArray = new int[loc];
        MPI_Recv(localArray, count, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for(i = pid*count; i < (pid+1)*count; i++){
            int disarium = disarium_number(localArray[i]);

            if ( disarium == i){
                checked[disarium] = 1;
                //std::cout << i << ' ' << disarium << '\n';
            }
        }
    }
    else{
        loc = count + (array_size % np);
        localArray = new int[loc];
        MPI_Recv(localArray, loc, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for(int i = 0; i < loc; i++){

        int disarium = disarium_number(localArray[i]);

        if ( disarium == i){
            checked[disarium] = 1;
            //std::cout << i << ' ' << disarium << '\n';
        }
    }

    end = MPI_Wtime();

    MPI_Finalize();

    std::cout<< end-start << endl;

    save_arrays_to_file(checked, numbers, array_size);

    return 0;
}