#include "acutest.h"
#include "json.hpp"
#include "Mach1Transcode.h"

#include <iostream>
#include <math.h>
#include <map>

/*
const std::vector<std::string> formats = {
	"CustomPoints"
	"Empty"
	"FuMa"
	"ACNSN3D"
	"M1Horizon"
	"M1HorizonS"
	"M1HorizonPairs"
	"M1Spatial"
	"M1SpatialS"
	"M1SpatialPairs"
	"M1SpatialPlus"
	"M1SpatialPlusPlus"
	"M1SpatialExtended"
	"M1SpatialExtendedPlus"
	"Mono"
	"Stereo"
	"LCR"
	"FiveOh"
	"FiveOneFilm"
	"FiveOneFilm_Cinema"
	"FiveOneSmpte"
	"FiveOneDts"
	"SixOh"
	"SevenOnePt"
	"SevenOnePt_Cinema"
	"ACNSN3DO2A"
	"FuMaO2A"
	"TBE"
	"ACNSN3DO3A"
	"FuMaO3A"
	"SevenOneSDDS"
	"SevenZeroSDDS"
	"SevenOneTwo"
	"SevenZeroTwo"
	"NineOne"
	"NineZero"
	"Stereo_Cinema"
	"FiveOneTwo"
	"FiveZeroTwo"
	"FiveOneFour"
	"FiveOneFourSMPTE"
	"FiveZeroFour"
	"SevenOneFour"
	"SevenZeroFour"
	"SevenZero_Cinema"
	"M1SpatialFaces"
	"NineOneFour"
	"NineOneSix"
	"MarcoSixteen"
	"ACNSN3DmaxRE1oa"
	"ACNSN3DmaxRE2oa"
	"ACNSN3DmaxRE3oa"
	"ACNSN3DmaxRE4oa"
	"ACNSN3DmaxRE5oa"
	"ACNSN3DmaxRE6oa"
	"ACNSN3DmaxRE7oa"
	"DolbyAtmosSevenOneTwo"
};
*/

void test_results(void)
{
	struct INPUT_DATA {
		std::string inputFormat;
		std::string outputFormat;
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
					"Test 1",
					{
						"ACNSN3D",
						"M1Spatial"
					},
					{
						{
							{ 0.125000000, 0.216601998, 0.216316000, 0.216601998, },
							{ 0.125000000, -0.216601998, 0.216316000, 0.216601998, },
							{ 0.125000000, 0.216601998, 0.216316000, -0.216601998, },
							{ 0.125000000, -0.216601998, 0.216316000, -0.216601998, },
							{ 0.125000000, 0.216601998, -0.216316000, 0.216601998, },
							{ 0.125000000, -0.216601998, -0.216316000, 0.216601998, },
							{ 0.125000000, 0.216601998, -0.216316000, -0.216601998, },
							{ 0.125000000, -0.216601998, -0.216316000, -0.216601998 }
						}
					}
				}
	};

	std::cout << std::endl;

	for (auto test : cases) {
		// encode part
		Mach1Transcode m1Transcode;

		m1Transcode.setInputFormat(m1Transcode.getFormatFromString(test.input.inputFormat));
		m1Transcode.setOutputFormat(m1Transcode.getFormatFromString(test.input.outputFormat));

		std::cout
			<< "testing " << test.name << ": "
			<< test.input.inputFormat << " > " << test.input.outputFormat;

		// make converison
		m1Transcode.processConversionPath();
		std::vector<std::vector<float>> matrix = m1Transcode.getMatrixConversion();

		int counter = 0;

		for (size_t i = 0; i < matrix.size(); i++) {
			for (size_t j = 0; j < matrix[j].size(); j++) {
				bool check = fabs(test.output.matrix[i][j] - matrix[i][j]) < 0.1;
				if (check == false) {
					TEST_CHECK_(check, "%s | Error with index [%d, %d]", test.name.c_str(), i, j);
					std::cout << "index: [" << i << " , " << j << "]: " << matrix[i][j] << ", should be: " << test.output.matrix[i][j];
					std::cout << std::endl;
				}
				counter += check;
			}
		}
		
		if (counter == m1Transcode.getInputNumChannels() * m1Transcode.getOutputNumChannels()) {
			std::cout << "... " << "\033[1;32mpassed\033[0m\n";
		}
	}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	getchar();
#endif
}

TEST_LIST = {
	{ "results", test_results },
	{ NULL, NULL }
};
