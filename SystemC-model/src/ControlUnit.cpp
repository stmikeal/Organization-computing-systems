#include "ControlUnit.h"

ControlUnit::ControlUnit(sc_module_name nm)
    : sc_module(nm),
      neuron_addr_link(slave_count),
      local_layer(0),
      local_neuron(0),
      device_load(0)
{
    // TODO

    for (size_t i = 0; i < slave_count; ++i)
        device_busy.push(i);

    SC_METHOD(load_data);
    sensitive << clk_i->posedge_event();
    dont_initialize();

    SC_METHOD(process);
    sensitive << clk_i->posedge_event();

    SC_METHOD(result);
    sensitive << clk_i->posedge_event();
    dont_initialize();

    SC_METHOD(read_mem_mapped);
    sensitive << core_wr_i->posedge_event();
    dont_initialize();

    SC_METHOD(read_mem_mapped);
    sensitive << core_busy_i->posedge_event();
    dont_initialize();
}

void ControlUnit::start()
{
    load_data_event.notify();
}

inline void ControlUnit::write_mem(float data, size_t addr)
{
    addr_o->write(addr);
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
    float value = data_io->read();
    return value;
}

void ControlUnit::load_data()
{
    wait(load_data_event);

    ioc_wr_o->write(1);
    wait(ioc_busy_i->negedge_event());

    input_size = read_mem(0);
    output_size = read_mem(1);
    size_t size = read_mem(2);
    inner_size.reserve(size + 2); 
    inner_size.resize(size + 2, 0);
    weight_local_addr = 3 + size; 
    value_local_addr = 3 + size;

    size_t prev_level = input_size;
    inner_size[0] = input_size;
    for (size_t i = 0; i < size; ++i)
    {
        inner_size[i + 1] = read_mem(3 + i);
        value_local_addr += prev_level + inner_size[i + 1];
        prev_level = inner_size[i + 1]; 
    }
    inner_size[size + 1] = output_size;
    value_local_addr += prev_level * output_size;
    process_event.notify();
}

void ControlUnit::process_mem_mapped()
{
    if (!mem_mapped.empty())
    {
        size_t device_to_save = mem_mapped.front().first;
        size_t value_to_save = mem_mapped.front().second;
        mem_mapped.pop();
        act_data_io->write(value_to_save);
        act_start_o->write(1);
        wait(act_data_io->event());
        act_start_o->write(0);
        value_to_save = act_data_io->read();
        write_mem(value_to_save, neuron_addr_link[device_to_save]);
    }
}

void ControlUnit::process()
{
    wait(process_event);

    while (local_layer != inner_size.size() - 1)
    {
        size_t device_to_start = device_busy.front();
        device_busy.pop();

        data_cnt_o[device_to_start]->write(inner_size[local_layer]);
        w_start_addr_o[device_to_start]->write(weight_local_addr);
        v_start_addr_o[device_to_start]->write(value_local_addr);
        load_o[device_to_start]->write(1);

        neuron_addr_link[device_to_start] = value_local_addr + inner_size[local_layer] + local_neuron;

        weight_local_addr += inner_size[local_layer];
        if (++local_neuron == inner_size[local_layer + 1])
        {
            value_local_addr += inner_size[local_layer];
            local_neuron = 0;
            ++local_layer;
        }

        ++device_load;

        wait();

        process_mem_mapped();

        wait();
    }

    while(device_load) {
        process_mem_mapped();
        wait();
    }

    result_event.notify();
}

void ControlUnit::result()
{
    wait(result_event);
    size_t addr = 3 + inner_size.size() + input_size * (inner_size[0] + 1) + output_size * inner_size[inner_size.size() - 1];
    for (size_t i = 0; i < inner_size.size(); ++i)
        addr += inner_size[i];
    for (size_t i = 0; i < inner_size.size() - 1; ++i)
        addr += inner_size[i] * inner_size[i + 1];
    ioc_res_addr_o->write(addr);
    ioc_rd_o->write(1);
    wait(ioc_busy_i->negedge_event());
}

void ControlUnit::read_mem_mapped()
{
    for (size_t i = 0; i < slave_count; ++i)
        if (core_wr_i[i]->read())
            mem_mapped.push({i, core_data_io[i]->read()});
}

void ControlUnit::free_device()
{
    for (size_t i = 0; i < slave_count; ++i)
        if (core_busy_i[i]->read()) {
            device_busy.push(i);
            --device_load;
        }
}
