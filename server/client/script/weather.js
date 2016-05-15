angular.module('weather',["googlechart"])

.controller("main", ['$scope','Restangular', '$location', function($scope, Restangular, $location){
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





}]);
