"use strict";
let express = require('express');
let path = require('path');
let cookieParser = require('cookie-parser');

let uuidV1 = require('uuid/v1');

let debug = require('debug')('vr:app');


let app = express();


app.use(cookieParser());
app.use(require('body-parser').json());


// static
app.use(express.static(__dirname + '/public'));


app.all('*', function (req, res, next) {
	if (req.cookies.id)
	{
		next();
	}
	else
	{
		req.cookies.id = uuidV1();

		res.cookie('id', req.cookies.id);

		tmpAns.init(req.cookies.id);
		
		next();
	}
});

const Answer = require('./mongo').Answer;

function _tmpAns() {
	let arr = Object.create(null);
	let tma = Object.create(null);

	const TM_ANS = 1000 * 60 * 60 * 4; /* 4 hours */

	let fnc = function (id) {
		if (typeof arr[id] === 'undefined')
		{
			fnc.init(id);
		}

		return {
			push: function (ans) {
				arr[id].push(ans);

				fnc.update(id);
			}
		};
	};

	fnc.init = function (id) {
		arr[id] = [];

		fnc.update(id);
	};

	fnc.update = function (id) {
		clearTimeout(tma[id]);

		tma[id] = setTimeout(fnc.end.bind(this), TM_ANS);
	};

	fnc.get = function (id) {
		return arr[id];
	};

	fnc.end = function (id) {
		clearTimeout(tma[id]);

		arr[id] = [];
	};
	
	return fnc;
}

let tmpAns = _tmpAns();

app.all('/save/*', function (req, res, next) {
	debug('Request: {id: ' + req.cookies.id + ', path: ' + req.path + '}');

	next();
});

app.post('/save/tmp', function (req, res) {
	let id = req.cookies.id;

	let ans = req.body;

	tmpAns(id).push(ans);

	res.end();
});

app.post('/save/end', function (req, res) {
	let id = req.cookies.id;

	let mail = req.body.mail;

	Answer.save(id, mail, tmpAns.get(id), function (err) {
		if (err)
		{
			debug(err);

			res.status(502).end();
		}
		else
		{
			tmpAns.end(id);

			res.end();
		}
	});
});

app.all('/login/' + process.env.LOGIN_KEY, function (req, res) {
	res.cookie('secret', process.env.ACCESS_SECRET);

	res.redirect('/stat.html');
	res.end();
});

app.all(['/get/*', '/stat.html'], function (req, res, next) {
	if (req.cookies.secret === process.env.ACCESS_SECRET && req.cookies.secret.length > 10)
	{
		debug('Request: {id: ' + req.cookies.id + ', path: ' + req.path + '}');

		next();
	}
	else
	{
		res.redirect('/');

		res.end();
	}
});

app.get('/get/all/count/:count', function (req, res) {
	Answer.getAll(+req.params.count, function (err, arrOfAns) {
		if (err)
		{
			res.status(502).end();
		}
		else
		{
			res.json(arrOfAns).end();
		}
	});
});


module.exports.app = app;