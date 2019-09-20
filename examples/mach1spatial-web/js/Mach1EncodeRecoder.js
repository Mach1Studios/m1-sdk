function Mach1EncodeRecoder(m1SoundPlayer) {
    let thus = this;

	let gainsBuffer = new Array();
	let timeBuffer = new Array();
	let recording = false;

	let _player = m1SoundPlayer;

	this.startRecord = function() {
		gainsBuffer = new Array();
		timeBuffer = new Array();
		recording = true;
    };
	
	this.pushGains = function(gains) {
		if(recording && _player.isPlaying()) {
			gainsBuffer.push(gains);
			timeBuffer.push(_player.currentTime());
		}
    };
 
	this.stopRecord = function() {
		if(recording) {
			recording = false;
		
			// console.log(_player.getBuffer());

			var dataLength = _player.getBuffer()[0].getChannelData(0).length;
			for (let k = 1; k < _player.getBuffer().length; k++) {
				dataLength = Math.min(dataLength, _player.getBuffer()[k].getChannelData(0).length);
			}		
			
			var bufferWave = new Float32Array(dataLength * 8);
			var idx = 0;
			
			// write sound player buffers to final buffer and apply gains to it
			let position = 0
			for (let k = 0; k < dataLength; k++) {
				position = k;

				var t = (k / _player.getBuffer()[0].sampleRate);
				
				if(timeBuffer[idx]<t) {
					while(idx < timeBuffer.length && timeBuffer[idx]<t) {
						idx++;
					}
				}
				
				if(idx >= timeBuffer.length) break;
				
				for (let i = 0; i < _player.getBuffer().length; i++) {
					for (let j = 0; j < gainsBuffer[idx][i].length; j++) {
						bufferWave[k * 8 + j] += gainsBuffer[idx][i][j] * _player.getBuffer()[i].getChannelData(0)[k];
					}
				}	
			}
			
			bufferWave = bufferWave.slice(0, 8 * position);

			// write to file
			var wav = new Wav({
				sampleRate: _player.getBuffer()[0].sampleRate,
				channels: 8
			});

			wav.setBuffer(bufferWave);

			var srclist = [];
			while (!wav.eof()) {
				srclist.push(wav.getBuffer(1024));
			}

			var b = new Blob(srclist, {
				type: 'audio/wav'
			});
			var URLObject = window.webkitURL || window.URL;
			var url = URLObject.createObjectURL(b);
			return url;
		}
	};
	
    return this;
}
 