#include "arithmetic.h"

string Arithmetic::compress(string &src) {
    probabilityTable = createProbabilityTable(src);
    return std::string();
}

map<char, pair<double, double>> Arithmetic::createProbabilityTable(string &src) {
    auto probTable = map<char, pair<double, double>>();
    map<char, double> prob = calcProbability(src);
    double lastEndValue = 0.0;
    // Start creating table
    for (pair<const char, double> &item: prob) {
        // Create a probability range
        double endValue = lastEndValue + item.second;
        pair<double, double> probRange(lastEndValue, endValue);
        // Save into the table
        probTable[item.first] = probRange;
        lastEndValue = endValue;
    }
    return probTable;
}

map<char, double> Arithmetic::calcProbability(string &src) {
    auto prob = map<char, double>();
    size_t totalBytes = src.size();
    // Count each byte
    for (char c:src) {
        ++prob[c];
    }
    // Change to probability
    for (pair<const char, double> &item : prob) {
        item.second /= totalBytes;
    }
    return prob;
}

void Arithmetic::mapTable(map<char, pair<double, double>> &table, pair<double, double> range) {
    double lastEndValue = range.first;
    double probRatio = (range.second - range.first) / (probTableEnd - probTableStart);
    // Start creating table
    for (pair<const char, pair<double, double>> &item: table) {
        double newEndValue = lastEndValue + (item.second.second - item.second.first) * probRatio;
        // Create a probability range
        pair<double, double> newProbRange(lastEndValue, newEndValue);
        // Save into the table
        item.second = newProbRange;
        lastEndValue = newEndValue;
    }
    probTableStart = range.first;
    probTableEnd = range.second;
}

string Arithmetic::decompress(string &src) {
    return std::string();
}
