module.exports = function (grunt) {
    require("time-grunt")(grunt);

    grunt.initConfig({
        pkg: grunt.file.readJSON('package.json'),
        opt: {
            client: {
                "tsMain": "lib",
                "tsTest": "test",

                "jsMainOut": "lib",
                "jsTestOut": "test"
            }
        },

        ts: {
          default: {
            tsconfig: {
              tsconfig: "./tsconfig.json",
              updateFiles:false
            }
          }
        },
        tsconfig: {
            main: {
            }
        },
        tslint: {
            options: {
                configuration: "tslint.json"
            },
            files: {
                src: [
                    '<%= opt.client.tsMain %>/**/*.ts',
                    '<%= opt.client.tsTest %>/**/*.ts',
                    '!<%= opt.client.tsMain %>/**/*.d.ts'
                ]
            }
        },
        typedoc: {
            main: {
                options: {
                    module: "<%= ts.options.module %>",
                    out: './docs',
                    name: '<%= pkg.name %>',
                    target: "<%= ts.options.target %>"
                },
                src: [
                    '<%= opt.client.tsMain %>/**/*.ts'
                ]
            }
        },
        dtsm: {
            client: {
                options: {
                    // optional: specify config file
                    confog: './dtsm.json'
                }
            }
        },
        clean: {
            clientScript: {
                src: [
                    // client
                    '<%= opt.client.jsMainOut %>/*.js',
                    '<%= opt.client.jsMainOut %>/*.d.ts',
                    '<%= opt.client.jsMainOut %>/*.js.map',
                    // client test
                    '<%= opt.client.jsTestOut %>/*.js',
                    '<%= opt.client.jsTestOut %>/*.js.map',
                    '<%= opt.client.jsTestOut %>/*.d.ts'
                ]
            },
            dtsm: {
                src: [
                    "typings/"
                ]
            }
        },
        dts_bundle: {
            build: {
                options: {
                    name: "fs-git",
                    main: "lib/fs.d.ts",
                    baseDir: "",
                    out: "fs-git.d.ts"
                }
            }
        },
        mochaTest: {
            test: {
                options: {
                    reporter: 'spec',
                    timeout: 10000,
                    require: [
                        function () {
                            require('espower-loader')({
                                cwd: process.cwd() + '/' + grunt.config.get("opt.client.jsTestOut"),
                                pattern: '**/*.js'
                            });
                        },
                        function () {
                            assert = require('power-assert');
                        }
                    ]
                },
                src: [
                    '<%= opt.client.jsTestOut %>/**/*Spec.js'
                ]
            }
        },
        conventionalChangelog: {
          options: {
            changelogOpts: {
              // conventional-changelog options go here
              preset: "angular"
           },
           context: {
              // context goes here
           },
           gitRawCommitsOpts: {
              // git-raw-commits options go here
           },
           parserOpts: {
              // conventional-commits-parser options go here
           },
           writerOpts: {
              // conventional-changelog-writer options go here
           }
          },
          release: {
            src: "CHANGELOG.md"
          }
        }
    });

    grunt.registerTask(
        'setup',
        ['clean', 'dtsm']);

    grunt.registerTask(
        'default',
        ['tsconfig', 'ts', 'tslint']);

    grunt.registerTask(
        'test',
        ['default', 'mochaTest']);

    require('load-grunt-tasks')(grunt);
};
