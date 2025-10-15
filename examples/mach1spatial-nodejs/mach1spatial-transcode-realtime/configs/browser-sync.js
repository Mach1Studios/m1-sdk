const browserSync = require('browser-sync');
const webpack = require('webpack');
const webpackDevMiddleware = require('webpack-dev-middleware');
const portscanner = require('portscanner');
const path = require('path');

const webpackConfig = require('./webpack.config.js');

const DEFAULT_PORT = 3000;
const DEFAULT_UI_PORT = 3001;

function findAvailablePort(startPort, maxPort = startPort + 100) {
  return new Promise((resolve, reject) => {
    portscanner.findAPortNotInUse(startPort, maxPort, '127.0.0.1', (error, port) => {
      if (error) {
        reject(error);
      } else {
        resolve(port);
      }
    });
  });
}

async function startServer() {
  try {
    const port = await findAvailablePort(DEFAULT_PORT);
    const uiPort = await findAvailablePort(DEFAULT_UI_PORT);

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

    browserSync({
      port: port,
      ui: {
        port: uiPort
      },
      server: {
        baseDir: path.join(__dirname, '../examples'),
        middleware: [devMiddleware],
        index: 'index.html'
      },
      files: [
        'examples/**/*.html',
        'examples/**/*.css',
        'examples/**/*.js'
      ],
      open: true,
      notify: false,
      logLevel: 'info'
    });

    console.log(`Server started at http://localhost:${port}`);
    console.log(`UI available at http://localhost:${uiPort}`);

  } catch (error) {
    console.error('Failed to start server:', error);
    process.exit(1);
  }
}

startServer();