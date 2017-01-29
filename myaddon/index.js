const bindings = require('bindings');
const addon = bindings('myaddon');

addon.delay(parseInt(process.argv[2], 10), function () {
  console.log('Done!');
});