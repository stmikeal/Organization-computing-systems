#ifndef _NEURAL_CORE_H_
#define _NEURAL_CORE_H_

#include "systemc.h"
#include <queue>

SC_MODULE(NeuralCore)
{
    sc_port<sc_signal_in_if<bool>> clk_i;
    sc_port<sc_signal_in_if<size_t>> data_cnt_i;
    sc_port<sc_signal_in_if<size_t>> w_start_addr_i;
    sc_port<sc_signal_in_if<size_t>> v_start_addr_i;
    sc_port<sc_signal_in_if<bool>> load_i;
    sc_port<sc_signal_out_if<bool>> busy_o;

    sc_port<sc_signal_out_if<size_t>> addr_o;
    sc_port<sc_signal_inout_if<float>> data_io;
    sc_port<sc_signal_out_if<bool>> wr_o;
    sc_port<sc_signal_out_if<bool>> rd_o;

    sc_event process_event;
    sc_event write_result_event;

    std::queue<float> weight_queue;
    std::queue<float> value_queue;

    SC_HAS_PROCESS(NeuralCore);
    NeuralCore();
    NeuralCore(sc_module_name nm);
    ~NeuralCore() {};

    void process();
    void write_result();
    void load_data();

private:
    float accumulator;
};

#endif