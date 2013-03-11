/*jshint undef:true node:true strict:false*/
var fs = require('fs');

var Crc32 = function() {
  this.table = [];
  this.value = null;

  var v;

  for(var i = 0; i < 256; ++i) {
    v = i;
    for(var j = 0; j < 8; ++j) {
      v = (v & 1) ? (0xedb88320 ^ (v >>> 1)) : (v >>> 1);
    }
    this.table[i] = v | 0;
  }
};

Crc32.prototype.start = function() {
  this.value = 0xffffffff;
};

Crc32.prototype.update = function(text) {
  for(var i = 0; i < text.length; ++i) {
    var c = text.charCodeAt(i);
    this.value = this.table[(this.value ^ c) & 0xFF] ^ (this.value >>> 8);
  }
};

Crc32.prototype.finalize = function() {
  return (this.value ^ 0xffffffff) >>> 0;
};

for(var i = 2; i < process.argv.length; ++i) {

  (function(filename) {
    var crc    = new Crc32();
    var stream = fs.createReadStream(filename, {
      encoding:   'binary',
      flags:      'r',
      bufferSize: 1024 * 1024
    }).addListener('data', function(chunk) {
      crc.update(chunk);
    }).addListener('open', function() {
      crc.start();
    }).addListener('close', function() {
      console.log(filename, crc.finalize().toString(16));
    });
  })(process.argv[i]);

}
