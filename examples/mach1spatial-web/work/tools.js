"use strict";


function fromData(obj) {
	/*
	let data = new FormData();
	data.append("json", JSON.stringify(obj));
	*/
	return JSON.stringify(obj);
}

function checkResponse(response) {
	// return response.json();

	if (response.status >= 400)
	{
		throw new Error('Response.status = ' + response.status);
	}

	return response;
}


function delay(cb, time)
{
	let timerId = setTimeout(function () {
		cb();

		stop();
	}, time);

	function stop() {
		clearTimeout(timerId);

		cb = () => {};
	}

	function now() {
		cb();

		stop();
	}

	return {
		stop: stop,
		now: now
	};
}

delay.ZERO = delay(()=>{}, 0);

module.exports.fromData = fromData;
module.exports.checkResponse = checkResponse;
module.exports.delay = delay;
