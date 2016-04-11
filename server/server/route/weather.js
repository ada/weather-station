require('./../model/weather.js');

var express = require("express");
var app = module.exports = express();
var Weather = mongo.model('Weather');

app.get('/', function(req, res) {
  Weather.find().sort({'created':'-1'})
  .exec(function(err, r) {
        if (err || !r || r.length == 0) {
          res.sendStatus(404);
        } else {
            res.status(200).send(r);
        }
    });
});

app.get('/post', function(req, res) {
  var w = new Weather();
  w.temperature = Math.random();
  w.save(function(err, r) {
    if (err)
      res.sendStatus(400);
    else
      res.status(200).send(r);
  });
});
