Module['onInited'] = function() {
};

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
        };

        this.Mach1DecodeAlgoType = {
            "Mach1DecodeAlgoSpatial": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial,
            "Mach1DecodeAlgoAltSpatial": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoAltSpatial,
            "Mach1DecodeAlgoHorizon": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoHorizon,
            "Mach1DecodeAlgoHorizonPairs": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoHorizonPairs,
            "Mach1DecodeAlgoSpatialPairs": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatialPairs,
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

//TODO: UPDATE THESE TO RECEIVE MACH1POINT
        this.setListenerPosition = function() {
            if (this._m1obj) {
                this._m1obj.setListenerPosition();
            }
        };

        this.setListenerRotation = function() {
            if (this._m1obj) {
                this._m1obj.setListenerRotation();
            }
        };

        this.setListenerRotationQuat = function() {
            if (this._m1obj) {
                this._m1obj.setListenerRotationQuat();
            }
        };

        this.setDecoderPosition = function() {
            if (this._m1obj) {
                this._m1obj.setDecoderPosition();
            }
        };

        this.setDecoderRotation = function() {
            if (this._m1obj) {
                this._m1obj.setDecoderRotation();
            }
        };

        this.setDecoderRotationQuat = function() {
            if (this._m1obj) {
                this._m1obj.setDecoderRotationQuat();
            }
        };

        this.setDecoderScale = function() {
            if (this._m1obj) {
                this._m1obj.setDecoderScale();
            }
        };

        this.evaluatePositionResults = function() {
            if (this._m1obj) {
                this._m1obj.evaluatePositionResults();
            }
        };

        this.getCoefficients = function() {
            if (this._m1obj) {
                return this._m1obj.getCoefficients();
            }
        };

        this.getDist = function() {
            if (this._m1obj) {
                return this._m1obj.getDist();
            }
        };

        this.getCurrentAngle = function() {
            if (this._m1obj) {
                return this._m1obj.getCurrentAngle();
            }
        };  

        this.getCoefficientsRotation = function() {
            if (this._m1obj) {
                return this._m1obj.getCoefficientsRotation();
            }
        };    
        
        this.setFilterSpeed = function(filterSpeed) {
            if (this._m1obj) {
                this._m1obj.setFilterSpeed(filterSpeed);
            }
        };

/* DEPRECATED START*/
        this.setUseFalloff = function(useFalloff) {
            if (this._m1obj) {
                this._m1obj.setUseFalloff(useFalloff);
            }
        };

        this.setFalloffCurve = function(falloffCurve) {
            if (this._m1obj) {
                this._m1obj.setFalloffCurve(falloffCurve);
            }
        }; 

        this.setUseClosestPointRotationMuteInside(useClosestPointRotationMuteInside) {
            if (this._m1obj) {
                this._m1obj.setUseClosestPointRotationMuteInside(useClosestPointRotationMuteInside);
            }
        };

        this.setCameraPosition = function() {
            if (this._m1obj) {
                this._m1obj.setCameraPosition();
            }
        };

        this.setCameraRotation = function() {
            if (this._m1obj) {
                this._m1obj.setCameraRotation();
            }
        };

        this.setCameraRotationQuat = function() {
            if (this._m1obj) {
                this._m1obj.setCameraRotationQuat();
            }
        };

        this.getVolumesWalls = function() {
            if (this._m1obj) {
                return this._m1obj.getVolumesWalls();
            }
        };

        this.getVolumeRotation = function() {
            if (this._m1obj) {
                return this._m1obj.getVolumeRotation();
            }
        };
/* DEPRECATED END*/

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

        this.setFalloffCurveBlendMode = function(falloffCurveBlendMode) {
            if (this._m1obj) {
                this._m1obj.setFalloffCurveBlendMode(falloffCurveBlendMode);
            }
        };

        this.setAttenuationCurveBlendMode = function(attenuationCurveBlendMode) {
            if (this._m1obj) {
                this._m1obj.setAttenuationCurveBlendMode = function(attenuationCurveBlendMode);
            }
        };

        this.getVolumesRoom = function() {
            if (this._m1obj) {
                return this._m1obj.getVolumesRoom();
            }
        };

        this.getCoefficientsInterior = function() {
            if (this._m1obj) {
                return this._m1obj.getCoefficientsInterior();
            }
        };

        this._m1obj = new (Module).Mach1DecodePositionalInternal();
    };
    
    Module['onInited']();
};
