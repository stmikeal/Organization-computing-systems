#ifndef _CONTROL_UNIT_H_
#define _CONTROL_UNIT_H_

#include "systemc.h"
#include "config.h"
#include <vector>
#include <queue>

SC_MODULE(ControlUnit)
{
  sc_in<bool> clk_i;
  sc_out<size_t> addr_o;
  sc_inout<float> data_io;
  sc_out<bool> wr_o;
  sc_out<bool> rd_o;

  sc_inout<float> act_data_io;
  sc_out<bool> act_start_o;

  sc_out<bool> ioc_wr_o;
  sc_out<bool> ioc_rd_o;
  sc_out<size_t> ioc_res_addr_o;
  sc_in<bool> ioc_busy_i;

  sc_port<sc_signal_in_if<bool>, slave_count> core_wr_i;
  sc_port<sc_signal_in_if<float>, slave_count> core_data_io;
  sc_port<sc_signal_in_if<bool>, slave_count> core_busy_i;

  sc_port<sc_signal_out_if<size_t>, slave_count> data_cnt_o;
  sc_port<sc_signal_out_if<size_t>, slave_count> w_start_addr_o;
  sc_port<sc_signal_out_if<size_t>, slave_count> v_start_addr_o;
  sc_port<sc_signal_out_if<bool>, slave_count> load_o;

  sc_event load_data_event;
  sc_event process_event;
  sc_event result_event;

  SC_HAS_PROCESS(ControlUnit);

  ControlUnit(sc_module_name nm);
  ~ControlUnit(){};

  void start();
  void load_data();
  void process();
  void result();

  void read_mem_mapped();
  void free_device();

private:
  inline void write_mem(float data, size_t addr);
  inline float read_mem(size_t addr);

  inline void process_mem_mapped();

  enum {
    START = 0,
    LOAD_DATA = 1,
    PROCESS = 2,
    RESULT = 3,
  } state;

  std::queue<std::pair<size_t, float>> mem_mapped;
  std::vector<bool> device_busy;
  size_t weight_local_addr;
  size_t value_local_addr;
  size_t local_layer;
  size_t local_neuron;
  size_t device_load;
  std::vector<size_t> neuron_addr_link;
  float input_size;
  float output_size;
  std::vector<float> inner_size;
};

#endif