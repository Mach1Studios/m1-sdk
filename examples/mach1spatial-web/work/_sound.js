"use strict";

let howl = require('howler');

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
 */

function Sound(audioFiles) {
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
		audio[side][i] = new Howl({
			urls: audioFiles[side][i],
			autoplay: false,
			loop: true,
			volumes: 1,
			onload: loadedAudio
		});

		if (side === 'left')
		{
			audio[side][i].stereo(-1);
		}
		else
		{
			audio[side][i].stereo(1);
		}
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


	function getVolumes() {
		return _volumes;
	}

	function setVolumes(volumes = _volumes) {
		_volumes = volumes;

		audio.volume(volumes[0]);

		return getVolumes();
	}

	function __TEST__soundSync(d) {
		__TEST__shift = d;
	}

	function rewind(time) {
		if (time < 0) {
			stop();

			return;
		}

		for (let side of ['left', 'right']) {
			for (let i in audio[side]) {
				if (audio[side][i].duration < time) {
					console.warn('Rewind(out of range):' + time + ' > ' + audioFiles[side][i].name + '.duration = ' + audio[side][i].duration);
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
		return audio['left'][0].currentTime + delta / 2;
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

					// statusEl.innerHTML += audioFiles[side][i].name + ". " + pos + " , volume: " + audio[side][i].volume + "<br/>";
				}
			}

			if ((max - min) * 1000 > maxPhaseError) {
				maxPhaseError = (max - min) * 1000;
			}

			delta = (max - min) * 1000;

			statusEl.innerHTML += "<br/>" + "<br/>phase error: " + (max - min) * 1000 + "ms<br/>";
			statusEl.innerHTML += "max phase error: " + maxPhaseError + "ms<br/>";
			statusEl.innerHTML += "<br/>player: " + player.currentTime() + "ms<br/>";
			statusEl.innerHTML += "player/sound error: " +
				(player.currentTime() - audio['left'][0].currentTime) * 1000 + "ms<br/>";

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

module.exports = Sound;