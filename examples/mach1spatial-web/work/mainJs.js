"use strict";

const aq = require('./react/aq.jsx');
let videojs = require('videojs');
require('./videojs/videojs-panorama.v5');
let THREE = require('THREE');
let $ = require('jQuery');

require('./css/main.less');
require('./css/loader.css');

let delay = require('./tools').delay;
/*
let audioFiles8 = {};
audioFiles8.left = ['./audio/m1spatial/T1.mp3', './audio/m1spatial/T2.mp3', './audio/m1spatial/T3.mp3', './audio/m1spatial/T4.mp3',
					'./audio/m1spatial/B5.mp3', './audio/m1spatial/B6.mp3', './audio/m1spatial/B7.mp3', './audio/m1spatial/B8.mp3'];
audioFiles8.right = ['./audio/m1spatial/T1.mp3', './audio/m1spatial/T2.mp3', './audio/m1spatial/T3.mp3', './audio/m1spatial/T4.mp3',
					'./audio/m1spatial/B5.mp3', './audio/m1spatial/B6.mp3', './audio/m1spatial/B7.mp3', './audio/m1spatial/B8.mp3'];
					*/

const audioFiles8 = ['./audio/m1spatial/T1.mp3', './audio/m1spatial/T2.mp3', './audio/m1spatial/T3.mp3', './audio/m1spatial/T4.mp3',
	'./audio/m1spatial/B5.mp3', './audio/m1spatial/B6.mp3', './audio/m1spatial/B7.mp3', './audio/m1spatial/B8.mp3'];

const FRAMES_PER_SECOND = 60;

const MAX_PHASE_ERROR = 80; //ms
const MAX_SOUND_PLAYER_ERROR = 0.1; //s

const MIN_DELAY_VALUE = 5; // 5ms
const MAX_DELAY_VALUE = 1000; // 1s

const Sound_TEST = require('./sound');


document.sound = {};

$(document).ready(function () {
	function endPartOfTest(player, sound) {
		player.dispose();
		sound.remove();
	}

	let voidEl = $('#void');

	aq([function (endCb) {
		let elemId = 'videojs-panorama-player_first-second-test';

		let statusEl = $('#status');

		videojs(elemId, {
			controls: false, // debug TODO: controls: false
			autoplay: false,
			loop: true,
			muted: true
		}, function() {

			let isPlay = false;

			let status = statusEl;

			let thus = this;
			let isVideoReady = false;
			let isDelete = false;

			let sound = new Sound_TEST(audioFiles8, thus);

			// DEBUG
			document.sound = sound;

			let videoElement = document.getElementById(elemId);
			let width = videoElement.offsetWidth;
			let height = videoElement.offsetHeight;

			let __TEST__orDelay = 0; let __TEST__timeoutOrDelay = delay(function() {
				sound.updateVolumes(0, 0);
			}, 0);
			let __TEST__orientationDelay = function (d) {
				__TEST__orDelay = d;
			};

			let __TEST__soundSyncDelay = 0;

			this.width(width);
			this.height(height);

			this.volume(0);

			this.panorama({
				clickAndDrag: true,
				showNotice: false,
				backToVerticalCenter: false,
				backToHorizonCenter: false,
				// clickToToggle: true,
				callback: function () {
					isVideoReady = true;
				}
			});

			this.on('pause', function () {
				isPlay = false;
				sound.pause();
			});


			this.on('seeked', function () {
				delay(function () {
					if (!isDelete)
					{
						sound.rewind(thus.currentTime());
					}
				}, 100);
			});

			this.on('play', function () {
				isPlay = true;
				sound.play();
			});

			function updateVolumes() {
				if (thus.getChild('Canvas')) {
					let vector = new THREE.Vector3();

					thus.getChild('Canvas').camera.getWorldDirection(vector);
					let alpha = Math.asin(vector.y) * 180 / Math.PI;
					let theta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

					if (theta < 0)
						theta = 360 + theta;

					if (__TEST__orDelay > 5)
					{
						// __TEST__timeoutOrDelay.now();

						__TEST__timeoutOrDelay = delay(function() {
							if (!isDelete)
							{
								sound.updateVolumes(-theta, -alpha);
							}
						}, __TEST__orDelay);
					}
					else
					{
						sound.updateVolumes(-theta, -alpha);
					}
				}
			}

			setTimeout(function __timeoutUpdateVolumes() {
				if (!isDelete && isVideoReady) {
					updateVolumes();

					if (Math.abs(sound.currentTime() - thus.currentTime()) > MAX_SOUND_PLAYER_ERROR && isPlay)
					{
						console.warn('Rebalance: player - sound error = ' + (thus.currentTime() - sound.currentTime()));
						console.warn('player.currentTime() = ', thus.currentTime());
						console.warn('sound.currentTime() = ', sound.currentTime());

						if (thus.currentTime() - sound.currentTime() < 1 && thus.currentTime() - sound.currentTime() > 0)
						{
							sound.rewind(thus.currentTime() + (thus.currentTime() - sound.currentTime())/4);
						}
						else
						{
							sound.rewind(thus.currentTime());
						}
					}

					let logHtml = `sound: ${(sound.currentTime() * 1000).toFixed()} <br/>
								   player: ${(thus.currentTime() * 1000).toFixed()} <br/><br/>
								   delay p - s: ${((thus.currentTime() - sound.currentTime()) * 1000).toFixed()} `;

					status.html(logHtml);

					setTimeout(__timeoutUpdateVolumes, 1000 / FRAMES_PER_SECOND);

				}
			}, 1000 / FRAMES_PER_SECOND);


			function Play() {
				if (isVideoReady == true && sound.isReady() == true) {
					sound.play();
					thus.play();
				}
			}

			setTimeout(function __playTimeout() {
				if (!isDelete)
				{
					if (isVideoReady == true && sound.isReady() == true) {
						Play();
						endCb(__TEST__arr);
					}
					else {
						setTimeout(__playTimeout, 100);
					}
				}
			}, 100);

			let end = function () {
				isDelete = true;
				sound.remove();
				thus.pause();
				thus.dispose();

				status = voidEl;
			};


			let __TEST__arr = [{
				hint: 'hint1',
				text: 'Orientation Latency',
				type: 'slider',
				number: 1,
				desc: '1',
				f: (x) => {
					return Math.min(Math.max(-1.0761133*Math.pow(10, -22)*Math.pow(x, 10)+5.3903494*Math.pow(10, -19)*Math.pow(x, 9)-
						1.1570519*Math.pow(10, -15)*Math.pow(x, 8)+1.3901943*Math.pow(10, -12)*Math.pow(x, 7)-
						1.0242364*Math.pow(10, -9)*Math.pow(x, 6)+4.7635732*Math.pow(10, -7)*Math.pow(x, 5)-
						0.13860339e-3*Math.pow(x, 4)+0.24064408e-1*Math.pow(x, 3)-2.2293843*Math.pow(x, 2)+81.224603*x+400., MIN_DELAY_VALUE), MAX_DELAY_VALUE);
				},
				change: __TEST__orientationDelay,
				end: end
			}, {
				hint: 'hint2',
				text: 'Orientation Latency',
				type: 'slider',
				desc: '1',
				number: 1,
				f: (x) => {
					return Math.min(Math.max(-1.0761133*Math.pow(10, -22)*Math.pow(x, 10)+5.3903494*Math.pow(10, -19)*Math.pow(x, 9)-
						1.1570519*Math.pow(10, -15)*Math.pow(x, 8)+1.3901943*Math.pow(10, -12)*Math.pow(x, 7)-
						1.0242364*Math.pow(10, -9)*Math.pow(x, 6)+4.7635732*Math.pow(10, -7)*Math.pow(x, 5)-
						0.13860339e-3*Math.pow(x, 4)+0.24064408e-1*Math.pow(x, 3)-2.2293843*Math.pow(x, 2)+81.224603*x+400., MIN_DELAY_VALUE), MAX_DELAY_VALUE);
				},
				change: __TEST__orientationDelay,
				end: end
			}, {
				hint: 'hint3',
				text: 'Orientation Latency',
				type: 'slider',
				desc: '1',
				number: 1,
				f: (x) => {
					return Math.min(Math.max(-1.0761133*Math.pow(10, -22)*Math.pow(x, 10)+5.3903494*Math.pow(10, -19)*Math.pow(x, 9)-
						1.1570519*Math.pow(10, -15)*Math.pow(x, 8)+1.3901943*Math.pow(10, -12)*Math.pow(x, 7)-
						1.0242364*Math.pow(10, -9)*Math.pow(x, 6)+4.7635732*Math.pow(10, -7)*Math.pow(x, 5)-
						0.13860339e-3*Math.pow(x, 4)+0.24064408e-1*Math.pow(x, 3)-2.2293843*Math.pow(x, 2)+81.224603*x+400., MIN_DELAY_VALUE), MAX_DELAY_VALUE);
				},
				change: __TEST__orientationDelay,
				end: end
			}];


		});
	}, function (endCb) {

		let statusEl = $('#status');

		let elemId = 'videojs-panorama-player_first-second-test';
		videojs(elemId, {
			controls: false,
			autoplay: false,
			loop: true,
			muted: true
		}, function() {
			let thus = this;
			let isVideoReady = false;
			let isDelete = false;
			let isPlay = false;

			let status = statusEl;

			let sound = new Sound_TEST(audioFiles8, thus);

			let videoElement = document.getElementById(elemId);
			let width = videoElement.offsetWidth;
			let height = videoElement.offsetHeight;

			let __TEST__orDelay = 0; let __TEST__timeoutOrDelay = delay(function() {
				sound.updateVolumes(0, 0);
			}, 0);
			let __TEST__orientationDelay = function (d) {
				__TEST__orDelay = d;
			};

			let __TEST__soundSyncDelay = 0;
			let __TEST__soundSync = function (d) {
				if (!isDelete)
				{
					__TEST__soundSyncDelay = d * (Math.random() > 0.5 ? 1 : -1) / 1000;
					sound.rewind(thus.currentTime() + __TEST__soundSyncDelay);
				}
			};

			this.width(width);
			this.height(height);

			this.volume(0);

			this.panorama({
				clickAndDrag: true,
				showNotice: false,
				backToVerticalCenter: false,
				backToHorizonCenter: false,
				// clickToToggle: true,
				callback: function () {
					isVideoReady = true;
				}
			});

			this.on('pause', function () {
				isPlay = false;
				sound.pause();
			});


			this.on('seeked', function () {
				delay(function () {
					if (!isDelete)
					{
						sound.rewind(thus.currentTime());
					}
				}, 100);
			});

			this.on('play', function () {
				isPlay = true;
				sound.play();
			});

			function updateVolumes() {
				if (thus.getChild('Canvas')) {
					let vector = new THREE.Vector3();

					thus.getChild('Canvas').camera.getWorldDirection(vector);
					let alpha = Math.asin(vector.y) * 180 / Math.PI;
					let theta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

					if (theta < 0)
						theta = 360 + theta;

					if (__TEST__orDelay > 5)
					{
						// __TEST__timeoutOrDelay.now();

						__TEST__timeoutOrDelay = delay(function() {
							if (!isDelete)
							{
								sound.updateVolumes(-theta, -alpha);
							}
						}, __TEST__orDelay);
					}
					else
					{
						sound.updateVolumes(-theta, -alpha);
					}
				}
			}

			setTimeout(function __timeoutUpdateVolumes() {
				if (!isDelete && isVideoReady) {
					updateVolumes();

					if (Math.abs(sound.currentTime() - thus.currentTime()) > MAX_SOUND_PLAYER_ERROR + Math.abs(__TEST__soundSyncDelay) && isPlay)
					{
						sound.rewind(thus.currentTime() + __TEST__soundSyncDelay);
					}

					let logHtml = `sound: ${(sound.currentTime() * 1000).toFixed()} <br/>
								   player: ${(thus.currentTime() * 1000).toFixed()} <br/><br/>
								   delay p - s: ${((thus.currentTime() - sound.currentTime()) * 1000).toFixed()} `;

					status.html(logHtml);

					setTimeout(__timeoutUpdateVolumes, 1000 / FRAMES_PER_SECOND);

				}
			}, 1000 / FRAMES_PER_SECOND);


			function Play() {
				if (isVideoReady == true && sound.isReady() == true) {
					sound.play();
					thus.play();
				}
			}

			setTimeout(function __playTimeout() {
				if (!isDelete)
				{
					if (isVideoReady == true && sound.isReady() == true) {
						Play();
						endCb(__TEST__arr);
					}
					else {
						setTimeout(__playTimeout, 100);
					}
				}
			}, 100);

			let end = function () {
				isDelete = true;
				sound.remove();
				thus.pause();
				thus.dispose();
			};

			let __TEST__arr = [{
				hint: 'hint4',
				text: 'Audio Sync Latency',
				type: 'slider',
				desc: '2',
				number: 2,
				f: (x) => {
					return Math.min(Math.max(-1.0761133*Math.pow(10, -22)*Math.pow(x, 10)+5.3903494*Math.pow(10, -19)*Math.pow(x, 9)-
						1.1570519*Math.pow(10, -15)*Math.pow(x, 8)+1.3901943*Math.pow(10, -12)*Math.pow(x, 7)-
						1.0242364*Math.pow(10, -9)*Math.pow(x, 6)+4.7635732*Math.pow(10, -7)*Math.pow(x, 5)-
						0.13860339e-3*Math.pow(x, 4)+0.24064408e-1*Math.pow(x, 3)-2.2293843*Math.pow(x, 2)+81.224603*x+400., MIN_DELAY_VALUE), MAX_DELAY_VALUE);
				},
				change: __TEST__soundSync,
				end: end
			}];


		});
	}, function (endCb) {
		const SOUND_CHECK_PATH = '/audio/White_noise.ogg';
		let audio = new Audio();
		audio.preload = 'auto';
		audio.addEventListener('canplaythrough', loadedAudio, false);
		// loop
		audio.addEventListener('ended', function() {
			this.currentTime = 0;
			this.play();
		}, false);
		// end loop
		audio.src = SOUND_CHECK_PATH;
		let delayTime = 0;

		let __TEST__arr = [{
			hint: 'hint5',
			text: 'Tactile Audio Latency',
			type: 'delayCheck',
			desc: '3',
			number: 3,
			f: (x) => {
				return Math.min(Math.max(-1.0761133*Math.pow(10, -22)*Math.pow(x, 10)+5.3903494*Math.pow(10, -19)*Math.pow(x, 9)-
					1.1570519*Math.pow(10, -15)*Math.pow(x, 8)+1.3901943*Math.pow(10, -12)*Math.pow(x, 7)-
					1.0242364*Math.pow(10, -9)*Math.pow(x, 6)+4.7635732*Math.pow(10, -7)*Math.pow(x, 5)-
					0.13860339e-3*Math.pow(x, 4)+0.24064408e-1*Math.pow(x, 3)-2.2293843*Math.pow(x, 2)+81.224603*x+400., MIN_DELAY_VALUE), MAX_DELAY_VALUE);
			},
			change: __TEST__changeDelay,
			end: end,
			play: reset
		}];

		let delayControls = delay.ZERO;

		function loadedAudio() {
			endCb(__TEST__arr);
		}

		function play() {
			audio.play();
		}

		function pause() {
			audio.pause();
		}

		function reset() {
			pause();
			audio.currentTime = 0;

			delayControls.stop();
			delayControls = delay(play, delayTime);
		}

		function __TEST__changeDelay(d){
			delayTime = d;
			pause();
		}

		function end() {
			pause();
			delayControls.stop();
		}
	}, function (endCb) {

		let statusEl = $('#status');

		let elemId = 'videojs-panorama-player_fourth-test';
		videojs(elemId, {
			controls: false,
			autoplay: false,
			loop: true,
			muted: true
		}, function() {
			let thus = this;
			let isVideoReady = false;
			let isDelete = false;
			let isPlay = false;

			let status = statusEl;

			let sound = new Sound_TEST(audioFiles8, thus);

			let videoElement = document.getElementById(elemId);
			let width = videoElement.offsetWidth;
			let height = videoElement.offsetHeight;

			let __TEST__orDelay = 0; let __TEST__timeoutOrDelay = delay(function() {
				sound.updateVolumes(0, 0);
			}, 0);

			this.width(width);
			this.height(height);

			this.volume(0);

			this.panorama({
				clickAndDrag: true,
				showNotice: false,
				backToVerticalCenter: false,
				backToHorizonCenter: false,
				// clickToToggle: true,
				callback: function () {
					isVideoReady = true;
				}
			});

			this.on('pause', function () {
				isPlay = false;
				sound.pause();
			});


			this.on('seeked', function () {
				delay(function () {
					if (!isDelete)
					{
						sound.rewind(thus.currentTime());
					}
				}, 100);
			});

			this.on('play', function () {
				isPlay = true;
				sound.play();
			});

			function updateVolumes() {
				if (thus.getChild('Canvas')) {
					let vector = new THREE.Vector3();

					thus.getChild('Canvas').camera.getWorldDirection(vector);
					let alpha = Math.asin(vector.y) * 180 / Math.PI;
					let theta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

					if (theta < 0)
						theta = 360 + theta;

					if (__TEST__orDelay > 5)
					{
						// __TEST__timeoutOrDelay.now();

						__TEST__timeoutOrDelay = delay(function() {
							if (!isDelete)
							{
								sound.updateVolumes(-theta, -alpha);
							}
						}, __TEST__orDelay);
					}
					else
					{
						sound.updateVolumes(-theta, -alpha);
					}
				}
			}

			setTimeout(function __timeoutUpdateVolumes() {
				if (!isDelete && isVideoReady) {
					updateVolumes();

					if (Math.abs(sound.currentTime() - thus.currentTime()) > MAX_SOUND_PLAYER_ERROR && isPlay)
					{
						console.warn('Rebalance: player - sound error = ' + (thus.currentTime() - sound.currentTime()));
						console.warn('player.currentTime() = ', thus.currentTime());
						console.warn('sound.currentTime() = ', sound.currentTime());

						if (thus.currentTime() - sound.currentTime() < 1 && thus.currentTime() - sound.currentTime() > 0)
						{
							sound.rewind(thus.currentTime() + (thus.currentTime() - sound.currentTime())/4);
						}
						else
						{
							sound.rewind(thus.currentTime());
						}
					}

					let logHtml = `sound: ${(sound.currentTime() * 1000).toFixed()} <br/>
								   player: ${(thus.currentTime() * 1000).toFixed()} <br/><br/>
								   delay p - s: ${((thus.currentTime() - sound.currentTime()) * 1000).toFixed()} `;

					status.html(logHtml);

					setTimeout(__timeoutUpdateVolumes, 1000 / FRAMES_PER_SECOND);

				}
			}, 1000 / FRAMES_PER_SECOND);

			function Play() {
				if (isVideoReady == true && sound.isReady() == true) {
					sound.play();
					thus.play();
				}
			}

			function __TEST__updateValues() {
				if (!isDelete)
				{
					let vector = new THREE.Vector3();

					thus.getChild('Canvas').camera.getWorldDirection(vector);

					let alpha = Math.asin(vector.y) * 180 / Math.PI;
					let beta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

					return {
						time: thus.currentTime(),
						alpha: alpha.toFixed(2),
						beta: beta.toFixed(2)
					};
				}
				else
				{
					return {
						isEnd: isDelete
					};
				}
			}

			setTimeout(function __playTimeout() {
				if (!isDelete)
				{
					if (isVideoReady == true && sound.isReady() == true) {
						Play();
						endCb(__TEST__arr);
					}
					else {
						setTimeout(__playTimeout, 100);
					}
				}
			}, 100);

			let end = function () {
				isDelete = true;
				sound.remove();
				thus.pause();
				thus.dispose();
			};

			let __TEST__arr = [{
				hint: 'hint6',
				text: 'Directional Accuracy Tool',
				type: 'orientationCheck',
				desc: '4',
				number: 2,
				f: (x) => {
					return Math.min(Math.max(-1.0761133*Math.pow(10, -22)*Math.pow(x, 10)+5.3903494*Math.pow(10, -19)*Math.pow(x, 9)-
						1.1570519*Math.pow(10, -15)*Math.pow(x, 8)+1.3901943*Math.pow(10, -12)*Math.pow(x, 7)-
						1.0242364*Math.pow(10, -9)*Math.pow(x, 6)+4.7635732*Math.pow(10, -7)*Math.pow(x, 5)-
						0.13860339e-3*Math.pow(x, 4)+0.24064408e-1*Math.pow(x, 3)-2.2293843*Math.pow(x, 2)+81.224603*x+400., MIN_DELAY_VALUE), MAX_DELAY_VALUE);
				},
				update: __TEST__updateValues,
				change: ()=>{},
				end: end
			}];


		});
	}]);

});


/*
 [{
 hint: 'NEW TEST',
 text: 'NEW TEST',
 type: 'slider',
 desc: 'test1',
 number: 2,
 f: (x) => {
 return Math.min(Math.max(-1.0761133*Math.pow(10, -22)*Math.pow(x, 10)+5.3903494*Math.pow(10, -19)*Math.pow(x, 9)-
 1.1570519*Math.pow(10, -15)*Math.pow(x, 8)+1.3901943*Math.pow(10, -12)*Math.pow(x, 7)-
 1.0242364*Math.pow(10, -9)*Math.pow(x, 6)+4.7635732*Math.pow(10, -7)*Math.pow(x, 5)-
 0.13860339e-3*Math.pow(x, 4)+0.24064408e-1*Math.pow(x, 3)-2.2293843*Math.pow(x, 2)+81.224603*x+400., MIN_DELAY_VALUE), MAX_DELAY_VALUE);
 },
 change: __TEST__soundSync
 }]
 */