#include <stddef.h>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>

using namespace std;

#define OUTPUT_SIZE 3
#define INPUT_SIZE 7
#define NEURON_SIZE 7
#define LAYER_COUNT 1
#define MIN_ALPHA 0.1
#define MAX_ALPHA 0.5

class NeuralNet
{
public:
    NeuralNet(size_t output_size = OUTPUT_SIZE, size_t input_size = INPUT_SIZE,
              size_t neuron_size = NEURON_SIZE, size_t layer_count = LAYER_COUNT)
        : output_size(output_size), input_size(input_size), neuron_size(neuron_size), layer_count(layer_count)
    {
        expected = vector<double>(output_size);

        layers = vector<vector<double>>(layer_count + 2);      // Appended input and output layer
        layers[0] = vector<double>(input_size * input_size);   // Input layer
        layers[layer_count + 1] = vector<double>(output_size); // Output layer
        for (int layer = 0; layer < layer_count; layer++)
        {
            layers[layer + 1] = vector<double>(neuron_size); // Inner layers
        }

        delta = vector<vector<double>>(layer_count + 1);
        layers[layer_count] = vector<double>(output_size);
        for (int layer = 0; layer < layer_count; layer++)
        {
            layers[layer] = vector<double>(neuron_size);
        }

        weight = vector<vector<vector<double>>>(layer_count + 1);
        weight[0] = vector<vector<double>>(neuron_size, vector<double>(input_size * input_size));
        weight[layer_count] = vector<vector<double>>(output_size, vector<double>(neuron_size));
        for (int w = 0; w < layer_count - 1; w++)
        {
            weight[w + 1] = vector<vector<double>>(neuron_size, vector<double>(neuron_size));
        }
    }

    void set_input(vector<vector<double>> input);
    void set_expected(vector<double> input);
    void train(void);
    double err(void);
    size_t apply(void);

private:
    double activation(double x);
    void count_neural_net(void);
    void clear_neural_net(void);
    void recal_alpha(void);
    void adj_weight(void);
    size_t output_size;
    size_t input_size;
    size_t neuron_size;
    size_t layer_count;
    double alpha;
    vector<double> expected;
    vector<vector<double>> layers;
    vector<vector<double>> delta;
    vector<vector<vector<double>>> weight;
};