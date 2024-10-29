#include "LocalMem.h"

LocalMem::LocalMem(sc_module_name nm)
    : sc_module(nm),
      addr_i("lm_addr"),
      data_io("lm_data"),
      wr_i("lm_wr"),
      rd_i("lm_rd")
{
    // printf("LocalMem constructor\n");
    SC_THREAD(bus_write);
    sensitive << clk_i.pos();

    SC_THREAD(bus_read);
    sensitive << clk_i.pos();
}

void LocalMem::bus_read()
{
    while (1)
    {
        // printf("LocalMem::bus_read\n");
        for (size_t i = 0; i < 1; ++i)
        {
            if (wr_i[i]->read())
            {
                mem[addr_i[i]->read()] = data_io[i]->read();
            }
        }
        wait();
    }
}

void LocalMem::bus_write()
{
    while (1)
    {
        // printf("LocalMem::bus_write\n");
        for (size_t i = 0; i < local_mem_slave_count; ++i)
        {
            if (prepared_write_queue[i].first) {
                data_io[i]->write(prepared_write_queue[i].second);
                prepared_write_queue[i] = {false, 0};
            }
            if (rd_i[i]->read())
            {
                //if (i == 0)
                //printf("on addr %d value %f\n", addr_i[i]->read(), mem[addr_i[i]->read()]);
                prepared_write_queue[i] = {true, mem[addr_i[i]->read()]};
            }
        }
        wait();
    }
}