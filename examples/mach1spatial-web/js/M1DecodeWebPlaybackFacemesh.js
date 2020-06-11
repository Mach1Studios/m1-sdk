// ------------------------
window.modeTracker = "";

function selectTracker() {
    var ele = document.getElementsByName("mode");
    for (i = 0; i < ele.length; i++) {
        if (ele[i].checked) {
            window.modeTracker = ele[i].value;
        }
    }
}

document.addEventListener('DOMContentLoaded', (event) => {
    selectTracker();
})

// ------------------------
function handleDeviceOrientation(event) {
    var x = event.beta;
    var y = event.alpha;
    var z = event.gamma;
    console.log(x, y, z);

    if (window.modeTracker == "facetracker") {
        window.yaw = x;
        window.pitch = y;
        window.roll = z;
    }
}

window.addEventListener("deviceorientation", handleDeviceOrientation);

// ------------------------ 
controls = new(function() {
    this.nPoint = 468;
    this.yawMultiplier = 2;
    this.pitchMultiplier = 1;
    this.rollMultiplier = 1;
    this.FOV = 35;
    this.filterSpeed = 0.9;
    this.oneEuroFilterBeta = 0.06;
})();

function setupDatGui() {
    const gui = new dat.GUI();
    //gui.add(controls, "nPoint", 300, 468); //468);
    gui.add(controls, "yawMultiplier", 0.0, 5.0);
    gui.add(controls, "pitchMultiplier", 0.0, 5.0);
    gui.add(controls, "rollMultiplier", 0.0, 5.0);
    gui.add(controls, "FOV", 30.0, 90.0);
    gui.add(controls, "filterSpeed", 0.1, 1.0);

    gui.add(controls, "oneEuroFilterBeta", 0.05, 0.1).onChange(function(value) {
        createOneEuroFilters();
    });
}

function radians_to_degrees(radians) {
    return radians * (180 / Math.PI);
}

function isMobile() {
    const isAndroid = /Android/i.test(navigator.userAgent);
    const isiOS = /iPhone|iPad|iPod/i.test(navigator.userAgent);
    return isAndroid || isiOS;
}

let model, ctx, videoWidth, videoHeight, video, canvas;

const mobile = isMobile();

async function setupCamera() {
    video = document.getElementById("video");

    const stream = await navigator.mediaDevices.getUserMedia({
        audio: false,
        video: {
            facingMode: "user",
            width: mobile ? undefined : 640,
            height: mobile ? undefined : 480,
        },
    });
    video.srcObject = stream;

    return new Promise((resolve) => {
        video.onloadedmetadata = () => {
            resolve(video);
        };
    });
}

async function renderPrediction() {
    const predictions = await model.estimateFaces(video);
    ctx.drawImage(video, 0, 0, videoWidth, videoHeight, 0, 0, canvas.width, canvas.height);

    document.getElementById("stats").innerHTML = "";
    if (predictions.length > 0) {
        predictions.forEach((prediction) => {
            try {
                document.getElementById("stats").innerHTML += "confidence: " + prediction.faceInViewConfidence.toFixed(4);
            } catch (err) {
                document.getElementById("stats").innerHTML = err.message;
            }

            const keypoints = prediction.scaledMesh;
            // console.log(keypoints[0][2])

            for (let i = 0; i < keypoints.length; i++) {
                const x = keypoints[i][0];
                const y = keypoints[i][1];

                ctx.fillStyle = "white";
                ctx.fillRect(x, y, 2, 2);

                if (parseInt(controls.nPoint) == i) {
                    ctx.fillStyle = "red";
                    ctx.fillRect(x, y, 6, 6);
                }

                if (i == 10 || i == 152) {
                    ctx.fillStyle = "green";
                    ctx.fillRect(x, y, 6, 6);
                }
                if (i == 234 || i == 454) {
                    ctx.fillStyle = "yellow";
                    ctx.fillRect(x, y, 6, 6);
                }
            }

            var pTop = new THREE.Vector3(prediction.mesh[10][0], prediction.mesh[10][1], prediction.mesh[10][2]);
            var pBottom = new THREE.Vector3(prediction.mesh[152][0], prediction.mesh[152][1], prediction.mesh[152][2]);
            var pLeft = new THREE.Vector3(prediction.mesh[234][0], prediction.mesh[234][1], prediction.mesh[234][2]);
            var pRight = new THREE.Vector3(prediction.mesh[454][0], prediction.mesh[454][1], prediction.mesh[454][2]);

            var pTB = pTop.clone().addScaledVector(pBottom, -1).normalize();
            var pLR = pLeft.clone().addScaledVector(pRight, -1).normalize();

            var yaw = radians_to_degrees(Math.PI / 2 - pLR.angleTo(new THREE.Vector3(0, 0, 1)));
            var pitch = radians_to_degrees(Math.PI / 2 - pTB.angleTo(new THREE.Vector3(0, 0, 1)));
            var roll = radians_to_degrees(Math.PI / 2 - pTB.angleTo(new THREE.Vector3(1, 0, 0)));

            if (yaw > parseFloat(controls.FOV)) {
                yaw = parseFloat(controls.FOV);
            }
            if (yaw < -parseFloat(controls.FOV)) {
                yaw = -parseFloat(controls.FOV);
            }
            if (pitch > parseFloat(controls.FOV)) {
                pitch = parseFloat(controls.FOV);
            }
            if (pitch < -parseFloat(controls.FOV)) {
                pitch = -parseFloat(controls.FOV);
            }
            if (roll > parseFloat(controls.FOV)) {
                roll = parseFloat(controls.FOV);
            }
            if (roll < -parseFloat(controls.FOV)) {
                roll = -parseFloat(controls.FOV);
            }
            yawOptimized = yaw * parseFloat(controls.yawMultiplier);
            pitchOptimized = pitch * parseFloat(controls.pitchMultiplier);
            rollOptimized = roll * parseFloat(controls.rollMultiplier);

            if (window.modeTracker == "facetracker") {
                window.yaw = yawOptimized;
                window.pitch = pitchOptimized;
                window.roll = rollOptimized;
            }
        });
    }

    requestAnimationFrame(renderPrediction);
}

async function trackerMain() {
    var info = document.getElementById("info");
    info.innerHTML = "loading...";
    document.getElementById("main").style.display = "none";

    await tf.setBackend("webgl");

    await setupCamera();
    video.play();
    videoWidth = video.videoWidth;
    videoHeight = video.videoHeight;
    video.width = videoWidth;
    video.height = videoHeight;

    canvas = document.getElementById("output");
    canvas.width = videoWidth;
    canvas.height = videoHeight;
    const canvasContainer = document.querySelector(".canvas-wrapper");
    canvasContainer.style = `width: ${videoWidth}px; height: ${videoHeight}px`;

    ctx = canvas.getContext("2d");
    ctx.translate(canvas.width, 0);
    ctx.scale(-1, 1);
    ctx.fillStyle = "#32EEDB";
    ctx.strokeStyle = "#32EEDB";

    model = await facemesh.load({
        maxFaces: 1
    });
    renderPrediction();

    info.innerHTML = "";
    document.getElementById("main").style.display = "";
}

document.addEventListener('DOMContentLoaded', (event) => {
    setupDatGui();
    trackerMain();
})

// ------------------------
// audio part
let m1Decode = null;

let m1DecodeModule = Mach1DecodeModule();
m1DecodeModule.onInited = function() {
    m1Decode = new m1DecodeModule.Mach1Decode();
    m1Decode.setPlatformType(m1Decode.Mach1PlatformType.Mach1PlatformDefault);
    m1Decode.setDecodeAlgoType(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
    m1Decode.setFilterSpeed(0.9);
};

var audioFiles8 = ["audio/m1spatial/T1.ogg", "audio/m1spatial/T2.ogg", "audio/m1spatial/T3.ogg", "audio/m1spatial/T4.ogg", "audio/m1spatial/B5.ogg", "audio/m1spatial/B6.ogg", "audio/m1spatial/B7.ogg", "audio/m1spatial/B8.ogg"];

let sound = new Mach1SoundPlayer();
sound.setup(audioFiles8);

function Decode(yaw, pitch, roll) {
    if (m1Decode != null && yaw != null && pitch != null && roll != null) {
        m1Decode.setFilterSpeed(controls.filterSpeed);
        m1Decode.beginBuffer();
        let decoded = m1Decode.decode(yaw, pitch, roll);
        m1Decode.endBuffer();

        sound.updateVolumes(decoded);

        var strDebug = "";
        decoded.forEach(function(d) {
            strDebug += d.toFixed(2) + " , ";
        });
    }
}

function Play() {
    sound.play();
}

function Stop() {
    sound.stop();
}


// ------------------------
// OSC part
const osc = new OSC();
osc.open({
    port: 9898
});

// ------------------------
// render part adopted from https://threejs.org/examples/webgl_materials_normalmap.html
var container, stats, loader;
var camera, scene, renderer;
var mesh, pivot;
var directionalLight, pointLight, ambientLight;

var mouseX = 0;
var mouseY = 0;

var targetX = 0;
var targetY = 0;

var width = 640; //window.innerWidth;
var height = 480; //window.innerHeight;

var windowHalfX;
var windowHalfY;

var composer, effectFXAA;

var fYaw;
var fPitch;
var fRoll;

var yaw = 0;
var pitch = 0;
var roll = 0;

window.createOneEuroFilters = function createOneEuroFilters() {
    fYaw = OneEuroFilter(60, 1.0, window.controls.oneEuroFilterBeta, 1.0);
    fPitch = OneEuroFilter(60, 1.0, window.controls.oneEuroFilterBeta, 1.0);
    fRoll = OneEuroFilter(60, 1.0, window.controls.oneEuroFilterBeta, 1.0);
};

function init() {
    container = document.getElementById("3dview"); //document.createElement("div");
    //document.body.appendChild(container);

    camera = new THREE.PerspectiveCamera(27, width / height, 1, 10000);
    camera.position.z = 2500;

    scene = new THREE.Scene();
    scene.background = new THREE.Color(0x474747);

    // LIGHTS
    ambientLight = new THREE.AmbientLight(0x474747);
    scene.add(ambientLight);

    pointLight = new THREE.PointLight(0xffffff, 1.25, 1000);
    pointLight.position.set(0, 0, 600);

    scene.add(pointLight);

    directionalLight = new THREE.DirectionalLight(0xffffff);
    directionalLight.position.set(1, -0.5, -1);
    scene.add(directionalLight);

    var material = new THREE.MeshPhongMaterial({
        color: 0x191919,
        specular: 0x50505,
        shininess: 25,
        normalScale: new THREE.Vector2(0.8, 0.8),
    });

    loader = new THREE.GLTFLoader();
    loader.load("https://threejs.org/examples/models/gltf/LeePerrySmith/LeePerrySmith.glb", function(gltf) {
        createScene(gltf.scene.children[0].geometry, 100, material);
    });

    renderer = new THREE.WebGLRenderer();
    renderer.setSize(width, height);
    container.appendChild(renderer.domElement);

    //
    stats = new Stats();
    //container.appendChild(stats.dom);

    // COMPOSER
    renderer.autoClear = false;

    var renderModel = new THREE.RenderPass(scene, camera);
    composer = new THREE.EffectComposer(renderer);
    composer.addPass(renderModel);

    // EVENTS
    container.addEventListener("mousemove", onDocumentMouseMove, false);
    window.addEventListener("resize", onWindowResize, false);

    onWindowResize();
}

function createScene(geometry, scale, material) {
    mesh = new THREE.Mesh(geometry, material);

    mesh.position.y = 120;
    mesh.scale.x = mesh.scale.y = mesh.scale.z = scale;

    pivot = new THREE.Group();
    pivot.position.set(0.0, -150.0, 0);
    pivot.add(mesh);

    scene.add(pivot);
}

function onWindowResize() {
    windowHalfX = width / 2;
    windowHalfY = height / 2;

    camera.aspect = width / height;
    camera.updateProjectionMatrix();

    renderer.setSize(width, height);
    composer.setSize(width, height);
}

function onDocumentMouseMove(event) {
    var rect = event.target.getBoundingClientRect();
    mouseX = (event.clientX - rect.left) / width;
    mouseY = (event.clientY - rect.top) / height;
}

function animate() {
    requestAnimationFrame(animate);

    if (window.modeTracker == "touch") {
        window.yaw = map(mouseX, 0, 1, -90, 90);
        window.pitch = map(mouseY, 0, 1, 70, -70);
        window.roll = 0;
    }

    if (window.yaw != null) yaw = fYaw.filter(window.yaw);
    if (window.pitch != null) pitch = fPitch.filter(window.pitch);
    if (window.roll != null) roll = fRoll.filter(window.roll);

    render();
    stats.update();

    Decode(yaw, pitch, roll);

    if (osc.status() == OSC.STATUS.IS_OPEN) {
        osc.send(new OSC.Message("/orientation", yaw, pitch, roll));
    } else if (osc.status() == OSC.STATUS.IS_CLOSED) {
        osc.open({
            port: 9898
        });
    }
}

const map = (value, x1, y1, x2, y2) => ((value - x1) * (y2 - x2)) / (y1 - x1) + x2;

function render() {
    if (mesh) {
        pivot.rotation.y = Math.PI - THREE.Math.degToRad(yaw);
        pivot.rotation.x = THREE.Math.degToRad(pitch);
        pivot.rotation.z = -THREE.Math.degToRad(roll);
    }
    composer.render();
}

document.addEventListener('DOMContentLoaded', (event) => {
    createOneEuroFilters();
    init();
    animate();
})