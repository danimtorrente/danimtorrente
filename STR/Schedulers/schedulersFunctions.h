#ifndef SF_H
#define SF_H

#include <iostream>
#include <vector>
#include <map>
using namespace std;

#define MAXIMUM_CHARACTERS_INLINE 100

class schedulers {
    private: 
        float UTILIZATION_FACTOR;
        float MAX_COST;
        int MIN_DEADLINE;
        int NUMBER_OF_TASKS;
        int h;
        int Ts;
        vector<float> c; // task cost
        vector<int> t; // task period
        vector<int> d; // task deadline
        multimap<int, int> orderRate; // tasks ordered from smaller period to max
        multimap<int, int> orderDead; // tasks ordered from smaller deadline to max
        multimap<int, int> orderCost; // tasks ordered from smaller cost to max

    public:
        schedulers();

        void read();

        int check();
        
        void cyclicScheduler();
        
        void rateMonotonic();

        void deadlineMonotonic();

        void EDF();     //Earliest Deadline First

        //utils

        bool g(int result, int y);

        void splitTasks();

        bool validateCyclicSchedule(int Ts);

        void printScheduleHeader(const string& schedulerName);
        
        void printScheduleEvent(int taskId, int start, int end);
        
        void printScheduleFooter(const string& schedulerName);
        
};

#endif