#pragma once

#include "compressor.h"

#include <map>

class Arithmetic : public Compressor {
private:
    map<char, pair<double, double>> probabilityTable;

    map<char, pair<double, double>> createProbabilityTable(string &src);

    map<char, double> calcProbability(string &src);

public:

    string compress(string &src) override;

    string decompress(string &src) override;
};
