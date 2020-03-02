#include "acutest.h"
#include "Mach1Encode.h"

void test_gains(void)
{
	struct INPUT {
		Mach1EncodeInputModeType inputMode;
		Mach1EncodeOutputModeType outputMode;
		bool isotropicEncode;
		bool autoOrbit;
		float rotation;
		float diverge;
		float pitch;
		float stereoSpread;
		float stereoRotate;
	};

	struct OUTPUT {
		std::vector<std::vector<float>> gains;
	};
	
	struct DATA {
		std::string name;
		INPUT input;
		OUTPUT output;
	};

	std::vector<DATA> data = {
		{
			"case 01",
			INPUT { Mach1EncodeInputModeMono, Mach1EncodeOutputMode4Ch, false, 0.0, 0.0, 0.0, false, 0.0, 0.0 },
			OUTPUT {
				{
					{ 0.25, 0.25, 0.25, 0.25 },
				},
			}
		},
		{
			"case 02",
			INPUT { Mach1EncodeInputModeMono, Mach1EncodeOutputMode8Ch, false, 0.0, 0.0, 0.0, false, 0.0, 0.0 },
			OUTPUT {
				{
					{ 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125 },
				},
			}
		},
	};
	
	for (auto d : data) {
		Mach1Encode m1Encode;

		m1Encode.setInputMode(d.input.inputMode);
		m1Encode.setOutputMode(d.input.outputMode);

		m1Encode.setIsotropicEncode(d.input.isotropicEncode);

		m1Encode.setRotation(d.input.rotation);
		m1Encode.setDiverge(d.input.diverge);
		m1Encode.setPitch(d.input.pitch);

		m1Encode.setAutoOrbit(d.input.autoOrbit);
		m1Encode.setStereoSpread(d.input.stereoSpread);
		m1Encode.setStereoRotate(d.input.stereoRotate);

		m1Encode.generatePointResults();
		//auto points = m1Encode.getPoints();
		auto gains = m1Encode.getGains();
		
		bool passed = true;
		for (size_t i = 0; i < gains.size(); i++) {
			for (size_t j = 0; j < gains[i].size(); j++) {
				bool check = abs(d.output.gains[i][j] - gains[i][j]) < 0.00001;
				if (check == false) {
					TEST_CHECK_(check, "%s pass. gain with index [%d][%d]", d.name.c_str(), i, j);
					passed = false;
				}
			}
		}
	}
}

TEST_LIST = {
	{ "gains", test_gains },
	{ NULL, NULL }
};
