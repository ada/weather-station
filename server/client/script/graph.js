app.controller("graph", ['$scope','Restangular', '$location', function($scope, Restangular, $location){

	angular.element(document).ready(function () {

  });

	$scope.$on('$routeUpdate', function(){
		$scope.newSearch();
	});
	$scope.searchOptions = $location.search();

	$scope.newSearch = function(){
		$scope.error = null;
		$location.path('/graph').search($.param($scope.searchOptions));
		$scope.loading = true;
		$scope.weatherData = [];
		Restangular.all('weather').getList($location.search()).then(
			function(r) {
				var TRows = [];
				var HRows = [];
				var UVRows = [];
				var CO2Rows = [];

				for (var i = 0; i < r.length; i++) {
					var d = new Date(Date.parse(r[i].created));
					var date =  d.getDate() + '/' + d.getMonth();
					TRows.push(createRow(date, r[i].temperature, " C"));
					HRows.push(createRow(date, r[i].humidity, "%"));
					UVRows.push(createRow(date, r[i].uv, " P"));
					CO2Rows.push(createRow(date, r[i].co2, "PPM"));
				}

				$scope.CDTemperature = createChartData("Temperature", TRows);
				$scope.CDHumidity = createChartData("Humidity", HRows);
				$scope.CDUV = createChartData("UV", UVRows);
				$scope.CDCO2 = createChartData("CO2", CO2Rows);
				$scope.weatherData = r;
				$scope.loading = false;
			}, function(err){
				$scope.loading = false;
				$scope.error = err;
			});
	};


	$scope.newSearch();

	function createRow(date, v, label){
		return {
			"c": [
				{"v": date},
				{"v": v, "f": v + label}
			]
		}
	}

	function createChartData(title, rows){
		return {
			"type": "LineChart",
			"displayed": false,
			"data": {
				"cols": [
					{
						"id": "month",
						"label": "Month",
						"type": "string",
						"p": {}
					},
					{
						"id": title,
						"label": title,
						"type": "number",
						"p": {}
					}
				],
				"rows": rows
			},
			"options": {
				"title": title,
				"isStacked": "false",
				"fill": 20,
				"displayExactValues": true,
				"vAxis": {
					"title": "Temperature",
					"gridlines": {
						"count": 10
					}
				},
				"hAxis": {
					"title": "Date"
				}
			},
			"formatters": {}
		}
	}

}]);
