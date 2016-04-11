module.exports = function(grunt) {
	grunt.initConfig({
		pkg: grunt.file.readJSON('package.json'),

		less: {
			production: {
				options: {
					paths: ["less"],
					cleancss: true,
				},
				files: {
					"build/app.css": "client/res/style/main.less"
				}

			}
		},

		copy: {
			main: {
				nonull: true,
				cwd: 'client/res/',
				src: '**',
				dest: 'build/',
				flatten: true,
				expand:true,
			},
			fonts : {
				cwd: 'client/res/font/',
				src: '**',
				dest: 'build/font/',
				expand:true,
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
		}
	});

grunt.loadNpmTasks('grunt-contrib-uglify');
grunt.loadNpmTasks('grunt-contrib-less');
grunt.loadNpmTasks('grunt-html2js');
grunt.loadNpmTasks('grunt-contrib-copy');
grunt.registerTask('libs', ['uglify:libs']);
grunt.registerTask('build', ['less', 'copy:main', 'copy:fonts', 'html2js', 'uglify', 'less']);
grunt.registerTask('default', ['less','html2js', 'uglify:app']);

};
