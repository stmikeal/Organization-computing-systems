#include "LocalMem.h"

LocalMem::LocalMem(sc_module_name nm)
    : sc_module(nm)
{
    SC_METHOD(bus_write);
    sensitive << clk_i->posedge_event();
    dont_initialize();

    SC_METHOD(bus_read);
    sensitive << clk_i->posedge_event();
    dont_initialize();
}

void LocalMem::bus_read()
{
    for (size_t i = 0; i < local_mem_slave_count; ++i)
    {
        if (wr_i[i]->read())
        {
            mem[addr_i[i]->read()] = data_io[i]->read();
        }
    }
}

void LocalMem::bus_write()
{
    for (size_t i = 0; i < local_mem_slave_count; ++i)
    {
        data_io[i]->write(prepared_write_queue[i]);
        prepared_write_queue[i] = 0;
        if (rd_i[i]->read())
        {
            prepared_write_queue[i] = mem[addr_i[i]->read()];
        }
    }
}