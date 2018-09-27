'use strict';
let path = require('path');
let webpack = require('webpack');
let ExtractTextPlugin = require("extract-text-webpack-plugin");


module.exports = {
	context: path.join(__dirname, 'work'),
	entry: {
		_main: './_mainJs.js',
		main: './mainJs.js',
		stat: './stat.js'
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
				path.resolve(__dirname, 'work')
			],
			exclude: path.resolve(__dirname, '/node_modules/'),
			loader: 'babel-loader',
			query: {
				presets: [
					'es2015'
				]
			}
		}, {
			test: /\.jsx$/,
			include: [
				path.resolve(__dirname, 'work', 'react')
			],
			exclude: path.resolve(__dirname, 'node_modules'),
			loader: 'babel-loader?presets[]=react,es2015',
			query: {
				presets: [
					'react',
					'es2015'
				]
			}
		}, {
			test: /\.less$/,
            include: [
                path.resolve(__dirname, 'work', 'css')
            ],
            exclude: path.resolve(__dirname, 'node_modules'),
            loader: 'style-loader!css-loader!less-loader'
		}, {
			test: /\.css$/,
            include: [
                path.resolve(__dirname, 'work', 'css')
            ],
            exclude: path.resolve(__dirname, 'node_modules'),
            loader: 'style-loader!css-loader'
		}, {
			test: /\.png$/,
			include: [
				path.resolve(__dirname, 'work', 'css')
			],
			exclude: path.resolve(__dirname, 'node_modules'),
			loader: 'url-loader?limit=8192'
		}]
	},
	devtool: "source-map"
};