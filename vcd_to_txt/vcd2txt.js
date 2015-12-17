var vcd = require('vcd');
var fs = require('fs');

var unshift_char = {'0x0d': '9', '0x0e': '|', '0x15': 'a', '0x16': '&', '0x1a': 'w', '0x1b': 's', '0x1c': 'q', '0x1d': 'z', '0x1e': 'é', '0x21': 'c', '0x22': 'x', '0x23': 'd', '0x24': 'e', '0x25': '\'', '0x26': '"', '0x29': ' ', '0x2a': 'v', '0x2b': 'f', '0x2c': 't', '0x2d': 'r', '0x2e': '(', '0x31': 'n', '0x32': 'b', '0x33': 'h', '0x34': 'g', '0x35': 'y', '0x36': '-', '0x39': 'm', '0x3a': ';', '0x3b': 'j', '0x3c': 'u', '0x3d': 'è', '0x3e': '_', '0x41': ';', '0x42': 'k', '0x43': 'i', '0x44': 'o', '0x45': 'à', '0x46': 'ç', '0x49': ':', '0x4a': '!', '0x4b': 'l', '0x4c': 'm', '0x4d': 'p', '0x4e': ')', '0x52': 'æ', '0x54': 'h', '0x55': '=', '0x5a': '\n', '0x5b': '"', '0x5d': 'ù', '0x61': '<', '0x66': '8', '0x69': '1', '0x6b': '4', '0x6c': '7', '0x70': '0', '0x71': ';', '0x12' : 'MAJ', '0xe': '²', '0x72': '2', '0x7a': '3', '0x73': '5', '0x74': '6', '0x75': '8', '0x7d': '9', '0xd': '\t'};

var unwanted_char = [];

var args = process.argv.slice(2);
var n = 0;
var str = '';

if(args.length != 1) {
    console.log("usage: nodejs vcd2txt.js file.vcd");
    process.exit(1);
}

var stream = fs.createReadStream(args[0]);

function remove(c) {
    var index = unwanted_char.indexOf(c);
    if (index > -1) {
	unwanted_char.splice(index, 1);
    }
}

stream.pipe(vcd.createStream({
    rename: {
	"clock": 'front',
	"data": 'data'
    },
    ignore: ["2", "3", "4", "5", "6", "7"]
}))
    .on('sample', function (index, changes, last) {
	// if last.front equal 1 then the clock is doing a new cycle so thats okay
	if(last.front == 1) {
	    str = (n == 0 || n == 9 || n == 10) ? str : last.data + str;
	    if(n == 10){
		var dat = parseInt(str, 2).toString(10);
		var hexVal = parseInt(str, 2).toString(16);
		var hexStr = "0x" + hexVal;
		
		// 240 is the end character, we throw this modafockeur out
		if(hexStr != 0xF0) {
		    if(unshift_char[hexStr] != undefined) {
			var c = unshift_char[hexStr];
			if(unwanted_char.indexOf('MAJ') > -1) {
			    c = c.toUpperCase();
			}
			if(unwanted_char.indexOf(c) > -1)
			    remove(c);
			else {
			    if(c != 'MAJ')
				process.stdout.write(c);
			    unwanted_char.push(c);
			}
		    } else
			console.log("\nRahlala encore un charactère pas pris en compte: " + hexStr);
		}		
		// a new journey to create a new char begin
		str = '';
	    }
	    n = (n+1) % 11;
	}
    })

stream.on('end', function() {
    console.log("");
});
