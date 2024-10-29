#ifndef _MEM_H
#define _MEM_H

#include "systemc.h"
#include "config.h"

SC_MODULE(LocalMem)
{
    sc_port<sc_signal_in_if<size_t>, local_mem_slave_count> addr_i;
    sc_port<sc_signal_inout_if<float>, local_mem_slave_count> data_io;
    sc_port<sc_signal_in_if<bool>, 1> wr_i;
    sc_port<sc_signal_in_if<bool>, local_mem_slave_count> rd_i;
    sc_in<bool> clk_i;

    SC_HAS_PROCESS(LocalMem);

    LocalMem(sc_module_name nm);
    ~LocalMem(){};

    void bus_write();
    void bus_read();

private:
    float mem[local_mem_size];
    std::pair<bool, float> prepared_write_queue[local_mem_slave_count];
};

#endif