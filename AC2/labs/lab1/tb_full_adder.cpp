// Verilator Example
// Norbertas Kremeris 2021
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vfull_adder.h"

#define MAX_SIM_TIME 50
#define VERIF_START_TIME 7
vluint64_t sim_time = 0;
vluint64_t posedge_cnt = 0;

// Adder input interface transaction item class
class AdderInTx {
    public:
        uint32_t x_i;
        uint32_t y_i;
        uint32_t c_i;
};

// Adder output interface transaction item class
class AdderOutTx {
    public:
        uint32_t s_o;
        uint32_t c_o;
};

// Adder scoreboard
class AdderScb {
    private:
        std::deque<AdderInTx*> in_q;
        
    public:
        // Input interface monitor port
        void writeIn(AdderInTx *tx){
            // Push the received transaction item into a queue for later
            in_q.push_back(tx);
        }

        // Output interface monitor port
        void writeOut(AdderOutTx* tx){
            // We should never get any data from the output interface
            // before an input gets driven to the input interface
            if(in_q.empty()){
                std::cout <<"Fatal Error in AdderScb: empty AdderInTx queue" << std::endl;
                exit(1);
            }

            // Grab the transaction item from the front of the input item queue
            AdderInTx* in;
            in = in_q.front();
            in_q.pop_front();


            // Received transaction
            if ((in->x_i + in->y_i + in->c_i) != (tx->s_o + 2*tx->c_o)) {
                std::cout << std::endl;
                std::cout << "AdderScb: add mismatch" << std::endl;
                std::cout << "  Input: x_i = " << in->x_i << ", y_i = " << in->y_i << ", c_i = " << in->c_i << std::endl;
                std::cout << "  Expected: s_o = " << (in->x_i + in->y_i + in->c_i)%2 << ", c_o = " << (in->x_i + in->y_i + in->c_i)/2 << std::endl;
                std::cout << "  Actual:   s_o = " << tx->s_o << ", c_o = " << tx->c_o << std::endl;
                std::cout << "  Simtime: " << sim_time << std::endl;
            }

            // As the transaction items were allocated on the heap, it's important
            // to free the memory after they have been used
            delete in;
            delete tx;
        }
};

// Adder input interface driver
class AdderInDrv {
    private:
        Vfull_adder *dut;
    public:
        AdderInDrv(Vfull_adder *dut){
            this->dut = dut;
        }

        void drive(AdderInTx *tx){
            // Don't drive anything if a transaction item doesn't exist
            if(tx != NULL){
                dut->x_i = tx->x_i;
                dut->y_i = tx->y_i;
                dut->c_i = tx->c_i;
                // Release the memory by deleting the tx item
                // after it has been consumed
                delete tx;
            }
        }
};

// Adder input interface monitor
class AdderInMon {
    private:
        Vfull_adder *dut;
        AdderScb *scb;
    public:
        AdderInMon(Vfull_adder *dut, AdderScb *scb){
            this->dut = dut;
            this->scb = scb;
        }

        void monitor(){
            // Create a new AdderInTx transaction item and populate
            // it with data observed at the interface pins
            AdderInTx *tx = new AdderInTx();
            tx->x_i = dut->x_i;
            tx->y_i = dut->y_i;
            tx->c_i = dut->c_i;

            // then pass the transaction item to the scoreboard
            scb->writeIn(tx);
        }
};

// Adder output interface monitor
class AdderOutMon {
    private:
        Vfull_adder *dut;
        AdderScb *scb;
    public:
        AdderOutMon(Vfull_adder *dut, AdderScb *scb){
            this->dut = dut;
            this->scb = scb;
        }

        void monitor(){
            // Create a new AdderOutTx transaction item and populate
            // it with result observed at the interface pins
            AdderOutTx *tx = new AdderOutTx();
            tx->s_o = dut->s_o;
            tx->c_o = dut->c_o;

            // then pass the transaction item to the scoreboard
            scb->writeOut(tx);
        }
};

// Adder random transaction generator
// This will allocate memory for an AdderInTx
// transaction item, randomise the data, and
// return a pointer to the transaction item object
AdderInTx* rndAdderInTx(){
    //100% chance of generating a transaction
    if(true){
        AdderInTx *tx = new AdderInTx();
        tx->x_i = rand() % 2; // 0 or 1
        tx->y_i = rand() % 2; // 0 or 1
        tx->c_i = rand() % 2; // 0 or 1
        return tx;
    } else {
        return NULL;
    }
}



int main(int argc, char** argv, char** env) {
    srand (time(NULL));
    Verilated::commandArgs(argc, argv);
    Vfull_adder *dut = new Vfull_adder;

    Verilated::traceEverOn(true);
    VerilatedVcdC *m_trace = new VerilatedVcdC;
    dut->trace(m_trace, 5);
    m_trace->open("waveform.vcd");

    AdderInTx   *tx;

    // Here we create the driver, scoreboard, input and output monitor blocks
    AdderInDrv  *drv    = new AdderInDrv(dut);
    AdderScb    *scb    = new AdderScb();
    AdderInMon  *inMon  = new AdderInMon(dut, scb);
    AdderOutMon *outMon = new AdderOutMon(dut, scb);

    uint clk = 0;

    while (sim_time < MAX_SIM_TIME) {
        clk ^= 1;

        // Do all the driving on a positive edge
        if (clk == 1){

            if (sim_time >= VERIF_START_TIME) {
                // Generate a randomised transaction item of type AluInTx
                tx = rndAdderInTx();

                // Pass the transaction item to the Adder input interface driver,
                // which drives the input interface based on the info in the
                // transaction item
                drv->drive(tx);

                // Monitor the input interface
                inMon->monitor();
            }
        }

        // Evaluate the design
        dut->eval();

        // Do all the monitoring on a positive edge

        if (clk == 1){
            if (sim_time >= VERIF_START_TIME) {
                // Monitor the output interface
                outMon->monitor();
            }
        }
        // end of positive edge processing

        m_trace->dump(sim_time);
        sim_time++;
    }

    m_trace->close();
    delete dut;
    delete outMon;
    delete inMon;
    delete scb;
    delete drv;
    exit(EXIT_SUCCESS);
}
