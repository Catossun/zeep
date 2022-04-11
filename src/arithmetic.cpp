#include "arithmetic.h"
#include <sstream>
#include <iomanip>

string Arithmetic::compress(string &src) {
    probabilityTable = createProbabilityTable(src);
    auto mappedProbabilityTable = probabilityTable;
    // Start compress
    for (size_t i = 0; i < src.size() - 1; ++i) {
        char c = src[i];
        pair<double, double> cRange = mappedProbabilityTable[c];
        mapTable(mappedProbabilityTable, cRange);
    }
    // Take the probability of the last byte
    pair<double, double> lastProbRange = mappedProbabilityTable[src[src.size() - 1]];
    double resultTag = (lastProbRange.first + lastProbRange.second) / 2;
    // Convert double to string
    stringstream ss;
    ss << fixed << setprecision(128) << resultTag;
    return ss.str();
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

void Arithmetic::readProbability(basic_istream<char> &stream) {
    char value;
    char separator;
    double rangeStart;
    double rangeEnd;
    while (stream.get(value) &&
           stream.get(separator) &&
           stream >> rangeStart &&
           stream.get(separator) &&
           stream >> rangeEnd) {
        auto range = pair<double, double>(rangeStart, rangeEnd);
        probabilityTable[value] = range;
        stream.get();
    }
}

void Arithmetic::writeProbability(basic_ostream<char> &stream) {
    for (auto item:probabilityTable) {
        stream << item.first;
        stream << ':';
        stream << fixed;
        stream << setprecision(32);
        stream << item.second.first;
        stream << ',';
        stream << item.second.second;
        stream << endl;
    }
}
