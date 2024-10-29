#include "NeuralCore.h"
#include "config.h"

NeuralCore::NeuralCore() : NeuralCore("nc") {};

NeuralCore::NeuralCore(sc_module_name nm)
    : sc_module(nm)
{
    // printf("NeuralCore constructor\n");

    state = LOAD_DATA;
    SC_THREAD(load_data);
    sensitive << clk_i.pos();

    SC_THREAD(process);
    sensitive << clk_i.pos();

    SC_THREAD(write_result);
    sensitive << clk_i.pos();
}

void NeuralCore::load_data()
{
    while (1)
    {
        rd_o.write(0);
        // printf("NeuralCore::load_data\n");
        while (state != LOAD_DATA || !load_i.read())
            wait();
        busy_o->write(1);
        size_t size = data_cnt_i->read();
        size_t w_addr = w_start_addr_i->read();
        size_t v_addr = v_start_addr_i->read();
        for (size_t i = 0; i < size; ++i)
        {
            addr_o->write(w_addr + i);
            rd_o->write(1);
            wait();
            rd_o->write(0);
            wait();
            wait(SC_ZERO_TIME);
            weight_queue.push(data_io->read());
            addr_o->write(v_addr + i);
            rd_o->write(1);
            wait();
            rd_o->write(0);
            wait();
            wait(SC_ZERO_TIME);
            value_queue.push(data_io->read());
        }
        state = PROCESS;
    }
}

void NeuralCore::process()
{
    while (1)
    {
        // printf("NeuralCore::process\n");
        while (state != PROCESS)
            wait();
        accumulator = 0;
        while (!weight_queue.empty() && !value_queue.empty())
        {
            for (size_t i = 0; i < neural_core_mul_size; ++i)
            {
                if (weight_queue.empty() || value_queue.empty())
                    break;
                // printf("%f ", value_queue.front());
                accumulator += weight_queue.front() * value_queue.front();
                weight_queue.pop();
                value_queue.pop();
            }
            // printf("\n");
            wait();
        }
        state = RESULT;
    }
}

void NeuralCore::write_result()
{
    while (1)
    {
        // printf("NeuralCore::write_result\n");
        while (state != RESULT)
            wait();
        data_io->write(accumulator);
        //printf("accumulator %f\n", accumulator);
        wr_o->write(1);
        wait();
        wr_o->write(0);
        wait();
        wait(SC_ZERO_TIME);
        busy_o->write(0);
        state = LOAD_DATA;
    }
}
