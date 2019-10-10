"use strict";

require('promise-decode-audio-data');

let _soundPlayer_preloadCache = {};
let _soundPlayer_audioCtx = new AudioContext();

function Mach1SoundPlayer() {
	let SOUND_COUNT = 0;
    let thus = this;

    let buffer;

    let _isFromBuffer = false;
 
    let smp;
    let gainNode;
    let volumes;
    let pannerNode;

    let _isSoundReady = false;
    let countOfReadySound = 0;

    let _isPlaying = false;
    let _isDeleted = false;

    let startTime = 0;
    let stopTime = 0;
	
	let needToPlay = false;
	let playLooped = false;
	let waitToPlay = 0;

    this.setup = function(input) {
		if(Object.getPrototypeOf(input) === AudioBuffer.prototype) {
			_isFromBuffer = true;
			
			let buf = input;

			SOUND_COUNT = buf.numberOfChannels * 2;
			buffer = buf;

			init();
			
			_isSoundReady = true;
				
			if(_isSoundReady && needToPlay) {
				thus.play(playLooped, waitToPlay);
				needToPlay = false;
			}
		}
		else if(Array.isArray(input)) {
			_isFromBuffer = false;

			let audioFiles = input;
			
			SOUND_COUNT = audioFiles.length * 2;
			buffer = initArray(audioFiles.length);

			init();

			for (let i = 0; i < audioFiles.length; ++i) {
				preload(audioFiles[i], i * 2);
			}
		}
		else {
			console.error("Mach1SoundPlayer can't parse input!");
		}
    };

    function init() {
		smp = initArray(SOUND_COUNT);
		gainNode = initArray(SOUND_COUNT);
		volumes = initArray(SOUND_COUNT);
		pannerNode = initArray(SOUND_COUNT);
    }

    function initArray(count) {
        return Array.apply(null, Array(count)).map(Number.prototype.valueOf, 0.0);
    }

    function preload(path, i) {
        console.time('load file ' + path);

        fetch(path, {
            method: 'GET',
            responseType: 'arrayBuffer',
            cache: 'force-cache'
        }).then((res) => {
            return res.arrayBuffer();
        }).then((blob) => {
            if (_soundPlayer_preloadCache[path]) {
                return Promise.resolve(_soundPlayer_preloadCache[path]);
            } else {
                return _soundPlayer_audioCtx.decodeAudioData(blob);
            }
        }).then((aBuffer) => {
            buffer[Math.floor(i / 2)] = aBuffer;

            if (!_soundPlayer_preloadCache[path]) {
                _soundPlayer_preloadCache[path] = aBuffer;
            }

            return i;
        }).then((i) => {
            console.log('Mach1Sound {path: ' + path + ', i: ' + i + ', ' + (i + 1) + '} loaded');
            console.timeEnd('load file ' + path);
			
            countOfReadySound += 2;
            _isSoundReady = (SOUND_COUNT == countOfReadySound);
			
			if(_isSoundReady && needToPlay) {
				thus.play(playLooped, waitToPlay);
				needToPlay = false;
			}
			
        }).catch((err) => {
            console.warn(err);
        });
    }

    function createAudio(looped, time) {
        if (thus.isReady() && !_isPlaying) {
            for (let i = 0, j = 0; j < SOUND_COUNT / 2; ++j, i += 2) {
                // left
				
                smp[i] = _soundPlayer_audioCtx.createBufferSource();
				if(_isFromBuffer) {
					smp[i].buffer = _soundPlayer_audioCtx.createBuffer(1, buffer.length / buffer.numberOfChannels, _soundPlayer_audioCtx.sampleRate);
					smp[i].buffer.copyToChannel(buffer.getChannelData(j), 0, 0);
				}
				else {
					smp[i].buffer = buffer[j];
				}

                gainNode[i] = _soundPlayer_audioCtx.createGain();
                gainNode[i].gain.value = 0;

                pannerNode[i] = _soundPlayer_audioCtx.createPanner();
                pannerNode[i].setPosition(-1, 0, 0); // left
                pannerNode[i].panningModel = "equalpower";


                smp[i].connect(pannerNode[i]);
                pannerNode[i].connect(gainNode[i]);
                gainNode[i].connect(_soundPlayer_audioCtx.destination);

                // right
                smp[i + 1] = _soundPlayer_audioCtx.createBufferSource();
				if(_isFromBuffer) {
					smp[i + 1].buffer = _soundPlayer_audioCtx.createBuffer(1, buffer.length / buffer.numberOfChannels, _soundPlayer_audioCtx.sampleRate);
					smp[i + 1].buffer.copyToChannel(buffer.getChannelData(j), 0, 0);
				}
				else {
					smp[i + 1].buffer = buffer[j];
				}

                gainNode[i + 1] = _soundPlayer_audioCtx.createGain();
                gainNode[i + 1].gain.value = 0;

                pannerNode[i + 1] = _soundPlayer_audioCtx.createPanner();
                pannerNode[i + 1].setPosition(1, 0, 0); // right
                pannerNode[i + 1].panningModel = "equalpower";

                smp[i + 1].connect(pannerNode[i + 1]);
                pannerNode[i + 1].connect(gainNode[i + 1]);
                gainNode[i + 1].connect(_soundPlayer_audioCtx.destination);
            }

            for (let i = 0; i < SOUND_COUNT; ++i) {
                smp[i].loop = looped;
                smp[i].start(0, time);
            }

            startTime = _soundPlayer_audioCtx.currentTime - time;
            _isPlaying = true;
        }
    }

 	this.getBuffer = function() {
		let arr = initArray(SOUND_COUNT / 2);
		for (let i = 0; i < SOUND_COUNT / 2; ++i) {
            arr[i] = smp[2 * i].buffer;
		}		
		return arr;
    };

    this.isReady = function() {
        return _isSoundReady && !_isDeleted;
    };

    this.isPlaying = function() {
        return _isPlaying;
    };

    this.play = function(looped = false, time = thus.currentTime()) {
        if (this.isReady() && !_isPlaying && !_isDeleted) {
            createAudio(looped, time);
            setVolumes();
        }
		else {
			needToPlay = true;
			playLooped = looped;
			waitToPlay = time;
		}
    };
	
    this.pause = function() {
        this.stop();
    };

    this.stop = function() {
        if (this.isReady() && _isPlaying && !_isDeleted) {
            _isPlaying = false;
			needToPlay = false;

            stopTime = _soundPlayer_audioCtx.currentTime;

            for (let i = 0; i < smp.length; ++i) {
                smp[i].stop();

                if (typeof smp[i].disconnect == 'function')
                    smp[i].disconnect();
            }
        }
    };

    this.remove = function() {
        if (this.isReady()) {
            for (let i = 0; i < smp.length; ++i) {
                smp[i].stop();
            }
        }

        _isDeleted = true;
    };

    this.currentTime = function() {
        if (!this.isReady() || !_isPlaying) {
            return stopTime - startTime > 0 ? stopTime - startTime : 0;
        } else {
            return _soundPlayer_audioCtx.currentTime - startTime;
        }
    };

    this.rewind = function(time) {
        if (time < 0) {
            time = 0;
        }

        this.stop();
        this.play(time);

        console.log('rewind');
    };

    this.getVolumes = function() {
        return volumes;
    };

    function setVolumes() {
        if (thus.isReady() && _isPlaying) {
            for (let i = 0; i < smp.length; ++i) {
                gainNode[i].gain.value = volumes[i];
            }
        }
    }

    this.updateVolumes = function(vols) {
        if (Array.isArray(vols)) {
            for (let i = 0; i < SOUND_COUNT; ++i) {
                volumes[i] = vols[i];
            }
        }

        if (_isPlaying) {
            setVolumes();
        }
    };

    return this;
}

module.exports = Mach1SoundPlayer;