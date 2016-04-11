var Schema = mongo.Schema;
var weatherSchema = new Schema({
  created:{
    type: Date,
    default: Date.now,
    required:true
  },
  temperature : Number,
  humidity : Number

});

mongo.model('Weather', weatherSchema);
