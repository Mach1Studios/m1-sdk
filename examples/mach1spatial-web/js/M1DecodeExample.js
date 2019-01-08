document.addEventListener("DOMContentLoaded", function() {
    var audioFiles = [
        'audio/m1spatial/T1.mp3',
        'audio/m1spatial/T2.mp3',
        'audio/m1spatial/T3.mp3',
        'audio/m1spatial/T4.mp3',
        'audio/m1spatial/B5.mp3',
        'audio/m1spatial/B6.mp3',
        'audio/m1spatial/B7.mp3',
        'audio/m1spatial/B8.mp3'
    ];

    var elemId = 'videojs-panorama-player';
    var statusElId = 'status';
    var mach1VideoPlayer = new Mach1VideoPlayer(audioFiles, elemId, statusElId);
});