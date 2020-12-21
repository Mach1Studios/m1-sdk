"use strict";

function _instanceof(left, right) {
  if (
    right != null &&
    typeof Symbol !== "undefined" &&
    right[Symbol.hasInstance]
  ) {
    return !!right[Symbol.hasInstance](left);
  } else {
    return left instanceof right;
  }
}

function _classCallCheck(instance, Constructor) {
  if (!_instanceof(instance, Constructor)) {
    throw new TypeError("Cannot call a class as a function");
  }
}

function _defineProperties(target, props) {
  for (var i = 0; i < props.length; i++) {
    var descriptor = props[i];
    descriptor.enumerable = descriptor.enumerable || false;
    descriptor.configurable = true;
    if ("value" in descriptor) descriptor.writable = true;
    Object.defineProperty(target, descriptor.key, descriptor);
  }
}

function _createClass(Constructor, protoProps, staticProps) {
  if (protoProps) _defineProperties(Constructor.prototype, protoProps);
  if (staticProps) _defineProperties(Constructor, staticProps);
  return Constructor;
}

function _defineProperty(obj, key, value) {
  if (key in obj) {
    Object.defineProperty(obj, key, {
      value: value,
      enumerable: true,
      configurable: true,
      writable: true
    });
  } else {
    obj[key] = value;
  }
  return obj;
}

function _classPrivateFieldSet(receiver, privateMap, value) {
  var descriptor = privateMap.get(receiver);
  if (!descriptor) {
    throw new TypeError("attempted to set private field on non-instance");
  }
  if (descriptor.set) {
    descriptor.set.call(receiver, value);
  } else {
    if (!descriptor.writable) {
      throw new TypeError("attempted to set read only private field");
    }
    descriptor.value = value;
  }
  return value;
}

function _classPrivateFieldGet(receiver, privateMap) {
  var descriptor = privateMap.get(receiver);
  if (!descriptor) {
    throw new TypeError("attempted to get private field on non-instance");
  }
  if (descriptor.get) {
    return descriptor.get.call(receiver);
  }
  return descriptor.value;
}

var _soundFilesCount = new WeakMap();

var _soundFilesCountReady = new WeakMap();

var _isDeleted = new WeakMap();

var _isFromBuffer = new WeakMap();

var _isPlaying = new WeakMap();

var _isSoundReady = new WeakMap();

var _buffer = new WeakMap();

var _volume = new WeakMap();

var _gainNode = new WeakMap();

var _gains = new WeakMap();

var _pannerNode = new WeakMap();

var _smp = new WeakMap();

var _cache = new WeakMap();

var _startTime = new WeakMap();

var _stopTime = new WeakMap();

var _currentTime = new WeakMap();

var _needToPlay = new WeakMap();

var _playLooped = new WeakMap();

var _waitToPlay = new WeakMap();

var _initArray = new WeakMap();

var _setGains = new WeakMap();

var _preload = new WeakMap();

/*
 * Nach1 Spatial Web SoundPlayer Example
 * Description: Example of an audio player for Mach1Decode API and spatial audio playback
 */

/* eslint-disable new-cap, no-alert */
var Mach1SoundPlayer = /*#__PURE__*/ (function () {
  // eslint-disable-line no-unused-vars

  /**
   * Private method which should calculate and return time before start playing,
   * based on Audio Context
   * @type {Number}
   */

  /**
   * Create new array with fixed item count, each item have zero value
   * @type {Array}
   */

  /**
   * Set default time value for a gain nodes (for all buffered sound files)
   * @type {}
   */

  /**
   * Downloads, caches, and sets default properties for the received file
   * @type {}
   */

  /**
   * @param {Array|AudioBuffer} input array with sound files paths [url]
   */
  function Mach1SoundPlayer(input) {
    var _this = this;

    _classCallCheck(this, Mach1SoundPlayer);

    _soundFilesCount.set(this, {
      writable: true,
      value: 0
    });

    _soundFilesCountReady.set(this, {
      writable: true,
      value: 0
    });

    _isDeleted.set(this, {
      writable: true,
      value: false
    });

    _isFromBuffer.set(this, {
      writable: true,
      value: false
    });

    _isPlaying.set(this, {
      writable: true,
      value: false
    });

    _isSoundReady.set(this, {
      writable: true,
      value: false
    });

    _buffer.set(this, {
      writable: true,
      value: void 0
    });

    _volume.set(this, {
      writable: true,
      value: 1.0
    });

    _gainNode.set(this, {
      writable: true,
      value: void 0
    });

    _gains.set(this, {
      writable: true,
      value: void 0
    });

    _pannerNode.set(this, {
      writable: true,
      value: void 0
    });

    _smp.set(this, {
      writable: true,
      value: void 0
    });

    _cache.set(this, {
      writable: true,
      value: {}
    });

    _defineProperty(
      this,
      "audioContext",
      window.AudioContext
        ? new window.AudioContext()
        : new window.webkitAudioContext()
    );

    _startTime.set(this, {
      writable: true,
      value: 0
    });

    _stopTime.set(this, {
      writable: true,
      value: 0
    });

    _currentTime.set(this, {
      writable: true,
      value: function value() {
        if (!_this.isReady() || !_classPrivateFieldGet(_this, _isPlaying)) {
          return _classPrivateFieldGet(_this, _stopTime) -
            _classPrivateFieldGet(_this, _startTime) >
            0
            ? _classPrivateFieldGet(_this, _stopTime) -
                _classPrivateFieldGet(_this, _startTime)
            : 0;
        }

        return (
          _this.audioContext.currentTime -
          _classPrivateFieldGet(_this, _startTime)
        );
      }
    });

    _needToPlay.set(this, {
      writable: true,
      value: false
    });

    _playLooped.set(this, {
      writable: true,
      value: false
    });

    _waitToPlay.set(this, {
      writable: true,
      value: 0
    });

    _initArray.set(this, {
      writable: true,
      value: function value() {
        var count =
          arguments.length > 0 && arguments[0] !== undefined
            ? arguments[0]
            : _classPrivateFieldGet(_this, _soundFilesCount);
        return new Array(count).fill(0.0);
      }
    });

    _setGains.set(this, {
      writable: true,
      value: function value() {
        if (_this.isReady() && _classPrivateFieldGet(_this, _isPlaying)) {
          for (
            var i = 0;
            i < _classPrivateFieldGet(_this, _smp).length;
            i += 1
          ) {
            _classPrivateFieldGet(_this, _gainNode)[i].gain.setTargetAtTime(
              _classPrivateFieldGet(_this, _gains)[i] *
                _classPrivateFieldGet(_this, _volume),
              _this.audioContext.currentTime,
              0.05
            );
          }
        }
      }
    });

    _preload.set(this, {
      writable: true,
      value: async function value(uri, number) {
        console.time("load file ".concat(uri));

        var getDecodedAudioData = function getDecodedAudioData(blob) {
          return new Promise(function (resolve) {
            if (_classPrivateFieldGet(_this, _cache)[uri]) {
              resolve(_classPrivateFieldGet(_this, _cache)[uri]);
            } else {
              _this.audioContext.decodeAudioData(
                blob,
                function (data) {
                  _classPrivateFieldGet(_this, _cache)[uri] = data;
                  resolve(data);
                },
                function () {
                  return console.error("AudioContext issue");
                }
              );
            }
          });
        };

        var options = {
          cache: "force-cache",
          method: "GET",
          responseType: "arrayBuffer"
        };

        try {
          var response = await fetch(uri, options);
          if (!response.ok) throw new Error(response.statusText);
          var blob = await response.arrayBuffer();
          var buffer = await getDecodedAudioData(blob);
          _classPrivateFieldGet(_this, _buffer)[number] = buffer;
          console.log(
            "[MACH1] Mach1SoundPlayer {path: "
              .concat(uri, ", i: ")
              .concat(number * 2, ", ")
              .concat(number * 2 + 1, "} loaded")
          );
          console.timeEnd("load file ".concat(uri));

          _classPrivateFieldSet(
            _this,
            _soundFilesCountReady,
            _classPrivateFieldGet(_this, _soundFilesCountReady) + 2
          );

          _classPrivateFieldSet(
            _this,
            _isSoundReady,
            _classPrivateFieldGet(_this, _soundFilesCountReady) ===
              _classPrivateFieldGet(_this, _soundFilesCount)
          );
        } catch (e) {
          _classPrivateFieldSet(_this, _isSoundReady, false);

          console.timeEnd("doesn't load file ".concat(uri));
          throw new Error(
            "Can't load sound files; Completed "
              .concat(_classPrivateFieldGet(_this, _soundFilesCountReady), "/")
              .concat(_classPrivateFieldGet(_this, _soundFilesCount))
          ); // NOTE: If need here can add some logs case or any requirement action
        }
      }
    });

    if (Object.getPrototypeOf(input) === AudioBuffer.prototype) {
      _classPrivateFieldSet(this, _isFromBuffer, true);

      var buf = input;

      _classPrivateFieldSet(this, _soundFilesCount, buf.numberOfChannels * 2);

      _classPrivateFieldSet(this, _buffer, buf);

      _classPrivateFieldSet(
        this,
        _gainNode,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      _classPrivateFieldSet(
        this,
        _gains,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      _classPrivateFieldSet(
        this,
        _pannerNode,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      _classPrivateFieldSet(
        this,
        _smp,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      _classPrivateFieldSet(this, _isSoundReady, true);
    } else if (Array.isArray(input)) {
      _classPrivateFieldSet(this, _isFromBuffer, false);

      var audioFiles = input;

      _classPrivateFieldSet(this, _soundFilesCount, audioFiles.length * 2);

      _classPrivateFieldSet(
        this,
        _buffer,
        _classPrivateFieldGet(this, _initArray).call(this, audioFiles.length)
      );

      _classPrivateFieldSet(
        this,
        _gainNode,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      _classPrivateFieldSet(
        this,
        _gains,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      _classPrivateFieldSet(
        this,
        _pannerNode,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      _classPrivateFieldSet(
        this,
        _smp,
        _classPrivateFieldGet(this, _initArray).call(
          this,
          _classPrivateFieldGet(this, _soundFilesCount)
        )
      );

      audioFiles.forEach(_classPrivateFieldGet(this, _preload));
    } else {
      console.error("Mach1SoundPlayer can't parse input!");
    }
  }
  /**
   * Return progress information in percent
   * @return {String} Percentages from 0 to 100 as a string [integer]
   */

  _createClass(Mach1SoundPlayer, [
    {
      key: "play",

      /**
       * Start playing sound files
       */
      value: function play(looped) {
        var time =
          arguments.length > 1 && arguments[1] !== undefined
            ? arguments[1]
            : _classPrivateFieldGet(this, _currentTime).call(this);

        if (
          this.isReady() &&
          !_classPrivateFieldGet(this, _isPlaying) &&
          !_classPrivateFieldGet(this, _isDeleted)
        ) {
          if (this.isReady() && !_classPrivateFieldGet(this, _isPlaying)) {
            for (
              var i = 0, j = 0;
              j < _classPrivateFieldGet(this, _soundFilesCount) / 2;
              j += 1, i += 2
            ) {
              _classPrivateFieldGet(this, _smp)[
                i
              ] = this.audioContext.createBufferSource();

              if (_classPrivateFieldGet(this, _isFromBuffer)) {
                _classPrivateFieldGet(this, _smp)[
                  i
                ].buffer = this.audioContext.createBuffer(
                  1,
                  _classPrivateFieldGet(this, _buffer).length /
                    _classPrivateFieldGet(this, _buffer).numberOfChannels,
                  this.audioContext.sampleRate
                );

                _classPrivateFieldGet(this, _smp)[i].buffer.copyToChannel(
                  _classPrivateFieldGet(this, _buffer).getChannelData(j),
                  0,
                  0
                );
              } else {
                _classPrivateFieldGet(this, _smp)[
                  i
                ].buffer = _classPrivateFieldGet(this, _buffer)[j];
              }

              _classPrivateFieldGet(this, _gainNode)[
                i
              ] = this.audioContext.createGain();
              _classPrivateFieldGet(this, _gainNode)[i].gain.value = 0;
              /**
               * Create left side players of coeffs
               */

              _classPrivateFieldGet(this, _pannerNode)[
                i
              ] = this.audioContext.createPanner();

              _classPrivateFieldGet(this, _pannerNode)[i].setPosition(-1, 0, 0); // left

              _classPrivateFieldGet(this, _pannerNode)[i].panningModel =
                "equalpower";

              _classPrivateFieldGet(this, _smp)[i].connect(
                _classPrivateFieldGet(this, _pannerNode)[i]
              );

              _classPrivateFieldGet(this, _pannerNode)[i].connect(
                _classPrivateFieldGet(this, _gainNode)[i]
              );

              _classPrivateFieldGet(this, _gainNode)[i].connect(
                this.audioContext.destination
              ); // RIGHT PLAYERS

              _classPrivateFieldGet(this, _smp)[
                i + 1
              ] = this.audioContext.createBufferSource();

              if (_classPrivateFieldGet(this, _isFromBuffer)) {
                _classPrivateFieldGet(this, _smp)[
                  i + 1
                ].buffer = this.audioContext.createBuffer(
                  1,
                  _classPrivateFieldGet(this, _buffer).length /
                    _classPrivateFieldGet(this, _buffer).numberOfChannels,
                  this.audioContext.sampleRate
                );

                _classPrivateFieldGet(this, _smp)[i + 1].buffer.copyToChannel(
                  _classPrivateFieldGet(this, _buffer).getChannelData(j),
                  0,
                  0
                );
              } else {
                _classPrivateFieldGet(this, _smp)[
                  i + 1
                ].buffer = _classPrivateFieldGet(this, _buffer)[j];
              }

              _classPrivateFieldGet(this, _gainNode)[
                i + 1
              ] = this.audioContext.createGain();
              _classPrivateFieldGet(this, _gainNode)[i + 1].gain.value = 0;
              /**
               * Create right side players of coeffs
               */

              _classPrivateFieldGet(this, _pannerNode)[
                i + 1
              ] = this.audioContext.createPanner();

              _classPrivateFieldGet(this, _pannerNode)[i + 1].setPosition(
                1,
                0,
                0
              ); // right

              _classPrivateFieldGet(this, _pannerNode)[i + 1].panningModel =
                "equalpower";

              _classPrivateFieldGet(this, _smp)[i + 1].connect(
                _classPrivateFieldGet(this, _pannerNode)[i + 1]
              );

              _classPrivateFieldGet(this, _pannerNode)[i + 1].connect(
                _classPrivateFieldGet(this, _gainNode)[i + 1]
              );

              _classPrivateFieldGet(this, _gainNode)[i + 1].connect(
                this.audioContext.destination
              );
            }

            for (
              var _i = 0;
              _i < _classPrivateFieldGet(this, _soundFilesCount);
              _i += 1
            ) {
              _classPrivateFieldGet(this, _smp)[_i].loop = looped;

              _classPrivateFieldGet(this, _smp)[_i].start(0, time);
            }

            _classPrivateFieldSet(
              this,
              _startTime,
              this.audioContext.currentTime - time
            );

            _classPrivateFieldSet(this, _isPlaying, true);
          }

          _classPrivateFieldGet(this, _setGains).call(this);
        } else {
          _classPrivateFieldSet(this, _needToPlay, true);

          _classPrivateFieldSet(this, _playLooped, looped);

          _classPrivateFieldSet(this, _waitToPlay, time);
        }
      }
      /**
       * Stopping play any sound file
       */
    },
    {
      key: "stop",
      value: function stop() {
        if (
          this.isReady() &&
          _classPrivateFieldGet(this, _isPlaying) &&
          !_classPrivateFieldGet(this, _isDeleted)
        ) {
          _classPrivateFieldSet(this, _isPlaying, false);

          _classPrivateFieldSet(this, _needToPlay, false);

          _classPrivateFieldSet(this, _stopTime, this.audioContext.currentTime); // FIXME: Need to change this part

          for (
            var i = 0;
            i < _classPrivateFieldGet(this, _smp).length;
            i += 1
          ) {
            _classPrivateFieldGet(this, _smp)[i].stop();

            if (
              typeof _classPrivateFieldGet(this, _smp)[i].disconnect ===
              "function"
            )
              _classPrivateFieldGet(this, _smp)[i].disconnect();
          }
        }
      }
      /**
       * Alias for the this.stop() method
       */
    },
    {
      key: "pause",
      value: function pause() {
        this.stop();
      }
    },
    {
      key: "remove",
      value: function remove() {
        if (this.isReady())
          _classPrivateFieldGet(this, _smp).forEach(function (smp) {
            return smp.stop();
          });

        _classPrivateFieldSet(this, _isDeleted, true);
      }
    },
    {
      key: "rewind",
      value: function rewind() {
        var time =
          arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 0;
        this.stop();
        this.play(time >= 0 ? time : 0);
      }
    },
    {
      key: "isReady",
      value: function isReady() {
        return (
          _classPrivateFieldGet(this, _isSoundReady) &&
          !_classPrivateFieldGet(this, _isDeleted)
        );
      }
    },
    {
      key: "isPlaying",
      value: function isPlaying() {
        return _classPrivateFieldGet(this, _isPlaying);
      }
    },
    {
      key: "getAudioContext",
      value: function getAudioContext() {
        return this.audioContext;
      }
    },
    {
      key: "progress",
      get: function get() {
        return (
          (_classPrivateFieldGet(this, _soundFilesCountReady) /
            _classPrivateFieldGet(this, _soundFilesCount)) *
          100
        ).toFixed(0);
      }
      /**
       * Setting gains for all files
       * @param  {Array} vols binding new gain values by index
       */
    },
    {
      key: "gains",
      set: function set(vols) {
        if (Array.isArray(vols)) {
          // FIXME: Need to refactor this part [switch to default array method like a forEach]
          for (var i = 0; i < vols.length; i += 1) {
            _classPrivateFieldGet(this, _gains)[i] = vols[i];
          }
        }

        if (_classPrivateFieldGet(this, _isPlaying)) {
          _classPrivateFieldGet(this, _setGains).call(this);
        }
      },
      /**
       * Getting gains for all files
       * @param  {Array} vols return last gain values by index
       */
      get: function get() {
        return _classPrivateFieldGet(this, _gains);
      }
      /**
       * Setting Master Gain/Volume
       * @param  {Array} volume
       */
    },
    {
      key: "volume",
      set: function set(vol) {
        _classPrivateFieldSet(this, _volume, parseFloat(vol));
      },
      /**
       * Return Master Gain/Volume
       * @return {String} Volume from 0 to 1 as a float
       */
      get: function get() {
        return _classPrivateFieldGet(this, _volume);
      }
    }
  ]);

  return Mach1SoundPlayer;
})();
