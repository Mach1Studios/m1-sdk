Module['Mach1Decode'] = function() {
    this._module = Mach1DecodeModule();
    this._m1obj = new this._module.Mach1DecodeInternal();

    this.setPlatformType(this._module.Mach1PlatformType.Mach1PlatformDefault);
    this.setDecodeAlgoType(this._module.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
    this.setFilterSpeed(1.0);

    this.Mach1PlatformType = {
        "Mach1PlatformDefault": this._module.Mach1PlatformType.Mach1PlatformDefaul,
        "Mach1PlatformUnity": this._module.Mach1PlatformType.Mach1PlatformUnity,
        "Mach1PlatformUE": this._module.Mach1PlatformType.Mach1PlatformUE,
        "Mach1PlatformOfEasyCam": this._module.Mach1PlatformType.Mach1PlatformOfEasyCam,
        "Mach1PlatformAndroid": this._module.Mach1PlatformType.Mach1PlatformAndroid,
        "Mach1PlatformiOSPortrait": this._module.Mach1PlatformType.Mach1PlatformiOSPortrait,
        "Mach1PlatformiOSLandscape": this._module.Mach1PlatformType.Mach1PlatformiOSLandscape,
    };

    this.Mach1DecodeAlgoType = {
        "Mach1DecodeAlgoSpatial": this._module.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial,
        "Mach1DecodeAlgoAltSpatial": this._module.Mach1DecodeAlgoType.Mach1DecodeAlgoAltSpatial,
        "Mach1DecodeAlgoHorizon": this._module.Mach1DecodeAlgoType.Mach1DecodeAlgoHorizon,
        "Mach1DecodeAlgoHorizonPairs": this._module.Mach1DecodeAlgoType.Mach1DecodeAlgoHorizonPairs,
        "Mach1DecodeAlgoSpatialPairs": this._module.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatialPairs,
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

};
