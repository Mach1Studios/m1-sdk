const webpack = require('webpack');
const webpackDevMiddleware = require('webpack-dev-middleware');
const webpackHotMiddleware = require('webpack-hot-middleware');
const path = require('path');

const webpackConfig = require('../webpack.config.js');

function createBundler() {
  const compiler = webpack(webpackConfig);
  
  const devMiddleware = webpackDevMiddleware(compiler, {
    publicPath: webpackConfig.output.publicPath,
    stats: {
      colors: true,
      chunks: false,
      modules: false,
      children: false,
      chunkModules: false
    }
  });

  const hotMiddleware = webpackHotMiddleware(compiler, {
    log: false,
    heartbeat: 2000
  });

  return {
    devMiddleware,
    hotMiddleware
  };
}

module.exports = createBundler;