var CronJob = require('cron').CronJob;

function update(){
  //Get weather data from Arduino server and save in database
  console.log("Running CronJob: fetching new data...");
}

module.exports.init = function(){
    console.log("Registering CronJob: Update");
    new CronJob('* * * * *', update, null, true); //run every minute
};
