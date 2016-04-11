var config                         = {};
config.port                        = 7777;
config.mongo                       = 'mongodb://localhost:27017/classifieds';
config.updateService               = {enabled:1, pattern:'41 20 * * *'};
module.exports = config;
