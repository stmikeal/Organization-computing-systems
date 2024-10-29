#include "Activation.h"

Activation::Activation(sc_module_name nm)
    : sc_module(nm),
      clk_i("activation_clk"),
      act_data_io("activation_data"),
      act_start_i("activation_start")
{
    // printf("Activation constructor\n");
    SC_THREAD(process);
    sensitive << clk_i.pos();
}

void Activation::process()
{
    while (1)
    {
        // printf("Activation::process\n");
        if (act_start_i.read())
        {
            float value = act_data_io->read();
            wait();
            act_data_io->write(1.0 / (exp(-value) + 1));
        }
        wait();
    }
}
