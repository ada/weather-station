var mongo = require('mongoose');
var config = require('./config');

module.exports = function(app) {
	mongo.connect(config.mongo);
	mongo.connection.on('error', console.error.bind(console, 'Mongo connection error:'));
	global.mongo = mongo;
};
