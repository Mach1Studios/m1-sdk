//  Mach1 Spatial SDK
//  Copyright © 2017-2020 Mach1. All rights reserved.

#include "Mach1EncodeCore.h"
#include "Mach1GenerateCoeffs.h"
#include "Mach1TranscodeAmbisonicFormats.h"
#include "Mach1TranscodeCore.h"
#include "Mach1TranscodeMicArrayFormats.h"
#include "Mach1TranscodeSurroundFormats.h"
#include "Mach1TranscodeVectorFormats.h"
#include "json/json.h"
#include <cstring>
#include <string.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>

#define _USE_MATH_DEFINES
#include <cmath>

std::string printFloatWithFormat(float value) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(8) << value;
    std::string str = ss.str();
    // Ensure that there is a decimal point somewhere (there should be)
    bool isInt = false;

    if (str.find('.') != std::string::npos) {
        // Remove trailing zeroes
        str = str.substr(0, str.find_last_not_of('0') + 1);
        // If the decimal point is now the last character, remove that as well
        if (str.find('.') == str.size() - 1) {
            str = str.substr(0, str.size() - 1);
            isInt = true;
        }
    }

    if (!isInt) {
        str = str + "f";
    }

    return str;
}

void printMatrixTransposed(std::vector<Mach1TranscodeMatrix> m, std::map<std::string, float> map_const) {
    for (int i = 0; i < m.size(); i++) {
        std::cout << "{"
                  << "\r\n";
        std::cout << "    \"" << m[i].formatFrom << "\", \"" << m[i].formatTo << "\""
                  << ","
                  << "\r\n";
        std::cout << "    {"
                  << "\r\n";

        int cnt = dynamic_cast<Mach1TranscodeCoeffs *>(m[i].channels[0])->data.size();

        for (int k = 0; k < cnt; k++) {

            std::cout << "        Mach1TranscodeChannel::Coeffs({ ";

            for (int j = 0; j < m[i].channels.size(); j++) {

                Mach1TranscodeChannelBase *channel = m[i].channels[j];
                Mach1TranscodeCoeffs *c = dynamic_cast<Mach1TranscodeCoeffs *>(channel);

                std::string const_name = "";

                if (k >= c->data.size()) {
                    //	std:: cout << "!!!";
                }
                float v = k < c->data.size() ? c->data[k] : 0;

                for (std::pair<std::string, float> element : map_const) {
                    if (fabs(element.second - v) < 0.00001) {
                        const_name = element.first;
                    } else if (fabs(-element.second - v) < 0.00001) {
                        const_name = "-" + element.first;
                    }
                }

                if (const_name == "") {
                    std::cout << printFloatWithFormat(v);
                } else {
                    std::cout << const_name;
                }
                if (j < m[i].channels.size() - 1)
                    std::cout << ", ";
            }
            std::cout << " }),"
                      << "\r\n";
        }
        // m[i].
        std::cout << "    }"
                  << "\r\n";
        std::cout << "},"
                  << "\r\n";
    }
    std::cout << "-------------------"
              << "\r\n";
}

void main() {
    std::map<std::string, float> map_const_ = {
        {"r2", sqrt(2.0)},
        {"r3", sqrt(3.0)},
        {"oor2", (1.0 / sqrt(2.0))},
        {"oor3", (1.0 / sqrt(3.0))},
        {"oor4", (1.0 / sqrt(4.0))},
        {"oor8", (1.0 / sqrt(8.0))},
        {"oor16", (1.0 / sqrt(16.0))},
        {"oo2r2", (1.0 / (2.0 * sqrt(2.0)))},
        {"r3or2", (sqrt(3.0) / sqrt(2.0))},
        {"r3o2", sqrt(3.0) / 2.0f},
        {"r2o2", sqrt(2.0) / 2.0f},
        {"r2o3", sqrt(2.0) / 3.0f},
        {"r2o4", sqrt(2.0) / 4.0f},
        {"r2o6", sqrt(2.0) / 6.0f},
        {"r2o8", sqrt(2.0) / 8.0f},
        {"r2o12", sqrt(2.0) / 12.0f},
        {"r2o20", sqrt(2.0) / 20.0f},
        {"oo8", 1.0f / 8.0f},
    };

    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesAmbisonic::getData();
        std::map<std::string, float> map_const = {
            {"r2", sqrt(2.0)},
            {"oor2", (1.0 / sqrt(2.0))},
            {"r3o2", sqrt(3.0) / 2.0f},
        };
        printMatrixTransposed(m, map_const);
    }
    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesSurround::getData();
        std::map<std::string, float> map_const = {
            {"oor4", (1.0 / sqrt(4.0))},
            {"oor8", (1.0 / sqrt(8.0))},
            {"oor16", (1.0 / sqrt(16.0))},
            {"r2o2", sqrt(2.0) / 2.0f},
            {"r2o4", sqrt(2.0) / 4.0f},
            {"r2o8", sqrt(2.0) / 8.0f},
            {"r2o20", sqrt(2.0) / 20.0f},
        };
        printMatrixTransposed(m, map_const);
    }
    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesVector::getData();
        std::map<std::string, float> map_const = {
            {"oor2", (1.0 / sqrt(2.0))},
            {"r2o3", sqrt(2.0) / 3.0f},
            {"r2o4", sqrt(2.0) / 4.0f},
        };
        printMatrixTransposed(m, map_const);
    }
    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesVector::getData();
        std::map<std::string, float> map_const = {
            {"r2o2", sqrt(2.0) / 2.0f},
            {"r2o4", sqrt(2.0) / 4.0f},
            {"r2o8", sqrt(2.0) / 8.0f},
        };
        printMatrixTransposed(m, map_const);
    }
    {
        std::vector<Mach1TranscodeMatrix> m = Mach1TranscodeConstants::MatricesMicArray::getData();
        std::map<std::string, float> map_const = {
            {"r2o2", sqrt(2.0) / 2.0f},
            {"r2o4", sqrt(2.0) / 4.0f},
            {"r2o8", sqrt(2.0) / 8.0f},
        };
        printMatrixTransposed(m, map_const);
    }
}
