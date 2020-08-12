Module['onRuntimeInitialized'] = function() {
    console.log('loaded Mach1Transcode');

    Module['Mach1Transcode'] = function() {
        this.Mach1TranscodeFormatType = {
            "Mach1TranscodeFormatEmpty": (Module).Mach1TranscodeFormatEmpty,
            "Mach1TranscodeFormatFuMa": (Module).Mach1TranscodeFormatFuMa,
            "Mach1TranscodeFormatACNSN3D": (Module).Mach1TranscodeFormatACNSN3D,
            "Mach1TranscodeFormatM1Horizon": (Module).Mach1TranscodeFormatM1Horizon,
            "Mach1TranscodeFormatM1HorizonS": (Module).Mach1TranscodeFormatM1HorizonS,
            "Mach1TranscodeFormatM1HorizonPairs": (Module).Mach1TranscodeFormatM1HorizonPairs,
            "Mach1TranscodeFormatM1Spatial": (Module).Mach1TranscodeFormatM1Spatial,
            "Mach1TranscodeFormatM1SpatialS": (Module).Mach1TranscodeFormatM1SpatialS,
            "Mach1TranscodeFormatM1SpatialPairs": (Module).Mach1TranscodeFormatM1SpatialPairs,
            "Mach1TranscodeFormatM1SpatialPlus": (Module).Mach1TranscodeFormatM1SpatialPlus,
            "Mach1TranscodeFormatM1SpatialPlusPlus": (Module).Mach1TranscodeFormatM1SpatialPlusPlus,
            "Mach1TranscodeFormatM1SpatialExtended": (Module).Mach1TranscodeFormatM1SpatialExtended,
            "Mach1TranscodeFormatM1SpatialExtendedPlus": (Module).Mach1TranscodeFormatM1SpatialExtendedPlus,
            "Mach1TranscodeFormatMono": (Module).Mach1TranscodeFormatMono,
            "Mach1TranscodeFormatStereo": (Module).Mach1TranscodeFormatStereo,
            "Mach1TranscodeFormatLCR": (Module).Mach1TranscodeFormatLCR,
            "Mach1TranscodeFormatFiveOh": (Module).Mach1TranscodeFormatFiveOh,
            "Mach1TranscodeFormatFiveOneFilm": (Module).Mach1TranscodeFormatFiveOneFilm,
            "Mach1TranscodeFormatFiveOneFilm_Cinema": (Module).Mach1TranscodeFormatFiveOneFilm_Cinema,
            "Mach1TranscodeFormatFiveOneSmpte": (Module).Mach1TranscodeFormatFiveOneSmpte,
            "Mach1TranscodeFormatFiveOneDts": (Module).Mach1TranscodeFormatFiveOneDts,
            "Mach1TranscodeFormatSixOh": (Module).Mach1TranscodeFormatSixOh,
            "Mach1TranscodeFormatSevenOnePt": (Module).Mach1TranscodeFormatSevenOnePt,
            "Mach1TranscodeFormatSevenOnePt_Cinema": (Module).Mach1TranscodeFormatSevenOnePt_Cinema,
            "Mach1TranscodeFormatACNSN3DO2A": (Module).Mach1TranscodeFormatACNSN3DO2A,
            "Mach1TranscodeFormatFuMaO2A": (Module).Mach1TranscodeFormatFuMaO2A,
            "Mach1TranscodeFormatTBE": (Module).Mach1TranscodeFormatTBE,
            "Mach1TranscodeFormatACNSN3DO3A": (Module).Mach1TranscodeFormatACNSN3DO3A,
            "Mach1TranscodeFormatFuMaO3A": (Module).Mach1TranscodeFormatFuMaO3A,
            "Mach1TranscodeFormatSevenOneSDDS": (Module).Mach1TranscodeFormatSevenOneSDDS,
            "Mach1TranscodeFormatSevenZeroSDDS": (Module).Mach1TranscodeFormatSevenZeroSDDS,
            "Mach1TranscodeFormatSevenOneTwo": (Module).Mach1TranscodeFormatSevenOneTwo,
            "Mach1TranscodeFormatSevenZeroTwo": (Module).Mach1TranscodeFormatSevenZeroTwo,
            "Mach1TranscodeFormatNineOne": (Module).Mach1TranscodeFormatNineOne,
            "Mach1TranscodeFormatNineZero": (Module).Mach1TranscodeFormatNineZero,
            "Mach1TranscodeFormatStereo_Cinema": (Module).Mach1TranscodeFormatStereo_Cinema,
            "Mach1TranscodeFormatFiveOneTwo": (Module).Mach1TranscodeFormatFiveOneTwo,
            "Mach1TranscodeFormatFiveZeroTwo": (Module).Mach1TranscodeFormatFiveZeroTwo,
            "Mach1TranscodeFormatFiveOneFour": (Module).Mach1TranscodeFormatFiveOneFour,
            "Mach1TranscodeFormatFiveZeroFour": (Module).Mach1TranscodeFormatFiveZeroFour,
            "Mach1TranscodeFormatSevenOneFour": (Module).Mach1TranscodeFormatSevenOneFour,
            "Mach1TranscodeFormatSevenZeroFour": (Module).Mach1TranscodeFormatSevenZeroFour,
            "Mach1TranscodeFormatSevenZero_Cinema": (Module).Mach1TranscodeFormatSevenZero_Cinema,
            "Mach1TranscodeFormatM1SpatialFaces": (Module).Mach1TranscodeFormatM1SpatialFaces,
            "Mach1TranscodeFormatNineOneFour": (Module).Mach1TranscodeFormatNineOneFour,
            "Mach1TranscodeFormatNineOneSix": (Module).Mach1TranscodeFormatNineOneSix,
            "Mach1TranscodeFormatMarcoSixteen": (Module).Mach1TranscodeFormatMarcoSixteen,
            "Mach1TranscodeFormatTTPoints": (Module).Mach1TranscodeFormatTTPoints,
            "Mach1TranscodeFormatACNSN3DmaxRE1oa": (Module).Mach1TranscodeFormatACNSN3DmaxRE1oa,
            "Mach1TranscodeFormatACNSN3DmaxRE2oa": (Module).Mach1TranscodeFormatACNSN3DmaxRE2oa,
            "Mach1TranscodeFormatACNSN3DmaxRE3oa": (Module).Mach1TranscodeFormatACNSN3DmaxRE3oa,
            "Mach1TranscodeFormatACNSN3DmaxRE4oa": (Module).Mach1TranscodeFormatACNSN3DmaxRE4oa,
            "Mach1TranscodeFormatACNSN3DmaxRE5oa": (Module).Mach1TranscodeFormatACNSN3DmaxRE5oa,
            "Mach1TranscodeFormatACNSN3DmaxRE6oa": (Module).Mach1TranscodeFormatACNSN3DmaxRE6oa,
            "Mach1TranscodeFormatACNSN3DmaxRE7oa": (Module).Mach1TranscodeFormatACNSN3DmaxRE7oa,
        };

        this.delete = function() {
            if (this._m1obj) {
                this._m1obj.delete();
                this._m1obj = null;
            }
        };

        this.getInputNumChannels = function() {
            if (this._m1obj) {
                return this._m1obj.getInputNumChannels();
            }
        };

        this.getOutputNumChannels = function() {
            if (this._m1obj) {
                return this._m1obj.getOutputNumChannels();
            }
        };

        this.setSpatialDownmixer = function(corrThreshold) {
            if (this._m1obj) {
                this._m1obj.setSpatialDownmixer(corrThreshold);
            }
        };

        this.getSpatialDownmixerPossibility = function() {
            if (this._m1obj) {
                this._m1obj.getSpatialDownmixerPossibility();
            }
        };

        this.setInputFormat = function(inFmt) {
            if (this._m1obj) {
                this._m1obj.setInputFormat(inFmt);
            }
        };

        this.setOutputFormat = function(outFmt) {
            if (this._m1obj) {
                this._m1obj.setOutputFormat(outFmt);
            }
        };

        this._m1obj = new(Module).Mach1TranscodeInternal();
    };
};