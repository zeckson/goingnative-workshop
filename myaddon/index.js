const bindings = require('bindings');
const addon = bindings('myaddon');

const interval = setInterval(function () {
  process.stdout.write('.')
}, 50);

addon.delay(process.argv[2], function () {
  clearInterval(interval);
  console.log('Done!');
});

process.stdout.write('Waiting');
