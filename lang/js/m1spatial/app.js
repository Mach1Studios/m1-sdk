"use strict";
let express = require('express');
let path = require('path');

let debug = require('debug')('sound:app');

let app = express();



// static
app.use(express.static(__dirname + '/public'));




module.exports.app = app;