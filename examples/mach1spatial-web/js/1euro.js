/**
 * Author: Florian Renaut (florian.renaut@gmail.com)
 * Details: http://www.lifl.fr/~casiez/1euro
 */

function OneEuroFilter(freq, mincutoff, beta, dcutoff){
	var that = {};
	var x = LowPassFilter(alpha(mincutoff));
	var dx = LowPassFilter(alpha(dcutoff));
	var lastTime = undefined;
	
	mincutoff = mincutoff || 1;
	beta = beta || 0;
	dcutoff = dcutoff || 1;
	
	function alpha(cutoff){
		var te = 1 / freq;
		var tau = 1 / (2 * Math.PI * cutoff);
		return 1 / (1 + tau / te);
	}
	
	that.filter = function(v, timestamp){
		if(lastTime !== undefined && timestamp !== undefined)
			freq = 1 / (timestamp - lastTime);
		lastTime = timestamp;
		var dvalue = x.hasLastRawValue() ? (v - x.lastRawValue()) * freq : 0;
		var edvalue = dx.filterWithAlpha(dvalue, alpha(dcutoff));
		var cutoff = mincutoff + beta * Math.abs(edvalue);
		return x.filterWithAlpha(v, alpha(cutoff));
	}
	
	return that;
}

function LowPassFilter(alpha, initval){
	var that = {};
	var y = initval || 0;
	var s = y;
	
	function lowpass(v){
		y = v;
		s = alpha * v + (1 - alpha) * s;
		return s;
	}
	
	that.filter = function(v){
		y = v;
		s = v;
		that.filter = lowpass;
		return s;
	}
	
	that.filterWithAlpha = function(v, a){
		alpha = a;
		return that.filter(v);
	}
	
	that.hasLastRawValue = function(){
		return that.filter === lowpass;
	}
	
	that.lastRawValue = function(){
		return y;
	}
	
	return that;
}
