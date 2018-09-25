'use strict';

let videojs = require('videojs');
require('./videojs/videojs-panorama.v5');
let THREE = require('THREE');
let $ = require('jQuery');

const eightChannelsAlgorithm = require('./sound').eightChannelsAlgorithm;

const FRAMES_PER_SECOND = 60;

function delay(cb, time)
{
	let timerId = setTimeout(cb, time);

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

const audioFiles4 = [{
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

const audioFiles8 = (function () {
	let audio = {};

	for (let side of ['left', 'right'])
	{
		audio[side] = [];

		for (let posH of ['l', 'h'])
		{
			for (let posFB of ['f', 'b'])
			{
				for (let posLR of (posFB == 'f' ? ['l', 'r'] : ['r', 'l']))
				{
					audio[side].push({
						src: "./audio/000.mp3", /* ./audio/side_posLRposFBposH.mp3 */
						name: side + '_' + posLR + posFB + posH
					});
				}
			}
		}
	}

	console.log(audio);

	return audio;
})();


let sound;

// DEBUG
window.sound = sound;
// END DEBUG


$(document).ready(function () {
	let player = window.player = videojs('videojs-panorama-player', {}, function() {
		let thus = this;

		let isPlayBeforeSeeking = false;
		let isVideoReady = false;

		sound = new Sound(audioFiles8, player);

		let videoElement = document.getElementById("videojs-panorama-player");
		let width = videoElement.offsetWidth;
		let height = videoElement.offsetHeight;

		this.width(width);
		this.height(height);

		this.volume(0);

		this.panorama({
			clickAndDrag: true,
			showNotice: false,
			backToVerticalCenter: false,
			backToHorizonCenter: false,
			// clickToToggle: true,
			callback: function() {
				isVideoReady = true;
			}
		});

		this.on('pause', function () {
			sound.pause();
		});


		this.on('seeked', function() {
			delay(function() {
				sound.rewind(thus.currentTime());
			}, 100);
		});

		this.on('play', function() {
			sound.play();
		});

		function updateVolumes() {
			if (thus.getChild('Canvas'))
			{
				let vector = new THREE.Vector3();

				thus.getChild('Canvas').camera.getWorldDirection(vector);
				let alpha = Math.asin(vector.y) * 180 / Math.PI;
				let theta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

				if (theta < 0)
					theta = 360 + theta;

				sound.updateVolumes(-theta, -alpha);
			}
		}

		setTimeout(function __timeoutUpdateVolumes(){
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
			if (isVideoReady == true && sound.isReady() == true)
			{
				Play();
			}
			else
			{
				setTimeout(__playTimeout, 100);
			}
		}, 100);

		__cb(thus, sound);
	});

});


function Sound(audioFiles, player) {
	let audio = {
		left: [8],
		right: [8]
	};
	
	let _volumes = {
		left: [8],
		right: [8]
	};
	let loaded = false;
	let loadedCount = 0;

	let isPlay = false;
	let isSoundReady = false;

	let maxPhaseError = 0;

	function preloadAudio(i, side) {
		audio[side][i] = new Audio();
		audio[side][i].addEventListener('canplaythrough', loadedAudio, false);
		audio[side][i].src = audioFiles[side][i].src;
	}

	function loadedAudio() {
		if (!loaded) {
			loadedCount++;
			loaded = (loadedCount == audioFiles.left.length + audioFiles.right.length);

			isSoundReady = loaded;
			document.getElementById("status").innerHTML = "loaded: " + (100 * loadedCount / (audioFiles.left.length + audioFiles.right.length));
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
		for (let i in volumes[side])
		{
			if (volumes[side].hasOwnProperty(i))
			{
				if (volumes[side][i] < 0 || volumes[side][i] > 1)
				{
					console.warn('Volumes error(out of range):' + volumes);

					return;
				}
			}
		}

		for (let i in audio[side])
		{
			audio[side][i].volume = volumes[side][i];
		}
	}

	function getVolumes() {
		return _volumes;
	}

	function setVolumes(volumes = _volumes)
	{
		setVolumeOnSide(volumes, 'left');
		setVolumeOnSide(volumes, 'right');

		_volumes = volumes;

		return getVolumes();
	}

	function rewind(time) {
		if (time < 0)
		{
			stop();

			return ;
		}

		for (let side of ['left', 'right'])
		{
			for (let i in audio[side])
			{
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
		if (!isPlay)
		{
			for (let side of ['left', 'right'])
			{
				for (let i in audio[side])
				{
					audio[side][i].play();
				}
			}

			isPlay = true;
		}
	}

	function pause() {
		if (isPlay)
		{
			for (let side of ['left', 'right'])
			{
				for (let i in audio[side])
				{
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

	function updateVolumes(yaw, pitch, roll = 0)
	{
		let vols = eightChannelsAlgorithm(yaw, pitch, roll);

		for (let i = 0; i < 8; ++i)
		{
			_volumes.left[i] = vols[2 * i];
		}

		for (let i = 0; i < 8; ++i)
		{
			_volumes.right[i] = vols[2 * i + 1];
		}

		setVolumes();
	}

	// auto start preloading
	for (let i in audioFiles.left) {
		preloadAudio(i, 'left');
	}
	for (let i in audioFiles.right) {
		preloadAudio(i, 'right');
	}

	setInterval(function() {
		if (loaded) {
			let min = 3600; // hour
			let max = 0;

			document.getElementById("status").innerHTML = "currentTime: <br/>";

			for (let side of ['left', 'right'])
			{
				for (let i in audio[side])
				{
					let pos = audio[side][i].currentTime;
					if (pos < min) min = pos;
					if (pos > max) max = pos;

					document.getElementById("status").innerHTML += audioFiles[side][i].name + ". " + pos + " , volume: " + audio[side][i].volume + "<br/>";
				}
			}

			if ((max - min) * 1000 > maxPhaseError)
			{
				maxPhaseError = (max - min) * 1000;
			}

			document.getElementById("status").innerHTML += "<br/>" + "<br/>phase error: " + (max - min) * 1000 + "ms<br/>";
			document.getElementById("status").innerHTML += "max phase error: " + maxPhaseError + "ms<br/>";
			document.getElementById("status").innerHTML += "<br/>player: " + player.currentTime() + "ms<br/>";
			document.getElementById("status").innerHTML += "player/sound error: " +
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
		isReady: isReady
	};
}


function __cb(player, sound) {
	let cameraRotationAngle = [0, 0];

	const POS_Y_H_SPHERE = 600;
	const SPHERE_DIAMETER = 100;

	function animate() {
		setTimeout(function () {
			requestAnimationFrame(animate);

			if (player.getChild('Canvas'))
			{
				let vector = new THREE.Vector3();

				player.getChild('Canvas').camera.getWorldDirection(vector);
				let alpha = Math.asin(vector.y) * 180 / Math.PI;
				let theta = Math.atan2(vector.x, vector.z) * 180 / Math.PI + 90;

				if (theta < 0) theta = 360 + theta;

				// console.log("cameraRotationAngle:", cameraRotationAngle);

				cameraRotationAngle= [theta, alpha];
		}
			else
			{
				console.warn("player.getChild(Canvas) undefined");

				cameraRotationAngle = [0, 0];
			}
			// animating/drawing code goes here

			render();
		}, 1000 / FRAMES_PER_SECOND);
	}

	let SCREEN_WIDTH = $("#helper").width();
	let SCREEN_HEIGHT = $("#helper").height();

	let container;
	let camera, scene, renderer, grid;
	let cameraRig;
	let cameraPerspective;
	let cameraPerspectiveHelper;

	let cameraOrtho, cameraOrthoHelper;

	let geometry = new THREE.SphereGeometry(SPHERE_DIAMETER, 32, 32);
	let materialRight = new THREE.MeshBasicMaterial({
		color: 0xff0000 // red
	});
	let materialLeft = new THREE.MeshBasicMaterial({
		color: 0x0000ff // blue
	});

	let sphere = {
		left: [],
		right: []
	};

	let pos = [
		[-1, -1], // front-left
		[1, -1], // front-right
		[-1, 1], // back-left
		[1, 1], // back-right
	];

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


		for (let i = 0; i < 4; i++) {
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

		for (let i = 4; i < 8; i++) {
			sphere.left[i] = new THREE.Mesh(geometry, materialLeft);
			sphere.right[i] = new THREE.Mesh(geometry, materialRight);

			sphere.left[i].position.x = 800 * (pos[i - 4][0] - pos[i - 4][1] * 0.1);
			sphere.left[i].position.y = POS_Y_H_SPHERE;
			sphere.left[i].position.z = 800 * (pos[i - 4][1] + pos[i - 4][0] * 0.1);

			sphere.right[i].position.x = 800 * (pos[i - 4][0] + pos[i - 4][1] * 0.1);
			sphere.right[i].position.y = POS_Y_H_SPHERE;
			sphere.right[i].position.z = 800 * (pos[i - 4][1] - pos[i - 4][0] * 0.1);

			scene.add(sphere.left[i]);
			scene.add(sphere.right[i]);
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
		let volumes = sound.getVolumes();

		for (let i = 0; i < 8; i++) {
			if (volumes.left[i] == 0) {
				sphere.left[i].visible = false;
			}
			else {
				sphere.left[i].visible = true;
				sphere.left[i].scale.set(volumes.left[i], volumes.left[i], volumes.left[i]);
			}

			if (volumes.right[i] == 0) {
				sphere.right[i].visible = false;
			}
			else {
				sphere.right[i].visible = true;
				sphere.right[i].scale.set(volumes.right[i], volumes.right[i], volumes.right[i]);
			}
			//  sphere[i].scale.set((i+1)*2,(i+1)*2,(i+1)*2);
		}
		// sphere[3].scale.set(10,10,10);

		cameraRig.rotation.set(0, 0, 0);
		cameraRig.rotateOnAxis((new THREE.Vector3(0, 1, 0)).normalize(), cameraRotationAngle[0] * Math.PI / 180);
		cameraRig.rotateOnAxis((new THREE.Vector3(1, 0, 0)).normalize(), cameraRotationAngle[1] * Math.PI / 180);

		renderer.clear();

		grid.material.opacity = 0.1;

		renderer.setViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		renderer.render(scene, camera);
	}

}