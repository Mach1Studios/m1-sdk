"use strict";

require('promise-decode-audio-data');

/*
 setVolumes +
 getVolumes +
 updateVolumes
 ----------- Sound3D_OldVersion: Sound3D_OldVersion,
 ----------- getAudio: getAudio,
 play +
 pause +
 rewind +
 stop +
 isReady +
 remove +
 ----------- soundSync
 currentTime +
* */

const SOUND_COUNT = 8 * 2;
let preloadCache = {};

function Mach1SoundPlayer(audioFiles) {
    const audioCtx = new AudioContext();
    let thus = this;


    let smp = initArray(SOUND_COUNT);
    let gainNode = initArray(SOUND_COUNT);
    let volumes = initArray(SOUND_COUNT);
    let pannerNode = initArray(SOUND_COUNT);

    let buffer = initArray(audioFiles.length);

    let isSoundReady = false;
    let countOfReadySound = 0;

    let isPlaying = false;
    let isDeleted = false;

    let startTime = 0;
    let stopTime = 0;


    function initArray(count) {
        return Array.apply(null, Array(count)).map(Number.prototype.valueOf, 0.0);
    }

    function preload(path, i) {
        console.time('load file' + path);

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
                return audioCtx.decodeAudioData(blob);
            }
        }).then((aBuffer) => {
            buffer[Math.floor(i / 2)] = aBuffer;

            if (!preloadCache[path]) {
                preloadCache[path] = aBuffer;
            }

            return i;
        }).then((i) => {
            console.log('Mach1Sound {path: ' + path + ', i: ' + i + ', ' + (i + 1) + '} loaded');

            console.timeEnd('load file' + path);
            countOfReadySound += 2;
            isSoundReady = (SOUND_COUNT == countOfReadySound);
        }).catch((err) => {
            console.warn(err);
        });
    }


    function createAudio(time) {
        if (thus.isReady() && !isPlaying) {
            for (let i = 0, j = 0; j < buffer.length; ++j, i += 2) {
                // left
                smp[i] = audioCtx.createBufferSource();
                smp[i].buffer = buffer[j];

                gainNode[i] = audioCtx.createGain();
                gainNode[i].gain.value = 0;

                pannerNode[i] = audioCtx.createPanner();
                pannerNode[i].setPosition(-1, 0, 0);
                pannerNode[i].panningModel = "equalpower";


                smp[i].connect(pannerNode[i]);
                pannerNode[i].connect(gainNode[i]);
                gainNode[i].connect(audioCtx.destination);

                // right
                smp[i + 1] = audioCtx.createBufferSource();
                smp[i + 1].buffer = buffer[j];

                gainNode[i + 1] = audioCtx.createGain();
                gainNode[i + 1].gain.value = 0;

                pannerNode[i + 1] = audioCtx.createPanner();
                pannerNode[i + 1].setPosition(1, 0, 0); // must be (1, 0, 0)
                pannerNode[i + 1].panningModel = "equalpower";

                smp[i + 1].connect(pannerNode[i + 1]);
                pannerNode[i + 1].connect(gainNode[i + 1]);
                gainNode[i + 1].connect(audioCtx.destination);
            }

            for (let i = 0; i < SOUND_COUNT; ++i) {
                smp[i].start(0, time);
            }

            startTime = audioCtx.currentTime - time;
            isPlaying = true;
        }
    }


    this.isReady = function() {
        return isSoundReady && !isDeleted;
    };

    this.play = function(time = thus.currentTime()) {
        if (this.isReady() && !isPlaying && !isDeleted) {
            createAudio(time);

            setVolumes();
        }
    };

    this.pause = function() {
        this.stop();
    };

    this.stop = function() {
        if (this.isReady() && isPlaying && !isDeleted) {
            isPlaying = false;

            stopTime = audioCtx.currentTime;

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

        isDeleted = true;
    };

    this.currentTime = function() {
        if (!this.isReady() || !isPlaying) {
            return stopTime - startTime > 0 ? stopTime - startTime : 0;
        } else {
            return audioCtx.currentTime - startTime;
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
        if (thus.isReady() && isPlaying) {
            for (let i = 0; i < smp.length; ++i) {
                gainNode[i].gain.value = volumes[i];
            }
        }

    }

    this.updateVolumes = function(yaw, pitch, roll = 0) {
        let vols = Mach1Decode.decode(yaw, pitch, roll);

        if (vols) {
            for (let i = 0; i < SOUND_COUNT; ++i) {
                volumes[i] = vols[i];
            }
        }

        if (isPlaying) {
            setVolumes();
        }
    };

    for (let i = 0; i < audioFiles.length; ++i) {
        preload(audioFiles[i], i * 2);
    }

    return this;
}

module.exports = Mach1SoundPlayer;