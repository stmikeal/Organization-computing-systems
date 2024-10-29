#include "NeuralCore.h"
#include "config.h"

NeuralCore::NeuralCore() : NeuralCore("nc") {};

NeuralCore::NeuralCore(sc_module_name nm)
    : sc_module(nm)
{
    SC_METHOD(load_data);
    sensitive << load_i->posedge_event();
    dont_initialize();

    SC_METHOD(process);
    sensitive << process_event;
    dont_initialize();

    SC_METHOD(write_result);
    sensitive << write_result_event;
    dont_initialize();
}

void NeuralCore::load_data()
{
    busy_o->write(1);
    size_t size = data_cnt_i->read();
    size_t w_addr = w_start_addr_i->read();
    size_t v_addr = v_start_addr_i->read();
    for (size_t i = 0; i < size; ++i)
    {
        addr_o->write(w_addr + i);
        rd_o->write(1);
        wait(clk_i->posedge_event());
        rd_o->write(0);
        weight_queue.push(data_io->read());
        addr_o->write(v_addr + i);
        rd_o->write(1);
        wait(clk_i->posedge_event());
        rd_o->write(0);
        value_queue.push(data_io->read());
    }
    process_event.notify();
}

void NeuralCore::process()
{
    accumulator = 0;
    while (!weight_queue.empty() && !value_queue.empty())
    {
        for (size_t i = 0; i < neural_core_mul_size; ++i)
        {
            if (weight_queue.empty() || value_queue.empty())
                break;
            accumulator += weight_queue.front() * value_queue.front();
            weight_queue.pop();
            value_queue.pop();
        }
        wait(clk_i->posedge_event());
    }
    process_event.notify();
}

void NeuralCore::write_result()
{
    data_io->write(accumulator);
    rd_o->write(1);
    wait(clk_i->posedge_event());
    rd_o->write(0);
    busy_o->write(0);
}
