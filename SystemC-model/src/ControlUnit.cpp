#include "ControlUnit.h"

ControlUnit::ControlUnit(sc_module_name nm)
    : sc_module(nm),
      neuron_addr_link(slave_count),
      local_layer(0),
      local_neuron(0),
      device_load(0),
      device_busy(0)
{
    // printf("ControlUnit constructor\n");
    for (size_t i = 0; i < slave_count; ++i)
        device_busy.push_back(false);

    SC_THREAD(load_data);
    sensitive << clk_i.pos();

    SC_THREAD(process);
    sensitive << clk_i.pos();

    SC_THREAD(result);
    sensitive << clk_i.pos();

    SC_THREAD(read_mem_mapped);
    sensitive << clk_i.pos();

    SC_THREAD(free_device);
    sensitive << clk_i.pos();
}

void ControlUnit::start()
{
    // cout << "ControlUnit::start" << endl;
    state = LOAD_DATA;
    for(size_t i = 0; i < slave_count; ++i)
        load_o[i]->write(0);
}

inline void ControlUnit::write_mem(float data, size_t addr)
{
    addr_o->write(addr);
    data_io->write(data);
    wr_o->write(1);
    wait();
    wr_o->write(0);
}

inline float ControlUnit::read_mem(size_t addr)
{
    addr_o->write(addr);
    rd_o->write(1);
    wait();
    rd_o->write(0);
    wait();
    wait(SC_ZERO_TIME);
    float value = data_io->read();
    return value;
}

void ControlUnit::load_data()
{
    while (1)
    {
        // cout << "ControlUnit::load_data" << endl;
        while (state != LOAD_DATA)
            wait();

        ioc_wr_o->write(1);
        wait();
        ioc_wr_o->write(0);
        while (!ioc_busy_i.read())
            wait();
        while (ioc_busy_i.read())
            wait();
        input_size = read_mem(0);
        output_size = read_mem(1);
        size_t size = read_mem(2);
        inner_size.reserve(size + 2);
        inner_size.resize(size + 2, 0);
        weight_local_addr = 3 + size;
        value_local_addr = 3 + size;

        inner_size[0] = input_size;
        for (size_t i = 0; i < size; ++i)
        {
            inner_size[i + 1] = read_mem(3 + i);
            value_local_addr += inner_size[i] * inner_size[i + 1];
        }
        inner_size[size + 1] = output_size;
        value_local_addr += inner_size[size] * output_size;
        // printf("value_local_addr %d\n", value_local_addr);
        state = PROCESS;
    }
}

void ControlUnit::process_mem_mapped()
{
    // printf("ControlUnit::process_mem_mapped");
    if (!mem_mapped.empty())
    {
        size_t device_to_save = mem_mapped.front().first;
        float value_to_save = mem_mapped.front().second;
        // printf("bef_value_to_save %f\n", value_to_save);
        // printf("Processed value of %d\n", device_to_save);
        mem_mapped.pop();
        act_data_io->write(value_to_save);
        act_start_o->write(1);
        wait();
        act_start_o->write(0);
        wait();
        wait(SC_ZERO_TIME);
        value_to_save = act_data_io->read();
        write_mem(value_to_save, neuron_addr_link[device_to_save]);
    }
}

void ControlUnit::process()
{
    while (1)
    {
        // printf("ControlUnit::process\n");
        while (state != PROCESS)
        {
            wait();
        }
        // printf("osize value %f\n", output_size);
        // printf("process started\n");
        while (local_layer != inner_size.size() - 1)
        {
            size_t device_to_start = slave_count;
                for (size_t i = 0; i < slave_count; ++i)
                    if (!device_busy[i])
                        device_to_start = i;
            if (device_to_start != slave_count)
            {
                // printf("Start working on %d\n", device_to_start);

                data_cnt_o[device_to_start]->write(inner_size[local_layer]);
                w_start_addr_o[device_to_start]->write(weight_local_addr);
                v_start_addr_o[device_to_start]->write(value_local_addr);
                load_o[device_to_start]->write(1);

                neuron_addr_link[device_to_start] = value_local_addr + inner_size[local_layer] + local_neuron;

                weight_local_addr += inner_size[local_layer];

                //printf("Neuron %d, layer %d, max %d\n", local_neuron, local_layer, inner_size[local_layer + 1]);
                if (++local_neuron == inner_size[local_layer + 1])
                {
                    // printf("value_local_addr %d\n", value_local_addr);
                    value_local_addr += inner_size[local_layer];
                    local_neuron = 0;
                    ++local_layer;
                }

                wait();
                ++device_load;
                device_busy[device_to_start] = true;
                wait();
                load_o[device_to_start]->write(0);
                
            }

            wait();

            process_mem_mapped();

            wait();
        }

        while (device_load)
        {
            process_mem_mapped();
            wait();
        }

        state = RESULT;
    }
}

void ControlUnit::result()
{
    while (1)
    {
        ioc_rd_o->write(0);
        while (state != RESULT)
            wait();
        // printf("ControlUnit::result\n");
        ioc_res_addr_o->write(value_local_addr);
        ioc_rd_o->write(1);
        wait();
        while (ioc_busy_i)
            wait();
    }
}

void ControlUnit::read_mem_mapped()
{
    while (1)
    {
        // printf("read_mem_mapped\n");
        for (size_t i = 0; i < slave_count; ++i)
            if (core_wr_i[i]->read()) {
                wait();
                mem_mapped.push({i, core_data_io[i]->read()});
            }
        wait();
    }
}

void ControlUnit::free_device()
{
    while (1)
    {
        // printf("free_device\n");
        for (size_t i = 0; i < slave_count; ++i)
            if (!core_busy_i[i]->read() && device_busy[i])
            {
                device_busy[i] = false;
                --device_load;
            }
        wait();
    }
}
