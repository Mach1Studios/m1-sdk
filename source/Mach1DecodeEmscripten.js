Module['onRuntimeInitialized'] = function() {
    console.log('loaded Mach1Decode');

    Module['Mach1Decode'] = function() {
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
            "Mach1DecodeAlgoHorizon_4": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoHorizon_4,
            "Mach1DecodeAlgoHorizonPairs": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoHorizonPairs,
            "Mach1DecodeAlgoSpatial_12": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_12,
            "Mach1DecodeAlgoSpatial_14": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_14,
            "Mach1DecodeAlgoSpatial_32": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_32,
            "Mach1DecodeAlgoSpatial_36": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_36,
            "Mach1DecodeAlgoSpatial_48": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_48,
            "Mach1DecodeAlgoSpatial_60": (Module).Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial_60,
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

        this.setFilterSpeed = function(filterSpeed) {
            if (this._m1obj) {
                this._m1obj.setFilterSpeed(filterSpeed);
            }
        };

        this.setRotation = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setRotation(yaw, pitch, roll);
            }
        };

        this.setRotationDegrees = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setRotationDegrees(yaw, pitch, roll);
            }
        };

        this.setRotationRadians = function(yaw, pitch, roll) {
            if (this._m1obj) {
                this._m1obj.setRotationRadians(yaw, pitch, roll);
            }
        };

        this.setRotationQuat = function(w, x, y, z) {
            if (this._m1obj) {
                this._m1obj.setRotationQuat(x, y, z, w);
            }
        };

        this.getFormatChannelCount = function() {
            if (this._m1obj) {
                return this._m1obj.getFormatChannelCount();
            }
        }

        this.getCurrentAngle = function() {
            if (this._m1obj) {
                return this._m1obj.getCurrentAngle();
            }
        };

        this.decode = function(yaw, pitch, roll, bufferSize, sampleIndex) {
            if (this._m1obj) {
                var v = this._m1obj.decode(yaw, pitch, roll, bufferSize ? bufferSize : 0, sampleIndex ? sampleIndex : 0);
                var a = [];
                for (var i = 0, c = v.size(); i < c; i++) {
                    a[i] = v.get(i);
                }
                v.delete();
                return a;
            }
        };

        this.decodeCoeffs = function(bufferSize, sampleIndex) {
            if (this._m1obj) {
                var v = this._m1obj.decodeCoeffs(bufferSize ? bufferSize : 0, sampleIndex ? sampleIndex : 0);
                var a = [];
                for (var i = 0, c = v.size(); i < c; i++) {
                    a[i] = v.get(i);
                }
                v.delete();
                return a;
            }
        };

        this.beginBuffer = function() {
            if (this._m1obj) {
                this._m1obj.beginBuffer();
            }
        };

        this.endBuffer = function() {
            if (this._m1obj) {
                this._m1obj.endBuffer();
            }
        };

        this._m1obj = new(Module).Mach1DecodeInternal();
    };
};