var express = require('express');
var app = express();
var config = require('./server/config/config');

require('./server/config/express')(app);
require('./server/config/mongo')(app);
require('./server/route/routes')(app);
require('./server/module/cron').init();

app.listen(config.port);
console.log("Server started on port %d. Environment: %s", config.port, process.env.NODE_ENV);
