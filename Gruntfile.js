module.exports = function(grunt) {
	
  grunt.initConfig({
    exec: {
      compile: {
      	command: 'gcc -Wfatal-errors -Werror fastbrain.cc -o fastbrain -Ofast -lstdc++',
      	stdout: true,
      	stderr: true
      },
      run: {
      	command: './fastbrain',
      	stdout: true,
      	stderr: true
      },
      debug: {
      	command: 'gcc -Wfatal-errors -Werror fastbrain.cc -o fastbrain.debug -g -O0 -lstdc++',
      	stdout: true,
      	stderr: true
      },
      lldb: {
      	command: 'lldb fastbrain.debug',
      	stdout: true,
      	stderr: true,
      	stdin: true
      },
      
    },

    
    watch: {
      files: ['fastbrain.cc'],
      tasks: ['exec:compile','exec:run']
    }
  });

  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-exec');
  grunt.registerTask('debug',['exec:debug']);

  grunt.registerTask('default', ['exec:compile','exec:run','watch']);

};
