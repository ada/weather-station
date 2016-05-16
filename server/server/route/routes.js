module.exports = function(app) {
  app.all("*", function(req, res, next){
    console.log(req.ip + ":" + req.originalUrl);
    next();
  });
    app.use('/api/weather', require('./weather'));
    app.get(new RegExp("^(?!/api).*$", 'i'), function(req, res) {
        res.render('index');
    });
};
