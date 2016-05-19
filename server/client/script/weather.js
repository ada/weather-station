angular.module('weather',["googlechart"])

.controller("main", ['$scope','Restangular', '$location', '$interval', function($scope, Restangular, $location, $interval){

	angular.element(document).ready(function () {
		//Date time
		$scope.currentDateTime = Date.now();
		$interval(function () {
			$scope.currentDateTime = Date.now(); }, 1000);

		//Device
		var lat = 59.6;
		var lng = 16.54;
		$scope.device = {
			id : "WS1",
			health : "Excellent",
			lat : lat,
			lng : lng,
			addr: "NA"
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
			altitude : moon.altitude,
			fraction : Number(moon2.fraction).toFixed(2)*100,
			phase : moonPhaseToString(moon2.phase),
			rise : moon3.rise.getHours() + ':' + moon3.rise.getMinutes(),
			set : moon3.set.getHours() + ':' + moon3.set.getMinutes(),
			alwaysUp : moon3.alwaysUp,
			alwaysDown : moon3.alwaysDown
		}
  });

	$scope.$on('$routeUpdate', function(){
		$scope.newSearch();
	});
	$scope.searchOptions = $location.search();

	$scope.newSearch = function(){
		$scope.error = null;
		$location.path('/').search($.param($scope.searchOptions));
		$scope.loading = true;
		$scope.weatherData = [];
		Restangular.all('weather').getList($location.search()).then(
			function(r) {
				$scope.weatherData = r;
				$scope.loading = false;
			}, function(err){
				$scope.loading = false;
				$scope.error = err;
			});
	};


	$scope.newSearch();

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


}]);
