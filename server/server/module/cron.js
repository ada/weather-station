var config = require('./../config/config');
var CronJob = require('cron').CronJob;

function update(){
  console.log("Running CronJob: fetching new data...");
}

module.exports.init = function(){
  if(config.updateService.enabled === 1){
    console.log("Registering CronJob: Update");
    new CronJob(config.updateService.pattern, update, null, true);
  }
};
