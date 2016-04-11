#Weather
The following commands should be run in the Windows Console application.

###Requirements
```
1. NodeJs
2. MongoDB
3. npm package manager  
```

###Installation
```
1. npm install
2. npm install -g supervisor
3. npm install -g grunt-cli
```


###Build
```
grunt build //Builds everything, suitable for production
grunt 		//build js, css and templates. Suitable for development
```

###Run
```
supervisor -e server  	 //Starts server
supervisor -e js server  //Start server and watch js files for changes
```

###Test
```
localhost:7777/api/weather //Should list the weather entries
localhost:7777/api/weather/post //should create a new random weathe entry
localhost:7777/ //should present the local client application made by Angular
```

###API
All communication to the API should have `Content-Type: application/json; charset=utf-8`
