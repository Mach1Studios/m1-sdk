Module['onRuntimeInitialized'] = function() {
	console.log('loaded Mach1Decode');
};

Module['Mach1Decode'] = function() {

	this.Mach1PlatformType = {
        "Mach1PlatformDefault": (Module).Mach1PlatformType.Mach1PlatformDefault,
        "Mach1PlatformUnity": (Module).Mach1PlatformType.Mach1PlatformUnity,
        "Mach1PlatformUE": (Module).Mach1PlatformType.Mach1PlatformUE,
        "Mach1PlatformOfEasyCam": (Module).Mach1PlatformType.Mach1PlatformOfEasyCam,
        "Mach1PlatformAndroid": (Module).Mach1PlatformType.Mach1PlatformAndroid,
        "Mach1PlatformiOSPortrait": (Module).Mach1PlatformType.Mach1PlatformiOSPortrait,
        "Mach1PlatformiOSLandscape": (Module).Mach1PlatformType.Mach1PlatformiOSLandscape,
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
            this._m1obj.setPlatformType(decodeAlgo);
        }
    };

    this.setFilterSpeed = function(filterSpeed) {
        if (this._m1obj) {
            this._m1obj.setFilterSpeed(filterSpeed);
        }
    };

    this.getCurrentAngle = function() {
        if (this._m1obj) {
            return this._m1obj.getCurrentAngle();
        }
    };

    this.decode = function(yaw, pitch, roll) {
        if (this._m1obj) {
            this._m1obj.beginBuffer();
            var v = this._m1obj.decode(yaw, pitch, roll, 0, 0);
            var a = [];
            for (var i = 0, c = v.size(); i < c; i++) {
                a[i] = v.get(i);
            }
            v.delete();
            this._m1obj.endBuffer();
            return a;
        }
    };

	
    this._m1obj = new (Module).Mach1DecodeInternal();
    this.setPlatformType(this.Mach1PlatformType.Mach1PlatformDefault);
    this.setDecodeAlgoType(this.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
    this.setFilterSpeed(1.0);
};