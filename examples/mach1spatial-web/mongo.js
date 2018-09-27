"use strict";

const mongoose = require('mongoose');
const debug = require('debug')('vr:mongo');

const url = process.env.DB_URL || 'mongodb://localhost:27017/vr';


let db = mongoose.connection;

db.on('error', (err) => {
	debug(err);

	throw new Error(err);
});

db.once('open', function() {
	debug('Connection to db open.');
});

mongoose.connect(url);

let schemaAnswer = {
	id: String,
	mail: String,
	ans: [{
		desc: String,
		value: String,
		num: Number
	}],
	count: {
		type: Number,
		default: 0
	}
};

let answer = mongoose.Schema(schemaAnswer, {collection: 'answers'});


answer.statics.save = function (id, mail, arrOfAns, cb) {
	let ans = new Answer({
		id: id,
		mail: mail,
		ans: arrOfAns
	});

	ans.saveEl(cb);
};

/**
 * select * where ans.count <= count
 * @param count number
 * @param cb    function(err, arr[{ans}])
 */
answer.statics.getAll = function (count, cb) {
	Answer.find({count: {$lt: count + 1}}).exec(function (err, arrOfAns) {
		if (err)
		{
			cb(err);
		}
		else
		{
			if (arrOfAns && arrOfAns.length > 0)
			{
				cb(null, arrOfAns);
			}
			else
			{
				cb(new Error("null"));
			}
		}
	})
};

answer.methods.saveEl = function (cb) {
	let id = this.id;
	let thus = this;

	Answer.find( { $or : [ { id : id }, {mail: thus.mail} ] })
		.select('id').exec().then(function (el) {
					if (typeof el === "undefined" || el.length == 0)
					{
						thus.count = 0;
					}
					else
					{
						thus.count = el.length;
					}

					thus.save().then(()=>{cb()}, cb);
			}).catch((err) => {
				cb(err);
			});
};

let Answer = mongoose.model('answer', answer);

module.exports.Answer = Answer;
