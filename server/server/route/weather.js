require('./../model/weather.js');

var express = require("express");
var app = module.exports = express();
var Weather = mongo.model('Weather');

app.get('/', function(req, res) {
    res.status(200).send("Hello world");
});
