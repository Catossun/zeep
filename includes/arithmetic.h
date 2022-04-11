#pragma once

#include "compressor.h"

#include <map>
#include <iostream>

class Arithmetic : public Compressor {
private:
    double probTableStart = 0;

    double probTableEnd = 1;

    map<char, pair<double, double>> probabilityTable;

    map<char, pair<double, double>> createProbabilityTable(string &src);

    map<char, double> calcProbability(string &src);

    void mapTable(map<char, pair<double, double>> &table, pair<double, double> range);

    pair<char, pair<double, double>> findRange(map<char, pair<double, double>> &table, double tag);

public:

    string compress(string &src) override;

    string decompress(string &src) override;

    void readProbability(basic_istream<char> &stream);

    void writeProbability(basic_ostream<char> &stream);
};
