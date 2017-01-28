const bindings = require('bindings');
const addon = bindings('myaddon');

console.log(addon.length(process.argv[2]));