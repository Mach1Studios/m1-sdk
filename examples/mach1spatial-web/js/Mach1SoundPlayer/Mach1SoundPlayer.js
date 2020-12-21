/*
 * Nach1 Spatial Web SoundPlayer Example
 * Description: Example of an audio player for Mach1Decode API and spatial audio playback
*/

/* eslint-disable new-cap, no-alert */

class Mach1SoundPlayer { // eslint-disable-line no-unused-vars
  #soundFilesCount = 0
  #soundFilesCountReady = 0

  #isDeleted = false
  #isFromBuffer = false
  #isPlaying = false
  #isSoundReady = false

  #buffer

  #volume = 1.0

  #gainNode
  #gains
  #pannerNode
  #smp

  #cache = {}
  audioContext = (window.AudioContext) ? new window.AudioContext() : new window.webkitAudioContext()

  #startTime = 0;
  #stopTime = 0;
  /**
   * Private method which should calculate and return time before start playing,
   * based on Audio Context
   * @type {Number}
   */
  #currentTime = () => {
    if (!this.isReady() || !this.#isPlaying) {
      return this.#stopTime - this.#startTime > 0 ? this.#stopTime - this.#startTime : 0;
    }
    return this.audioContext.currentTime - this.#startTime;
  }

  #needToPlay = false;
  #playLooped = false;
  #waitToPlay = 0;
  /**
   * Create new array with fixed item count, each item have zero value
   * @type {Array}
   */
  #initArray = (count = this.#soundFilesCount) => new Array(count).fill(0.0)
  /**
   * Set default time value for a gain nodes (for all buffered sound files)
   * @type {}
   */
  #setGains = () => {
    if (this.isReady() && this.#isPlaying) {
      for (let i = 0; i < this.#smp.length; i += 1) {
        this.#gainNode[i].gain.setTargetAtTime(this.#gains[i] * this.#volume, this.audioContext.currentTime, 0.05);
      }
    }
  }

  /**
   * Downloads, caches, and sets default properties for the received file
   * @type {}
   */
  #preload = async (uri, number) => {
    console.time(`load file ${uri}`);

    const getDecodedAudioData = (blob) => new Promise((resolve) => {
      if (this.#cache[uri]) {
        resolve(this.#cache[uri]);
      } else {
        this.audioContext.decodeAudioData(blob, (data) => {
          this.#cache[uri] = data;
          resolve(data);
        }, () => console.error('AudioContext issue'));
      }
    });
    const options = {
      cache: 'force-cache',
      method: 'GET',
      responseType: 'arrayBuffer',
    };

    try {
      const response = await fetch(uri, options);
      if (!response.ok) throw new Error(response.statusText);

      const blob = await response.arrayBuffer();
      const buffer = await getDecodedAudioData(blob);

      this.#buffer[number] = buffer;

      console.log(`[MACH1] Mach1SoundPlayer {path: ${uri}, i: ${number * 2}, ${number * 2 + 1}} loaded`);
      console.timeEnd(`load file ${uri}`);

      this.#soundFilesCountReady += 2;
      this.#isSoundReady = (this.#soundFilesCountReady === this.#soundFilesCount);
    } catch (e) {
      this.#isSoundReady = false;
      console.timeEnd(`doesn't load file ${uri}`);

      throw new Error(`Can't load sound files; Completed ${this.#soundFilesCountReady}/${this.#soundFilesCount}`);
      // NOTE: If need here can add some logs case or any requirement action
    }
  }

  /**
   * @param {Array|AudioBuffer} input array with sound files paths [url]
   */
  constructor(input) {
    if (Object.getPrototypeOf(input) === AudioBuffer.prototype) {
      this.#isFromBuffer = true;

      const buf = input;

      this.#soundFilesCount = buf.numberOfChannels * 2;

      this.#buffer = buf;

      this.#gainNode = this.#initArray(this.#soundFilesCount);
      this.#gains = this.#initArray(this.#soundFilesCount);
      this.#pannerNode = this.#initArray(this.#soundFilesCount);
      this.#smp = this.#initArray(this.#soundFilesCount);

      this.#isSoundReady = true;
    } else if (Array.isArray(input)) {
      this.#isFromBuffer = false;
      const audioFiles = input;

      this.#soundFilesCount = audioFiles.length * 2;

      this.#buffer = this.#initArray(audioFiles.length);

      this.#gainNode = this.#initArray(this.#soundFilesCount);
      this.#gains = this.#initArray(this.#soundFilesCount);
      this.#pannerNode = this.#initArray(this.#soundFilesCount);
      this.#smp = this.#initArray(this.#soundFilesCount);

      audioFiles.forEach(this.#preload);
    } else {
      console.error("Mach1SoundPlayer can't parse input!");
    }
  }

  /**
   * Return progress information in percent
   * @return {String} Percentages from 0 to 100 as a string [integer]
   */
  get progress() {
    return ((this.#soundFilesCountReady / this.#soundFilesCount) * 100).toFixed(0);
  }

  /**
   * Setting gains for all files
   * @param  {Array} vols binding new gain values by index
   */
  set gains(vols) {
    if (Array.isArray(vols)) {
      // FIXME: Need to refactor this part [switch to default array method like a forEach]
      for (let i = 0; i < vols.length; i += 1) {
        this.#gains[i] = vols[i];
      }
    }

    if (this.#isPlaying) {
      this.#setGains();
    }
  }

  /**
   * Getting gains for all files
   * @param  {Array} vols return last gain values by index
   */
  get gains() {
    return this.#gains;
  }

 /**
   * Setting Master Gain/Volume
   * @param  {Array} volume
   */
  set volume(vol) {
    this.#volume = parseFloat(vol);
  }
  
  /**
   * Return Master Gain/Volume
   * @return {String} Volume from 0 to 1 as a float
   */
  get volume() {
    return this.#volume;
  }

  /**
   * Start playing sound files
   */
  play(looped, time = this.#currentTime()) {
    if (this.isReady() && !this.#isPlaying && !this.#isDeleted) {
      if (this.isReady() && !this.#isPlaying) {
        for (let i = 0, j = 0; j < this.#soundFilesCount / 2; j += 1, i += 2) {
          this.#smp[i] = this.audioContext.createBufferSource();
          if (this.#isFromBuffer) {
            this.#smp[i].buffer = this.audioContext.createBuffer(
              1, this.#buffer.length / this.#buffer.numberOfChannels, this.audioContext.sampleRate
            );
            this.#smp[i].buffer.copyToChannel(this.#buffer.getChannelData(j), 0, 0);
          } else {
            this.#smp[i].buffer = this.#buffer[j];
          }

          this.#gainNode[i] = this.audioContext.createGain();
          this.#gainNode[i].gain.value = 0;

          /**
           * Create left side players of coeffs
           */
          this.#pannerNode[i] = this.audioContext.createPanner();
          this.#pannerNode[i].setPosition(-1, 0, 0); // left
          this.#pannerNode[i].panningModel = 'equalpower';

          this.#smp[i].connect(this.#pannerNode[i]);
          this.#pannerNode[i].connect(this.#gainNode[i]);
          this.#gainNode[i].connect(this.audioContext.destination);

          // RIGHT PLAYERS
          this.#smp[i + 1] = this.audioContext.createBufferSource();
          if (this.#isFromBuffer) {
            this.#smp[i + 1].buffer = this.audioContext.createBuffer(
              1, this.#buffer.length / this.#buffer.numberOfChannels, this.audioContext.sampleRate
            );
            this.#smp[i + 1].buffer.copyToChannel(this.#buffer.getChannelData(j), 0, 0);
          } else {
            this.#smp[i + 1].buffer = this.#buffer[j];
          }

          this.#gainNode[i + 1] = this.audioContext.createGain();
          this.#gainNode[i + 1].gain.value = 0;

          /**
           * Create right side players of coeffs
           */
          this.#pannerNode[i + 1] = this.audioContext.createPanner();
          this.#pannerNode[i + 1].setPosition(1, 0, 0); // right
          this.#pannerNode[i + 1].panningModel = 'equalpower';

          this.#smp[i + 1].connect(this.#pannerNode[i + 1]);
          this.#pannerNode[i + 1].connect(this.#gainNode[i + 1]);
          this.#gainNode[i + 1].connect(this.audioContext.destination);
        }

        for (let i = 0; i < this.#soundFilesCount; i += 1) {
          this.#smp[i].loop = looped;
          this.#smp[i].start(0, time);
        }

        this.#startTime = this.audioContext.currentTime - time;
        this.#isPlaying = true;
      }
      this.#setGains();
    } else {
      this.#needToPlay = true;
      this.#playLooped = looped;
      this.#waitToPlay = time;
    }
  }

  /**
   * Stopping play any sound file
   */
  stop() {
    if (this.isReady() && this.#isPlaying && !this.#isDeleted) {
      this.#isPlaying = false;
      this.#needToPlay = false;

      this.#stopTime = this.audioContext.currentTime;

      // FIXME: Need to change this part
      for (let i = 0; i < this.#smp.length; i += 1) {
        this.#smp[i].stop();

        if (typeof this.#smp[i].disconnect === 'function') this.#smp[i].disconnect();
      }
    }
  }

  /**
   * Alias for the this.stop() method
   */
  pause() {
    this.stop();
  }

  remove() {
    if (this.isReady()) this.#smp.forEach((smp) => smp.stop());
    this.#isDeleted = true;
  }

  rewind(time = 0) {
    this.stop();
    this.play(time >= 0 ? time : 0);
  }

  isReady() {
    return this.#isSoundReady && !this.#isDeleted;
  }

  isPlaying() {
    return this.#isPlaying;
  }

  getAudioContext() {
    return this.audioContext;
  }
}
