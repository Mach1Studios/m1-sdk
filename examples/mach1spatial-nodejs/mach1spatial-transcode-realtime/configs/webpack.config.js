const path = require('path');

module.exports = {
  mode: 'development',
  entry: './src/index.js',
  output: {
    path: path.resolve(__dirname, '../examples'),
    filename: 'main.bundle.js',
    publicPath: '/'
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env'],
            plugins: ['@babel/plugin-transform-runtime']
          }
        }
      }
    ]
  },
  experiments: {
    topLevelAwait: true
  },
  resolve: {
    extensions: ['.js'],
    alias: {
      '@': path.resolve(__dirname, '../src')
    },
    fallback: {
      "path": false,
      "fs": false,
      "os": false,
      "crypto": false,
      "stream": false,
      "util": false,
      "buffer": false,
      "process": false
    }
  },
  devtool: 'source-map',
  stats: {
    colors: true,
    chunks: false,
    modules: false,
    children: false,
    chunkModules: false
  }
};
