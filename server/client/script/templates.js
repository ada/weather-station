angular.module('templates-main', ['/view/404.html', '/view/footer.html', '/view/main.html', '/view/navbar.html']);

angular.module("/view/404.html", []).run(["$templateCache", function($templateCache) {
  $templateCache.put("/view/404.html",
    "<div class=\"container\">Page not found!</div>");
}]);

angular.module("/view/footer.html", []).run(["$templateCache", function($templateCache) {
  $templateCache.put("/view/footer.html",
    "<div class=\"container\"><small>Copyright 2016 - MIT license</small></div>");
}]);

angular.module("/view/main.html", []).run(["$templateCache", function($templateCache) {
  $templateCache.put("/view/main.html",
    "<div class=\"container\"><form class=\"navbar-form\" ng-submit=\"newSearch()\"><div class=\"form-group\"><label for=\"device\">Device ID:</label><input type=\"text\" ng-model=\"searchOptions.device\" class=\"form-control\" placeholder=\"ws1\"></div><div class=\"form-group\"><label for=\"device\">Date:</label><input type=\"text\" ng-model=\"searchOptions.from\" class=\"form-control\" placeholder=\"From\"> <input type=\"text\" ng-model=\"searchOptions.to\" class=\"form-control\" placeholder=\"To\"></div><div class=\"form-group\"><label for=\"device\">Sort:</label><input type=\"text\" ng-model=\"searchOptions.sort\" class=\"form-control\" placeholder=\"-created\"></div><div class=\"form-group\"><label for=\"submit\"></label><button type=\"submit\" ng-click=\"newSearch()\" ng-disabled=\"loading\">Fetch</button></div><div class=\"form-group\" ng-show=\"error\" style=\"color: red\"><label>Error</label><pre style=\"display: inline-flex\">{{error | json}}</pre></div></form><hr><table style=\"text-aling:left\"><thead><th width=\"10%\">Date</th><th width=\"10%\">Temperature</th><th width=\"10%\">Humidity</th></thead><tbody><tr ng-repeat=\"w in weatherData\"><td ng-bind=\"w.created | date:'yyyy-MM-dd HH:mm:ss'\"></td><td ng-bind=\"w.temperature\"></td><td ng-bind=\"w.humidity\"></td></tr></tbody></table></div>");
}]);

angular.module("/view/navbar.html", []).run(["$templateCache", function($templateCache) {
  $templateCache.put("/view/navbar.html",
    "<div class=\"container\"><h1>Weather Station Data Viewer</h1></div>");
}]);
