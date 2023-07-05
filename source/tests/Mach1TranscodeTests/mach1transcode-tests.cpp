#include "Mach1Transcode.h"
#include "acutest.h"
#include "json.hpp"

#include <iostream>
#include <map>
#include <math.h>

void test_results(void) {
    struct INPUT_DATA {
        bool isCustomPointsJson;
        std::string inputFormat;
        std::string outputFormat;
        std::string customJson;
    };

    struct OUTPUT_DATA {
        std::vector<std::vector<float>> matrix;
    };

    struct CASE {
        std::string name;
        INPUT_DATA input;
        OUTPUT_DATA output;
    };

    std::vector<CASE> cases = {
        {
            "case: TRANSCODE | M1SPATIAL-8 -> M1SPATIAL-4 | Basic Check #1",
            {
                false,
                "M1Spatial-8",
                "M1Spatial-4",
                ""
            },
            {{
                {0.70710678f, 0, 0, 0},
                {0, 0.70710678f, 0, 0},
                {0, 0, 0.70710678f, 0},
                {0, 0, 0, 0.70710678f},
                {0.70710678f, 0, 0, 0},
                {0, 0.70710678f, 0, 0},
                {0, 0, 0.70710678f, 0},
                {0, 0, 0, 0.70710678f},
            }}
        },
        {
            "case: TRANSCODE | ACNSN3DmaxRE1oa -> M1SPATIAL-8 | Basic Check #2",
            {   
                false,
                "ACNSN3DmaxRE1oa",
                "M1Spatial-8",
                ""
            },
            {{
                {0.11956964f, 0.23359211f, 0.23359495f, 0.23358522f, 0.23357949f, 0.2336008f, 0.23359843f, 0.11956925f},
                {0.10084855f, -0.09768785f, 0.18741015f, -0.18741174f, 0.18741104f, -0.18740843f, 0.09767668f, -0.10084811f},
                {0.1008354f, 0.18743709f, 0.18744147f, 0.09766876f, -0.09767947f, -0.18745011f, -0.18744992f, -0.10083508f},
                {0.10084416f, 0.18740816f, -0.09768243f, -0.18742256f, 0.18740852f, 0.09767484f, -0.18741411f, -0.1008406f},
            }}
        },
        // TODO: ADD THIS TEST WHEN ::Panner() mode is made
        {
            "case: TRANSCODE | 1.0 -> M1SPATIAL-4 | Panner Check #1",
            {
                false,
                "1.0",
                "M1Spatial-4",
                ""
            },
            {{
                {0.353553f, 0.353553f, 0.353553f, 0.353553f},
            }}
        },
        {
            "case: TRANSCODE | CustomPoints Cartesian",
            {
                true,
                "CustomPoints",
                "M1Spatial-8",
                R"(
                {
                    "points": [
                        {
                            "x": 1.0,
                            "y": 1.0,
                            "z": 0.0,
                            "azimuth": 0.0,
                            "elevation": 0.0,
                            "diverge": 0.0,
                            "usePolar": false,
                            "name": "L",
                            "children": []
                        }
                    ]
                }
                )"
            },
            {{
                {0.0, 0.5, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0},
            }}
        },
        {
            "case: TRANSCODE | CustomPoints Polar",
            {
                true,
                "CustomPoints",
                "M1Spatial-8",
                R"(
                {
                    "points": [
                        {
                            "x": 0.0,
                            "y": 0.0,
                            "z": 0.0,
                            "azimuth": -45.0,
                            "elevation": 0.0,
                            "diverge": 1.0,
                            "usePolar": true,
                            "name": "L"
                        }
                    ]
                }
                )"
            },
            {{
                {0.500000, 0.000000, 0.000000, 0.000000, 0.500000, 0.000000, 0.000000, 0.000000},
            }}
        },
    };

    std::cout << std::endl;

    for (auto test : cases) {
        // encode part
        Mach1Transcode m1Transcode;

        if (test.input.isCustomPointsJson) {
            m1Transcode.setInputFormatCustomPointsJson(test.input.customJson);
        } else {
            m1Transcode.setInputFormat(m1Transcode.getFormatFromString(test.input.inputFormat));
        }
        m1Transcode.setOutputFormat(m1Transcode.getFormatFromString(test.input.outputFormat));

        std::cout
            << "testing " << test.name << ": "
            << test.input.inputFormat << " > " << test.input.outputFormat;

        // make converison
        m1Transcode.processConversionPath();
        std::vector<std::vector<float>> matrix = m1Transcode.getMatrixConversion();

        int counter = 0;

        for (size_t i = 0; i < matrix.size(); i++) {
            for (size_t j = 0; j < matrix[i].size(); j++) {
                // TODO: Finalize the index [i] x [j] relationship after we finish the design for the coeffsFlipped() function
                bool check = fabs(test.output.matrix[j][i] - matrix[i][j]) < 0.01;
                if (check == false) {
                    TEST_CHECK_(check, "%s | Error with index [%d, %d]", test.name.c_str(), i, j);
                    std::cout << "index: [" << i << " , " << j << "]: " << matrix[i][j] << ", should be: " << test.output.matrix[j][i];
                    std::cout << std::endl;
                }
                counter += check;
            }
        }

        std::vector<std::string> formats = m1Transcode.getAllFormatNames();

        if (counter == m1Transcode.getInputNumChannels() * m1Transcode.getOutputNumChannels()) {
            std::cout << " "
                      << "\033[1;32mpassed\033[0m\n";
        }
    }

#if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    getchar();
#endif
}

TEST_LIST = {
    {"results", test_results},
    {NULL, NULL}};
