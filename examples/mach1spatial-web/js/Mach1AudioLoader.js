"use strict";

// require('promise-decode-audio-data');

let _audioLoader_preloadCache = {};
let _audioLoader_audioCtx = new AudioContext();

function Mach1AudioLoader(audioFiles, channelsCount) {
    const FILES_COUNT = audioFiles.length;
    const CHANNELS_COUNT = channelsCount;

    let thus = this;

    let buffer = initArray(FILES_COUNT);
    let finalBuffer = null;

    let _isSoundReady = false;
    let _countOfReadySound = 0;

	let loadedCallback = function() {};

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
            if (_audioLoader_preloadCache[path]) {
                return Promise.resolve(_audioLoader_preloadCache[path]);
            } else {
                return _audioLoader_audioCtx.decodeAudioData(blob);
            }
        }).then((aBuffer) => {
            buffer[i] = aBuffer;

            if (!_audioLoader_preloadCache[path]) {
                _audioLoader_preloadCache[path] = aBuffer;
            }

            return i;
        }).then((i) => {
            console.log('Mach1AudioLoader {path: ' + path + ', i: ' + i + '} loaded');
            console.timeEnd('load file ' + path);

            _countOfReadySound++;
            if (_countOfReadySound == FILES_COUNT) {

                var minLength = Number.MAX_VALUE;
                for (let i = 0; i < buffer.length; i++) {
                    //minLength = Math.min(minLength, buffer[i].length / buffer[i].numberOfChannels);
                    //TODO: Why does this fix length of play issues?
                    minLength = buffer[i].length*channelsCount;
                }

                finalBuffer = new AudioBuffer({
                    numberOfChannels: channelsCount,
                    sampleRate: _audioLoader_audioCtx.sampleRate,
                    length: minLength
                });

                for (let i = 0; i < minLength; i++) {
                    for (let c = 0; c < channelsCount; c++) {
                        if (FILES_COUNT == 1 && buffer[0].numberOfChannels == CHANNELS_COUNT) {
                            finalBuffer.getChannelData(c)[i] = buffer[0].getChannelData(c)[i];
                        } else if (FILES_COUNT == CHANNELS_COUNT) {
                            finalBuffer.getChannelData(c)[i] = buffer[c].getChannelData(0)[i];
                        } else {
                            finalBuffer.getChannelData(c)[i] = buffer[0].getChannelData(0)[i];
                        }
                    }

                }
				
                _isSoundReady = true;

				loadedCallback();
            }

        }).catch((err) => {
            console.warn(err);
        });
    }

	this.addLoadedCallback = function(callback) {
	  loadedCallback = callback;
	}

    this.getBuffer = function() {
        return _isSoundReady ? finalBuffer : null;
    };

    this.isReady = function() {
        return _isSoundReady;
    };

    for (let i = 0; i < FILES_COUNT; i++) {
        preload(audioFiles[i], i);
    }

    return this;
}

// module.exports = Mach1SoundPlayer;