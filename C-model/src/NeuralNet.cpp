#include "NeuralNet.h"

double NeuralNet::activation(double x)
{
    return 1 / (1 + exp(-x));
}

void NeuralNet::set_input(vector<vector<double>> input)
{
    if (input.size() != input_size)
    {
        throw invalid_argument("Wrong input size!");
    }
    for (size_t line = 0; line < input.size(); line++)
    {
        for (size_t idx = 0; idx < input[line].size(); idx++)
        {
            layers[0][line * input_size + idx] = input[line][idx];
        }
    }
}

void NeuralNet::set_expected(vector<double> input)
{
    if (input.size() != output_size)
    {
        throw invalid_argument("Wrong input size!");
    }
    expected = input;
}

void NeuralNet::count_neural_net(void)
{
    for (size_t layer = 0; layer <= layer_count; layer++)
    {
        for (size_t neuron = 0; neuron < weight[layer].size(); neuron++)
        {
            for (size_t input = 0; input < weight[layer][neuron].size(); input++)
            {
                layers[layer + 1][neuron] += layers[layer][input] * weight[layer][neuron][input];
            }
            layers[layer + 1][neuron] = activation(layers[layer + 1][neuron]);
        }
    }
}

void NeuralNet::clear_neural_net(void)
{
    for (size_t layer = 0; layer <= layer_count; layer++)
        for (size_t neuron = 0; neuron < layers[layer + 1].size(); neuron++)
            layers[layer + 1][neuron] = 0;
}

double NeuralNet::err(void)
{
    double e = 0;
    for (size_t exp = 0; exp <= output_size; exp++)
    {
        e += abs(expected[exp] - layers[layer_count + 1][exp]);
    }
    return e / 2;
}

void NeuralNet::recal_alpha(void)
{
    double e = err();
    double rel_e = 2 * abs(e) / output_size;
    alpha = rel_e * (MAX_ALPHA - MIN_ALPHA) + MIN_ALPHA;
}

void NeuralNet::adj_weight(void)
{
    for (size_t exp = 0; exp < output_size; exp++)
    {
        double t = expected[exp], y = layers[layer_count + 1][exp];
        delta[layer_count][exp] = y * (1 - y) * (t - y);
    }
    for (int layer = layer_count - 1; layer >= 0; layer--)
    {
        for (size_t input = 0; input < layers[layer + 1].size(); input++)
        {
            double next_sum = 0;
            for (size_t next_neuron = 0; next_neuron < layers[layer + 2].size(); next_neuron++)
            {
                next_sum += delta[layer + 1][next_neuron] * weight[layer + 1][next_neuron][input];
            }
            delta[layer][input] = layers[layer + 1][input] * (1 - layers[layer + 1][input]) * next_sum;
        }
    }
    for (size_t layer = 0; layer < layer_count + 1; layer++)
    {
        for (size_t output = 0; output < weight[layer].size(); output++)
        {
            for (size_t input = 0; input < weight[layer][output].size(); input++)
            {
                weight[layer][output][input] += alpha * delta[layer][output] * layers[layer][input];
            }
        }
    }
}

vector<double> NeuralNet::_res(void)
{
    return layers[layer_count + 1];
}

void NeuralNet::train(void)
{
    clear_neural_net();
    count_neural_net();
    recal_alpha();
    if (err() > MAX_ERR)
        adj_weight();
}

size_t NeuralNet::apply(void)
{
    clear_neural_net();
    count_neural_net();
    return distance(layers[layer_count + 1].begin(), max_element(layers[layer_count + 1].begin(), layers[layer_count + 1].end()));
}