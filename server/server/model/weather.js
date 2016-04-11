var Schema = mongo.Schema;
var weatherSchema = new Schema({
  created:{
    type: Date,
    default: Date.now,
    required:true
  },
  temperature:{
    type : Number,
    required: true
  }

});

mongo.model('Weather', weatherSchema);
