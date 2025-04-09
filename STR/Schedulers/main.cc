#include <iostream>
#include <vector>
#include "customFunctions.h"
#include "schedulersFunctions.h"
//#include <stdio.h>
//#include <stdlib.h>
using namespace std;


int main (int argc, char *argv[]) {

    schedulers rtos;
    rtos.read(); // read input.txt file and insert value in c, t and d 
    if (rtos.check() < 0) return -1;
    rtos.rateMonotonic();
    cout << endl;
    rtos.deadlineMonotonic();
    cout << endl;
    rtos.EDF();
    cout << endl;
    //rtos.cyclicScheduler(order, vector<int>& currentSequence, int *bestCost, vector<int>& bestSequence, vector<bool>& used);
    return 0;
}