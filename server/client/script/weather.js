app.controller("weather", ['$scope','Restangular', '$location', '$interval', function($scope, Restangular, $location, $interval){

	$scope.wid = {
		DHT11_Temperature_0 : 0,
		DHT11_Humidity_0 : 1,
		DHT11_Temperature_1 :2,
		DHT11_Humidity_1 : 3,
		BMP085_Pressure : 4,
		BMP085_Temperature : 5,
		BMP085_Altitude : 6,
		BMP085_Pressure_Raw : 7,
		BMP085_Temperature_Raw : 8,
		MQ2_Raw_0 : 9,
		MQ2_Raw_1 : 10,
		MQ4_Raw_0 : 11,
		MQ4_Raw_1 : 12,
		Bluesky_Rain_Drop_0 : 13,
		Bluesky_Rain_Drop_1 : 14,
		Bluesky_Rain_Drop_Delta_0 :15,
		Bluesky_Rain_Drop_Delta_1 : 16,
		Neo6mGPS_Info_Satelite_Count : 17,
		Neo6mGPS_Time_Centisecond : 18,
		Neo6mGPS_Time_Second : 19,
		Neo6mGPS_Time_Minute : 20,
		Neo6mGPS_Time_Hour : 21,
		Neo6mGPS_Time_Day : 22,
		Neo6mGPS_Time_Month : 23,
		Neo6mGPS_Time_Year : 24,
		Neo6mGPS_Location_Latidude : 25,
		Neo6mGPS_Location_Longitude :26,
		Neo6mGPS_Altitude_Meter : 27
	};

	angular.element(document).ready(function () {

		$scope.connectToWebSocket("ws://localhost:82/");
		var lat = 59.6;
		var lng = 16.54;

		//Date time
		$scope.currentDateTime = Date.now();
		$interval(function () {$scope.currentDateTime = Date.now(); }, 500);

		// Sun
		var sun = SunCalc.getTimes(new Date(), lat, lng);
		var sun2 = SunCalc.getPosition(new Date(), lat, lng);
		$scope.sun = {
			rise : sun.sunrise.getHours() + ':' + sun.sunrise.getMinutes(),
			set : sun.sunset.getHours() + ':' + sun.sunset.getMinutes(),
			solarNoon : sun.solarNoon.getHours() + ':' + sun.solarNoon.getMinutes(),
			altitude : Number(sun2.altitude).toFixed(2)
		}


		// Moon
		var phaseStr;
		var moon = SunCalc.getMoonPosition(new Date(), lat, lng);
		var moon2 = SunCalc.getMoonIllumination(new Date());
		var moon3 = SunCalc.getMoonTimes(new Date(), lat, lng);
		console.log(moon3);
		$scope.moon = {
			distance : Number(moon.distance).toFixed(2),
			altitude : Number(moon.altitude).toFixed(2),
			fraction : Number(moon2.fraction).toFixed(2)*100,
			phase : moonPhaseToString(moon2.phase),
			//rise : moon3.rise.getHours() + ':' + moon3.rise.getMinutes(),
			set : moon3.set.getHours() + ':' + moon3.set.getMinutes(),
			alwaysUp : moon3.alwaysUp,
			alwaysDown : moon3.alwaysDown,
		}
  });

	function moonPhaseToString(mp){
		var str;
		if      (mp === 0.00)              {str = "Nymåne"}
		else if (mp >   0.00 && mp < 0.25) {str = "Snart halvmåne"}
		else if (mp === 0.25)              {str = "Halvmåne första kvarteret"}
		else if (mp >   0.25 && mp < 0.50) {str = "Snart fullmåne"}
		else if (mp === 0.50)              {str = "Fullmåne"}
		else if (mp >   0.50 && mp < 0.75) {str = "Snart halvmåne"}
		else if (mp === 0.75)              {str = "Halvmåne sista kvarteret"}
		else if (mp >   0.75 && mp <= 1.0) {str = "Snart nymåne"}
		else                               {str = "Invalid moon phase"}
		return str;
	}


	$scope.connectToWebSocket = function(address) {
		 if ("WebSocket" in window) {
				var ws = new WebSocket(address);

				ws.onopen = function() {
					 console.log("WebSocket: connection opened");
				};

				ws.onmessage = function (evt) {
					 $scope.w = evt.data.split(',');
				};

				ws.onclose = function() {
					 console.log("WebSocket: connection closed");
				};
		 }
		 else {
				console.log("WebSocket: browser not supported.");
		 }
	}

}]);
