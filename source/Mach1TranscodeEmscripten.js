Module['onRuntimeInitialized'] = function() {
    console.log('loaded Mach1Transcode');

    Module['Mach1Transcode'] = function() {
        this.delete = function() {
            if (this._m1obj) {
                this._m1obj.delete();
                this._m1obj = null;
            }
        };

        this.getInputNumChannels = function() {
            if (this._m1obj) {
                return this._m1obj.getInputNumChannels();
            }
        };

        this.getOutputNumChannels = function() {
            if (this._m1obj) {
                return this._m1obj.getOutputNumChannels();
            }
        };

        this.getFormatFromString = function(str) {
            if (this._m1obj) {
                return this._m1obj.getFormatFromString(str);
            }
        };

        this.getFormatName = function(fmt) {
            if (this._m1obj) {
                return this._m1obj.getFormatName(fmt);
            }
        };

        this.processNormalization = function(bufs) {
            if (this._m1obj) {
                return this._m1obj.processNormalization(bufs);
            }
        };

        this.processMasterGain = function(bufs, masterGain) {
            if (this._m1obj) {
                this._m1obj.processMasterGain(bufs, masterGain);
            }
        };

        this.db2level = function(db) {
            if (this._m1obj) {
                return this._m1obj.db2level(db);
            }
        };

        this.level2db = function(level) {
            if (this._m1obj) {
                return this._m1obj.level2db(level);
            }
        };

        this.setLFESub = function(subChannelIndices, sampleRate) {
            if (this._m1obj) {
                this._m1obj.setLFESub(subChannelIndices, sampleRate);
            }
        };

        this.setSpatialDownmixer = function(corrThreshold) {
            if (this._m1obj) {
                this._m1obj.setSpatialDownmixer(corrThreshold);
            }
        };

        this.getSpatialDownmixerPossibility = function() {
            if (this._m1obj) {
                return this._m1obj.getSpatialDownmixerPossibility();
            }
        };

        this.setInputFormat = function(inFmt) {
            if (this._m1obj) {
                this._m1obj.setInputFormat(inFmt);
            }
        };

        this.setInputFormatCustomPointsJson = function(inJson) {
            if (this._m1obj) {
                this._m1obj.setInputFormatTTJson(inJson);
            }
        };

        this.setInputFormatCustomPoints = function(points) {
            if (this._m1obj) {
                this._m1obj.setInputFormatTTPoints(points);
            }
        };

        this.setOutputFormat = function(outFmt) {
            if (this._m1obj) {
                this._m1obj.setOutputFormat(outFmt);
            }
        };

        this.setOutputFormatCustomPointsJson = function(outJson) {
            if (this._m1obj) {
                this._m1obj.setOutputFormatTTJson(outJson);
            }
        };

        this.setOutputFormatCustomPoints = function(points) {
            if (this._m1obj) {
                this._m1obj.setOutputFormatTTPoints(points);
            }
        };

        this.processConversionPath = function() {
            if (this._m1obj) {
                return this._m1obj.processConversionPath();
            }
        };

        this.getMatrixConversion = function() {
            if (this._m1obj) {
                var obj = this._m1obj.getMatrixConversion();
                var arr = [];
                for (var i = 0; i < obj.size(); i++) {
                    arr[i] = [];
                    var obj2 = obj.get(i);
                    for (var j = 0; j < obj2.size(); j++) {
                        arr[i][j] = obj2.get(j);
                    }

                }
                return arr;
            }
        };

        this.processConversion = function(inBufs, outBufs) {
            if (this._m1obj) {
                return this._m1obj.processConversion(inBufs, outBufs);
            }
        };

        this.getFormatConversionPath = function() {
            if (this._m1obj) {
                var obj = this._m1obj.getFormatConversionPath();
                var arr = [];
                for (var i = 0; i < obj.size(); i++) {
                    arr[i] = obj.get(i);
                }
                return arr;
            }
        };

        this._m1obj = new(Module).Mach1TranscodeInternal();
    };
};