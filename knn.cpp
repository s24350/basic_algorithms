#include <algorithm>
#include <cmath>
#include <fstream>
#include <map>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "file_reader.h"

int k = 17; //knn hyper parameter

using case_ = std::pair<std::vector<double>, std::string>;

auto getDividedDataTable(std::string const &filepath) -> std::vector<case_>;

auto excludeClass(std::vector<std::string> vector) -> case_;

auto computeDistance(std::vector<double> const &the, std::vector<double> const &other) -> double;

auto getDistanceToCase(case_ const& case_testing, std::vector<case_> const& dataTable_div_training) -> std::vector<std::pair<double, case_>>;

auto getResult(std::map<std::string,int> const& map) -> std::string;

auto main() -> int {

    auto filepath_training = std::string("../knn/data_training.csv");
    auto filepath_testing = std::string("../knn/data_testing.csv");
    auto dataTable_div_training = getDividedDataTable(filepath_training);
    auto dataTable_div_testing = getDividedDataTable(filepath_testing);

    for (auto case_testing: dataTable_div_testing) {
        fmt::println("For case {} :", case_testing);
        auto distanceToCase = getDistanceToCase(case_testing, dataTable_div_training);
        auto map = std::map<std::string,int>();
        for (int i = 0; i < k; ++i) {
            map[distanceToCase[i].second.second]+=1;
        }
        fmt::println("result: {}\n", getResult(map));
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

auto computeDistance(std::vector<double> const &the, std::vector<double> const &other) -> double {
    auto size = the.size() < other.size() ? the.size() : other.size();
    auto distance = double(0);
    for (int i = 0; i < size; ++i) {
        distance += pow(the[i] - other[i], 2.0);
    }
    return sqrt(distance);
}

auto getDistanceToCase(case_ const& case_testing, std::vector<case_> const& dataTable_div_training) -> std::vector<std::pair<double, case_>> {
    auto distanceToCase = std::vector<std::pair<double, case_>>();
    for (auto case_training: dataTable_div_training) {
        auto d = computeDistance(case_testing.first, case_training.first);
        auto distanceToCaseInstance = std::pair<double, case_>();
        distanceToCaseInstance.first = d;
        distanceToCaseInstance.second = case_training;
        distanceToCase.push_back(distanceToCaseInstance);
    }
    std::ranges::sort(distanceToCase,{},[](std::pair<double, case_> c){
        return c.first;
    });
    return distanceToCase;
}

auto getResult(std::map<std::string,int> const& map) -> std::string{
    auto comparator = 0;
    auto result = std::string();
    fmt::println("{} nearest neighbours below: ",k);
    for(auto e : map){
        fmt::println("{}", e);
        if (e.second>comparator){
            result = e.first;
            comparator = e.second;
        }
    }
    return result;
}

