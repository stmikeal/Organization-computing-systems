#include "NeuralNet.h"

#include <fstream>
#include <filesystem>
#include <iostream>

using namespace std::filesystem;

int main(int argc, char *argv[])
{
    NeuralNet neuralNet;
    vector<double> expected(3);
    vector<vector<double>> input(7, vector<double>(7));
    double old_mean_err = 0;
    for (int i = 0; i < 1000; i++)
    {
        double mean_err = 0;
        int mean_count = 0;
        for (auto &train_path : directory_iterator("./input/train"))
        {
            ifstream fin(train_path.path());
            fin >> expected[0] >> expected[1] >> expected[2];
            neuralNet.set_expected(expected);
            for (size_t i = 0; i < 7; i++)
                for (size_t j = 0; j < 7; j++)
                    fin >> input[i][j];
            neuralNet.set_input(input);
            neuralNet.train();
            mean_err += neuralNet.err();
            mean_count++;
        }
    }
    return 0;
}