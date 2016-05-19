var Schema = mongo.Schema;
var weatherSchema = new Schema({
  created:{
    type: Date,
    default: Date.now,
    required:true
  },

  temperature : {
    type : Number,
    required:true
  },
  humidity : {
    type : Number,
    required:true
  },
  co2 : {
    type : Number,
    required:true
  },
  uv : {
    type : Number,
    required:true
  },
  device : {
    type : String,
    required:true
  },


});

mongo.model('Weather', weatherSchema);
