
var Mach1Decode = {
    _m1obj: null,
    delete: function() {
        if (this._m1obj) {
            this._m1obj.delete();
            this._m1obj = null;
        }
    },
    decode: function(yaw, pitch, roll) {
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
    },
    onRuntimeInitialized: function() {
        var obj = new Mach1Decode.Mach1Decode();

        obj.setPlatformType(Mach1Decode.Mach1PlatformType.Mach1PlatformDefault);
        obj.setDecodeAlgoType(Mach1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
        obj.setFilterSpeed(1.0);

        console.log("Mach1Decode module is loaded");

        this._m1obj = obj;
    }
};
