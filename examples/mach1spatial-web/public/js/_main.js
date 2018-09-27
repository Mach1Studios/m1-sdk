var __DEBUG__ =
/******/ (function(modules) { // webpackBootstrap
/******/ 	// The module cache
/******/ 	var installedModules = {};
/******/
/******/ 	// The require function
/******/ 	function __webpack_require__(moduleId) {
/******/
/******/ 		// Check if module is in cache
/******/ 		if(installedModules[moduleId])
/******/ 			return installedModules[moduleId].exports;
/******/
/******/ 		// Create a new module (and put it into the cache)
/******/ 		var module = installedModules[moduleId] = {
/******/ 			i: moduleId,
/******/ 			l: false,
/******/ 			exports: {}
/******/ 		};
/******/
/******/ 		// Execute the module function
/******/ 		modules[moduleId].call(module.exports, module, module.exports, __webpack_require__);
/******/
/******/ 		// Flag the module as loaded
/******/ 		module.l = true;
/******/
/******/ 		// Return the exports of the module
/******/ 		return module.exports;
/******/ 	}
/******/
/******/
/******/ 	// expose the modules object (__webpack_modules__)
/******/ 	__webpack_require__.m = modules;
/******/
/******/ 	// expose the module cache
/******/ 	__webpack_require__.c = installedModules;
/******/
/******/ 	// identity function for calling harmony imports with the correct context
/******/ 	__webpack_require__.i = function(value) { return value; };
/******/
/******/ 	// define getter function for harmony exports
/******/ 	__webpack_require__.d = function(exports, name, getter) {
/******/ 		if(!__webpack_require__.o(exports, name)) {
/******/ 			Object.defineProperty(exports, name, {
/******/ 				configurable: false,
/******/ 				enumerable: true,
/******/ 				get: getter
/******/ 			});
/******/ 		}
/******/ 	};
/******/
/******/ 	// getDefaultExport function for compatibility with non-harmony modules
/******/ 	__webpack_require__.n = function(module) {
/******/ 		var getter = module && module.__esModule ?
/******/ 			function getDefault() { return module['default']; } :
/******/ 			function getModuleExports() { return module; };
/******/ 		__webpack_require__.d(getter, 'a', getter);
/******/ 		return getter;
/******/ 	};
/******/
/******/ 	// Object.prototype.hasOwnProperty.call
/******/ 	__webpack_require__.o = function(object, property) { return Object.prototype.hasOwnProperty.call(object, property); };
/******/
/******/ 	// __webpack_public_path__
/******/ 	__webpack_require__.p = "";
/******/
/******/ 	// Load entry module and return exports
/******/ 	return __webpack_require__(__webpack_require__.s = 202);
/******/ })
/************************************************************************/
/******/ ({

/***/ 102:
/***/ (function(module, exports, __webpack_require__) {

"use strict";
/* WEBPACK VAR INJECTION */(function(global) {/* global Uint32Array, Promise */



var BaseAudioContext = __webpack_require__(87);
var OfflineAudioContext = global.OfflineAudioContext || global.webkitOfflineAudioContext;

if (OfflineAudioContext) {
  var silent = new Uint32Array([
    0x46464952, 0x00000038, 0x45564157, 0x20746d66,
    0x00000010, 0x00010001, 0x0000ac44, 0x00015888,
    0x00100002, 0x61746164, 0x00000014, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
  ]).buffer;

  var isPromiseBased = (function() {
    var context = new OfflineAudioContext(1, 128, 44100);

    return context.decodeAudioData(silent, function() {}, function() {});
  })();

  if (!isPromiseBased) {
    var decodeAudioData = BaseAudioContext.prototype.decodeAudioData;

    BaseAudioContext.prototype.decodeAudioData = function(audioData, successCallback, errorCallback) {
      var _this = this;
      var promise = new Promise(function(resolve, reject) {
        return decodeAudioData.call(_this, audioData, resolve, reject);
      });

      promise.then(successCallback, errorCallback);

      return promise;
    };

    BaseAudioContext.prototype.decodeAudioData.original = decodeAudioData;
  }
}

/* WEBPACK VAR INJECTION */}.call(exports, __webpack_require__(52)))

/***/ }),

/***/ 187:
/***/ (function(module, exports, __webpack_require__) {

"use strict";


/**
 * (m)Point
 * one uppercase letter is mPoint
 * @param x_ coordinate
 * @param y_ coordinate
 * @param z_ coordinate
 */

function Point(x_ = 0, y_ = 0, z_ = 0)
{
	this.x = x_;
	this.y = y_;
	this.z = z_;
}

/**
 * depth copy
 * @param A copy this = A; if A is undefined return copy of this
 * @return {Point}
 */
Point.prototype.copy = function(A) {
	if (arguments.length == 1)
	{
		this.x = A.x; this.y = A.y; this.z = A.z;

		return this;
	}
	else
	{
		return new Point(this.x, this.y, this.z);
	}
};

/**  const
 * sqrt(A * A)
 * @return {number}
 */
Point.prototype.length = function () {
	return Math.sqrt(Point.dot(this, this));
};

/**  const
 * A[i]
 * @param i = [0..2]
 * @return {number} i coordinate
 */
Point.prototype.at = function(i) {
	switch (i) {
		case 0:
			i = 'x';
			break;
		case 1:
			i = 'y';
			break;
		case 2:
			i = 'z';
			break;
		default:
			// Wrong arg
			i = 'x';
	}

	return this[i];
};

/**
 * A / length
 * @return {Point}        old
 */
Point.prototype.normalize = function() {
	let length = this.length();

	if(length > 0)
	{
		this.x /= length;
		this.y /= length;
		this.z /= length;
	}

	return this;
};

Point.prototype.getNormalize = function() {
	let length = this.length();
	if (length > 0)
	{
		return new Point(this.x / length, this.y / length, this.z / length);
	}
	else
	{
		return new Point();
	}
};

/**    const
 * rotate this on angle_ grad regarding axis Axis_
 * @param angle_ in grad
 * @param Axis_ Point <=> axis of rotate
 * @return {Point}        new
 */
Point.prototype.getRotated = function(angle_, Axis_) {
	let Axis = Axis_.getNormalize();
	let angle = angle_ * Point.DEG_TO_RAD;

	let sinA = Math.sin( angle );
	let cosA = Math.cos( angle );
	let cosB = 1 - cosA;

	let nx = this.x * (Axis.x * Axis.x * cosB + cosA)
		+ this.y * (Axis.x * Axis.y*cosB - Axis.z * sinA)
		+ this.z * (Axis.x * Axis.z * cosB + Axis.y * sinA);
	let ny = this.x * (Axis.y * Axis.x * cosB + Axis.z * sinA)
		+ this.y * (Axis.y * Axis.y * cosB + cosA)
		+ this.z * (Axis.y * Axis.z * cosB - Axis.x * sinA);
	let nz = this.x * (Axis.z * Axis.x * cosB - Axis.y * sinA)
		+ this.y * (Axis.z * Axis.y * cosB + Axis.x * sinA)
		+ this.z * (Axis.z * Axis.z * cosB + cosA);

	return new Point(nx, ny, nz);
};

/**
 * rotate this on angle_ deg regarding axis Axis_
 * @param angle_ in deg
 * @param Axis_ Point <=> axis of rotate
 * @return {Point}        old
 */
Point.prototype.rotate = function(angle_, Axis_) {
	return this.copy(this.getRotated(angle_,  Axis_));
};

Point.DEG_TO_RAD = Math.PI / 180;

/**
 * transform deg to rad
 * @param angle in deg
 * @return {number} in rad
 */
Point.degToRad = function (angle) {
	return angle * Point.DEG_TO_RAD;
};

/**
 * (a.x + b.x, ...)
 * A + B = B + A
 * @param A first Point   const
 * @param B second Point  const
 * @return {Point}          new
 */
Point.plus = function(A, B) {
	return new Point(A.x + B.x, A.y + B.y, A.z + B.z);
};

/**
 * (a.x * f, ...)
 * @param A Point         const
 * @param f scalar
 * @return {Point}        new
 */
Point.scalarMult = function(A, f) {
	return new Point(A.x * f, A.y * f, A.z * f);
};

/**
 * (a.x * b.x + ...)
 * A * B = B * A
 * @param A Point         const
 * @param B Point         const
 * @return {number}
 */
Point.dot = function(A, B) {
	return (A.x * B.x + A.y * B.y + A.z * B.z);
};

/**
 * (a.x - b.x, ...)
 * @param A Point         const
 * @param B Point         const
 * @return {Point}        new
 */
Point.minus = function(A, B) {
	return Point.plus(A, Point.scalarMult(B, -1));
};



module.exports = Point;

/***/ }),

/***/ 202:
/***/ (function(module, exports, __webpack_require__) {

"use strict";


var videojs = __webpack_require__(85);
__webpack_require__(82);
var THREE = __webpack_require__(84);
var $ = __webpack_require__(83);

var eightChannelsAlgorithm = __webpack_require__(81).eightChannelsAlgorithm;

var FRAMES_PER_SECOND = 60;

function delay(cb, time) {
	var timerId = setTimeout(cb, time);

	function stop() {
		clearTimeout(timerId);
	}

	function now() {
		cb();

		stop();
	}

	return {
		stop: stop,
		now: now
	};
}

var audioFiles4 = [{
	src: "./audio/000.mp3",
	name: "front"
}, {
	src: "./audio/270.mp3",
	name: "right"
}, {
	src: "./audio/180.mp3",
	name: "back"
}, {
	src: "./audio/090.mp3",
	name: "left"
}];

var audioFiles8 = function () {
	var audio = {};

	var _arr = ['left', 'right'];
	for (var _i = 0; _i < _arr.length; _i++) {
		var side = _arr[_i];
		audio[side] = [];

		var _arr2 = ['l', 'h'];
		for (var _i2 = 0; _i2 < _arr2.length; _i2++) {
			var posH = _arr2[_i2];var _arr3 = ['f', 'b'];

			for (var _i3 = 0; _i3 < _arr3.length; _i3++) {
				var posFB = _arr3[_i3];var _iteratorNormalCompletion = true;
				var _didIteratorError = false;
				var _iteratorError = undefined;

				try {
					for (var _iterator = (posFB == 'f' ? ['l', 'r'] : ['r', 'l'])[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
						var posLR = _step.value;

						audio[side].push({
							src: "./audio/000.mp3", /* ./audio/side_posLRposFBposH.mp3 */
							name: side + '_' + posLR + posFB + posH
						});
					}
				} catch (err) {
					_didIteratorError = true;
					_iteratorError = err;
				} finally {
					try {
						if (!_iteratorNormalCompletion && _iterator.return) {
							_iterator.return();
						}
					} finally {
						if (_didIteratorError) {
							throw _iteratorError;
						}
					}
				}
			}
		}
	}

	console.log(audio);

	return audio;
}();

var sound = void 0;

// DEBUG
window.sound = sound;
// END DEBUG


$(document).ready(function () {
	var player = window.player = videojs('videojs-panorama-player', {}, function () {
		var thus = this;

		var isPlayBeforeSeeking = false;
		var isVideoReady = false;

		sound = new Sound(audioFiles8, player);

		var videoElement = document.getElementById("videojs-panorama-player");
		var width = videoElement.offsetWidth;
		var height = videoElement.offsetHeight;

		this.width(width);
		this.height(height);

		this.volume(0);

		this.panorama({
			clickAndDrag: true,
			showNotice: false,
			backToVerticalCenter: false,
			backToHorizonCenter: false,
			// clickToToggle: true,
			callback: function callback() {
				isVideoReady = true;
			}
		});

		this.on('pause', function () {
			sound.pause();
		});

		this.on('seeked', function () {
			delay(function () {
				sound.rewind(thus.currentTime());
			}, 100);
		});

		this.on('play', function () {
			sound.play();
		});

		function updateVolumes() {
			if (thus.getChild('Canvas')) {
				var vector = new THREE.Vector3();

				thus.getChild('Canvas').camera.getWorldDirection(vector);
				var alpha = Math.asin(vector.y) * 180 / Math.PI;
				var theta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

				if (theta < 0) theta = 360 + theta;

				sound.updateVolumes(-theta, -alpha);
			}
		}

		setTimeout(function __timeoutUpdateVolumes() {
			updateVolumes();

			setTimeout(__timeoutUpdateVolumes, 1000 / FRAMES_PER_SECOND);
		}, 1000 / FRAMES_PER_SECOND);

		function Play() {
			if (isVideoReady == true && sound.isReady() == true) {
				sound.play();
				player.play();
			}
		}

		setTimeout(function __playTimeout() {
			if (isVideoReady == true && sound.isReady() == true) {
				Play();
			} else {
				setTimeout(__playTimeout, 100);
			}
		}, 100);

		__cb(thus, sound);
	});
});

function Sound(audioFiles, player) {
	var audio = {
		left: [8],
		right: [8]
	};

	var _volumes = {
		left: [8],
		right: [8]
	};
	var loaded = false;
	var loadedCount = 0;

	var isPlay = false;
	var isSoundReady = false;

	var maxPhaseError = 0;

	function preloadAudio(i, side) {
		audio[side][i] = new Audio();
		audio[side][i].addEventListener('canplaythrough', loadedAudio, false);
		audio[side][i].src = audioFiles[side][i].src;
	}

	function loadedAudio() {
		if (!loaded) {
			loadedCount++;
			loaded = loadedCount == audioFiles.left.length + audioFiles.right.length;

			isSoundReady = loaded;
			document.getElementById("status").innerHTML = "loaded: " + 100 * loadedCount / (audioFiles.left.length + audioFiles.right.length);
		}
	}

	function Sound3D_OldVersion(angle) {
		var volumes = [0.0, 0.0, 0.0, 0.0];

		volumes[0] = 1.0 - Math.min(1.0, Math.min(360 - angle, angle) / 90.0);
		volumes[1] = 1.0 - Math.min(1.0, Math.abs(90 - angle, angle) / 90.0);
		volumes[2] = 1.0 - Math.min(1.0, Math.abs(180 - angle, angle) / 90.0);
		volumes[3] = 1.0 - Math.min(1.0, Math.abs(270 - angle, angle) / 90.0);

		return volumes;
	}

	function setVolumeOnSide(volumes, side) {
		for (var i in volumes[side]) {
			if (volumes[side].hasOwnProperty(i)) {
				if (volumes[side][i] < 0 || volumes[side][i] > 1) {
					console.warn('Volumes error(out of range):' + volumes);

					return;
				}
			}
		}

		for (var _i4 in audio[side]) {
			audio[side][_i4].volume = volumes[side][_i4];
		}
	}

	function getVolumes() {
		return _volumes;
	}

	function setVolumes() {
		var volumes = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : _volumes;

		setVolumeOnSide(volumes, 'left');
		setVolumeOnSide(volumes, 'right');

		_volumes = volumes;

		return getVolumes();
	}

	function rewind(time) {
		if (time < 0) {
			stop();

			return;
		}

		var _arr4 = ['left', 'right'];
		for (var _i5 = 0; _i5 < _arr4.length; _i5++) {
			var side = _arr4[_i5];
			for (var i in audio[side]) {
				if (audio[side][i].duration < time) {
					console.warn('Rewind(out of range):' + time + ' > ' + audioFiles[side][i].name + '.duration = ' + audio[side][i].duration);
					stop();

					return;
				}
				audio[side][i].currentTime = time;
			}
		}
	}

	function getAudio() {
		return audio;
	}

	function play() {
		if (!isPlay) {
			var _arr5 = ['left', 'right'];

			for (var _i6 = 0; _i6 < _arr5.length; _i6++) {
				var side = _arr5[_i6];
				for (var i in audio[side]) {
					audio[side][i].play();
				}
			}

			isPlay = true;
		}
	}

	function pause() {
		if (isPlay) {
			var _arr6 = ['left', 'right'];

			for (var _i7 = 0; _i7 < _arr6.length; _i7++) {
				var side = _arr6[_i7];
				for (var i in audio[side]) {
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
		return isSoundReady;
	}

	function updateVolumes(yaw, pitch) {
		var roll = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 0;

		var vols = eightChannelsAlgorithm(yaw, pitch, roll);

		for (var i = 0; i < 8; ++i) {
			_volumes.left[i] = vols[2 * i];
		}

		for (var _i8 = 0; _i8 < 8; ++_i8) {
			_volumes.right[_i8] = vols[2 * _i8 + 1];
		}

		setVolumes();
	}

	// auto start preloading
	for (var i in audioFiles.left) {
		preloadAudio(i, 'left');
	}
	for (var _i9 in audioFiles.right) {
		preloadAudio(_i9, 'right');
	}

	setInterval(function () {
		if (loaded) {
			var min = 3600; // hour
			var max = 0;

			document.getElementById("status").innerHTML = "currentTime: <br/>";

			var _arr7 = ['left', 'right'];
			for (var _i10 = 0; _i10 < _arr7.length; _i10++) {
				var side = _arr7[_i10];
				for (var _i11 in audio[side]) {
					var pos = audio[side][_i11].currentTime;
					if (pos < min) min = pos;
					if (pos > max) max = pos;

					document.getElementById("status").innerHTML += audioFiles[side][_i11].name + ". " + pos + " , volume: " + audio[side][_i11].volume + "<br/>";
				}
			}

			if ((max - min) * 1000 > maxPhaseError) {
				maxPhaseError = (max - min) * 1000;
			}

			document.getElementById("status").innerHTML += "<br/>" + "<br/>phase error: " + (max - min) * 1000 + "ms<br/>";
			document.getElementById("status").innerHTML += "max phase error: " + maxPhaseError + "ms<br/>";
			document.getElementById("status").innerHTML += "<br/>player: " + player.currentTime() + "ms<br/>";
			document.getElementById("status").innerHTML += "player/sound error: " + (player.currentTime() - audio['left'][0].currentTime) * 1000 + "ms<br/>";
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
		isReady: isReady
	};
}

function __cb(player, sound) {
	var cameraRotationAngle = [0, 0];

	var POS_Y_H_SPHERE = 600;
	var SPHERE_DIAMETER = 100;

	function animate() {
		setTimeout(function () {
			requestAnimationFrame(animate);

			if (player.getChild('Canvas')) {
				var vector = new THREE.Vector3();

				player.getChild('Canvas').camera.getWorldDirection(vector);
				var alpha = Math.asin(vector.y) * 180 / Math.PI;
				var theta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

				if (theta < 0) theta = 360 + theta;

				// console.log("cameraRotationAngle:", cameraRotationAngle);

				cameraRotationAngle = [theta, alpha];
			} else {
				console.warn("player.getChild(Canvas) undefined");

				cameraRotationAngle = [0, 0];
			}
			// animating/drawing code goes here

			render();
		}, 1000 / FRAMES_PER_SECOND);
	}

	var SCREEN_WIDTH = $("#helper").width();
	var SCREEN_HEIGHT = $("#helper").height();

	var container = void 0;
	var camera = void 0,
	    scene = void 0,
	    renderer = void 0,
	    grid = void 0;
	var cameraRig = void 0;
	var cameraPerspective = void 0;
	var cameraPerspectiveHelper = void 0;

	var cameraOrtho = void 0,
	    cameraOrthoHelper = void 0;

	var geometry = new THREE.SphereGeometry(SPHERE_DIAMETER, 32, 32);
	var materialRight = new THREE.MeshBasicMaterial({
		color: 0xff0000 // red
	});
	var materialLeft = new THREE.MeshBasicMaterial({
		color: 0x0000ff // blue
	});

	var sphere = {
		left: [],
		right: []
	};

	var pos = [[-1, -1], // front-left
	[1, -1], // front-right
	[-1, 1], // back-left
	[1, 1]];

	init();
	animate();

	function init() {

		container = document.getElementById("helper");
		//			container = document.createElement( 'div' );
		//			document.body.appendChild( container );

		scene = new THREE.Scene();

		cameraPerspective = new THREE.PerspectiveCamera(50, SCREEN_WIDTH / SCREEN_HEIGHT, 150, 1200);
		cameraPerspective.fov = 15;

		cameraPerspectiveHelper = new THREE.CameraHelper(cameraPerspective);
		scene.add(cameraPerspectiveHelper);

		camera = new THREE.PerspectiveCamera(50, SCREEN_WIDTH / SCREEN_HEIGHT, 1, 10000);
		camera.position.y = 1000;
		camera.position.z = 2000;
		camera.lookAt(cameraPerspective.position);

		//
		cameraOrtho = new THREE.OrthographicCamera(0.5 * SCREEN_WIDTH / -2, 0.5 * SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_HEIGHT / -2, 150, 3000);
		cameraOrthoHelper = new THREE.CameraHelper(cameraOrtho);
		scene.add(cameraOrthoHelper);
		//

		// counteract different front orientation of cameras vs rig

		cameraOrtho.rotation.y = Math.PI / 2;
		cameraPerspective.rotation.y = 0;

		cameraRig = new THREE.Object3D();
		cameraRig.add(cameraPerspective);
		cameraRig.add(cameraOrtho);

		//cameraRig.up = new THREE.Vector3(0, 1, 0);
		// cameraRig.rotation.up = new THREE.Vector3(0, 1, 0);

		cameraRig.position.set(0, 0, 0);

		scene.add(cameraRig);

		for (var i = 0; i < 4; i++) {
			sphere.left[i] = new THREE.Mesh(geometry, materialLeft);
			sphere.right[i] = new THREE.Mesh(geometry, materialRight);

			sphere.left[i].position.x = 800 * (pos[i][0] - pos[i][1] * 0.1);
			sphere.left[i].position.y = 0;
			sphere.left[i].position.z = 800 * (pos[i][1] + pos[i][0] * 0.1);

			sphere.right[i].position.x = 800 * (pos[i][0] + pos[i][1] * 0.1);
			sphere.right[i].position.y = 0;
			sphere.right[i].position.z = 800 * (pos[i][1] - pos[i][0] * 0.1);

			scene.add(sphere.left[i]);
			scene.add(sphere.right[i]);
		}

		for (var _i12 = 4; _i12 < 8; _i12++) {
			sphere.left[_i12] = new THREE.Mesh(geometry, materialLeft);
			sphere.right[_i12] = new THREE.Mesh(geometry, materialRight);

			sphere.left[_i12].position.x = 800 * (pos[_i12 - 4][0] - pos[_i12 - 4][1] * 0.1);
			sphere.left[_i12].position.y = POS_Y_H_SPHERE;
			sphere.left[_i12].position.z = 800 * (pos[_i12 - 4][1] + pos[_i12 - 4][0] * 0.1);

			sphere.right[_i12].position.x = 800 * (pos[_i12 - 4][0] + pos[_i12 - 4][1] * 0.1);
			sphere.right[_i12].position.y = POS_Y_H_SPHERE;
			sphere.right[_i12].position.z = 800 * (pos[_i12 - 4][1] - pos[_i12 - 4][0] * 0.1);

			scene.add(sphere.left[_i12]);
			scene.add(sphere.right[_i12]);
		}

		// ------------------------

		grid = new THREE.GridHelper(3000, 20);
		grid.material.transparent = true;
		grid.setColors(0x555555, 0xbbbbbb);
		// grid.position.y = -200;
		scene.add(grid);

		// let particles = new THREE.PointCloud( geometry, new THREE.PointCloudMaterial( { color: 0x888888 } ) );
		// scene.add( particles );

		renderer = new THREE.WebGLRenderer({
			antialias: true,
			alpha: true
		});
		renderer.setClearColor(0xeeeeee, 1); // the default
		renderer.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		renderer.domElement.style.position = "relative";
		container.appendChild(renderer.domElement);

		renderer.autoClear = false;

		window.addEventListener('resize', onWindowResize, false);
	}

	function onWindowResize(event) {

		SCREEN_WIDTH = $("#helper").width();
		SCREEN_HEIGHT = $("#helper").height();

		renderer.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

		camera.aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
		camera.updateProjectionMatrix();

		cameraPerspective.aspect = SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraPerspective.updateProjectionMatrix();
	}

	// ????? MAGIC
	function __DELETE_THIS____animate() {
		requestAnimationFrame(animate);
		render();
		// controls.update();
	}

	function render() {
		cameraPerspective.updateProjectionMatrix();
		cameraPerspectiveHelper.update();
		cameraPerspectiveHelper.visible = true;
		cameraOrthoHelper.visible = false;

		//cameraRig.lookAt( mesh.position );
		var volumes = sound.getVolumes();

		for (var i = 0; i < 8; i++) {
			if (volumes.left[i] == 0) {
				sphere.left[i].visible = false;
			} else {
				sphere.left[i].visible = true;
				sphere.left[i].scale.set(volumes.left[i], volumes.left[i], volumes.left[i]);
			}

			if (volumes.right[i] == 0) {
				sphere.right[i].visible = false;
			} else {
				sphere.right[i].visible = true;
				sphere.right[i].scale.set(volumes.right[i], volumes.right[i], volumes.right[i]);
			}
			//  sphere[i].scale.set((i+1)*2,(i+1)*2,(i+1)*2);
		}
		// sphere[3].scale.set(10,10,10);

		cameraRig.rotation.set(0, 0, 0);
		cameraRig.rotateOnAxis(new THREE.Vector3(0, 1, 0).normalize(), cameraRotationAngle[0] * Math.PI / 180);
		cameraRig.rotateOnAxis(new THREE.Vector3(1, 0, 0).normalize(), cameraRotationAngle[1] * Math.PI / 180);

		renderer.clear();

		grid.material.opacity = 0.1;

		renderer.setViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		renderer.render(scene, camera);
	}
}

/***/ }),

/***/ 52:
/***/ (function(module, exports) {

var g;

// This works in non-strict mode
g = (function() {
	return this;
})();

try {
	// This works if eval is allowed (see CSP)
	g = g || Function("return this")() || (1,eval)("this");
} catch(e) {
	// This works if the window reference is available
	if(typeof window === "object")
		g = window;
}

// g can still be undefined, but nothing to do about it...
// We return undefined, instead of nothing here, so it's
// easier to handle this case. if(!global) { ...}

module.exports = g;


/***/ }),

/***/ 81:
/***/ (function(module, exports, __webpack_require__) {

"use strict";


__webpack_require__(102);

var eightChannelsAlgorithm = __webpack_require__(86).eightChannelsAlgorithm;

var SOUND_COUNT = 8 * 2;

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

var preloadCache = {};

function initArray(count) {
	var arr = [];
	arr.length = count;

	arr.forEach(function (i, el, arr) {
		return arr[i] = 0;
	});

	return arr;
}

function Sound(audioFiles) {
	var audioCtx = new AudioContext();
	var thus = this;

	var smp = initArray(SOUND_COUNT);
	var gainNode = initArray(SOUND_COUNT);
	var volumes = initArray(SOUND_COUNT);
	var pannerNode = initArray(SOUND_COUNT);
	var buffer = initArray(audioFiles.length);
	var isSoundReady = false;
	var countOfReadySound = 0;

	var isPlay = false;
	var isDelete = false;
	var startTime = 0;
	var stopTime = 0;

	function preload(path, i) {
		console.time('load file' + path);

		fetch(path, {
			method: 'GET',
			responseType: 'arrayBuffer',
			cache: 'force-cache'
		}).then(function (res) {
			return res.arrayBuffer();
		}).then(function (blob) {
			if (preloadCache[path]) {
				return Promise.resolve(preloadCache[path]);
			} else {
				return audioCtx.decodeAudioData(blob);
			}
		}).then(function (aBuffer) {
			buffer[Math.floor(i / 2)] = aBuffer;

			if (!preloadCache[path]) {
				preloadCache[path] = aBuffer;
			}

			return i;
		}).then(function (i) {
			console.log('Sound {path: ' + path + ', i: ' + i + ', ' + (i + 1) + '} loaded');

			console.timeEnd('load file' + path);
			countOfReadySound += 2;
			isSoundReady = SOUND_COUNT == countOfReadySound;
		}).catch(function (err) {
			console.warn(err);
		});
	}

	function createAudio(time) {
		if (thus.isReady() && !isPlay) {
			for (var i = 0, j = 0; j < buffer.length; ++j, i += 2) {
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

			for (var _i = 0; _i < SOUND_COUNT; ++_i) {
				smp[_i].start(0, time);
			}

			startTime = audioCtx.currentTime - time;
			isPlay = true;
		}
	}

	this.isReady = function () {
		return isSoundReady && !isDelete;
	};

	this.play = function () {
		var time = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : thus.currentTime();

		if (this.isReady() && !isPlay && !isDelete) {
			createAudio(time);

			setVolumes();
		}
	};

	this.pause = function () {
		this.stop();
	};

	this.stop = function () {
		if (this.isReady() && isPlay && !isDelete) {
			isPlay = false;

			stopTime = audioCtx.currentTime;

			for (var i = 0; i < smp.length; ++i) {
				smp[i].stop();

				if (typeof smp[i].disconnect == 'function') smp[i].disconnect();
			}
		}
	};

	this.remove = function () {
		if (this.isReady()) {
			for (var i = 0; i < smp.length; ++i) {
				smp[i].stop();
			}
		}

		isDelete = true;
	};

	this.currentTime = function () {
		if (!this.isReady() || !isPlay) {
			return stopTime - startTime > 0 ? stopTime - startTime : 0;
		} else {
			return audioCtx.currentTime - startTime;
		}
	};

	this.rewind = function (time) {
		if (time < 0) {
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
		if (thus.isReady() && isPlay) {
			for (var i = 0; i < smp.length; ++i) {
				gainNode[i].gain.value = volumes[i];
			}
		}
	}

	this.updateVolumes = function (yaw, pitch) {
		var roll = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 0;

		var vols = eightChannelsAlgorithm(yaw, pitch, roll);

		for (var i = 0; i < SOUND_COUNT; ++i) {
			volumes[i] = vols[i];
		}

		if (isPlay) {
			setVolumes();
		}
	};

	for (var i = 0; i < audioFiles.length; ++i) {
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

/***/ }),

/***/ 82:
/***/ (function(module, exports, __webpack_require__) {

"use strict";
var require;var require;

var _typeof2 = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

(function e(t, n, r) {
    function s(o, u) {
        if (!n[o]) {
            if (!t[o]) {
                var a = typeof require == "function" && require;if (!u && a) return require(o, !0);if (i) return i(o, !0);var f = new Error("Cannot find module '" + o + "'");throw f.code = "MODULE_NOT_FOUND", f;
            }var l = n[o] = { exports: {} };t[o][0].call(l.exports, function (e) {
                var n = t[o][1][e];return s(n ? n : e);
            }, l, l.exports, e, t, n, r);
        }return n[o].exports;
    }var i = typeof require == "function" && require;for (var o = 0; o < r.length; o++) {
        s(r[o]);
    }return s;
})({ 1: [function (require, module, exports) {
        'use strict';

        console.log(videojs);
        function Intervalometer(cb) {
            var rafId = void 0;
            var previousLoopTime = void 0;
            function loop(now) {
                // must be requested before cb() because that might call .stop()
                rafId = requestAnimationFrame(loop);
                cb(now - (previousLoopTime || now)); // ms since last call. 0 on start()
                previousLoopTime = now;
            }
            this.start = function () {
                if (!rafId) {
                    // prevent double starts
                    loop(0);
                }
            };
            this.stop = function () {
                cancelAnimationFrame(rafId);
                rafId = null;
                previousLoopTime = 0;
            };
        }

        function preventEvent(element, eventName, toggleProperty, preventWithProperty) {
            function handler(e) {
                if (Boolean(element[toggleProperty]) === Boolean(preventWithProperty)) {
                    e.stopImmediatePropagation();
                    // console.log(eventName, 'prevented on', element);
                }
                delete element[toggleProperty];
            }
            element.addEventListener(eventName, handler, false);

            // Return handler to allow to disable the prevention. Usage:
            // const preventionHandler = preventEvent(el, 'click');
            // el.removeEventHandler('click', preventionHandler);
            return handler;
        }

        function proxyProperty(object, propertyName, sourceObject, copyFirst) {
            function get() {
                return sourceObject[propertyName];
            }
            function set(value) {
                sourceObject[propertyName] = value;
            }

            if (copyFirst) {
                set(object[propertyName]);
            }

            Object.defineProperty(object, propertyName, { get: get, set: set });
        }

        /*
        File imported from: https://github.com/bfred-it/poor-mans-symbol
        Until I configure rollup to import external libs into the IIFE bundle
        */

        var _Symbol = typeof Symbol === 'undefined' ? function (description) {
            return '@' + (description || '@') + Math.random();
        } : Symbol;

        var isNeeded = /iPhone|iPod/i.test(navigator.userAgent);

        var ಠ = _Symbol();
        var ಠevent = _Symbol();
        var ಠplay = _Symbol('nativeplay');
        var ಠpause = _Symbol('nativepause');

        /**
         * UTILS
         */

        function getAudioFromVideo(video) {
            var audio = new Audio();
            audio.src = video.currentSrc || video.src;
            audio.crossOrigin = video.crossOrigin;
            return audio;
        }

        var lastRequests = [];
        lastRequests.i = 0;

        function setTime(video, time) {
            // allow one timeupdate event every 200+ ms
            if ((lastRequests.tue || 0) + 200 < Date.now()) {
                video[ಠevent] = true;
                lastRequests.tue = Date.now();
            }
            video.currentTime = time;
            lastRequests[++lastRequests.i % 3] = time * 100 | 0 / 100;
        }

        function isPlayerEnded(player) {
            return player.driver.currentTime >= player.video.duration;
        }

        function update(timeDiff) {
            // console.log('update');
            var player = this;
            if (player.video.readyState >= player.video.HAVE_FUTURE_DATA) {
                if (!player.hasAudio) {
                    player.driver.currentTime = player.video.currentTime + timeDiff * player.video.playbackRate / 1000;
                    if (player.video.loop && isPlayerEnded(player)) {
                        player.driver.currentTime = 0;
                    }
                }
                setTime(player.video, player.driver.currentTime);
            }

            // console.assert(player.video.currentTime === player.driver.currentTime, 'Video not updating!');

            if (player.video.ended) {
                player.video.pause(true);
            }
        }

        /**
         * METHODS
         */

        function play() {
            // console.log('play')
            var video = this;
            var player = video[ಠ];

            // if it's fullscreen, the developer the native player
            if (video.webkitDisplayingFullscreen) {
                video[ಠplay]();
                return;
            }

            if (!video.paused) {
                return;
            }
            player.paused = false;

            if (!video.buffered.length) {
                video.load();
            }

            player.driver.play();
            player.updater.start();

            video.dispatchEvent(new Event('play'));

            // TODO: should be fired later
            video.dispatchEvent(new Event('playing'));
        }
        function pause(forceEvents) {
            // console.log('pause')
            var video = this;
            var player = video[ಠ];

            player.driver.pause();
            player.updater.stop();

            // if it's fullscreen, the developer the native player.pause()
            // This is at the end of pause() because it also
            // needs to make sure that the simulation is paused
            if (video.webkitDisplayingFullscreen) {
                video[ಠpause]();
            }

            if (player.paused && !forceEvents) {
                return;
            }

            player.paused = true;
            video.dispatchEvent(new Event('pause'));
            if (video.ended) {
                video[ಠevent] = true;
                video.dispatchEvent(new Event('ended'));
            }
        }

        /**
         * SETUP
         */

        function addPlayer(video, hasAudio) {
            var player = video[ಠ] = {};
            player.paused = true; // track whether 'pause' events have been fired
            player.hasAudio = hasAudio;
            player.video = video;
            player.updater = new Intervalometer(update.bind(player));

            if (hasAudio) {
                player.driver = getAudioFromVideo(video);
            } else {
                player.driver = {
                    muted: true,
                    paused: true,
                    pause: function pause() {
                        player.driver.paused = true;
                    },
                    play: function play() {
                        player.driver.paused = false;
                        // media automatically goes to 0 if .play() is called when it's done
                        if (isPlayerEnded(player)) {
                            setTime(video, 0);
                        }
                    },
                    get ended() {
                        return isPlayerEnded(player);
                    }
                };
            }

            // .load() causes the emptied event
            // the alternative is .play()+.pause() but that triggers play/pause events, even worse
            // possibly the alternative is preventing this event only once
            video.addEventListener('emptied', function () {
                if (player.driver.src && player.driver.src !== video.currentSrc) {
                    // console.log('src changed', video.currentSrc);
                    setTime(video, 0);
                    video.pause();
                    player.driver.src = video.currentSrc;
                }
            }, false);

            // stop programmatic player when OS takes over
            video.addEventListener('webkitbeginfullscreen', function () {
                if (!video.paused) {
                    // make sure that the <audio> and the syncer/updater are stopped
                    video.pause();

                    // play video natively
                    video[ಠplay]();
                } else if (hasAudio && !player.driver.buffered.length) {
                    // if the first play is native,
                    // the <audio> needs to be buffered manually
                    // so when the fullscreen ends, it can be set to the same current time
                    player.driver.load();
                }
            });
            if (hasAudio) {
                video.addEventListener('webkitendfullscreen', function () {
                    // sync audio to new video position
                    player.driver.currentTime = video.currentTime;
                    // console.assert(player.driver.currentTime === video.currentTime, 'Audio not synced');
                });

                // allow seeking
                video.addEventListener('seeking', function () {
                    if (lastRequests.indexOf(video.currentTime * 100 | 0 / 100) < 0) {
                        // console.log('User-requested seeking');
                        player.driver.currentTime = video.currentTime;
                    }
                });
            }
        }

        function overloadAPI(video) {
            var player = video[ಠ];
            video[ಠplay] = video.play;
            video[ಠpause] = video.pause;
            video.play = play;
            video.pause = pause;
            proxyProperty(video, 'paused', player.driver);
            proxyProperty(video, 'muted', player.driver, true);
            proxyProperty(video, 'playbackRate', player.driver, true);
            proxyProperty(video, 'ended', player.driver);
            proxyProperty(video, 'loop', player.driver, true);
            preventEvent(video, 'seeking');
            preventEvent(video, 'seeked');
            preventEvent(video, 'timeupdate', ಠevent, false);
            preventEvent(video, 'ended', ಠevent, false); // prevent occasional native ended events
        }

        function enableInlineVideo(video) {
            var hasAudio = arguments.length <= 1 || arguments[1] === undefined ? true : arguments[1];
            var onlyWhenNeeded = arguments.length <= 2 || arguments[2] === undefined ? true : arguments[2];

            if (onlyWhenNeeded && !isNeeded || video[ಠ]) {
                return;
            }
            addPlayer(video, hasAudio);
            overloadAPI(video);
            video.classList.add('IIV');
            if (!hasAudio && video.autoplay) {
                video.play();
            }
        }

        module.exports = enableInlineVideo;
    }, {}], 2: [function (require, module, exports) {
        'use strict';

        var _Detector = require('../lib/Detector');

        var _Detector2 = _interopRequireDefault(_Detector);

        var _MobileBuffering = require('../lib/MobileBuffering');

        var _MobileBuffering2 = _interopRequireDefault(_MobileBuffering);

        function _interopRequireDefault(obj) {
            return obj && obj.__esModule ? obj : { default: obj };
        }

        /**
         * Created by yanwsh on 4/3/16.
         */

        var HAVE_ENOUGH_DATA = 4;

        var Canvas = function Canvas(baseComponent) {
            var settings = arguments.length <= 1 || arguments[1] === undefined ? {} : arguments[1];

            return {
                constructor: function init(player, options) {
                    this.settings = options;
                    this.width = player.el().offsetWidth, this.height = player.el().offsetHeight;
                    this.lon = options.initLon, this.lat = options.initLat, this.phi = 0, this.theta = 0;
                    this.videoType = options.videoType;
                    this.clickToToggle = options.clickToToggle;
                    this.mouseDown = false;
                    this.isUserInteracting = false;
                    //define scene
                    this.scene = new THREE.Scene();
                    //define camera
                    this.camera = new THREE.PerspectiveCamera(options.initFov, this.width / this.height, 1, 2000);
                    this.camera.target = new THREE.Vector3(0, 0, 0);
                    //define render
                    this.renderer = new THREE.WebGLRenderer();
                    this.renderer.setPixelRatio(window.devicePixelRatio);
                    this.renderer.setSize(this.width, this.height);
                    this.renderer.autoClear = false;
                    this.renderer.setClearColor(0x000000, 1);

                    //define texture
                    var video = settings.getTech(player);
                    this.supportVideoTexture = _Detector2.default.supportVideoTexture();
                    if (!this.supportVideoTexture) {
                        this.helperCanvas = player.addChild("HelperCanvas", {
                            video: video,
                            width: this.width,
                            height: this.height
                        });
                        var context = this.helperCanvas.el();
                        this.texture = new THREE.Texture(context);
                    } else {
                        this.texture = new THREE.Texture(video);
                    }

                    video.style.display = "none";

                    this.texture.generateMipmaps = false;
                    this.texture.minFilter = THREE.LinearFilter;
                    this.texture.maxFilter = THREE.LinearFilter;
                    this.texture.format = THREE.RGBFormat;
                    //define geometry
                    var geometry = this.videoType === "equirectangular" ? new THREE.SphereGeometry(500, 60, 40) : new THREE.SphereBufferGeometry(500, 60, 40).toNonIndexed();
                    if (this.videoType === "fisheye") {
                        var normals = geometry.attributes.normal.array;
                        var uvs = geometry.attributes.uv.array;
                        for (var i = 0, l = normals.length / 3; i < l; i++) {
                            var x = normals[i * 3 + 0];
                            var y = normals[i * 3 + 1];
                            var z = normals[i * 3 + 2];

                            var r = Math.asin(Math.sqrt(x * x + z * z) / Math.sqrt(x * x + y * y + z * z)) / Math.PI;
                            if (y < 0) r = 1 - r;
                            var theta = x == 0 && z == 0 ? 0 : Math.acos(x / Math.sqrt(x * x + z * z));
                            if (z < 0) theta = theta * -1;
                            uvs[i * 2 + 0] = -0.8 * r * Math.cos(theta) + 0.5;
                            uvs[i * 2 + 1] = 0.8 * r * Math.sin(theta) + 0.5;
                        }
                        geometry.rotateX(options.rotateX);
                        geometry.rotateY(options.rotateY);
                        geometry.rotateZ(options.rotateZ);
                    }
                    geometry.scale(-1, 1, 1);
                    //define mesh
                    this.mesh = new THREE.Mesh(geometry, new THREE.MeshBasicMaterial({ map: this.texture }));
                    //this.mesh.scale.x = -1;
                    this.scene.add(this.mesh);
                    this.el_ = this.renderer.domElement;
                    this.el_.classList.add('vjs-video-canvas');

                    options.el = this.el_;
                    baseComponent.call(this, player, options);

                    this.attachControlEvents();
                    this.player().on("play", function () {
                        this.time = new Date().getTime();
                        this.animate();
                    }.bind(this));

                    if (options.callback) options.callback();
                },

                attachControlEvents: function attachControlEvents() {
                    this.on('mousemove', this.handleMouseMove.bind(this));
                    this.on('touchmove', this.handleMouseMove.bind(this));
                    this.on('mousedown', this.handleMouseDown.bind(this));
                    this.on('touchstart', this.handleMouseDown.bind(this));
                    this.on('mouseup', this.handleMouseUp.bind(this));
                    this.on('touchend', this.handleMouseUp.bind(this));
                    if (this.settings.scrollable) {
                        this.on('mousewheel', this.handleMouseWheel.bind(this));
                        this.on('MozMousePixelScroll', this.handleMouseWheel.bind(this));
                    }
                    this.on('mouseenter', this.handleMouseEnter.bind(this));
                    this.on('mouseleave', this.handleMouseLease.bind(this));
                },

                handleResize: function handleResize() {
                    this.width = this.player().el().offsetWidth, this.height = this.player().el().offsetHeight;
                    this.camera.aspect = this.width / this.height;
                    this.camera.updateProjectionMatrix();
                    this.renderer.setSize(this.width, this.height);
                },

                handleMouseUp: function handleMouseUp(event) {
                    this.mouseDown = false;
                    if (this.clickToToggle) {
                        var clientX = event.clientX || event.changedTouches[0].clientX;
                        var clientY = event.clientY || event.changedTouches[0].clientY;
                        var diffX = Math.abs(clientX - this.onPointerDownPointerX);
                        var diffY = Math.abs(clientY - this.onPointerDownPointerY);
                        if (diffX < 0.1 && diffY < 0.1) this.player().paused() ? this.player().play() : this.player().pause();
                    }
                },

                handleMouseDown: function handleMouseDown(event) {
                    event.preventDefault();
                    var clientX = event.clientX || event.touches[0].clientX;
                    var clientY = event.clientY || event.touches[0].clientY;
                    this.mouseDown = true;
                    this.onPointerDownPointerX = clientX;
                    this.onPointerDownPointerY = clientY;
                    this.onPointerDownLon = this.lon;
                    this.onPointerDownLat = this.lat;
                },

                handleMouseMove: function handleMouseMove(event) {
                    var clientX = event.clientX || event.touches[0].clientX;
                    var clientY = event.clientY || event.touches[0].clientY;
                    if (this.settings.clickAndDrag) {
                        if (this.mouseDown) {
                            this.lon = (this.onPointerDownPointerX - clientX) * 0.2 + this.onPointerDownLon;
                            this.lat = (clientY - this.onPointerDownPointerY) * 0.2 + this.onPointerDownLat;
                        }
                    } else {
                        var x = event.pageX - this.el_.offsetLeft;
                        var y = event.pageY - this.el_.offsetTop;
                        this.lon = x / this.width * 430 - 225;
                        this.lat = y / this.height * -180 + 90;
                    }
                },

                handleMobileOrientation: function handleMobileOrientation(event) {
                    if (typeof event.rotationRate === "undefined") return;
                    var x = event.rotationRate.alpha;
                    var y = event.rotationRate.beta;

                    if (window.matchMedia("(orientation: portrait)").matches) {
                        this.lon = this.lon - y * this.settings.mobileVibrationValue;
                        this.lat = this.lat + x * this.settings.mobileVibrationValue;
                    } else if (window.matchMedia("(orientation: landscape)").matches) {
                        var orientationDegree = -90;
                        if (typeof window.orientation != "undefined") {
                            orientationDegree = window.orientation;
                        }

                        this.lon = orientationDegree == -90 ? this.lon + x * this.settings.mobileVibrationValue : this.lon - x * this.settings.mobileVibrationValue;
                        this.lat = orientationDegree == -90 ? this.lat + y * this.settings.mobileVibrationValue : this.lat - y * this.settings.mobileVibrationValue;
                    }
                },

                handleMouseWheel: function handleMouseWheel(event) {
                    event.stopPropagation();
                    event.preventDefault();
                    // WebKit
                    if (event.wheelDeltaY) {
                        this.camera.fov -= event.wheelDeltaY * 0.05;
                        // Opera / Explorer 9
                    } else if (event.wheelDelta) {
                        this.camera.fov -= event.wheelDelta * 0.05;
                        // Firefox
                    } else if (event.detail) {
                        this.camera.fov += event.detail * 1.0;
                    }
                    this.camera.fov = Math.min(this.settings.maxFov, this.camera.fov);
                    this.camera.fov = Math.max(this.settings.minFov, this.camera.fov);
                    this.camera.updateProjectionMatrix();
                },

                handleMouseEnter: function handleMouseEnter(event) {
                    this.isUserInteracting = true;
                },

                handleMouseLease: function handleMouseLease(event) {
                    this.isUserInteracting = false;
                },

                animate: function animate() {
                    this.requestAnimationId = requestAnimationFrame(this.animate.bind(this));
                    if (!this.player().paused()) {
                        if (typeof this.texture !== "undefined" && (!this.isPlayOnMobile && this.player().readyState() === HAVE_ENOUGH_DATA || this.isPlayOnMobile && this.player().hasClass("vjs-playing"))) {
                            var ct = new Date().getTime();
                            if (ct - this.time >= 30) {
                                this.texture.needsUpdate = true;
                                this.time = ct;
                            }
                            if (this.isPlayOnMobile) {
                                var currentTime = this.player().currentTime();
                                if (_MobileBuffering2.default.isBuffering(currentTime)) {
                                    if (!this.player().hasClass("vjs-panorama-mobile-inline-video-buffering")) {
                                        this.player().addClass("vjs-panorama-mobile-inline-video-buffering");
                                    }
                                } else {
                                    if (this.player().hasClass("vjs-panorama-mobile-inline-video-buffering")) {
                                        this.player().removeClass("vjs-panorama-mobile-inline-video-buffering");
                                    }
                                }
                            }
                        }
                    }
                    this.render();
                },

                render: function render() {
                    if (!this.isUserInteracting) {
                        var symbolLat = this.lat > this.settings.initLat ? -1 : 1;
                        var symbolLon = this.lon > this.settings.initLon ? -1 : 1;
                        if (this.settings.backToVerticalCenter) {
                            this.lat = this.lat > this.settings.initLat - Math.abs(this.settings.returnStepLat) && this.lat < this.settings.initLat + Math.abs(this.settings.returnStepLat) ? this.settings.initLat : this.lat + this.settings.returnStepLat * symbolLat;
                        }
                        if (this.settings.backToHorizonCenter) {
                            this.lon = this.lon > this.settings.initLon - Math.abs(this.settings.returnStepLon) && this.lon < this.settings.initLon + Math.abs(this.settings.returnStepLon) ? this.settings.initLon : this.lon + this.settings.returnStepLon * symbolLon;
                        }
                    }
                    this.lat = Math.max(this.settings.minLat, Math.min(this.settings.maxLat, this.lat));
                    this.phi = THREE.Math.degToRad(90 - this.lat);
                    this.theta = THREE.Math.degToRad(this.lon);
                    this.camera.target.x = 500 * Math.sin(this.phi) * Math.cos(this.theta);
                    this.camera.target.y = 500 * Math.cos(this.phi);
                    this.camera.target.z = 500 * Math.sin(this.phi) * Math.sin(this.theta);
                    this.camera.lookAt(this.camera.target);

                    if (!this.supportVideoTexture) {
                        this.helperCanvas.update();
                    }
                    this.renderer.clear();
                    this.renderer.render(this.scene, this.camera);
                },

                playOnMobile: function playOnMobile() {
                    this.isPlayOnMobile = true;
                    if (this.settings.autoMobileOrientation) window.addEventListener('devicemotion', this.handleMobileOrientation.bind(this));
                },

                el: function el() {
                    return this.el_;
                }
            };
        };

        module.exports = Canvas;
    }, { "../lib/Detector": 3, "../lib/MobileBuffering": 5 }], 3: [function (require, module, exports) {
        'use strict';

        var _typeof = typeof Symbol === "function" && _typeof2(Symbol.iterator) === "symbol" ? function (obj) {
            return typeof obj === "undefined" ? "undefined" : _typeof2(obj);
        } : function (obj) {
            return obj && typeof Symbol === "function" && obj.constructor === Symbol ? "symbol" : typeof obj === "undefined" ? "undefined" : _typeof2(obj);
        };

        /**
         * @author alteredq / http://alteredqualia.com/
         * @author mr.doob / http://mrdoob.com/
         */

        var Detector = {

            canvas: !!window.CanvasRenderingContext2D,
            webgl: function () {

                try {

                    var canvas = document.createElement('canvas');return !!(window.WebGLRenderingContext && (canvas.getContext('webgl') || canvas.getContext('experimental-webgl')));
                } catch (e) {

                    return false;
                }
            }(),
            workers: !!window.Worker,
            fileapi: window.File && window.FileReader && window.FileList && window.Blob,

            Check_Version: function Check_Version() {
                var rv = -1; // Return value assumes failure.

                if (navigator.appName == 'Microsoft Internet Explorer') {

                    var ua = navigator.userAgent,
                        re = new RegExp("MSIE ([0-9]{1,}[\\.0-9]{0,})");

                    if (re.exec(ua) !== null) {
                        rv = parseFloat(RegExp.$1);
                    }
                } else if (navigator.appName == "Netscape") {
                    /// in IE 11 the navigator.appVersion says 'trident'
                    /// in Edge the navigator.appVersion does not say trident
                    if (navigator.appVersion.indexOf('Trident') !== -1) rv = 11;else {
                        var ua = navigator.userAgent;
                        var re = new RegExp("Edge\/([0-9]{1,}[\\.0-9]{0,})");
                        if (re.exec(ua) !== null) {
                            rv = parseFloat(RegExp.$1);
                        }
                    }
                }

                return rv;
            },

            supportVideoTexture: function supportVideoTexture() {
                //ie 11 and edge 12 doesn't support video texture.
                var version = this.Check_Version();
                return version === -1 || version >= 13;
            },

            getWebGLErrorMessage: function getWebGLErrorMessage() {

                var element = document.createElement('div');
                element.id = 'webgl-error-message';

                if (!this.webgl) {

                    element.innerHTML = window.WebGLRenderingContext ? ['Your graphics card does not seem to support <a href="http://khronos.org/webgl/wiki/Getting_a_WebGL_Implementation" style="color:#000">WebGL</a>.<br />', 'Find out how to get it <a href="http://get.webgl.org/" style="color:#000">here</a>.'].join('\n') : ['Your browser does not seem to support <a href="http://khronos.org/webgl/wiki/Getting_a_WebGL_Implementation" style="color:#000">WebGL</a>.<br/>', 'Find out how to get it <a href="http://get.webgl.org/" style="color:#000">here</a>.'].join('\n');
                }

                return element;
            },

            addGetWebGLMessage: function addGetWebGLMessage(parameters) {

                var parent, id, element;

                parameters = parameters || {};

                parent = parameters.parent !== undefined ? parameters.parent : document.body;
                id = parameters.id !== undefined ? parameters.id : 'oldie';

                element = Detector.getWebGLErrorMessage();
                element.id = id;

                parent.appendChild(element);
            }

        };

        // browserify support
        if ((typeof module === 'undefined' ? 'undefined' : _typeof(module)) === 'object') {

            module.exports = Detector;
        }
    }, {}], 4: [function (require, module, exports) {
        "use strict";

        /**
         * Created by wensheng.yan on 5/23/16.
         */

        var element = document.createElement('canvas');
        element.className = "vjs-video-helper-canvas";

        var HelperCanvas = function HelperCanvas(baseComponent) {
            return {
                constructor: function init(player, options) {
                    this.videoElement = options.video;
                    this.width = options.width;
                    this.height = options.height;

                    element.width = this.width;
                    element.height = this.height;
                    element.style.display = "none";
                    options.el = element;

                    this.context = element.getContext('2d');
                    this.context.drawImage(this.videoElement, 0, 0, this.width, this.height);
                    baseComponent.call(this, player, options);
                },

                getContext: function getContext() {
                    return this.context;
                },

                update: function update() {
                    this.context.drawImage(this.videoElement, 0, 0, this.width, this.height);
                },

                el: function el() {
                    return element;
                }
            };
        };

        module.exports = HelperCanvas;
    }, {}], 5: [function (require, module, exports) {
        "use strict";

        /**
         * Created by yanwsh on 6/6/16.
         */

        var MobileBuffering = {
            prev_currentTime: 0,
            counter: 0,

            isBuffering: function isBuffering(currentTime) {
                if (currentTime == this.prev_currentTime) this.counter++;else this.counter = 0;
                this.prev_currentTime = currentTime;
                if (this.counter > 10) {
                    //not let counter overflow
                    this.counter = 10;
                    return true;
                }
                return false;
            }
        };

        module.exports = MobileBuffering;
    }, {}], 6: [function (require, module, exports) {
        "use strict";

        var _typeof = typeof Symbol === "function" && _typeof2(Symbol.iterator) === "symbol" ? function (obj) {
            return typeof obj === "undefined" ? "undefined" : _typeof2(obj);
        } : function (obj) {
            return obj && typeof Symbol === "function" && obj.constructor === Symbol ? "symbol" : typeof obj === "undefined" ? "undefined" : _typeof2(obj);
        };

        /**
         * Created by yanwsh on 4/4/16.
         */

        var Notice = function Notice(baseComponent) {
            var element = document.createElement('div');
            element.className = "vjs-video-notice-label";

            return {
                constructor: function init(player, options) {
                    if (_typeof(options.NoticeMessage) == "object") {
                        element = options.NoticeMessage;
                        options.el = options.NoticeMessage;
                    } else if (typeof options.NoticeMessage == "string") {
                        element.innerHTML = options.NoticeMessage;
                        options.el = element;
                    }

                    baseComponent.call(this, player, options);
                },

                el: function el() {
                    return element;
                }
            };
        };

        module.exports = Notice;
    }, {}], 7: [function (require, module, exports) {
        'use strict';

        /**
         * Created by wensheng.yan on 4/4/16.
         */

        function whichTransitionEvent() {
            var t;
            var el = document.createElement('fakeelement');
            var transitions = {
                'transition': 'transitionend',
                'OTransition': 'oTransitionEnd',
                'MozTransition': 'transitionend',
                'WebkitTransition': 'webkitTransitionEnd'
            };

            for (t in transitions) {
                if (el.style[t] !== undefined) {
                    return transitions[t];
                }
            }
        }

        function mobileAndTabletcheck() {
            var check = false;
            (function (a) {
                if (/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|mobile.+firefox|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows ce|xda|xiino|android|ipad|playbook|silk/i.test(a) || /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(a.substr(0, 4))) check = true;
            })(navigator.userAgent || navigator.vendor || window.opera);
            return check;
        }

        function isIos() {
            return (/iPhone|iPad|iPod/i.test(navigator.userAgent)
            );
        }

        function isRealIphone() {
            return (/iPhone|iPod/i.test(navigator.platform)
            );
        }

        module.exports = {
            whichTransitionEvent: whichTransitionEvent,
            mobileAndTabletcheck: mobileAndTabletcheck,
            isIos: isIos,
            isRealIphone: isRealIphone
        };
    }, {}], 8: [function (require, module, exports) {
        /**
         * Created by yanwsh on 4/3/16.
         */
        'use strict';

        Object.defineProperty(exports, "__esModule", {
            value: true
        });

        var _Util = require('./lib/Util');

        var _Util2 = _interopRequireDefault(_Util);

        var _Detector = require('./lib/Detector');

        var _Detector2 = _interopRequireDefault(_Detector);

        var _iphoneInlineVideo = require('iphone-inline-video');

        var _iphoneInlineVideo2 = _interopRequireDefault(_iphoneInlineVideo);

        function _interopRequireDefault(obj) {
            return obj && obj.__esModule ? obj : { default: obj };
        }

        var runOnMobile = _Util2.default.mobileAndTabletcheck();

        // Default options for the plugin.
        var defaults = {
            clickAndDrag: runOnMobile,
            showNotice: true,
            NoticeMessage: "Please use your mouse drag and drop the video.",
            autoHideNotice: 3000,
            //limit the video size when user scroll.
            scrollable: true,
            initFov: 75,
            maxFov: 105,
            minFov: 51,
            //initial position for the video
            initLat: 0,
            initLon: -180,
            //A float value back to center when mouse out the canvas. The higher, the faster.
            returnStepLat: 0.5,
            returnStepLon: 2,
            backToVerticalCenter: !runOnMobile,
            backToHorizonCenter: !runOnMobile,
            clickToToggle: false,

            //limit viewable zoom
            minLat: -85,
            maxLat: 85,
            videoType: "equirectangular",

            rotateX: 0,
            rotateY: 0,
            rotateZ: 0,

            autoMobileOrientation: false,
            mobileVibrationValue: _Util2.default.isIos() ? 0.022 : 1
        };

        function playerResize(player) {
            var canvas = player.getChild('Canvas');
            return function () {
                player.el().style.width = window.innerWidth + "px";
                player.el().style.height = window.innerHeight + "px";
                canvas.handleResize();
            };
        }

        function fullscreenOnIOS(player, clickFn) {
            var resizeFn = playerResize(player);
            player.controlBar.fullscreenToggle.off("tap", clickFn);
            player.controlBar.fullscreenToggle.on("tap", function fullscreen() {
                var canvas = player.getChild('Canvas');
                if (!player.isFullscreen()) {
                    //set to fullscreen
                    player.isFullscreen(true);
                    player.enterFullWindow();
                    resizeFn();
                    window.addEventListener("devicemotion", resizeFn);
                } else {
                    player.isFullscreen(false);
                    player.exitFullWindow();
                    player.el().style.width = "";
                    player.el().style.height = "";
                    canvas.handleResize();
                    window.removeEventListener("devicemotion", resizeFn);
                }
            });
        }

        /**
         * Function to invoke when the player is ready.
         *
         * This is a great place for your plugin to initialize itself. When this
         * function is called, the player will have its DOM and child components
         * in place.
         *
         * @function onPlayerReady
         * @param    {Player} player
         * @param    {Object} [options={}]
         */
        var onPlayerReady = function onPlayerReady(player, options, settings) {
            player.addClass('vjs-panorama');
            if (!_Detector2.default.webgl) {
                PopupNotification(player, {
                    NoticeMessage: _Detector2.default.getWebGLErrorMessage(),
                    autoHideNotice: options.autoHideNotice
                });
                if (options.callback) {
                    options.callback();
                }
                return;
            }
            player.addChild('Canvas', options);
            var canvas = player.getChild('Canvas');
            if (runOnMobile) {
                var videoElement = settings.getTech(player);
                if (_Util2.default.isRealIphone()) {
                    (0, _iphoneInlineVideo2.default)(videoElement, true);
                }
                if (_Util2.default.isIos()) {
                    fullscreenOnIOS(player, settings.getFullscreenToggleClickFn(player));
                }
                player.addClass("vjs-panorama-mobile-inline-video");
                player.removeClass("vjs-using-native-controls");
                canvas.playOnMobile();
            }
            if (options.showNotice) {
                player.on("playing", function () {
                    PopupNotification(player, options);
                });
            }
            canvas.hide();
            player.on("play", function () {
                canvas.show();
            });
            player.on("fullscreenchange", function () {
                canvas.handleResize();
            });
        };

        var PopupNotification = function PopupNotification(player) {
            var options = arguments.length <= 1 || arguments[1] === undefined ? {
                NoticeMessage: ""
            } : arguments[1];

            var notice = player.addChild('Notice', options);

            if (options.autoHideNotice > 0) {
                setTimeout(function () {
                    notice.addClass("vjs-video-notice-fadeOut");
                    var transitionEvent = _Util2.default.whichTransitionEvent();
                    var hide = function hide() {
                        notice.hide();
                        notice.removeClass("vjs-video-notice-fadeOut");
                        notice.off(transitionEvent, hide);
                    };
                    notice.on(transitionEvent, hide);
                }, options.autoHideNotice);
            }
        };

        var plugin = function plugin() {
            var settings = arguments.length <= 0 || arguments[0] === undefined ? {} : arguments[0];

            /**
             * A video.js plugin.
             *
             * In the plugin function, the value of `this` is a video.js `Player`
             * instance. You cannot rely on the player being in a "ready" state here,
             * depending on how the plugin is invoked. This may or may not be important
             * to you; if not, remove the wait for "ready"!
             *
             * @function panorama
             * @param    {Object} [options={}]
             *           An object of options left to the plugin author to define.
             */
            var videoTypes = ["equirectangular", "fisheye"];
            var panorama = function panorama(options) {
                var _this = this;

                if (settings.mergeOption) options = settings.mergeOption(defaults, options);
                if (videoTypes.indexOf(options.videoType) == -1) defaults.videoType;
                this.ready(function () {
                    onPlayerReady(_this, options, settings);
                });
            };

            // Include the version number.
            panorama.VERSION = '0.0.7';

            return panorama;
        };

        exports.default = plugin;
    }, { "./lib/Detector": 3, "./lib/Util": 7, "iphone-inline-video": 1 }], 9: [function (require, module, exports) {
        'use strict';

        var _Canvas = require('./lib/Canvas');

        var _Canvas2 = _interopRequireDefault(_Canvas);

        var _Notice = require('./lib/Notice');

        var _Notice2 = _interopRequireDefault(_Notice);

        var _HelperCanvas = require('./lib/HelperCanvas');

        var _HelperCanvas2 = _interopRequireDefault(_HelperCanvas);

        var _plugin = require('./plugin');

        var _plugin2 = _interopRequireDefault(_plugin);

        function _interopRequireDefault(obj) {
            return obj && obj.__esModule ? obj : { default: obj };
        }

        function getTech(player) {
            return player.tech({ IWillNotUseThisInPlugins: true }).el();
        }

        function getFullscreenToggleClickFn(player) {
            return player.controlBar.fullscreenToggle.handleClick;
        }

        var component = videojs.getComponent('Component');
        var canvas = (0, _Canvas2.default)(component, {
            getTech: getTech
        });
        videojs.registerComponent('Canvas', videojs.extend(component, canvas));

        var notice = (0, _Notice2.default)(component);
        videojs.registerComponent('Notice', videojs.extend(component, notice));

        var helperCanvas = (0, _HelperCanvas2.default)(component);
        videojs.registerComponent('HelperCanvas', videojs.extend(component, helperCanvas));

        // Register the plugin with video.js.

        videojs.plugin('panorama', (0, _plugin2.default)({
            mergeOption: function mergeOption(defaults, options) {
                return videojs.mergeOptions(defaults, options);
            },
            getTech: getTech,
            getFullscreenToggleClickFn: getFullscreenToggleClickFn
        }));
    }, { "./lib/Canvas": 2, "./lib/HelperCanvas": 4, "./lib/Notice": 6, "./plugin": 8 }] }, {}, [9]);

/***/ }),

/***/ 83:
/***/ (function(module, exports) {

module.exports = $;

/***/ }),

/***/ 84:
/***/ (function(module, exports) {

module.exports = THREE;

/***/ }),

/***/ 85:
/***/ (function(module, exports) {

module.exports = videojs;

/***/ }),

/***/ 86:
/***/ (function(module, exports, __webpack_require__) {

"use strict";


var Point = __webpack_require__(187);

//
// Map utility
//
var __FLT_EPSILON__ = 1e-10;

/**
 *
 * @param value number
 * @param inputMin number
 * @param inputMax number
 * @param outputMin number
 * @param outputMax number
 * @param isClamp bool
 * @return {number}
 */
function mmap(value, inputMin, inputMax, outputMin, outputMax, isClamp) {

	if (Math.abs(inputMin - inputMax) < __FLT_EPSILON__) {
		return outputMin;
	} else {
		var outVal = (value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin;

		if (isClamp) {
			if (outputMax < outputMin) {
				if (outVal < outputMax) outVal = outputMax;else if (outVal > outputMin) outVal = outputMin;
			} else {
				if (outVal > outputMax) outVal = outputMax;else if (outVal < outputMin) outVal = outputMin;
			}
		}
		return outVal;
	}
}
/**
 *
 * @param a number
 * @param min number
 * @param max number
 * @return {number}
 */
function clamp(a, min, max) {
	return a < min ? min : a > max ? max : a;
}

/**
 * @param a angle in deg
 * @param min
 * @param max
 * max - min == 360
 * @return {number} angle in [min...max]
 */
function alignAngle(a) {
	var min = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : -180;
	var max = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 180;

	while (a < min) {
		a += 360;
	}while (a > max) {
		a -= 360;
	}return a;
}

//
// End map utility
//

/**
 *  Eight channel audio format.
 *
 *  Order of input angles:
 * @param yaw  yaw in angles
 * @param pitch pitch in angles
 * @param roll  roll in angles (default = 0)
 * @return {Array} volumes[8]
 */
function eightChannelsAlgorithm(yaw, pitch) {
	var roll = arguments.length > 2 && arguments[2] !== undefined ? arguments[2] : 0;

	//Orientation input safety clamps/alignment
	pitch = alignAngle(pitch, -180, 180);
	pitch = clamp(pitch, -90, 90); // -90, 90

	yaw = alignAngle(yaw, 0, 360);

	roll = alignAngle(roll, -180, 180);
	roll = clamp(roll, -90, 90); // -90, 90

	var coefficients = [8];
	coefficients[0] = 1. - Math.min(1., Math.min(360. - yaw, yaw) / 90.);
	coefficients[1] = 1. - Math.min(1., Math.abs(90. - yaw) / 90.);
	coefficients[2] = 1. - Math.min(1., Math.abs(180. - yaw) / 90.);
	coefficients[3] = 1. - Math.min(1., Math.abs(270. - yaw) / 90.);

	var tiltHigh = mmap(roll, -90., 90., 0., 1., true);
	var tiltLow = 1. - tiltHigh;

	//ISSUE//
	//Able to kill stereo by making both pitch and tilt at max or min values together without proper clamps

	var result = [];
	result.length = 16;
	result[0] = coefficients[0] * tiltHigh * 2.0; // 1 left
	result[1] = coefficients[3] * tiltHigh * 2.0; //   right
	result[2] = coefficients[1] * tiltLow * 2.0; // 2 left
	result[3] = coefficients[0] * tiltLow * 2.0; //   right
	result[4] = coefficients[3] * tiltLow * 2.0; // 3 left
	result[5] = coefficients[2] * tiltLow * 2.0; //   right
	result[6] = coefficients[2] * tiltHigh * 2.0; // 4 left
	result[7] = coefficients[1] * tiltHigh * 2.0; //   right

	result[0 + 8] = coefficients[0] * tiltLow * 2.0; // 1 left
	result[1 + 8] = coefficients[3] * tiltLow * 2.0; //   right
	result[2 + 8] = coefficients[1] * tiltHigh * 2.0; // 2 left
	result[3 + 8] = coefficients[0] * tiltHigh * 2.0; //   right
	result[4 + 8] = coefficients[3] * tiltHigh * 2.0; // 3 left
	result[5 + 8] = coefficients[2] * tiltHigh * 2.0; //   right
	result[6 + 8] = coefficients[2] * tiltLow * 2.0; // 4 left
	result[7 + 8] = coefficients[1] * tiltLow * 2.0; //   right

	var pitchHigherHalf = mmap(pitch, 90., -90., 0., 1., true);
	var pitchLowerHalf = 1. - pitchHigherHalf;

	for (var i = 0; i < 8; ++i) {
		result[i] *= pitchLowerHalf;
		result[i + 8] *= pitchHigherHalf;
	}

	return result;
}

//
//  Eight pairs audio format.
//
//  Order of input angles:
//  Y = yaw in angles
//  P = pitch in angles
//  R = roll in angles
//

function eightPairsAlgorithm(yaw, pitch, roll) {

	//Orientation input safety clamps/alignment
	pitch = alignAngle(pitch, -180, 180);
	pitch = clamp(pitch, -90, 90); // -90, 90

	yaw = alignAngle(yaw, 0, 360);

	var volumes = [8];
	volumes[0] = 1. - Math.min(1., Math.min(360. - yaw, yaw) / 90.);
	volumes[1] = 1. - Math.min(1., Math.abs(90. - yaw) / 90.);
	volumes[2] = 1. - Math.min(1., Math.abs(180. - yaw) / 90.);
	volumes[3] = 1. - Math.min(1., Math.abs(270. - yaw) / 90.);

	var pitchHigherHalf = mmap(pitch, 90., -90., 0., 1., true);
	var pitchLowerHalf = 1. - pitchHigherHalf;

	var result = [];
	result.push(volumes[0] * pitchHigherHalf);
	result.push(volumes[1] * pitchHigherHalf);
	result.push(volumes[2] * pitchHigherHalf);
	result.push(volumes[3] * pitchHigherHalf);
	result.push(volumes[4] * pitchLowerHalf);
	result.push(volumes[5] * pitchLowerHalf);
	result.push(volumes[6] * pitchLowerHalf);
	result.push(volumes[7] * pitchLowerHalf);

	return result;
}

module.exports.eightPairsAlgorithm = eightPairsAlgorithm;
module.exports.eightChannelsAlgorithm = eightChannelsAlgorithm;

/***/ }),

/***/ 87:
/***/ (function(module, exports, __webpack_require__) {

/* WEBPACK VAR INJECTION */(function(global) {var AudioContext = global.AudioContext || global.webkitAudioContext;
var OfflineAudioContext = global.OfflineAudioContext || global.webkitOfflineAudioContext;
var BaseAudioContext = global.BaseAudioContext || (OfflineAudioContext && Object.getPrototypeOf(OfflineAudioContext));

module.exports = typeof BaseAudioContext === "function" ? BaseAudioContext : AudioContext;

/* WEBPACK VAR INJECTION */}.call(exports, __webpack_require__(52)))

/***/ })

/******/ });
//# sourceMappingURL=_main.js.map