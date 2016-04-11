var mongo = require('mongoose');

module.exports = function(app) {
	mongo.connect('mongodb://u1:p1@ds013250.mlab.com:13250/w1');
	mongo.connection.on('error', console.error.bind(console, 'Mongo connection error:'));
	global.mongo = mongo;
};
