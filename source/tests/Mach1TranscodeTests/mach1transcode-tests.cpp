#include "acutest.h"
#include "json.hpp"
#include "Mach1Transcode.h"

#include <iostream>
#include <math.h>
#include <map>

const float r2 = (float)sqrt(2.0);
const float r3 = (float)sqrt(3.0);
const float oor2 = (float)(1.0 / sqrt(2.0));
const float oor3 = (float)(1.0 / sqrt(3.0));
const float oor4 = (float)(1.0 / sqrt(4.0));
const float oor8 = (float)(1.0 / sqrt(8.0));
const float oor16 = (float)(1.0 / sqrt(16.0));
const float oo2r2 = (float)(1.0 / (2.0 * sqrt(2.0)));
const float r3or2 = (float)(sqrt(3.0) / sqrt(2.0));
const float r3o2 = (float)sqrt(3.0) / 2.0f;
const float r2o2 = (float)sqrt(2.0) / 2.0f;
const float r2o3 = (float)sqrt(2.0) / 3.0f;
const float r2o4 = (float)sqrt(2.0) / 4.0f;
const float r2o6 = (float)sqrt(2.0) / 6.0f;
const float r2o8 = (float)sqrt(2.0) / 8.0f;
const float r2o12 = (float)sqrt(2.0) / 12.0f;
const float r2o20 = (float)sqrt(2.0) / 20.0f;
const float oo8 = (float)1.0f / 8.0f;

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
			"Basic Check",
			{
				"M1Spatial",
				"M1Horizon"
			},
			{
				{
					{ oor2, 0, 0, 0, oor2, 0, 0, 0 },
					{ 0, oor2, 0, 0, 0, oor2, 0, 0 },
					{ 0, 0, oor2, 0, 0, 0, oor2, 0 },
					{ 0, 0, 0, oor2, 0, 0, 0, oor2 }
				}
			}
		},
		{
			"Basic Check 2",
			{
				"ACNSN3DmaxRE1oa", 
				"M1Spatial"
			},
			{ 	
				{
					{ 0.1195696368813515f, 0.1008485481142998f, 0.1008353978395462f, 0.1008441597223282f },
					{ 0.2335921078920364f, -0.09768784791231155f, 0.1874370872974396f, 0.1874081641435623f },
					{ 0.2335949540138245f, 0.1874101459980011f, 0.1874414682388306f, -0.09768243134021759f },
					{ 0.2335852235555649f, -0.1874117404222488f, 0.09766875952482224f, -0.1874225586652756f },
					{ 0.2335794866085052f, 0.1874110400676727f, -0.09767947345972061f, 0.187408521771431f },
					{ 0.2336007952690125f, -0.1874084323644638f, -0.187450110912323f, 0.09767483919858932f },
					{ 0.2335984259843826f, 0.09767667949199677f, -0.1874499171972275f, -0.1874141097068787f },
					{ 0.1195692494511604f, -0.1008481085300446f, -0.1008350774645805f, -0.1008405983448029f }
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
			for (size_t j = 0; j < matrix[i].size(); j++) {
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
