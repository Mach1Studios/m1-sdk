// webpack.config.js
const path = require('path');

module.exports = {
    entry: ["./Mach1SoundPlayer.js"],
    output: {
        path: path.resolve(__dirname, "./js"),
        filename: 'Mach1SoundPlayer.js',
        library: 'Mach1SoundPlayer',
    },
	mode: 'production',
    optimization: {
        minimize: true, // true | false
    },
    node: {
        fs: 'empty'
    },
    performance: {
        maxEntrypointSize: 2048000,
        maxAssetSize: 2048000
    },
};