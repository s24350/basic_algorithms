#include <algorithm>
#include <experimental/random>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>

#include "file_reader.h"

int k = 3; //clusters

using table = std::vector<std::vector<std::string>>;
using clusters = std::vector<std::pair<std::vector<double>, int>>;

auto getHeaderFromFile(std::string const &path) -> std::string;

auto initialize(table const &dataTable) -> std::vector<std::pair<std::vector<double>, int>>;

auto createCentroids(clusters const &fullDataTable) -> std::vector<std::vector<double>>;

auto operator+(std::vector<double> const &x, std::vector<double> const &c) {
    auto size = x.size() < c.size() ? x.size() : c.size();
    auto added = std::vector<double>();
    for (int i = 0; i < size; ++i) {
        added.push_back(0);
        added[i] += x[i];
        added[i] += c[i];
    }
    return added;
}

auto createSingleCentroid(clusters const &fullDataTable, int size) -> std::vector<double>;

auto computeDistance(std::vector<double> const &the, std::vector<double> const &other) -> double;

auto reorganize(clusters &fullDataTable, std::vector<std::vector<double>> const &centroids) -> bool;

auto main() -> int {

    /**
     * this is dataset which is going to be clustered
     */
    auto filepath_in = std::string("../k-means/data_set.csv");
    auto dataTable = getDataFromFile(filepath_in);

    auto fullDataTable = initialize(dataTable);
    auto centroids = createCentroids(fullDataTable);

    while(reorganize(fullDataTable, centroids)){
        auto changedCentroids = createCentroids(fullDataTable);
        for (int i = 0; i < centroids.size(); ++i) {
            centroids[i]=changedCentroids[i];
        }
    }
    /**
     * output file is created
     */
    auto file = std::fstream("../k-means/data_clustered.csv", std::ios::out | std::ios::trunc);
    auto printer = getHeaderFromFile(filepath_in)+std::string(";cluster\n");
    for (auto v: fullDataTable) {
        printer+=fmt::format("{}",fmt::join(v.first, ";"))+fmt::format(";{}",v.second);
        fmt::println(file, "{}", printer);
        printer=std::string("");
    }
}
auto getHeaderFromFile(std::string const &path) -> std::string{
    auto line = std::string();
    auto file = std::fstream(path);
    std::getline(file, line);
    return line;
}

auto initialize(table const &dataTable) -> clusters {
    auto fullDataTable = clusters();
    for (auto vectorX: dataTable) {
        auto numericalX = std::vector<double>();
        for (auto e: vectorX) {
            numericalX.push_back(std::stod(e));
        }
        auto cluster = std::experimental::randint(1, k);
        fullDataTable.push_back(std::make_pair(numericalX,cluster));
    }
    return fullDataTable;
}

auto createCentroids(clusters const &fullDataTable) -> std::vector<std::vector<double>> {
    auto centroids = std::vector<std::vector<double>>();
    for (int i = 1; i <= k; ++i) {
        auto singleCentroid = createSingleCentroid(fullDataTable, i);
        centroids.push_back(singleCentroid);
    }
    return centroids;
}

auto createSingleCentroid(clusters const &fullDataTable, int i) -> std::vector<double> {
    auto singleCentroid = std::vector<double>(k);
    auto counter = 0;
    for (auto vectorX: fullDataTable) {
        if (vectorX.second == i) {
            singleCentroid = singleCentroid + vectorX.first;
            counter++;
        }
    }
    for (auto &c: singleCentroid) {
        c /= counter;
    }
    return singleCentroid;
}

auto reorganize(clusters &fullDataTable, std::vector<std::vector<double>> const &centroids) -> bool {
    bool clusteringHasChanged = false;
    for (auto &vectorX: fullDataTable) {
        auto distance = computeDistance(vectorX.first, centroids[vectorX.second-1]);
        for (int i = 0; i < centroids.size(); ++i) {
            if (computeDistance(vectorX.first, centroids[i]) < distance) {
                vectorX.second = i+1;
                clusteringHasChanged = true;
            }
        }
    }
    return clusteringHasChanged;
}

auto computeDistance(std::vector<double> const &the, std::vector<double> const &other) -> double {
    auto size = the.size() < other.size() ? the.size() : other.size();
    auto distance = double(0);
    for (int i = 0; i < size; ++i) {
        distance += pow(the[i] - other[i], 2.0);
    }
    return sqrt(distance);
}