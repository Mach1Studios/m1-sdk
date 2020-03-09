Module['onInited'] = function() {
};

Module['onRuntimeInitialized'] = function() {
    console.log('loaded Mach1Encode');

	Module['Mach1Encode'] = function() {
		this.Mach1EncodeInputModeType = {
			"Mach1EncodeInputModeMono": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeMono,
			"Mach1EncodeInputModeStereo": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeStereo,
			"Mach1EncodeInputModeQuad": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeQuad,
			"Mach1EncodeInputModeAFormat": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeAFormat,
			"Mach1EncodeInputModeBFormat": (Module).Mach1EncodeInputModeType.Mach1EncodeInputModeBFormat,
		};

		this.Mach1EncodeOutputModeType = {
			"Mach1EncodeOutputMode4Ch": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputMode4Ch,
			"Mach1EncodeOutputMode8Ch": (Module).Mach1EncodeOutputModeType.Mach1EncodeOutputMode8Ch,
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

		this.getPoints = function() {
			if (this._m1obj) {
				var arr = this._m1obj.getPoints();
				var points = [];
				for (var i = 0; i < arr.size(); i++) {
					points[i] = {
						x: arr.get(i).x,
						y: arr.get(i).y,
						z: arr.get(i).z
					};
				}
				arr.delete();
				arr = null;
				
				return points
			}
		};

		this.getGains = function() {
			if (this._m1obj) {
				var arr = this._m1obj.getGains();
				var gains = [];
				for (var i = 0; i < arr.size(); i++) {
					gains[i] = [];
					var _arr = arr.get(i);
					for (var j = 0; j < _arr.size(); j++) {
						gains[i][j] = _arr.get(j);
					}
					_arr.delete();
					_arr = null;
				}
				arr.delete();
				arr = null;

				return gains;
			}
		};

		this.getResultingVolumesDecoded = function(decodeType, decodeResult) {
			if (this._m1obj) {
				var vec = new (Module).VectorFloat();
				for (var i = 0; i < decodeResult.length; i++) {
					vec.push_back(decodeResult[i]);
				}
				
				var arr = this._m1obj.getResultingVolumesDecoded(decodeType, vec);
				var volumes = [];
				for (var i = 0; i < arr.size(); i++) {
					volumes[i] = arr.get(i);
				}
				arr.delete();
				arr = null;

				vec.delete();
				vec = null;

				return volumes;
			}
		};

		this.getPointsNames = function() {
			if (this._m1obj) {
				var arr = this._m1obj.getPointsNames();
				var pointsNames = [];
				for (var i = 0; i < arr.size(); i++) {
					pointsNames[i] = arr.get(i);
				}
				arr.delete();
				arr = null;

				return pointsNames;
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
				this._m1obj.setAzimuthDegrees(rotation);
			}
		};

    this.setAzimuth = function(azimuth) {
      if (this._m1obj) {
        this._m1obj.setAzimuth(azimuth);
      }
    };

    this.setAzimuthDegrees = function(azimuth) {
      if (this._m1obj) {
        this._m1obj.setAzimuthDegrees(azimuth);
      }
    };

     this.setAzimuthRadians = function(azimuth) {
      if (this._m1obj) {
        this._m1obj.setAzimuthRadians(azimuth);
      }
    };

		this.setDiverge = function(diverge) {
			if (this._m1obj) {
				this._m1obj.setDiverge(diverge);
			}
		};

		this.setPitch = function(pitch) {
			if (this._m1obj) {
				this._m1obj.setElevationDegrees(pitch);
			}
		};

    this.setElevation = function(elevation) {
      if (this._m1obj) {
        this._m1obj.setElevation(elevation);
      }
    };

    this.setElevationDegrees = function(elevation) {
      if (this._m1obj) {
        this._m1obj.setElevationDegrees(elevation);
      }
    };

    this.setElevationRadians = function(elevation) {
      if (this._m1obj) {
        this._m1obj.setElevationRadians(elevation);
      }
    };

    this.setIsotropicEncode = function(isotropicEncode) {
      if (this._m1obj) {
        this._m1obj.setIsotropicEncode(isotropicEncode);
      }
    };

		this.setStereoRotate = function(stereoRotate) {
			if (this._m1obj) {
				this._m1obj.setOrbitRotationDegrees(stereoRotate);
			}
		};

    this.setOrbitRotation = function(orbitRotation) {
      if (this._m1obj) {
        this._m1obj.setOrbitRotation(orbitRotation);
      }
    };

    this.setOrbitRotationDegrees = function(orbitRotation) {
      if (this._m1obj) {
        this._m1obj.setOrbitRotationDegrees(orbitRotation);
      }
    };

    this.setOrbitRotationRadians = function(orbitRotation) {
      if (this._m1obj) {
        this._m1obj.setOrbitRotationRadians(orbitRotation);
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

		this._m1obj = new(Module).Mach1EncodeInternal();
	};

	Module['onInited']();
};
