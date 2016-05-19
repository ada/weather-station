var app = angular.module('weatherStation', ['ngRoute','restangular','templates-main','angularMoment', 'googlechart']);

app.config(['$routeProvider', '$locationProvider', 'RestangularProvider',
  function ($routeProvider, $locationProvider, RestangularProvider) {
  $locationProvider.html5Mode(true).hashPrefix('!');
  RestangularProvider.setBaseUrl(
    location.protocol + '//' + location.hostname +
    (location.port ? ':' + location.port : '') + '/api');
  RestangularProvider.setDefaultHttpFields({cache: true});
  RestangularProvider.setDefaultHeaders(
    {'Content-Type': 'application/json; charset=utf-8'});
  RestangularProvider.setRestangularFields({id: "_id"});

  $routeProvider
      .when('/404', {
          title: 'Not found',
          templateUrl: '/view/404.html'
      })
      .when('/graph', {
        title: 'Vädergrafer',
        templateUrl: '/view/graph.html',
        controller: 'graph',
        reloadOnSearch: false
      })
      .when('/', {
        title: 'Weather',
        templateUrl: '/view/weather.html',
        controller: 'weather'
      })
      .otherwise({
        redirectTo: '/404'
      });
}]);

app.run(function(amMoment) {
    amMoment.changeLocale('sv');
});
