'use strict';

let path = require('path');
let webpack = require('webpack');

module.exports = {
    context: path.join(__dirname, 'work'),
    entry: {
        main8: './mainJs8',
        main16: './mainJs16'
    },
    output: {
        path: path.join(__dirname, 'public', 'js'),
        filename: '[name].js',
        library: "__DEBUG__" // TODO: delete this
    },
    externals: {
        videojs: "videojs",
        THREE: "THREE",
        jQuery: "$"
    },
    module: {
        loaders: [{
            test: /\.js$/,
            include: [
                path.resolve(__dirname, 'public', 'js')
            ],
            exclude: path.resolve(__dirname, '/node_modules/'),
            loader: 'babel?presets[]=es2015'
        }]
    },
    devtool: "source-map"
};