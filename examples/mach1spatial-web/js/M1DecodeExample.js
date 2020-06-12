document.addEventListener("DOMContentLoaded", function() {
    var audioFiles = [
        'audio/m1spatial/T1.ogg',
        'audio/m1spatial/T2.ogg',
        'audio/m1spatial/T3.ogg',
        'audio/m1spatial/T4.ogg',
        'audio/m1spatial/B5.ogg',
        'audio/m1spatial/B6.ogg',
        'audio/m1spatial/B7.ogg',
        'audio/m1spatial/B8.ogg'
    ];

    var elemId = 'videojs-panorama-player';
    var statusElId = 'status';
    var mach1VideoPlayer = new Mach1VideoPlayer(audioFiles, elemId, statusElId);
});