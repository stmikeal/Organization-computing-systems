#include "IOController.h"

#include <iostream>
#include <fstream>
#include <string>

IOController::IOController(sc_module_name nm)
    : sc_module(nm)
{
    SC_METHOD(mem_write);
    sensitive << ioc_wr_i->posedge_event();
    dont_initialize();

    SC_METHOD(mem_read);
    sensitive << ioc_rd_i->posedge_event();
    dont_initialize();
}

void IOController::mem_write()
{
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
        wait(clk_i->posedge_event());
        wr_o->write(0);
    }
    
    ioc_busy_o->write(0);
}

void IOController::mem_read()
{
    size_t addr = ioc_res_addr_i->read();
    addr_o->write(addr++);
    rd_o->write(1);
    wait(clk_i->posedge_event());
    std::cout << "Circle result: " <<  data_io->read() << std::endl;
    addr_o->write(addr++);
    rd_o->write(1);
    wait(clk_i->posedge_event());
    std::cout << "Square result: " <<  data_io->read() << std::endl;
    addr_o->write(addr++);
    rd_o->write(1);
    wait(clk_i->posedge_event());
    std::cout << "Triangle result: " <<  data_io->read() << std::endl;
    sc_stop();
}
