angular.module('weather',["googlechart"])

.controller("main", ['$scope','Restangular', function($scope, Restangular){

	$scope.test = "Hello from Angular";
	$scope.chartObject = {}; //https://bouil.github.io/angular-google-chart/#/generic/LineChart
	$scope.chartObject.type = 'LineChart';
	$scope.chartObject.options = {
			'title': 'Västerås'
	};


	Restangular.all('weather').getList().then(function(r) {
	  $scope.w = r;
		var latest = r[0];
		var rows = [];
		var cols = [
	      {id: "t", label: "Topping", type: "string"},
	      {id: "s", label: "Temperature", type: "number"}
	  ];

		angular.forEach(r, function(e){
			rows.push({c: [
          {v: e.created},
          {v: e.temperature},
      ]})
		});


		$scope.chartObject.data = {"cols": cols, "rows": rows};

	});






}]);
