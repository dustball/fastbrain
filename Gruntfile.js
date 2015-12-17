module.exports = function(grunt) {
    
    grunt.initConfig({
        exec: {
            compile: {
                command: 'gcc -Wfatal-errors -Werror main.cc -o fastbrain -Ofast -lstdc++ -std=c++11 ',
                stdout: true,
                stderr: true
            },
            run: {
                command: './fastbrain',
                stdout: true,
                stderr: true
            },
            kill: {
                command: 'killall -9 fastbrain',
                stdout: true,
                stderr: true
            },
            debug: {
                command: 'gcc -Wfatal-errors -Werror main.cc -o fastbrain -g -O0 -lstdc++ -std=c++11 ',
                stdout: true,
                stderr: true
            },
            indent: {
                command: 'astyle --style=google -s4 *.cc',
                stdout: true,
                stderr: true
            },
            lldb: {
                command: 'lldb fastbrain',
                stdout: true,
                stderr: true,
                stdin: true
            },
            graphvis: {
                command: 'dot -Tpng brain.gv > brain.png',
                stdout: true,
                stderr: true,
                stdin: true
            },
        },
        watch: {
            files: ['fastbrain.cc','worldmap.cc','webserver.cc','main.cc'],
            tasks: ['exec:compile','exec:kill'] // 'exec:run'
        }
    });
        
    grunt.loadNpmTasks('grunt-contrib-watch');
    grunt.loadNpmTasks('grunt-exec');
    grunt.registerTask('gb',['exec:graphvis']);
    grunt.registerTask('debug',['exec:debug']);
    grunt.registerTask('indent',['exec:indent']);
    grunt.registerTask('loop',['exec:run','loop']);

    grunt.registerTask('default', ['exec:compile','watch']);

};
