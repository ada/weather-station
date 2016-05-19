module.exports = function(grunt) {
	grunt.initConfig({
		pkg: grunt.file.readJSON('package.json'),
		less: {
			main:{
				options: {
					paths: ["less"],
					cleancss: true,
				},
				files: {
					"build/app.css": "client/less/bootstrap.less"
				}
			}
		},

		copy: {
			main: {
				nonull: true,
				cwd: 'client/',
				src: '*.html',
				dest: 'build/',
				flatten: true,
				expand: true,
			}
		},

		html2js: {
			options: {
				base:'client/',
				rename:function (moduleName) {
					return moduleName.replace('view/', '/view/');
				},
				htmlmin: {
					collapseWhitespace: true,
					removeComments: true
				}
			},
			main: {
				src: ['client/view/*.html'],
				dest: 'client/script/templates.js'
			},
		},

		uglify: {
			libs: {
				options: {
					mangle: false,
					beautify : false,
				},
				files: [{
					expand: false,
					src: [
						'client/script/libs/*.js'
					],
					dest: 'build/libs.js'
				}]
			},
			app: {
				options: {
					mangle: false,
					beautify : false,
				},
				files: [{
					expand: false,
					src: [
						'client/script/*.js',
					],
					dest: 'build/app.js'
				}]
			}
		},

		clean: {
			builddir : ['build/*'],
		  templates: ['client/script/templates.js']
		}
	});

grunt.loadNpmTasks('grunt-html2js');
grunt.loadNpmTasks('grunt-contrib-uglify');
grunt.loadNpmTasks('grunt-contrib-less');
grunt.loadNpmTasks('grunt-contrib-copy');
grunt.loadNpmTasks('grunt-contrib-clean');

//grunt.registerTask('clean', ['clean:templates', 'clean:builddir']);
grunt.registerTask('libs', ['uglify:libs']);
grunt.registerTask('default', ['less:main', 'copy:main', 'html2js', 'uglify:app', 'clean:templates']);

};
