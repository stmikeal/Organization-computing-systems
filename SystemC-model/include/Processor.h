#ifndef _PROCESSOR_H
#define _PROCESSOR_H

#include "Activation.h"
#include "ControlUnit.h"
#include "IOController.h"
#include "LocalMem.h"
#include "NeuralCore.h"
#include "config.h"

SC_MODULE(Processor)
{
    sc_in<bool> clk_i;

    Activation activation;
    ControlUnit controlUnit;
    IOController ioController;
    LocalMem localMem;
    sc_vector<NeuralCore> neuralCore;

    SC_HAS_PROCESS(Processor);

    Processor(sc_module_name nm);
    ~Processor(){};

    void start();

    sc_signal<float> act_data;
    sc_signal<bool> act_start;

    sc_signal<bool> ioc_wr;
    sc_signal<bool> ioc_rd;
    sc_signal<size_t> ioc_res_addr;
    sc_signal<bool> ioc_busy;

    std::vector<sc_signal<size_t>> addr;
    std::vector<sc_signal<float>> data;
    std::vector<sc_signal<bool>> wr;
    std::vector<sc_signal<bool>> rd;

    std::vector<sc_signal<size_t>> data_cnt;
    std::vector<sc_signal<size_t>> w_start_addr;
    std::vector<sc_signal<size_t>> v_start_addr;
    std::vector<sc_signal<bool>> load;
    std::vector<sc_signal<bool>> busy;
};

#endif