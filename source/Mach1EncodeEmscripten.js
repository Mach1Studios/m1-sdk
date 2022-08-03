Module['onRuntimeInitialized'] = function() {
    console.log('loaded Mach1Encode');

    Module['Mach1Encode'] = function() {
        this.Mach1EncodeInputModeType = {
            "Mach1EncodeInputModeMono": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeMono,
            "Mach1EncodeInputModeStereo": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeStereo,
            "Mach1EncodeInputModeQuad": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeQuad,
            "Mach1EncodeInputModeLCRS": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeLCRS,
            "Mach1EncodeInputModeAFormat": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeAFormat,
            "Mach1EncodeInputModeBFOAACN": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeBFOAACN,
            "Mach1EncodeInputModeBFOAFUMA": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeBFOAFUMA,
            "Mach1EncodeInputModeB2OAACN": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeB2OAACN,
            "Mach1EncodeInputModeB2OAFUMA": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeB2OAFUMA,
            "Mach1EncodeInputModeB3OAACN": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeB3OAACN,
            "Mach1EncodeInputModeB3OAFUMA": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeB3OAFUMA,
            "Mach1EncodeInputModeLCR": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeLCR,
            "Mach1EncodeInputMode5dot0": (Module).Mach1EncodeInputModeType.Mach1EncodeInputMode5dot0,
            "Mach1EncodeInputMode5dot1Film": (Module).Mach1EncodeInputModeType.Mach1EncodeInputMode5dot1Film,
            "Mach1EncodeInputMode5dot1DTS": (Module).Mach1EncodeInputModeType.Mach1EncodeInputMode5dot1DTS,
            "Mach1EncodeInputMode5dot1SMTPE": (Module).Mach1EncodeInputModeType.Mach1EncodeInputMode5dot1SMTPE,
        };

        this.Mach1EncodeOutputModeType = {
            "Mach1EncodeOutputModeM1Horizon_4": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Horizon_4,
            "Mach1EncodeOutputModeM1Spatial_8": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial_8,
            "Mach1EncodeOutputModeM1Spatial_12": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial_12,
            "Mach1EncodeOutputModeM1Spatial_14": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial_14,
            "Mach1EncodeOutputModeM1Spatial_16": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial_16,
            "Mach1EncodeOutputModeM1Spatial_18": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial_18,
            "Mach1EncodeOutputModeM1Spatial_20": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial_20,
            "Mach1EncodeOutputModeM1Spatial_32": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial_32,
        };

        this.Mach1EncodePannerMode = {
            "Mach1EncodePannerModeIsotropicLinear": (Module).Mach1EncodePannerMode.Mach1EncodePannerModeIsotropicLinear,
            "Mach1EncodePannerModeIsotropicEqualPower": (Module).Mach1EncodePannerMode.Mach1EncodePannerModeIsotropicEqualPower,
            "Mach1EncodePannerModePeriphonicLinear": (Module).Mach1EncodePannerMode.Mach1EncodePannerModePeriphonicLinear,
        };

        this.Mach1DecodeAlgoType = {
            "Mach1DecodeAlgoSpatial_8": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_8,
            "Mach1DecodeAlgoSpatialAlt_8": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatialAlt_8,
            "Mach1DecodeAlgoHorizon_4": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoHorizon_4,
            "Mach1DecodeAlgoHorizonPairs": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoHorizonPairs,
            "Mach1DecodeAlgoSpatialPairs": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatialPairs,
            "Mach1DecodeAlgoSpatial_12": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_12,
            "Mach1DecodeAlgoSpatial_14": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_14,
            "Mach1DecodeAlgoSpatial_16": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_16,
            "Mach1DecodeAlgoSpatial_18": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_18,
            "Mach1DecodeAlgoSpatial_20": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_20,
            "Mach1DecodeAlgoSpatial_32": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_32,
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

        this.getResultingCoeffsDecoded  = function(decodeType, decodeResult) {
            if (this._m1obj) {
                var vec = new(Module).VectorFloat();
                for (var i = 0; i < decodeResult.length; i++) {
                    vec.push_back(decodeResult[i]);
                }

                var arr = this._m1obj.getResultingCoeffsDecoded(decodeType, vec);
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

        this.getResultingVolumesDecoded = function(decodeType, decodeResult) {
            if (this._m1obj) {
                var vec = new(Module).VectorFloat();
                for (var i = 0; i < decodeResult.length; i++) {
                    vec.push_back(decodeResult[i]);
                }

                var arr = this._m1obj.getResultingVolumesDecoded(decodeType, vec);
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

        this.setInputMode = function(inputMode) {
            if (this._m1obj) {
                this._m1obj.setInputMode(inputMode);
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

        this.setPannerMode = function(mode) {
            if (this._m1obj) {
                this._m1obj.setPannerMode(mode);
            }
        };

        this.setIsotropicEncode = function(isotropicEncode) {
            if (this._m1obj) {
                this._m1obj.setIsotropicEncode(isotropicEncode);
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

        this.setAutoOrbit = function(autoOrbit) {
            if (this._m1obj) {
                this._m1obj.setAutoOrbit(autoOrbit);
            }
        };

        this._m1obj = new(Module).Mach1EncodeInternal();
    };
};