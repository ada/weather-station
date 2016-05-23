app.controller("weather", ['$scope','Restangular', '$location', '$interval', function($scope, Restangular, $location, $interval){

	angular.element(document).ready(function () {

		$scope.createWebSocket("ws://localhost:82/");
		var lat = 59.6;
		var lng = 16.54;

		//Date time
		$scope.currentDateTime = Date.now();
		$interval(function () {
			$scope.currentDateTime = Date.now(); }, 1000);

		//Air
		$scope.air = {
			temperature: 12,
			humidity:50,
			pressure : 12,
			methane : 155,
			buthane : 89,
			co2 : 1990
		}

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
		$scope.moon = {
			distance : Number(moon.distance).toFixed(2),
			altitude : Number(moon.altitude).toFixed(2),
			fraction : Number(moon2.fraction).toFixed(2)*100,
			phase : moonPhaseToString(moon2.phase),
			rise : moon3.rise.getHours() + ':' + moon3.rise.getMinutes(),
			set : moon3.set.getHours() + ':' + moon3.set.getMinutes(),
			alwaysUp : moon3.alwaysUp,
			alwaysDown : moon3.alwaysDown
		}

		//Device
		$scope.device = {
			id : "WS1",
			health : "Excellent",
			lat : lat,
			lng : lng,
			addr: "NA",
			RSSI : 0
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

	const Kind_RSSI = 120;

	$scope.createWebSocket = function(host){
		if ("WebSocket" in window)
    {
       var ws = new WebSocket(host);

       ws.onopen = function()
       {
          console.log("Websocket opened");
       };

       ws.onmessage = function (evt)
       {
          var d = JSON.parse(evt.data);
          console.log("Websocket data:", d);

					switch (d.Type) {
						case Kind_RSSI:
							var delta = $scope.device.RSSI - d.Value;
							if(delta > 0)
								$("#rssi").addClass("positive").delay(1000).removeClass("positive");
							else
								$("#rssi").addClass("negative").delay(1000).removeClass("negative");
							$scope.device.RSSI = d.Value;
							break;
						default:
							$scope.air.temperature = d.Value;
					}
       };

       ws.onclose = function()
       {
          console.log("Websocket closed");
       };
    }

    else
    {
       console.log("WebSocket NOT supported by your Browser!");
    }
	}

}]);
