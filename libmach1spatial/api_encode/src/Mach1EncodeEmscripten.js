Module['onRuntimeInitialized'] = function() {
    console.log('loaded Mach1Encode');

    Module['Mach1Encode'] = function() {
        this.Mach1EncodeInputMode = {
            "Mono": (Module).Mach1EncodeInputMode.Mono,
            "Stereo": (Module).Mach1EncodeInputMode.Stereo,
            "LCR": (Module).Mach1EncodeInputMode.LCR,
            "Quad": (Module).Mach1EncodeInputMode.Quad,
            "LCRS": (Module).Mach1EncodeInputMode.LCRS,
            "AFormat": (Module).Mach1EncodeInputMode.AFormat,
            "FiveDotZero": (Module).Mach1EncodeInputMode.otZero,
            "FiveDotOneFilm": (Module).Mach1EncodeInputMode.FiveDotOneFilm,
            "FiveDotOneDTS": (Module).Mach1EncodeInputMode.FiveDotOneDTS,
            "FiveDotOneSMTPE": (Module).Mach1EncodeInputMode.FiveDotOneSMTPE,
            "BFOAACN": (Module).Mach1EncodeInputMode.BFOAACN,
            "BFOAFUMA": (Module).Mach1EncodeInputMode.BFOAFUMA,
            "B2OAACN": (Module).Mach1EncodeInputMode.B2OAACN,
            "B2OAFUMA": (Module).Mach1EncodeInputMode.B2OAFUMA,
            "B3OAACN": (Module).Mach1EncodeInputMode.B3OAACN,
            "B3OAFUMA": (Module).Mach1EncodeInputMode.B3OAFUMA,
        };

        this.Mach1EncodeOutputMode = {
            "M1Spatial_4": (Module).Mach1EncodeOutputMode.M1Spatial_4,
            "M1Spatial_8": (Module).Mach1EncodeOutputMode.M1Spatial_8,
            "M1Spatial_12": (Module).Mach1EncodeOutputMode.M1Spatial_12,
            "M1Spatial_14": (Module).Mach1EncodeOutputMode.M1Spatial_14,
        };

        this.Mach1EncodePannerMode = {
            "IsotropicLinear": (Module).Mach1EncodePannerMode.IsotropicLinear,
            "IsotropicEqualPower": (Module).Mach1EncodePannerMode.IsotropicEqualPower,
            "PeriphonicLinear": (Module).Mach1EncodePannerMode.PeriphonicLinear,
        };

        this.Mach1DecodeMode = {
            "M1Spatial_4": (Module).Mach1DecodeMode.M1Spatial_4,
            "M1Spatial_8": (Module).Mach1DecodeMode.M1Spatial_8,
            "M1Spatial_12": (Module).Mach1DecodeMode.M1Spatial_12,
            "M1Spatial_14": (Module).Mach1DecodeMode.M1Spatial_14,
        };

        this.delete = function() {
            if (this._m1obj) {
                this._m1obj.delete();
                this._m1obj = null;
            }
        };

        this.getPoints = function() {
            if (this._m1obj) {
                var arr = this._m1obj.getPoints();
                var points = [];
                for (var i = 0; i < arr.size(); i++) {
                    points[i] = {
                        x: arr.get(i).x,
                        y: arr.get(i).y,
                        z: arr.get(i).z
                    };
                }
                arr.delete();
                arr = null;

                return points
            }
        };

        this.getGains = function() {
            if (this._m1obj) {
                var arr = this._m1obj.getGains();
                var gains = [];
                for (var i = 0; i < arr.size(); i++) {
                    gains[i] = [];
                    var _arr = arr.get(i);
                    for (var j = 0; j < _arr.size(); j++) {
                        gains[i][j] = _arr.get(j);
                    }
                    _arr.delete();
                    _arr = null;
                }
                arr.delete();
                arr = null;

                return gains;
            }
        };

        this.getResultingCoeffsDecoded  = function(decodeMode, decodeResult) {
            if (this._m1obj) {
                var vec = new(Module).VectorFloat();
                for (var i = 0; i < decodeResult.length; i++) {
                    vec.push_back(decodeResult[i]);
                }

                var arr = this._m1obj.getResultingCoeffsDecoded(decodeMode, vec);
                var volumes = [];
                for (var i = 0; i < arr.size(); i++) {
                    volumes[i] = arr.get(i);
                }
                arr.delete();
                arr = null;

                vec.delete();
                vec = null;

                return volumes;
            }
        };

        this.getPointsNames = function() {
            if (this._m1obj) {
                var arr = this._m1obj.getPointsNames();
                var pointsNames = [];
                for (var i = 0; i < arr.size(); i++) {
                    pointsNames[i] = arr.get(i);
                }
                arr.delete();
                arr = null;

                return pointsNames;
            }
        };

        this.getGainsForInputChannelNamed = function(channelName) {
            if (this._m1obj) {
                return this._m1obj.getGainsForInputChannelNamed(channelName);
            }
        };

        this.generatePointResults = function() {
            if (this._m1obj) {
                this._m1obj.generatePointResults();
            }
        };

        this.getPointsCount = function() {
            if (this._m1obj) {
                return this._m1obj.getPointsCount();
            }
        };

        this.getInputMode = function() {
            if (this._m1obj) {
                return this._m1obj.getInputMode();
            }
        };

        this.getOutputMode = function() {
            if (this._m1obj) {
                return this._m1obj.getOutputMode();
            }
        };

        this.getInputChannelsCount = function() {
            if (this._m1obj) {
                return this._m1obj.getInputChannelsCount();
            }
        };

        this.getOutputChannelsCount = function() {
            if (this._m1obj) {
                return this._m1obj.getOutputChannelsCount();
            }
        };

        this.setInputMode = function(inputMode) {
            if (this._m1obj) {
                this._m1obj.setInputMode(inputMode);
            }
        };

        this.setOutputMode = function(outputMode) {
            if (this._m1obj) {
                this._m1obj.setOutputMode(outputMode);
            }
        };

        this.setAzimuth = function(azimuth) {
            if (this._m1obj) {
                this._m1obj.setAzimuth(azimuth);
            }
        };

        this.setAzimuthDegrees = function(azimuth) {
            if (this._m1obj) {
                this._m1obj.setAzimuthDegrees(azimuth);
            }
        };

        this.setAzimuthRadians = function(azimuth) {
            if (this._m1obj) {
                this._m1obj.setAzimuthRadians(azimuth);
            }
        };

        this.setDiverge = function(diverge) {
            if (this._m1obj) {
                this._m1obj.setDiverge(diverge);
            }
        };

        this.setPitch = function(pitch) {
            if (this._m1obj) {
                this._m1obj.setElevationDegrees(pitch);
            }
        };

        this.setElevation = function(elevation) {
            if (this._m1obj) {
                this._m1obj.setElevation(elevation);
            }
        };

        this.setElevationDegrees = function(elevation) {
            if (this._m1obj) {
                this._m1obj.setElevationDegrees(elevation);
            }
        };

        this.setElevationRadians = function(elevation) {
            if (this._m1obj) {
                this._m1obj.setElevationRadians(elevation);
            }
        };

        this.setAutoOrbit = function(autoOrbit) {
            if (this._m1obj) {
                this._m1obj.setAutoOrbit(autoOrbit);
            }
        };

        this.setOrbitRotation = function(orbitRotation) {
            if (this._m1obj) {
                this._m1obj.setOrbitRotation(orbitRotation);
            }
        };

        this.setOrbitRotationDegrees = function(orbitRotation) {
            if (this._m1obj) {
                this._m1obj.setOrbitRotationDegrees(orbitRotation);
            }
        };

        this.setOrbitRotationRadians = function(orbitRotation) {
            if (this._m1obj) {
                this._m1obj.setOrbitRotationRadians(orbitRotation);
            }
        };

        this.setStereoSpread = function(stereoSpread) {
            if (this._m1obj) {
                this._m1obj.setStereoSpread(stereoSpread);
            }
        };

        this.setPannerMode = function(pannerMode) {
            if (this._m1obj) {
                this._m1obj.setPannerMode(pannerMode);
            }
        };

        this.setFrontSurroundPerspective = function(frontSurroundPerspective) {
            if (this._m1obj) {
                this._m1obj.setFrontSurroundPerspective(frontSurroundPerspective);
            }
        };

        this.setOutputGain = function(outputGainMultipler, isDecibel) {
            if (this._m1obj) {
                this._m1obj.setOutputGain(outputGainMultipler, isDecibel);
            }
        };

        this._m1obj = new(Module).Mach1EncodeInternal();
    };
};
