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
    "<div class=\"container\">{{test}}</div>");
}]);

angular.module("/view/navbar.html", []).run(["$templateCache", function($templateCache) {
  $templateCache.put("/view/navbar.html",
    "<div class=\"container\"><h1>Weather station 1</h1></div>");
}]);
