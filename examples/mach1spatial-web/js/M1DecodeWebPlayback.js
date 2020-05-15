"use strict";

let THREECAMERA = null;
let ASPECTRATIO = null;

let m1Decode = null;

let m1DecodeModule = Mach1DecodeModule();
m1DecodeModule.onInited = function() {
    m1Decode = new(m1DecodeModule).Mach1Decode();
    m1Decode.setPlatformType(m1Decode.Mach1PlatformType.Mach1PlatformDefault);
    m1Decode.setDecodeAlgoType(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
    m1Decode.setFilterSpeed(0.95);
}

var audioFiles8 = [
    'audio/m1spatial/T1.mp3',
    'audio/m1spatial/T2.mp3',
    'audio/m1spatial/T3.mp3',
    'audio/m1spatial/T4.mp3',
    'audio/m1spatial/B5.mp3',
    'audio/m1spatial/B6.mp3',
    'audio/m1spatial/B7.mp3',
    'audio/m1spatial/B8.mp3'
];

let sound = new Mach1SoundPlayer();
sound.setup(audioFiles8);
//sound.play();

function Play() {
	sound.play();
}

function Stop() {
	sound.stop();
}

// callback: launched if a face is detected or lost.
function detect_callback(faceIndex, isDetected) {
    if (isDetected) {
        console.log('INFO in detect_callback(): DETECTED');
    } else {
        console.log('INFO in detect_callback(): LOST');
    }
}

// build the 3D. called once when Jeeliz Face Filter is OK
function init_threeScene(spec) {
    const threeStuffs = THREE.JeelizHelper.init(spec, detect_callback);

    // CREATE A CUBE:
    const cubeGeometry = new THREE.BoxGeometry(1, 1, 1);
    const cubeMaterial = new THREE.MeshNormalMaterial();
    const threeCube = new THREE.Mesh(cubeGeometry, cubeMaterial);
    threeCube.frustumCulled = false;
    threeStuffs.faceObject.add(threeCube);

    // CREATE THE CAMERA:
    ASPECTRATIO = spec.canvasElement.width / spec.canvasElement.height;
    THREECAMERA = new THREE.PerspectiveCamera(20, ASPECTRATIO, 0.1, 100);
} // end init_threeScene()

// entry point:
function main() {
    JeelizResizer.size_canvas({
        canvasId: 'jeeFaceFilterCanvas',
        callback: function(isError, bestVideoSettings) {
            init_faceFilter(bestVideoSettings);
        }
    })
} //end main()

function rad2deg(angle) {
    return angle * 57.29577951308232 // angle / Math.PI * 180
}

function deg2rad(angle) {
    return angle * 0.017453292519943295 // (angle / 180) * Math.PI;
}

function init_faceFilter(videoSettings) {
    JEEFACEFILTERAPI.init({
        canvasId: 'jeeFaceFilterCanvas',
        NNCpath: 'js/NNCwideAngles.json', // Neuron net with wide angles
        maxFacesDetected: 1,
        stabilizationSettings: { // adjust stabilization parameters for more responsivity
            'translationFactorRange': [0.001, 0.003],
            'rotationFactorRange': [0.002, 0.015],
            'qualityFactorRange': [0.91, 0.98],
            'alphaRange': [0.07, 1]
        },

        callbackReady: function(errCode, spec) {
            if (errCode) {
                console.log('AN ERROR HAPPENS. ERR =', errCode);
                return;
            }

            console.log('INFO : JEEFACEFILTERAPI IS READY');
            init_threeScene(spec);
        },

        //c alled at each render iteration (drawing loop):
        callbackTrack: function(detectState) {
            THREE.JeelizHelper.render(detectState, THREECAMERA);
            //console.log(ASPECTRATIO, THREECAMERA.fov/2, ASPECTRATIO* deg2rad(THREECAMERA.fov));

            if (THREE.JeelizHelper.get_isDetected()) {
                // move the cube in order to fit the head:
                const tanFOV = Math.tan(ASPECTRATIO * deg2rad(THREECAMERA.fov)); // tan(FOV/2), in radians
                const W = detectState.s; // relative width of the detection window (1-> whole width of the detection window)
                const D = 1 / (2 * W * tanFOV); // distance between the front face of the cube and the camera

                // coords in 2D of the center of the detection window in the viewport:
                const xv = detectState.x;
                const yv = detectState.y;

                // coords in 3D of the center of the cube (in the view coordinates system):
                var z = -D - 0.5; // minus because view coordinate system Z goes backward. -0.5 because z is the coord of the center of the cube (not the front face)
                var x = xv * D * tanFOV;
                var y = yv * D * tanFOV / ASPECTRATIO;

                var pos = [x, y, 8 + z]; // x y z
                var rot = [rad2deg(-detectState.ry), rad2deg(-detectState.rx), rad2deg(detectState.rz)]; // y p r

                if (m1Decode != null) {
                    m1Decode.beginBuffer();
                    let decoded = m1Decode.decode(rot[0], rot[1], rot[2]);
                    m1Decode.endBuffer();

                    sound.updateVolumes(decoded);
                    
					var strDebug = "";
					decoded.forEach(function(d) {
						strDebug += d.toFixed(2) + " , ";
					});
                    console.log('decoded = ', strDebug);
                }

                /*
                                if (m1DecodePositional != null) {
                                    m1DecodePositional.setListenerPosition(rot[0], rot[1], rot[2]);
                                    m1DecodePositional.evaluatePositionResults();
                                    var decoded = m1DecodePositional.getCoefficients();
                                    console.log('decoded = ', decoded);
                                }
                */
                // move and rotate the cube:
                //console.log("position", pos);
                //console.log("rotation", rot); //"XYZ" rotation order;
            }

        }
    }); //end JEEFACEFILTERAPI.init call
}