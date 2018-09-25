"use strict";

require('promise-decode-audio-data');

const eightChannelsAlgorithm = require('./channelAlg').eightChannelsAlgorithm;

const SOUND_COUNT = 8 * 2;

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

let preloadCache = {};

function initArray(count) {
	let arr = [];
	arr.length = count;

	arr.forEach((i, el, arr) => arr[i] = 0);

	return arr;
}

function Sound(audioFiles) {
	const audioCtx = new AudioContext();
	let thus = this;

	let smp = initArray(SOUND_COUNT);
	let gainNode = initArray(SOUND_COUNT);
	let volumes = initArray(SOUND_COUNT);
	let pannerNode = initArray(SOUND_COUNT);
	let buffer = initArray(audioFiles.length);
	let isSoundReady = false;
	let countOfReadySound = 0;

	let isPlay = false;
	let isDelete = false;
	let startTime = 0;
	let stopTime = 0;


	function preload(path, i) {
		console.time('load file' + path);

		fetch(path, {
			method: 'GET',
			responseType: 'arrayBuffer',
			cache: 'force-cache'
		}).then((res) => {
			return res.arrayBuffer();
		}).then((blob) => {
			if (preloadCache[path])
			{
				return Promise.resolve(preloadCache[path]);
			}
			else
			{
				return audioCtx.decodeAudioData(blob);
			}
		}).then((aBuffer) => {
			buffer[Math.floor(i / 2)] = aBuffer;

			if (!preloadCache[path])
			{
				preloadCache[path] = aBuffer;
			}

			return i;
		}).then((i) => {
			console.log('Sound {path: ' + path + ', i: ' + i + ', ' + (i + 1) + '} loaded');

			console.timeEnd('load file' + path);
			countOfReadySound += 2;
			isSoundReady = (SOUND_COUNT == countOfReadySound);
		}).catch((err) => {
			console.warn(err);
		});
	}


	function createAudio(time) {
		if (thus.isReady() && !isPlay)
		{
			for (let i = 0, j = 0; j < buffer.length; ++j, i += 2)
			{
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

            for (let i = 0; i < SOUND_COUNT; ++i)
            {
                smp[i].start(0, time);
            }

            startTime = audioCtx.currentTime - time;
			isPlay = true;
        }
    }


	this.isReady = function () {
		return isSoundReady && !isDelete;
	};

	this.play = function (time = thus.currentTime()) {
		if (this.isReady() && !isPlay && !isDelete)
		{
			createAudio(time);

			setVolumes();
		}
	};

	this.pause = function () {
		this.stop();
	};

	this.stop = function () {
        if (this.isReady() && isPlay && !isDelete)
        {
            isPlay = false;

            stopTime = audioCtx.currentTime;

            for (let i = 0; i < smp.length; ++i)
            {
                smp[i].stop();

                if (typeof smp[i].disconnect == 'function')
	                smp[i].disconnect();
            }
        }
	};

	this.remove = function () {
		if (this.isReady())
		{
            for (let i = 0; i < smp.length; ++i)
            {
                smp[i].stop();
            }
        }

		isDelete = true;
	};

	this.currentTime = function () {
		if (!this.isReady() || !isPlay)
		{
			return stopTime - startTime > 0 ? stopTime - startTime : 0;
		}
		else
		{
			return audioCtx.currentTime - startTime;
		}
	};

	this.rewind = function (time) {
		if (time < 0)
		{
			time = 0;
		}

		this.stop();
		this.play(time);

		console.log('rewind');
	};

	this.getVolumes = function () {
		return volumes;
	};

	function setVolumes() {
		if (thus.isReady() && isPlay)
		{
			for (let i = 0; i < smp.length; ++i)
			{
				gainNode[i].gain.value = volumes[i];
			}
		}

	}

	this.updateVolumes = function(yaw, pitch, roll = 0) {
		let vols = eightChannelsAlgorithm(yaw, pitch, roll);

		for (let i = 0; i < SOUND_COUNT; ++i) {
			volumes[i] = vols[i];
		}

		if (isPlay)
		{
            setVolumes();
		}
	};

	for (let i = 0; i < audioFiles.length; ++i)
	{
		preload(audioFiles[i], i * 2);
	}

	return this;
}

/*
function Sound(audioFiles, player) {
	let audio = {
		left: [8],
		right: [8]
	};

	let statusEl = document.getElementById("status");
	let voidEl = document.getElementById('void');

	let isDelete = false;

	let delta = 0;

	let _volumes = {
		left: [8],
		right: [8]
	};

	let __TEST__timeShift = {
		left: [1, 0, 0, 1, 1, 0, 0, 1],
		right: [1, 0, 0, 1, 1, 0, 0, 1]
	};

	let __TEST__shift = 0;

	let loaded = false;
	let loadedCount = 0;

	let isPlay = false;
	let isSoundReady = false;

	let maxPhaseError = 0;


	function preloadAudio(i, side) {
		audio[side][i] = new Audio();
		audio[side][i].preload = 'auto';
		audio[side][i].addEventListener('canplaythrough', loadedAudio, false);
		audio[side][i].src = audioFiles[side][i];
	}

	function loadedAudio() {
		if (!loaded) {
			loadedCount++;
			loaded = (loadedCount == audioFiles.left.length + audioFiles.right.length);

			isSoundReady = loaded;
			statusEl.innerHTML = "loaded: " + (100 * loadedCount / (audioFiles.left.length + audioFiles.right.length));
		}
	}

	function Sound3D_OldVersion(angle) {
		let volumes = [0.0, 0.0, 0.0, 0.0];

		volumes[0] = 1.0 - Math.min(1.0, Math.min(360 - angle, angle) / 90.0);
		volumes[1] = 1.0 - Math.min(1.0, Math.abs(90 - angle, angle) / 90.0);
		volumes[2] = 1.0 - Math.min(1.0, Math.abs(180 - angle, angle) / 90.0);
		volumes[3] = 1.0 - Math.min(1.0, Math.abs(270 - angle, angle) / 90.0);

		return volumes;
	}

	function setVolumeOnSide(volumes, side) {
		for (let i in volumes[side]) {
			if (volumes[side].hasOwnProperty(i)) {
				if (volumes[side][i] < 0 || volumes[side][i] > 1) {
					console.warn('Volumes error(out of range):' + volumes);

					return;
				}
			}
		}

		for (let i in audio[side]) {
			audio[side][i].volume = volumes[side][i];
		}
	}

	function getVolumes() {
		return _volumes;
	}

	function setVolumes(volumes = _volumes) {
		if (isReady() && !isDelete)
		{
			setVolumeOnSide(volumes, 'left');
			setVolumeOnSide(volumes, 'right');

			_volumes = volumes;
		}

		return getVolumes();
	}

	function __TEST__soundSync(d) {
		__TEST__shift = d;
	}

	function rewind(time) {
		if (time < 0) {
			time = 0;
		}

		for (let side of ['left', 'right']) {
			for (let i in audio[side]) {
				if (audio[side][i].duration < time) {
					console.warn('Rewind(out of range):' + time + ' > ' + audioFiles[side][i] + '.duration = ' + audio[side][i].duration);
					stop();

					return;
				}
				audio[side][i].currentTime = time + __TEST__timeShift[side][i] * __TEST__shift / 1000;
			}
		}
	}

	function getAudio() {
		return audio;
	}

	function play() {
		if (!isPlay && !isDelete) {
			for (let side of ['left', 'right']) {
				for (let i in audio[side]) {
					audio[side][i].play();
				}
			}

			isPlay = true;
		}
	}

	function pause() {
		if (isPlay && !isDelete) {
			for (let side of ['left', 'right']) {
				for (let i in audio[side]) {
					audio[side][i].pause();
				}
			}

			isPlay = false;
		}
	}

	function stop() {
		rewind(0);
		pause();
	}

	function isReady() {
		return isSoundReady
	}

	function currentTime() {
		return (audio['left'][0].currentTime + audio['right'][0].currentTime) / 2;
	}

	function updateVolumes(yaw, pitch, roll = 0) {
		let vols = eightChannelsAlgorithm(yaw, pitch, roll);

		for (let i = 0; i < 8; ++i) {
			_volumes.left[i] = vols[2 * i];
		}

		for (let i = 0; i < 8; ++i) {
			_volumes.right[i] = vols[2 * i + 1];
		}

		setVolumes();

		return delta;
	}

	function remove() {
		stop();

		clearInterval(logTimeout);

		statusEl = voidEl;
		isDelete = true;
	}

	// auto start preloading
	for (let i in audioFiles.left) {
		preloadAudio(i, 'left');
	}
	for (let i in audioFiles.right) {
		preloadAudio(i, 'right');
	}

	let logTimeout = setInterval(function () {
		if (loaded) {
			let min = 3600; // hour
			let max = 0;

			statusEl.innerHTML = "currentTime: <br/>";

			for (let side of ['left', 'right']) {
				for (let i in audio[side]) {
					let pos = audio[side][i].currentTime;
					if (pos < min) {
						min = pos;
					}
					if (pos > max) {
						max = pos;
					}

					// statusEl.innerHTML += audioFiles[side][i] + ". " + pos + " , volume: " + audio[side][i].volume + "<br/>";
				}
			}

			if ((max - min) * 1000 > maxPhaseError) {
				maxPhaseError = (max - min) * 1000;
			}

			delta = (max - min) * 1000;

			statusEl.innerHTML += "sound: " + (currentTime() * 1000).toFixed() + "ms";
			statusEl.innerHTML += "<br/>" + "<br/>phase error: " + ((max - min) * 1000).toFixed() + "ms<br/>";
			statusEl.innerHTML += "max phase error: " + maxPhaseError.toFixed() + "ms<br/>";
			statusEl.innerHTML += "<br/>player: " + (player.currentTime() * 1000).toFixed() + "ms<br/>";
			statusEl.innerHTML += "player/sound error: " +
				((player.currentTime() - audio['left'][0].currentTime) * 1000).toFixed() + "ms<br/>";

		}
	}, 100);

	return {
		setVolumes: setVolumes,
		getVolumes: getVolumes,
		updateVolumes: updateVolumes,
		Sound3D_OldVersion: Sound3D_OldVersion,
		getAudio: getAudio,
		play: play,
		pause: pause,
		rewind: rewind,
		stop: stop,
		isReady: isReady,
		remove: remove,
		soundSync: __TEST__soundSync,
		currentTime: currentTime
	};
}
*/

module.exports = Sound;