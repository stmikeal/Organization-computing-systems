#ifndef _ACTIVATION_H_
#define _ACTIVATION_H_

#include "systemc.h"

SC_MODULE(Activation)
{
    sc_in<bool> clk_i;
    sc_inout<float> act_data_io;
    sc_in<bool> act_start_i;

    SC_HAS_PROCESS(Activation);

    Activation(sc_module_name nm);
    ~Activation() {};

    void process();
};

#endif