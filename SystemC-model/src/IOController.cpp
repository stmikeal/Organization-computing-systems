#include "IOController.h"

#include <iostream>
#include <fstream>
#include <string>

IOController::IOController(sc_module_name nm)
    : sc_module(nm)
{
    // printf("IOController constructor\n");
    SC_THREAD(mem_write);
    sensitive << clk_i.pos();

    SC_THREAD(mem_read);
    sensitive << clk_i.pos();
}

void IOController::mem_write()
{
    while (1)
    {
        // printf("IOController::mem_write\n");
        while (!ioc_wr_i.read())
            wait();
        ioc_busy_o->write(1);

        std::ifstream ifs;
        ifs.open("input/mem.txt");

        size_t addr = 0;
        while (ifs.is_open() && !ifs.eof())
        {
            float cell;
            ifs >> cell;
            addr_o->write(addr++);
            data_io->write(cell);
            wr_o->write(1);
            wait();
            wr_o->write(0);
        }
        wait();
        ioc_busy_o->write(0);
    }
}

void IOController::mem_read()
{
    while (1)
    {
        while (!ioc_rd_i.read())
            wait();
        ioc_busy_o.write(1);
        size_t addr = ioc_res_addr_i->read();
        //printf("addr: %d", addr);
        addr_o->write(addr);
        rd_o->write(1);
        wait();
        rd_o->write(0);
        wait();
        wait(SC_ZERO_TIME);
        printf("Circle result: %f\n", data_io->read());
        addr_o->write(addr + 1);
        rd_o->write(1);
        wait();
        rd_o->write(0);
        wait();
        wait(SC_ZERO_TIME);
        printf("Square result: %f\n", data_io->read());
        addr_o->write(addr + 2);
        rd_o->write(1);
        wait();
        rd_o->write(0);
        wait();
        wait(SC_ZERO_TIME);
        printf("Triangle result: %f\n", data_io->read());
        ioc_busy_o.write(0);
        sc_stop();
    }
}
