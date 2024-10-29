#ifndef _IO_CONTROLLER_H_
#define _IO_CONTROLLER_H_

#include "systemc.h"

SC_MODULE(IOController)
{
  sc_in<bool> clk_i;
  sc_out<size_t> addr_o;
  sc_inout<float> data_io;
  sc_out<bool> wr_o;
  sc_out<bool> rd_o;
  sc_in<bool> ioc_wr_i;
  sc_in<bool> ioc_rd_i;
  sc_in<size_t> ioc_res_addr_i;
  sc_out<bool> ioc_busy_o;

  SC_HAS_PROCESS(IOController);

  IOController(sc_module_name nm);
  ~IOController() {};

  void mem_write();
  void mem_read();

};

#endif