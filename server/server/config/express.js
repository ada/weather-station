var express = require('express');
var bodyParser = require('body-parser');
var rootPath = require('path').normalize(__dirname + '/../..');


module.exports = function(app) {
    app.set('title', 'Weather Station 1')
    .use(express.static(rootPath + '/build'))
    .set('view engine', 'html')
    .set('views', rootPath + '/build')
    .engine('html', require('swig').renderFile)
    .use(bodyParser.urlencoded({extended : true}))
    .use(bodyParser.json())
    .set('view cache', false)
    .disable('etag');
};
