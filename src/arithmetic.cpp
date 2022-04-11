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

string Arithmetic::decompress(string &src) {
    return std::string();
}
