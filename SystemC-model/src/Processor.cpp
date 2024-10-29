#include "Processor.h"

Processor::Processor(sc_module_name nm)
    : sc_module(nm),
    clk_i("processor_clk"),
    activation("act"),
    controlUnit("cu"),
    ioController("ioc"),
    localMem("lm"),
    neuralCore("nc", slave_count),
    addr(local_mem_slave_count),
    data(local_mem_slave_count),
    wr(local_mem_slave_count),
    rd(local_mem_slave_count),
    data_cnt(slave_count),
    w_start_addr(slave_count),
    v_start_addr(slave_count),
    load(slave_count),
    busy(slave_count)
{
    // printf("Processor constructor\n");
    for (size_t i = 0; i < slave_count; ++i) {
        neuralCore[i].clk_i(clk_i);
    }

    activation.clk_i(clk_i);
    controlUnit.clk_i(clk_i);
    ioController.clk_i(clk_i);
    localMem.clk_i(clk_i);

    ioController.addr_o(addr[0]);
    controlUnit.addr_o(addr[0]);
    localMem.addr_i(addr[0]);
    ioController.data_io(data[0]);
    controlUnit.data_io(data[0]);
    localMem.data_io(data[0]);
    ioController.wr_o(wr[0]);
    controlUnit.wr_o(wr[0]);
    localMem.wr_i(wr[0]);
    ioController.rd_o(rd[0]);
    controlUnit.rd_o(rd[0]);
    localMem.rd_i(rd[0]);

    controlUnit.ioc_res_addr_o(ioc_res_addr);
    ioController.ioc_res_addr_i(ioc_res_addr);
    controlUnit.ioc_busy_i(ioc_busy);
    ioController.ioc_busy_o(ioc_busy);
    controlUnit.ioc_wr_o(ioc_wr);
    ioController.ioc_wr_i(ioc_wr);
    controlUnit.ioc_rd_o(ioc_rd);
    ioController.ioc_rd_i(ioc_rd);

    activation.act_data_io(act_data);
    controlUnit.act_data_io(act_data);
    activation.act_start_i(act_start);
    controlUnit.act_start_o(act_start);

    for (size_t i = 0; i < slave_count; ++i) {
        neuralCore[i].data_cnt_i(data_cnt[i]);
        controlUnit.data_cnt_o(data_cnt[i]);
        neuralCore[i].w_start_addr_i(w_start_addr[i]);
        controlUnit.w_start_addr_o(w_start_addr[i]);
        neuralCore[i].v_start_addr_i(v_start_addr[i]);
        controlUnit.v_start_addr_o(v_start_addr[i]);
        neuralCore[i].load_i(load[i]);
        controlUnit.load_o(load[i]);
        neuralCore[i].busy_o(busy[i]);
        controlUnit.core_busy_i(busy[i]);
        neuralCore[i].addr_o(addr[i + 1]);
        localMem.addr_i(addr[i + 1]);
        neuralCore[i].data_io(data[i + 1]);
        localMem.data_io(data[i + 1]);
        controlUnit.core_data_io(data[i + 1]);
        neuralCore[i].wr_o(wr[i + 1]);
        controlUnit.core_wr_i(wr[i + 1]);
        neuralCore[i].rd_o(rd[i + 1]);
        localMem.rd_i(rd[i + 1]);
    }
    
    SC_METHOD(start);
}

void Processor::start()
{
    // printf("Starting\n");
    controlUnit.start();
}