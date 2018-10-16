"use strict";

/*
const videojs = require('./video.min.js');
const THREE = require('./three.min.js');
//import panorama from './videojs-panorama.js' 
*/
 
 const FRAMES_PER_SECOND = 60;
 const MAX_SOUND_PLAYER_ERROR = 0.1; //s

function Mach1VideoPlayer(audioFiles8, elemId, statusElId) {

 const vjs = videojs(elemId, {
        controls: false, // debug TODO: controls: false
        autoplay: false,
        loop: true,
        muted: true
    }, function() {

        let thus = this;
        let isVideoReady = false;
        let isDelete = false;
        let isPlay = false;

        let status = document.getElementById(statusElId);

        let sound = new Mach1SoundPlayer(audioFiles8, thus);

        // DEBUG
        document.sound = sound;

        let videoElement = document.getElementById(elemId);
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

        this.on('pause', function() {
            isPlay = false;
            sound.pause();
        });


        this.on('seeked', function() {
            setTimeout(function() {
                if (!isDelete) {
                    sound.rewind(thus.currentTime());
                }
            }, 100);
        });

        this.on('play', function() {
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

                sound.updateVolumes(-theta, -alpha);
            }
        }

        setTimeout(function __timeoutUpdateVolumes() {
            if (!isDelete && isVideoReady) {
                updateVolumes();

                if (Math.abs(sound.currentTime() - thus.currentTime()) > MAX_SOUND_PLAYER_ERROR && isPlay) {
                    console.warn('Rebalance: player - sound error = ' + (thus.currentTime() - sound.currentTime()));
                    console.warn('player.currentTime() = ', thus.currentTime());
                    console.warn('sound.currentTime() = ', sound.currentTime());

                    if (thus.currentTime() - sound.currentTime() < 1 && thus.currentTime() - sound.currentTime() > 0) {
                        sound.rewind(thus.currentTime() + (thus.currentTime() - sound.currentTime()) / 4);
                    } else {
                        sound.rewind(thus.currentTime());
                    }
                }

                let logHtml = `sound: ${(sound.currentTime() * 1000).toFixed()} <br/>
								   player: ${(thus.currentTime() * 1000).toFixed()} <br/><br/>
								   delay p - s: ${((thus.currentTime() - sound.currentTime()) * 1000).toFixed()} `;

                status.innerHTML = logHtml;

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
            if (!isDelete) {
                if (isVideoReady == true && sound.isReady() == true) {
                    Play();
                } else {
                    setTimeout(__playTimeout, 100);
                }
            }
        }, 100);


    });
	
    return this;
}

//module.exports = Mach1VideoPlayer;