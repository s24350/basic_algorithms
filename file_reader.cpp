//
// Created by s24350 on 16.07.2024.
//
#include <fstream>
#include <sstream>
#include "file_reader.h"

auto getDataFromFile(std::string const &path, char separator) -> std::vector<std::vector<std::string>> {
    auto outerVec = std::vector<std::vector<std::string>>();
    auto innerVec = std::vector<std::string>();
    auto line = std::string();
    auto word = std::string();

    auto file = std::fstream(path);

    std::getline(file, line);//without header

    while (std::getline(file, line)) {
        auto lineAsStream = std::stringstream(line);
        while (std::getline(lineAsStream, word, separator)) {
            innerVec.push_back(word);
        }
        outerVec.push_back(innerVec);
        innerVec.clear();
    }
    return outerVec;
}