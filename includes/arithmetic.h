#pragma once

#include "compressor.h"

#include <map>

class Arithmetic : public Compressor {
private:
    double probTableStart = 0;

    double probTableEnd = 1;

    map<char, pair<double, double>> probabilityTable;

    map<char, pair<double, double>> createProbabilityTable(string &src);

    map<char, double> calcProbability(string &src);

    void mapTable(map<char, pair<double, double>> &table, pair<double, double> range);

public:

    string compress(string &src) override;

    string decompress(string &src) override;
};
