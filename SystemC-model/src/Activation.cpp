#include "Activation.h"

Activation::Activation(sc_module_name nm)
    : sc_module(nm)
{
    SC_METHOD(process);
    sensitive << act_start_i->posedge_event();
    dont_initialize();
}

void Activation::process()
{
    float value = act_data_io->read();
    wait(clk_i->posedge_event());
    act_data_io->write(1.0/(exp(-value) + 1));
}
