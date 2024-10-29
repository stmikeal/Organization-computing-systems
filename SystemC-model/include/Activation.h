#ifndef _ACTIVATION_H_
#define _ACTIVATION_H_

#include "systemc.h"

SC_MODULE(Activation)
{
    sc_port<sc_signal_in_if<bool>> clk_i;
    sc_port<sc_signal_inout_if<float>> act_data_io;
    sc_port<sc_signal_in_if<bool>> act_start_i;

    SC_HAS_PROCESS(Activation);

    Activation(sc_module_name nm);
    ~Activation() {};

    void process();
};

#endif