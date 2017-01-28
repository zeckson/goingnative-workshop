const bindings = require('bindings');
const addon = bindings('myaddon');

addon.print(process.argv[2]);