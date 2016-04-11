module.exports = function(app) {
    app.use('/api/weather', require('./weather'));
    app.get(new RegExp("^(?!/api).*$", 'i'), function(req, res) {
        res.render('index');
    });
};
