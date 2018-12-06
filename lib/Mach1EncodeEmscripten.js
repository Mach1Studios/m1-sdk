Module['Mach1Encode'] = function() {
    this._module = Mach1EncodeModule();
    this._m1obj = new _module.Mach1EncodeInternal();

    this.Mach1EncodeInputModeType = {
        "Mach1EncodeInputModeMono": this._module.Mach1EncodeInputModeType.Mach1EncodeInputModeMono,
        "Mach1EncodeInputModeStereo": this._module.Mach1EncodeInputModeType.Mach1EncodeInputModeStereo,
        "Mach1EncodeInputModeQuad": this._module.Mach1EncodeInputModeType.Mach1EncodeInputModeQuad,
        "Mach1EncodeInputModeAFormat": this._module.Mach1EncodeInputModeType.Mach1EncodeInputModeAFormat,
        "Mach1EncodeInputModeBFormat": this._module.Mach1EncodeInputModeType.Mach1EncodeInputModeBFormat,
    };

    this.Mach1EncodeOutputModeType = {
        "Mach1EncodeOutputMode4Ch": this._module.Mach1EncodeOutputModeType.Mach1EncodeOutputMode4Ch,
        "Mach1EncodeOutputMode8Ch": this._module.Mach1EncodeOutputModeType.Mach1EncodeOutputMode8Ch,
    };

    this.delete = function() {
        if (this._m1obj) {
            this._m1obj.delete();
            this._m1obj = null;
        }
    };

    this.getPoints = function() {
        if (this._m1obj) {
            return this._m1obj.getPoints();
        }
    };

    this.getGains = function() {
        if (this._m1obj) {
            return this._m1obj.getGains();
        }
    };

    this.getPointsNames = function() {
        if (this._m1obj) {
            return this._m1obj.getPointsNames();
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

    this.setRotation = function(rotation) {
        if (this._m1obj) {
            this._m1obj.setRotation(rotation);
        }
    };

    this.setDiverge = function(diverge) {
        if (this._m1obj) {
            this._m1obj.setDiverge(diverge);
        }
    };

    this.setPitch = function(pitch) {
        if (this._m1obj) {
            this._m1obj.setPitch(pitch);
        }
    };

    this.setStereoRotate = function(stereoRotate) {
        if (this._m1obj) {
            this._m1obj.setStereoRotate(stereoRotate);
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

    this.setIsotropicEncode = function(isotropicEncode) {
        if (this._m1obj) {
            this._m1obj.setIsotropicEncode(isotropicEncode);
        }
    };

};