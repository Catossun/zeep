#pragma once

#include "compressor.h"

class Arithmetic : public Compressor {
public:
    string compress(string &src) override;

    string decompress(string &src) override;
};
