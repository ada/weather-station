var express = require('express');
var app = express();

require('./server/config/express')(app);
require('./server/config/mongo')(app);
require('./server/route/routes')(app);
//require('./server/module/cron').init();

app.listen(7777);
console.log("Server started. Environment: %s", process.env.NODE_ENV);
