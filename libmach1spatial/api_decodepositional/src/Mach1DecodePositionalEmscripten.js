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

        this.Mach1DecodeMode = {
            "M1Spatial_4": (Module).Mach1DecodeMode.M1DecodeSpatial_4,
            "M1Spatial_8": (Module).Mach1DecodeMode.M1DecodeSpatial_8,
            "M1Spatial_14": (Module).Mach1DecodeMode.M1DecodeSpatial_14,
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

        this.setDecodeMode = function(decodeMode) {
            if (this._m1obj) {
                this._m1obj.setDecodeMode(decodeMode);
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

        this.getDist = function() {
            if (this._m1obj) {
                return this._m1obj.getDist();
            }
        };

        this.getFormatChannelCount = function() {
            if (this._m1obj) {
                return this._m1obj.getFormatChannelCount();
            }
        };

        this.getFormatCoeffCount = function() {
            if (this._m1obj) {
                return this._m1obj.getFormatCoeffCount();
            }
        };

        this.getCoefficients = function() {
            if (this._m1obj) {
                var vec = new(Module).VectorFloat();
                vec.resize(this._m1obj.getFormatCoeffCount(), 0);
                this._m1obj.getCoefficients(vec);
                var arr = [];
                for (var i = 0; i < vec.size(); i++) {
                    arr[i] = vec.get(i);
                }
                return arr;
            }
        };

        this.getCurrentAngle = function() {
            if (this._m1obj) {
                var point = this._m1obj.getCurrentAngle()
                return [point.x, point.y, point.z];
            }
        };

        this.getPositionalRotation = function() {
            if (this._m1obj) {
                var point = this._m1obj.getPositionalRotation()
                return [point.x, point.y, point.z];
            }
        };

        this.setFilterSpeed = function(filterSpeed) {
            if (this._m1obj) {
                this._m1obj.setFilterSpeed(filterSpeed);
            }
        };

        this._m1obj = new(Module).Mach1DecodePositionalInternal();
    };
};
