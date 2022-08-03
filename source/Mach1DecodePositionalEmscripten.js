Module['onRuntimeInitialized'] = function() {
    console.log('loaded Mach1DecodePositional');

    Module['Mach1DecodePositional'] = function() {
        this.Mach1PlatformType = {
            "Mach1PlatformDefault": (Module).Mach1PlatformType.Mach1PlatformDefault,
            "Mach1PlatformUnity": (Module).Mach1PlatformType.Mach1PlatformUnity,
            "Mach1PlatformUE": (Module).Mach1PlatformType.Mach1PlatformUE,
            "Mach1PlatformOfEasyCam": (Module).Mach1PlatformType.Mach1PlatformOfEasyCam,
            "Mach1PlatformAndroid": (Module).Mach1PlatformType.Mach1PlatformAndroid,
            "Mach1PlatformiOS": (Module).Mach1PlatformType.Mach1PlatformiOS,
            "Mach1PlatformiOSTableTop_ZVertical": (Module).Mach1PlatformiOSTableTop_ZVertical,
            "Mach1PlatformiOSPortraitHandheld_YVertical": (Module).Mach1PlatformiOSPortraitHandheld_YVertical,
            "Mach1PlatformiOSPortrait_YawOnly": (Module).Mach1PlatformiOSPortrait_YawOnly,
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
        };

        this.delete = function() {
            if (this._m1obj) {
                this._m1obj.delete();
                this._m1obj = null;
            }
        };

        this.setPlatformType = function(platform) {
            if (this._m1obj) {
                this._m1obj.setPlatformType(platform);
            }
        };

        this.setDecodeAlgoType = function(decodeAlgo) {
            if (this._m1obj) {
                this._m1obj.setDecodeAlgoType(decodeAlgo);
            }
        };

        this.setMuteWhenOutsideObject = function(muteWhenOutsideObject) {
            if (this._m1obj) {
                this._m1obj.setMuteWhenOutsideObject(muteWhenOutsideObject);
            }
        };

        this.setMuteWhenInsideObject = function(muteWhenInsideObject) {
            if (this._m1obj) {
                this._m1obj.setMuteWhenInsideObject(muteWhenInsideObject);
            }
        };

        this.setUseAttenuation = function(useAttenuation) {
            if (this._m1obj) {
                this._m1obj.setUseAttenuation(useAttenuation);
            }
        };

        this.setAttenuationCurve = function(attenuationCurve) {
            if (this._m1obj) {
                this._m1obj.setAttenuationCurve(attenuationCurve);
            }
        };

        this.setUsePlaneCalculation = function(usePlaneCalculation) {
            if (this._m1obj) {
                this._m1obj.setUsePlaneCalculation(usePlaneCalculation);
            }
        };

        this.setUseYawForRotation = function(useYawForRotation) {
            if (this._m1obj) {
                this._m1obj.setUseYawForRotation(useYawForRotation);
            }
        };

        this.setUsePitchForRotation = function(usePitchForRotation) {
            if (this._m1obj) {
                this._m1obj.setUsePitchForRotation(usePitchForRotation);
            }
        };

        this.setUseRollForRotation = function(useRollForRotation) {
            if (this._m1obj) {
                this._m1obj.setUseRollForRotation(useRollForRotation);
            }
        };

        this.setListenerPosition = function(x, y, z) {
            if (this._m1obj) {
                this._m1obj.setListenerPosition({
                    x: x,
                    y: y,
                    z: z
                });
            }
        };

        this.setListenerRotation = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setListenerRotation({
                    x: yaw,
                    y: pitch,
                    z: roll
                });
            }
        };

        this.setListenerRotationQuat = function(x, y, z, w) {
            if (this._m1obj) {
                this._m1obj.setListenerRotationQuat({
                    x: x,
                    y: y,
                    z: z,
                    w: w
                });
            }
        };

        this.setDecoderAlgoPosition = function(x, y, z) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoPosition({
                    x: x,
                    y: y,
                    z: z
                });
            }
        };

        this.setDecoderAlgoRotation = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoRotation({
                    x: yaw,
                    y: pitch,
                    z: roll
                });
            }
        };

        this.setDecoderAlgoRotationQuat = function(x, y, z, w) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoRotationQuat({
                    x: x,
                    y: y,
                    z: z,
                    w: w
                });
            }
        };

        this.setDecoderAlgoScale = function(x, y, z) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoScale({
                    x: x,
                    y: y,
                    z: z
                });
            }
        };

        this.evaluatePositionResults = function() {
            if (this._m1obj) {
                this._m1obj.evaluatePositionResults();
            }
        };

        this.getCoefficients = function() {
            if (this._m1obj) {
                var vec = new(Module).VectorFloat();
                vec.resize(18, 0);
                this._m1obj.getCoefficients(vec);
                var arr = [];
                for (var i = 0; i < vec.size(); i++) {
                    arr[i] = vec.get(i);
                }
                return arr;
            }
        };

        this.getDist = function() {
            if (this._m1obj) {
                return this._m1obj.getDist();
            }
        };

        this.getCurrentAngle = function() {
            if (this._m1obj) {
                var point = this._m1obj.getCurrentAngle()
                return [point.x, point.y, point.z];
            }
        };

        this.getCoefficientsRotation = function() {
            if (this._m1obj) {
                var point = this._m1obj.getCoefficientsRotation()
                return [point.x, point.y, point.z];
            }
        };

        this.setFilterSpeed = function(filterSpeed) {
            if (this._m1obj) {
                this._m1obj.setFilterSpeed(filterSpeed);
            }
        };

        /* Experimental Functions/Features */
        this.setUseBlendMode = function(useBlendMode) {
            if (this._m1obj) {
                this._m1obj.setUseBlendMode(useBlendMode);
            }
        };

        this.setIgnoreTopBottom = function(ignoreTopBottom) {
            if (this._m1obj) {
                this._m1obj.setIgnoreTopBottom(ignoreTopBottom);
            }
        };

        this.setAttenuationCurveBlendMode = function(attenuationCurveBlendMode) {
            if (this._m1obj) {
                this._m1obj.setAttenuationCurveBlendMode(attenuationCurveBlendMode);
            }
        };

        this.getCoefficientsInterior = function() {
            if (this._m1obj) {
                var vec = new(Module).VectorFloat();
                vec.resize(18, 0);
                this._m1obj.getCoefficientsInterior(vec);
                var arr = [];
                for (var i = 0; i < vec.size(); i++) {
                    arr[i] = vec.get(i);
                }
                return arr;
            }
        };

        this._m1obj = new(Module).Mach1DecodePositionalInternal();
    };
};