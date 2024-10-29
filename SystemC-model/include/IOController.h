#ifndef _IO_CONTROLLER_H_
#define _IO_CONTROLLER_H_

#include "systemc.h"

SC_MODULE(IOController)
{
  sc_port<sc_signal_in_if<bool>> clk_i;
  sc_port<sc_signal_out_if<size_t>> addr_o;
  sc_port<sc_signal_inout_if<float>> data_io;
  sc_port<sc_signal_out_if<bool>> wr_o;
  sc_port<sc_signal_out_if<bool>> rd_o;
  sc_port<sc_signal_in_if<bool>> ioc_wr_i;
  sc_port<sc_signal_in_if<bool>> ioc_rd_i;
  sc_port<sc_signal_in_if<bool>> ioc_res_addr_i;
  sc_port<sc_signal_out_if<bool>> ioc_busy_o;

  SC_HAS_PROCESS(IOController);

  IOController(sc_module_name nm);
  ~IOController() {};

  void mem_write();
  void mem_read();

};

#endif