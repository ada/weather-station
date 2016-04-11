angular.module('weatherStation', ['ngRoute','restangular','templates-main','weather'])

.config(['$routeProvider', '$locationProvider', 'RestangularProvider', function ($routeProvider, $locationProvider, RestangularProvider) {
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
      .when('/', {
        title: 'Weather',
        templateUrl: '/view/main.html',
        controller: 'main',
      })
      .otherwise({
        redirectTo: '/404'
      });
}]);
