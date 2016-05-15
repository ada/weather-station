require('./../model/weather.js');

var express = require("express");
var app = module.exports = express();
var Weather = mongo.model('Weather');

app.get('/', function(req, res) {
  var po = {}; //Page Options
  var fo = {'created':{}}; //Find Options
  var ro = {}; //Return Options
  var so = {}; //Sort options
  var search = req.query;

  if(search.device)
    fo["device"] = search.device;

  if(search.sort){
    var fields = ['created', 'temperature', 'humidity'];
    var order = 1;

    if(search.sort[0] === '-'){
      search.sort = search.sort.substring(1);
      order = -1;
    }

    if(fields.indexOf(search.sort) > -1){
      so[search.sort] = 1 * order;
    }
  } else {
    so['created'] = -1;
  }

  if (search.from) {
    fo['created']['$gte'] = parseDate(search.from);
  }

  if (search.to) {
    fo['created']['$lte'] = parseDate(search.to);
  }

  if (!search.from && !search.to) {
    var time = new Date();
    time.setDate(time.getDate()-7); //7 days ago
    fo['created'] =  {'$gte' : time }
  }

  //Pagination
  if (!search.page)
      search.page = 1; //Default page

  if (!search.limit || search.limit > 25)
      search.limit = 25; //default limit

  po.limit = search.limit;
  po.skip = (search.page - 1) * po.limit;

  //Return options
  ro.__v = 0;

  Weather.find(fo, ro, po).sort(so).exec(function(err, r) {
    if (!r || r.length == 0)
      res.status(404).send("No results were found");
    else if (err)
      res.sendStatus(404);
    else
      res.status(200).send(r);
  });

});

app.get('/post', function(req, res) {
  var w = new Weather(req.query); //req.body
  w.save(function(err, r) {
    if (err)
      res.status(400).send(err);
    else
      res.status(201).send("created");
  });
});

app.get('/flush', function(req, res) {
  Weather.remove({}, function(err, r) {
    if (err)
      res.status(400).send(err);
    else
      res.status(200).send("flushed");
  })
});


// parse a date in yyyy-mm-dd format
function parseDate(arg) {
  var parts = arg.split('-');
  // new Date(year, month [, day [, hours[, minutes[, seconds[, ms]]]]])
  return new Date(parts[0], parts[1]-1, parts[2]); // Note: months are 0-based
}
