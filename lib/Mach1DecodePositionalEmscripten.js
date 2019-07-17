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
        this.setListenerPosition = function(x, y, z) {
            if (this._m1obj) {
                this._m1obj.setListenerPosition({x: x, y: y, z: z});
            }
        };

        this.setListenerRotation = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setListenerRotation({x: yaw, y: pitch, z: roll});
            }
        };

        this.setListenerRotationQuat = function(x, y, z, w) {
            if (this._m1obj) {
                this._m1obj.setListenerRotationQuat({x: x, y: y, z: z, w: w});
            }
        };

        this.setDecoderAlgoPosition = function(x, y, z) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoPosition({x: x, y: y, z: z});
            }
        };

        this.setDecoderAlgoRotation = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoRotation({x: yaw, y: pitch, z: roll});
            }
        };

        this.setDecoderAlgoRotationQuat = function(x, y, z, w) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoRotationQuat({x: x, y: y, z: z, w: w});
            }
        };

        this.setDecoderAlgoScale = function(x, y, z) {
            if (this._m1obj) {
                this._m1obj.setDecoderAlgoScale({x: x, y: y, z: z});
            }
        };

        this.evaluatePositionResults = function() {
            if (this._m1obj) {
                this._m1obj.evaluatePositionResults();
            }
        };

        this.getCoefficients = function() {
            if (this._m1obj) {
				var vec = new (Module).VectorFloat();
				vec.resize(18, 0);
				this._m1obj.getCoefficients(vec);
				
				var arr = [];
				for (let i = 0; i < vec.size(); i++) {
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
                return [ point.x, point.y, point.z ];
            }
        };  

        this.getCoefficientsRotation = function() {
            if (this._m1obj) {
				var point = this._m1obj.getCoefficientsRotation()
                return [ point.x, point.y, point.z ];
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

        this.setUseClosestPointRotationMuteInside = function(useClosestPointRotationMuteInside) {
            if (this._m1obj) {
                this._m1obj.setUseClosestPointRotationMuteInside(useClosestPointRotationMuteInside);
            }
        };

        this.setCameraPosition = function(x, y, z) {
            if (this._m1obj) {
                this._m1obj.setCameraPosition({x: x, y: y, z: z});
            }
        };

        this.setCameraRotation = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setCameraRotation({x: yaw, y: pitch, z: roll});
            }
        };

        this.setCameraRotationQuat = function(x, y, z, w) {
            if (this._m1obj) {
                this._m1obj.setCameraRotationQuat({x: x, y: y, z: z, w: w});
            }
        };

        this.getVolumesWalls = function() {
            if (this._m1obj) {
				var vec = new (Module).VectorFloat();
				vec.resize(18, 0);
				this._m1obj.getVolumesWalls(vec);
				
				var arr = [];
				for (let i = 0; i < vec.size(); i++) {
				  arr[i] = vec.get(i);
				}
                return arr;
            }
        };

        this.getVolumeRotation = function() {
            if (this._m1obj) {
				var point = this._m1obj.getVolumeRotation()
                return [ point.x, point.y, point.z ];
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
                this._m1obj.setAttenuationCurveBlendMode(attenuationCurveBlendMode);
            }
        };

        this.getVolumesRoom = function() {
            if (this._m1obj) {
				var vec = new (Module).VectorFloat();
				vec.resize(18, 0);
				this._m1obj.getVolumesRoom(vec);
				
				var arr = [];
				for (let i = 0; i < vec.size(); i++) {
				  arr[i] = vec.get(i);
				}
                return arr;
            }
        };

        this.getCoefficientsInterior = function() {
            if (this._m1obj) {
				var vec = new (Module).VectorFloat();
				vec.resize(18, 0);
				this._m1obj.getCoefficientsInterior(vec);
				
				var arr = [];
				for (let i = 0; i < vec.size(); i++) {
				  arr[i] = vec.get(i);
				}
                return arr;
            }
        };

        this._m1obj = new (Module).Mach1DecodePositionalInternal();
    };
    
    Module['onInited']();
};
