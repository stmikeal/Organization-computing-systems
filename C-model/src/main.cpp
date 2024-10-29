#include "NeuralNet.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <time.h>
#pragma comment( lib, "Pdh.lib" )

using namespace std::filesystem;

#define LEARNING_BARRIER 0.001
#define MAX_BARRIER 3
#define MAX_ERR 0.1

int main(int argc, char *argv[])
{
    NeuralNet neuralNet;
    vector<double> expected(3);
    vector<vector<double>> input(7, vector<double>(7));
    double old_mean_err = 10;
    double count_barrier = 0;
    double mean_err;
    int epoch;
    clock_t start = clock();
    for (epoch = 0; epoch < 50; epoch++)
    {
        mean_err = 0;
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
        mean_err /= mean_count;
        if (abs(old_mean_err - mean_err) < LEARNING_BARRIER || mean_err > old_mean_err)
            count_barrier++;
        else
            count_barrier = 0;
        old_mean_err = mean_err;
        if (count_barrier == MAX_BARRIER && mean_err < MAX_ERR) {
            break;
        }
    }
    cout << "Train stopped on " << epoch << " epoch, time: " << (double)(clock() - start)/CLOCKS_PER_SEC << endl;
    cout << "Mean err on train is " << mean_err << endl;

    mean_err = 0;
    int mean_count = 0;
    start = clock();
    for (auto &val_path : directory_iterator("./input/validate"))
    {
        ifstream fin(val_path.path());
        fin >> expected[0] >> expected[1] >> expected[2];
        neuralNet.set_expected(expected);
        for (size_t i = 0; i < 7; i++)
            for (size_t j = 0; j < 7; j++)
                fin >> input[i][j];
        neuralNet.set_input(input);
        size_t classified = neuralNet.apply();
        if (!expected[classified]) {
            cout << "Error on image " << val_path.path() << endl;
            cout << "Expected: ";
            if (expected[0]) 
                cout << "Circle" << endl;
            else if (expected[1]) 
                cout << "Square" << endl;
            else
                cout << "Triangle" << endl;
            cout << "Actual: ";
            if (classified == 0) 
                cout << "Circle" << endl;
            else if (classified == 1) 
                cout << "Square" << endl;
            else
                cout << "Triangle" << endl;
        } else {
            cout << "Success on image " << val_path.path() << endl;
        }
        cout << "Circle accuracy: " << neuralNet._res()[0] << endl;;
        cout << "Square accuracy: " << neuralNet._res()[1] << endl;;
        cout << "Triangle accuracy: " << neuralNet._res()[2] << endl;;
        mean_err += neuralNet.err();
        mean_count++;
    }

    mean_err /= mean_count;
    cout << "Validation time: " << (double)(clock() - start)/CLOCKS_PER_SEC << endl;
    cout << "Mean err on validation is " << mean_err << endl;

    neuralNet._print_w();

    return 0;
}
