#include "NeuralNet.h"

#include <fstream>
#include <filesystem>

using namespace std::filesystem;

int main(int argc, char *argv[])
{
    NeuralNet neuralNet{};
    vector<double> expected(3);
    vector<vector<double>> input(7, vector<double>(7));
    for (auto &train_path : directory_iterator("./input/"))
    {
        ifstream fin(train_path.path());
        fin >> expected[0] >> expected[1] >> expected[2];
        neuralNet.set_expected(expected);
        for (size_t i = 0; i < 7; i++)
            for (size_t j = 0; j < 7; j++)
                fin >> input[i][j];
        neuralNet.set_input(input);
        neuralNet.train();
    }
    return 0;
}