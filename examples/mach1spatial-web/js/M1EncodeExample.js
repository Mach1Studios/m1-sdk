document.addEventListener("DOMContentLoaded", function() {

	let m1Decode = null;
	Mach1DecodeModule().then(function(m1DecodeModule) {
		m1Decode = new(m1DecodeModule).Mach1Decode();
		m1Decode.setPlatformType(m1Decode.Mach1PlatformType.Mach1PlatformOfEasyCam);
		m1Decode.setDecodeAlgoType(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
		m1Decode.setFilterSpeed(0.95);
	});

    let m1Encode = null;
	Mach1EncodeModule().then(function(m1EncodeModule) {		
		m1Encode = new(m1EncodeModule).Mach1Encode();
	});
	
    const FRAMES_PER_SECOND = 60;

    var audioFiles;
    var mach1SoundPlayer;

    const gui = new dat.GUI();

    var params = {
        inputKind: 0, // mono
        outputKind: 1, // 8 ch
        rotation: 0,
        diverge: 0.5,
        pitch: 0,
        enableIsotropicEncode: true,

        sRotation: 0,
        sSpread: 0.5,
        autoOrbit: true,

        decoderRotationY: 0,
        decoderRotationP: 0,
        decoderRotationR: 0,
    };

	function loadSounds() {
    if (params.inputKind == 0) { // Input: MONO
			audioFiles = ['audio/mono/1.ogg'];
    }
    else if (params.inputKind == 1) { // Input: STERO
			audioFiles = ['audio/stereo/M1_SDKDemo_Orchestral_Stereo_L.ogg', 'audio/stereo/M1_SDKDemo_Orchestral_Stereo_R.ogg'];
    }
    else if (params.inputKind == 2) {
      audioFiles = ['audio/quad/guitar-m1horizon.ogg'];
      mach1AudioLoader = new Mach1AudioLoader(audioFiles, 4);
    }
		else {
			audioFiles = ['audio/mono/1.ogg'];
		}
		
		if(mach1SoundPlayer) {
			mach1SoundPlayer.remove();
		}
		
		mach1SoundPlayer = new Mach1SoundPlayer();
		mach1SoundPlayer.setup(audioFiles);
 	};

    // three js
    function createTextLabel() {
        var div = document.createElement('div');
        div.className = 'text-label';
        div.style.position = 'absolute';
        div.style.width = 100;
        div.style.height = 100;
        div.innerHTML = "";
        div.style.top = -1000;
        div.style.left = -1000;
        div.style.color = "#ffffff";

        return {
            element: div,
            parent: false,
            position: new THREE.Vector3(0, 0, 0),
            setHTML: function(html) {
                this.element.innerHTML = html;
            },
            setParent: function(threejsobj) {
                this.parent = threejsobj;
            },
            updatePosition: function() {
                if (parent) {
                    this.position.copy(this.parent.position);
                }

                var coords2d = this.get2DCoords(this.position, camera);
                this.element.style.left = coords2d.x + 'px';
                this.element.style.top = coords2d.y + 'px';
            },
            get2DCoords: function(position, camera) {
                var vector = position.project(camera);
                vector.x = (vector.x + 1) / 2 * window.innerWidth;
                vector.y = -(vector.y - 1) / 2 * window.innerHeight;
                return vector;
            }
        };
    }

    var textlabels = [];
    var spheres = [];
    var lines = [];
    var container = document.getElementById('container');

    var scene = new THREE.Scene();
    var camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
    var renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    //document.body.appendChild(renderer.domElement);
    container.appendChild(renderer.domElement);

    var controls = new THREE.OrbitControls(camera, renderer.domElement);

    camera.position.set(0, 2, 2.5);
    camera.lookAt(scene.position);

    var size = 10;
    var divisions = 10;
    var gridHelper = new THREE.GridHelper(size, divisions);
    scene.add(gridHelper);


    var sphereGeometry = new THREE.SphereGeometry(0.1, 16, 16);
    var sphereMaterial = new THREE.MeshBasicMaterial({
        color: 0xff0000
    });

    var lineMaterial = new THREE.LineBasicMaterial({
        color: 0xaaaaaa
    });


    var dir = new THREE.Vector3(1, 0, 0);
    var origin = new THREE.Vector3(0, 0, 0);
    var length = 1;
    var hex = 0xff0000;
    var arrowHelper = new THREE.ArrowHelper(dir.normalize(), origin, length, hex);
    scene.add(arrowHelper);

    var geo = new THREE.Geometry()
    geo.vertices.push( new THREE.Vector3(0, 0, 0) )
    geo.vertices.push( new THREE.Vector3(0, 0.5, 0) )
	var arrowLineHelper = new THREE.Line(geo, new THREE.LineBasicMaterial({ color: hex }));
    scene.add(arrowLineHelper);

	
    function onWindowResize() {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
        renderer.setSize(window.innerWidth, window.innerHeight);
    }
    window.addEventListener('resize', onWindowResize, false);


    function animate() {
        requestAnimationFrame(animate);

        //sphere.position.x += 0.001; 
        controls.update(); // required if controls.enableDamping or controls.autoRotate are set to true
        for (var i = 0; i < textlabels.length; i++) {
            textlabels[i].updatePosition();
        }

        renderer.render(scene, camera);
    }
    animate();


	loadSounds();

    // gui
    var elementSRotation;
    var elementSSpread;
    var elementAutoOrbit;

    function toggleInputOutputKind() {
        if (params.inputKind == 0) { // Input: MONO
            m1Encode.setInputMode(m1Encode.Mach1EncodeInputModeType.Mach1EncodeInputModeMono);
        }
        if (params.inputKind == 1) { // Input: STERO
            m1Encode.setInputMode(m1Encode.Mach1EncodeInputModeType.Mach1EncodeInputModeStereo);
        }
        if (params.inputKind == 2) { // Input: Quad
            m1Encode.setInputMode(m1Encode.Mach1EncodeInputModeType.Mach1EncodeInputModeQuad);
        }
        if (params.inputKind == 3) { // Input: AFORMAT
            m1Encode.setInputMode(m1Encode.Mach1EncodeInputModeType.Mach1EncodeInputModeAFormat);
        }
        if (params.inputKind == 4) { // Input: 1OA ACNSN3D
            m1Encode.setInputMode(m1Encode.Mach1EncodeInputModeType.Mach1EncodeInputModeBFOAACN);
        }
        if (params.inputKind == 5) { // Input: 1OA FuMa
            m1Encode.setInputMode(m1Encode.Mach1EncodeInputModeType.Mach1EncodeInputModeBFOAFUMA);
        }

        if (params.outputKind == 0) { // Output: Mach1Horizon / Quad 4CH
            m1Encode.setOutputMode(m1Encode.Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Horizon);
        }
        if (params.outputKind == 1) { // Output: Mach1Spatial / Cuboid 8CH
            m1Encode.setOutputMode(m1Encode.Mach1EncodeOutputModeType.Mach1EncodeOutputModeM1Spatial);
        }

        // Resets the Decoding input when changing Encoding output between Mach1Spatial and Mach1Horizon
        if (params.outputKind == 0) { // Output: Mach1Horizon / Quad
            m1Decode.setDecodeAlgoType(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoHorizon);
        }
        if (params.outputKind == 1) { // Output: Mach1Spatial / Cuboid
            m1Decode.setDecodeAlgoType(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial);
        }

        var inputKindStereo = (params.inputKind == 1); // stereo

        elementSRotation.hidden = !inputKindStereo;
        elementSSpread.hidden = !inputKindStereo;
        elementAutoOrbit.hidden = !inputKindStereo;

        m1Encode.generatePointResults();

        var pointsNames = m1Encode.getPointsNames();
        var points = m1Encode.getPoints();

        // cleanup
        for (var i = 0; i < spheres.length; i++) {
            scene.remove(spheres[i]);
            scene.remove(lines[i]);
        }
        spheres = [];
        lines = [];

        for (var i = 0; i < textlabels.length; i++) {
            textlabels[i].element.parentNode.removeChild(textlabels[i].element);
        }
        textlabels = [];

        // add elements
        for (var i = 0; i < m1Encode.getPointsCount(); i++) {
            // sphere 
            var sphere = new THREE.Mesh(sphereGeometry, sphereMaterial);
            sphere.position.set(points[i].x * 2 - 1, points[i].y * 2 - 1, points[i].z * 2 - 1);
            spheres.push(sphere);
            scene.add(sphere);


            // line
            var geometry = new THREE.BufferGeometry(); // geometry
            var pointsCount = 3;
            var positions = new Float32Array(pointsCount * 3); // 3 vertices per point
            geometry.addAttribute('position', new THREE.BufferAttribute(positions, 3)); // attributes
            geometry.setDrawRange(0, pointsCount);

            var line = new THREE.Line(geometry, lineMaterial);
            lines.push(line);
            scene.add(line);

            // ui text
            var text = createTextLabel();
            text.setHTML(pointsNames[i]);
            text.setParent(sphere);
            textlabels.push(text);
            container.appendChild(text.element);
        }
    }

    var folder = gui.addFolder("Encoder settings");
    folder.open();
    folder.add(params, 'inputKind', {
        'MONO': 0,
        'STEREO': 1,
        'QUAD': 2,
        'AFORMAT': 3,
        '1OA:ACNSN3D': 4,
        '1OA:FUMA': 5
    }).name('Input type').onChange(function() {
        update();
		loadSounds();
        toggleInputOutputKind();
    });
    folder.add(params, 'outputKind', {
        "Mach1Horizon/4CH": 0,
        "Mach1Spatial/8CH": 1
    }).name('Output type').onChange(function() {
        update();
        toggleInputOutputKind();
    });

    folder.add(params, 'rotation', 0, 1, 0.01).name('Rotation').onChange(update);
    folder.add(params, 'diverge', -0.707, 0.707, 0.01).name('Diverge').onChange(update);
    folder.add(params, 'pitch', -1, 1, 0.01).name('Pitch').onChange(update);
    folder.add(params, 'enableIsotropicEncode').name('Isotropic encode').onChange(update);

    elementSRotation = folder.add(params, 'sRotation', -180, 180, 1).name('S Rotation').onChange(update).__li;
    elementSSpread = folder.add(params, 'sSpread', 0, 1, 0.01).name('S Spread').onChange(update).__li;
    elementAutoOrbit = folder.add(params, 'autoOrbit').name('Auto orbit').onChange(update).__li;

    folder = gui.addFolder('Decoder settings');
    folder.open();
    folder.add(params, 'decoderRotationY', 0, 360, 1).name('Decoder Yaw');
    folder.add(params, 'decoderRotationP', -90, 90, 1).name('Decoder Pitch');
    folder.add(params, 'decoderRotationR', -180, 180, 1).name('Decoder Roll');

    // update 
    function update() {
        m1Encode.setAzimuth(params.rotation);
        m1Encode.setElevation(params.pitch);
        m1Encode.setDiverge(params.diverge);
        m1Encode.setStereoRotate(params.sRotation);
        m1Encode.setStereoSpread(params.sSpread);
        m1Encode.setAutoOrbit(params.autoOrbit);
        m1Encode.setIsotropicEncode(params.enableIsotropicEncode);
    }

	function __playTimeout() {
        if (m1Decode && m1Encode && mach1SoundPlayer && mach1SoundPlayer.isReady() ) {
			update();
			toggleInputOutputKind();
			
            mach1SoundPlayer.play(true);
        } 
	}
	
	function __timeoutUpdateVolumes() {
        if (mach1SoundPlayer && mach1SoundPlayer.isPlaying()) {

            m1Encode.generatePointResults();

			/*
            var rotation = new THREE.Euler().setFromQuaternion(camera.quaternion);
            rotation.x = Math.fround(THREE.Math.radToDeg(rotation.x));
            rotation.y = Math.fround(THREE.Math.radToDeg(rotation.y));
            rotation.z = Math.fround(THREE.Math.radToDeg(rotation.z));
			*/
			
            m1Decode.beginBuffer();
            var decoded = m1Decode.decode(params.decoderRotationY, params.decoderRotationP, params.decoderRotationR);// rotation.x, rotation.y, rotation.z);
            m1Decode.endBuffer();

			/* 
			// legacy style
			
			var vol = [0, 0];
            var gains = m1Encode.getGains();

            // left & right channels
            for (let j = 0; j < 8; j++) {
                vol[0] += (decoded[2 * j + 0]) * gains[0][j];
                vol[1] += (decoded[2 * j + 1]) * gains[gains.length > 1 ? 1 : 0][j];
            }
			
			console.log(vol);
			*/
			
			var vol = [];
			if (params.outputKind == 0) { // Output: Mach1Horizon / Quad
				vol = m1Encode.getResultingVolumesDecoded(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoHorizon, decoded);
			}
			if (params.outputKind == 1) { // Output: Mach1Spatial / Cuboid
				vol = m1Encode.getResultingVolumesDecoded(m1Decode.Mach1DecodeAlgoType.Mach1DecodeAlgoSpatial, decoded);
			}
			//console.log(vol);

            var points = m1Encode.getPoints();
            var pointsNames = m1Encode.getPointsNames();

            for (var i = 0; i < m1Encode.getPointsCount(); i++) {
                spheres[i].position.set(points[i].x * 2 - 1, points[i].y * 2 - 1, points[i].z * 2 - 1);

                var positions = lines[i].geometry.attributes.position.array;

				var idx = 0;
                positions[idx++] = 0;
                positions[idx++] = 0;
                positions[idx++] = 0;

                positions[idx++] = spheres[i].position.x;
                positions[idx++] = 0;
                positions[idx++] = spheres[i].position.z;

                positions[idx++] = spheres[i].position.x;
                positions[idx++] = spheres[i].position.y;
                positions[idx++] = spheres[i].position.z;

                lines[i].geometry.attributes.position.needsUpdate = true;
            }

            mach1SoundPlayer.updateVolumes(vol);

            var angle = m1Decode.getCurrentAngle();
 
			arrowHelper.rotation.set(0,0,0);
            arrowHelper.rotation.y = THREE.Math.degToRad(angle.x) * -1; // yaw
	        arrowHelper.rotation.z = THREE.Math.degToRad(angle.y) * -1 - Math.PI / 2; // pitch
	
			// rotate
			var vec = new THREE.Euler();
			vec.x = 0;
			vec.y = THREE.Math.degToRad(angle.x) * -1; // yaw
			vec.z = THREE.Math.degToRad(angle.y) * -1; // pitch
			var rotationMatrixOrig = new THREE.Matrix4();
			rotationMatrixOrig.makeRotationFromEuler(vec);
				
			var rotationMatrixRoll = new THREE.Matrix4();
			rotationMatrixRoll.makeRotationAxis( new THREE.Vector3(1, 0, 0), THREE.Math.degToRad(angle.z) * -1 );
			rotationMatrixOrig.multiply(rotationMatrixRoll); // post multiply
			
			arrowLineHelper.matrix = rotationMatrixOrig;
			arrowLineHelper.rotation.setFromRotationMatrix( arrowLineHelper.matrix );
        }
	}
	
    setInterval(__playTimeout, 100);
    setInterval(__timeoutUpdateVolumes, 1000 / FRAMES_PER_SECOND);
});