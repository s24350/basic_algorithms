#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "file_reader.h"

using table = std::vector<std::vector<std::string>>;

auto getPossibleTargetVariable(table const &dataTable_training) -> std::set<std::string>;

auto computeBaseProbability(table const &dataTable_training) -> std::vector<std::pair<std::string, double>>;

auto
computeSingleTargetProbability(table const &dataTable_training, std::string const &target,
                               int targetAnswersNo) -> double;

auto computeProbabilities(std::vector<std::pair<std::string, double>> probabilities,
                          std::vector<std::string> const &row_testing,
                          table const &dataTable_training) -> std::vector<std::pair<std::string, double>>;

auto countCases(int i, table const& dataTable_training) -> int;

auto main() -> int {

    auto filepath_training = std::string("../bayes/data_training.csv");
    auto filepath_testing = std::string("../bayes/data_testing.csv");
    auto dataTable_training = getDataFromFile(filepath_training);
    auto dataTable_testing = getDataFromFile(filepath_testing);

    auto targetProbabilities = computeBaseProbability(dataTable_training);
    fmt::println("targetProbabilities={}", targetProbabilities);
    for (auto row_testing: dataTable_testing) {
        auto probabilities = computeProbabilities(targetProbabilities, row_testing, dataTable_training);
        fmt::println("overallProbabilities={}", probabilities);
    }
}

auto getPossibleTargetVariable(table const &dataTable_training) -> std::set<std::string> {
    auto targetVariables = std::set<std::string>();
    for (auto row: dataTable_training) {
        auto it = row.end() - 1;
        targetVariables.insert(*it);
    }
    return targetVariables;
}

auto computeBaseProbability(table const &dataTable_training)
-> std::vector<std::pair<std::string, double>> {
    auto probabilities = std::vector<std::pair<std::string, double>>();
    auto targetVariables = getPossibleTargetVariable(dataTable_training);
    for (auto target: targetVariables) {
        auto singleProbability = computeSingleTargetProbability(dataTable_training, target, targetVariables.size());
        probabilities.push_back(std::make_pair(target, singleProbability));
    }
    return probabilities;
}

//targetAnswersNo is used for LaPlace
auto
computeSingleTargetProbability(table const &dataTable_training, std::string const &target,
                               int targetAnswersNo) -> double {
    auto counter = 1;
    auto rowsCount = dataTable_training.size();
    for (auto row: dataTable_training) {
        if (*(row.end() - 1) == target) {
            counter++;
        }
    }
    return ((double) counter / (rowsCount + targetAnswersNo));
}

auto computeProbabilities(std::vector<std::pair<std::string, double>> probabilities,
                          std::vector<std::string> const &row_testing,
                          table const &dataTable_training) -> std::vector<std::pair<std::string, double>> {
    for (auto &item: probabilities) {
        for (int i = 0; i < row_testing.size(); ++i) {
            auto targetCounter = countCases(i,dataTable_training); //bias is meant to implement LaPlace
            auto attributeInstanceCounter = 1;
            for (auto row_training: dataTable_training) {
                auto it = row_training.end() - 1;
                if (*it == item.first) {
                    targetCounter++;
                    if (row_testing[i] == row_training[i]) {
                        attributeInstanceCounter++;
                    }
                }
            }
            item.second*=((double)attributeInstanceCounter/(double)targetCounter);
        }
    }
    return probabilities;
}

auto countCases(int i, table const& dataTable_training) -> int{
    auto options = std::set<std::string>();
    for(auto row_training : dataTable_training){
        options.insert(row_training[i]);
    }
    return options.size();
}