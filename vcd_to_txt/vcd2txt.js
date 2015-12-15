var vcd = require('vcd');
var fs = require('fs');

var unshift_char = {'0x0d': '9', '0x0e': '|', '0x15': 'q', '0x16': '1', '0x1a': 'z', '0x1b': 's', '0x1c': 'a', '0x1d': 'w', '0x1e': '2', '0x21': 'c', '0x22': 'x', '0x23': 'd', '0x24': 'e', '0x25': '4', '0x26': '3', '0x29': ' ', '0x2a': 'v', '0x2b': 'f', '0x2c': 't', '0x2d': 'r', '0x2e': '5', '0x31': 'n', '0x32': 'b', '0x33': 'h', '0x34': 'g', '0x35': 'y', '0x36': '6', '0x39': ',', '0x3a': 'm', '0x3b': 'j', '0x3c': 'u', '0x3d': '7', '0x3e': '8', '0x41': ',', '0x42': 'k', '0x43': 'i', '0x044': 'o', '0x45': '0', '0x46': '9', '0x49': '.', '0x4a': '-', '0x4b': 'l', '0x4c': 'ø', '0x4d': 'p', '0x4e': '+', '0x52': 'æ', '0x54': 'å', '0x55': '\\', '0x5a': '13', '0x5b': '"', '0x5d': '\'', '0x61': '<', '0x66': '8', '0x69': '1', '0x6b': '4', '0x6c': '7', '0x70': '0', '0x71': ',' };

var n = 0;
var str = '';
//var vcdname = '../serial_to_vcd/test.vcd';
var vcdname = './test.vcd';

fs.createReadStream(vcdname)
    .pipe(vcd.createStream({
  	rename: {
	    "1": 'front',
	    "0": 'data'
	},
	ignore: ["2", "3", "4", "5", "6", "7"]
    }))
    .on('begin', function (state) {
	console.log("State:");
	console.log(state);   
	// state contains date, variable definitions, etc. 
    })
    .on('sample', function (index, changes, last) {
	n = (n+1) % 11;
	str = (n == 0 || n == 9 || n == 10) ? str : str + last.data;

	if(n == 10){
	    var dat = parseInt(str, 2).toString(10);
	    var hexVal = parseInt(str, 2).toString(16);
	    var hexStr = "0x" + hexVal;
//	    console.log(hexStr);
	    if(dat != 240)
		console.log(unshift_char[hexStr]);
	    str = '';
	}

	// console.log("Sample:");
	// console.log(index, changes, last);
	// index = number of sample 
	// changes = hash of changed vars (by name) 
	// last = last state of all vars 
    })
