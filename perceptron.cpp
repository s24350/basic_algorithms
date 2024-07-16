#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "file_reader.h"

double alpha = 0.1; //learning rate
double beta = 0.1; //threshold learning rate
double th_ = 0.1;

auto zero_class_name = std::string("setosa");//not active
auto first_class_name = std::string("virginica");//active

using case_ = std::pair<std::vector<double>, std::string>;

struct Perceptron {
    std::vector<double> weights;
    double theta; //threshold value
    std::string zeroClassName;
    std::string firstClassName;

    Perceptron(std::vector<double> const &w, double th, std::string const &zeroClassName,
               std::string const &firstClassName)
            : weights(w), theta(th), zeroClassName(zeroClassName), firstClassName(firstClassName) {}

    static auto initialize(int size) -> Perceptron {
        auto w = std::vector<double>();
        for (int i = 0; i < size; ++i) {
            w.push_back(1.0 / size);
        }
        return Perceptron(w, th_, zero_class_name, first_class_name);
    };

    auto dotProduct(std::vector<double> const &x, std::vector<double> const &w) -> double {
        auto size = x.size() < w.size() ? x.size() : w.size();
        auto scalar = double(0);
        for (int i = 0; i < size; ++i) {
            scalar += (x[i] * w[i]);
        }
        return scalar;
    }

    auto computeOutput(case_ const &c) -> std::string {
        //activate - 1 - firstClassName | not activate - 0 - zeroClassName
        return dotProduct(c.first, weights) >= theta ? firstClassName : zeroClassName;
    }

    auto learn(case_ const& c) -> void {
        auto d = 0;
        auto y = 0;
        if (c.second == firstClassName) {
            d = 1;
        }
        if (computeOutput(c) == firstClassName) {
            y = 1;
        }

        auto size = weights.size();
        //if perceptron computed output is different from value training set than (d-y)!=0
        for (int i = 0; i < size; ++i) {
            weights[i] += ((d - y) * c.first[i] * alpha);
        }
        normalizeVector(weights);
        //adapt threshold
        theta+=(d - y)*(-1)*beta;
    }

    auto normalizeVector(std::vector<double> & vector)-> void{
        auto length = getLength(vector);
        for(auto & e : vector){
            e/=length;
        }
    }

    auto getLength(std::vector<double> const& vector) -> double {
        double length = 0.0;
        for(auto e : vector){
            length+= pow(e,2);
        }
        return sqrt(length);
    }
};

auto getDividedDataTable(std::string const &filepath) -> std::vector<case_>;

auto excludeClass(std::vector<std::string> vector) -> case_;

auto main() -> int {

    auto filepath_training = std::string("../perceptron/data_training.csv");
    auto filepath_testing = std::string("../perceptron/data_testing.csv");
    auto dataTable_div_training = getDividedDataTable(filepath_training);
    auto dataTable_div_testing = getDividedDataTable(filepath_testing);

    auto size = dataTable_div_training[0].first.size();//size of first case in training set

    auto perceptron = Perceptron::initialize(size);

    for(auto case_training : dataTable_div_training){
        perceptron.learn(case_training);
    }
    for(auto case_testing : dataTable_div_testing){
        auto output = perceptron.computeOutput(case_testing);
        fmt::println("testing set expected value is: {}", case_testing.second);
        fmt::println("perceptron output is: {}", output);
    }
}

auto getDividedDataTable(std::string const &filepath) -> std::vector<case_> {
    auto dataTable = getDataFromFile(filepath);
    auto dataTable_div = std::vector<std::pair<std::vector<double>, std::string>>();
    for (auto e: dataTable) {
        dataTable_div.push_back(excludeClass(e));
    }
    return dataTable_div;
}



auto excludeClass(std::vector<std::string> vector) -> case_ {
    auto pair = case_();
    pair.second = *(vector.end() - 1);
    vector.pop_back();
    for (auto item: vector) {
        pair.first.push_back(std::stod(item));
    }
    return pair;
}