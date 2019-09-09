"use strict";

require('promise-decode-audio-data');

let preloadCache = {};

function Mach1SoundPlayer(audioFiles, soundCount, audioCtx = null) {
	const SOUND_COUNT = audioFiles.length * 2;
    const _audioCtx = (audioCtx == null) ? new AudioContext() : audioCtx;
    let thus = this;

    let smp = initArray(SOUND_COUNT);
    let gainNode = initArray(SOUND_COUNT);
    let volumes = initArray(SOUND_COUNT);
    let pannerNode = initArray(SOUND_COUNT);

    let buffer = initArray(audioFiles.length);

    let _isSoundReady = false;
    let countOfReadySound = 0;

    let _isPlaying = false;
    let _isDeleted = false;

    let startTime = 0;
    let stopTime = 0;
	
	let needToPlay = false;
	let playLooped = false;
	let waitToPlay = 0;

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
            if (preloadCache[path]) {
                return Promise.resolve(preloadCache[path]);
            } else {
                return _audioCtx.decodeAudioData(blob);
            }
        }).then((aBuffer) => {
            buffer[Math.floor(i / 2)] = aBuffer;

            if (!preloadCache[path]) {
                preloadCache[path] = aBuffer;
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
            for (let i = 0, j = 0; j < buffer.length; ++j, i += 2) {
                // left
                smp[i] = _audioCtx.createBufferSource();
                smp[i].buffer = buffer[j];

                gainNode[i] = _audioCtx.createGain();
                gainNode[i].gain.value = 0;

                pannerNode[i] = _audioCtx.createPanner();
                pannerNode[i].setPosition(-1, 0, 0); // left
                pannerNode[i].panningModel = "equalpower";


                smp[i].connect(pannerNode[i]);
                pannerNode[i].connect(gainNode[i]);
                gainNode[i].connect(_audioCtx.destination);

                // right
                smp[i + 1] = _audioCtx.createBufferSource();
                smp[i + 1].buffer = buffer[j];

                gainNode[i + 1] = _audioCtx.createGain();
                gainNode[i + 1].gain.value = 0;

                pannerNode[i + 1] = _audioCtx.createPanner();
                pannerNode[i + 1].setPosition(1, 0, 0); // right
                pannerNode[i + 1].panningModel = "equalpower";

                smp[i + 1].connect(pannerNode[i + 1]);
                pannerNode[i + 1].connect(gainNode[i + 1]);
                gainNode[i + 1].connect(_audioCtx.destination);
            }

            for (let i = 0; i < SOUND_COUNT; ++i) {
                smp[i].loop = looped;
                smp[i].start(0, time);
            }

            startTime = _audioCtx.currentTime - time;
            _isPlaying = true;
        }
    }

    this.getAudioContext = function() {
        return _audioCtx;
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

            stopTime = _audioCtx.currentTime;

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
            return _audioCtx.currentTime - startTime;
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

    for (let i = 0; i < audioFiles.length; ++i) {
        preload(audioFiles[i], i * 2);
    }

    return this;
}

module.exports = Mach1SoundPlayer;