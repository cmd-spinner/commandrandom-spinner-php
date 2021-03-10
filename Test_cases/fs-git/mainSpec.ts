"use strict";

require("es6-promise").polyfill();

try {
    // optional
    require("source-map-support").install();
} catch (e) {
}

import * as assert from "power-assert";
import * as fsgit from "../lib/";

import * as fs from "fs";
import * as child_process from "child_process";

describe("fs-git", () => {
    before(done => {
        if (fs.existsSync("./test/fixture/vanilla")) {
            done();
            return;
        }
        child_process.exec("git clone test/fixture/bare.git test/fixture/vanilla", err=> {
            if (err) {
                done(err);
            }
            child_process.exec("git checkout -b develop origin/develop", { cwd: "test/fixture/vanilla" }, err=> {
                done(err);
            });
        });
    });

    let matrix: { name: string; gitDir: string; }[] = [
        {
            name: "bare repository",
            gitDir: "./test/fixture/bare.git"
        },
        {
            name: "cloned repository",
            gitDir: "./test/fixture/vanilla/.git"
        }
    ];
    matrix.forEach(dirInfo => {
        it("show refs in " + dirInfo.name, () => {
            return fsgit.open(dirInfo.gitDir, "master").then(fs => {
                fs.showRef().then(refs => {
                    assert(refs.length !== 0);
                    refs.forEach(ref => {
                        assert(/^[0-9a-f]+$/.test(ref.ref));
                        assert(/^refs\//.test(ref.name));
                    });
                });
            });
        });

        it("open master branch in " + dirInfo.name, () => {
            return fsgit.open(dirInfo.gitDir, "master").then(fs => {
                let fileList = fs.fileList().then(files=> {
                    assert(files.length !== 0);
                    assert(files.filter(file=> file.path === "master-branch-only.js").length === 1);
                });
                let file = fs.file("subdir2/master-branch-only.js").then(fileInfo=> {
                    assert(fileInfo.type === "blob");
                });
                let readFile = fs.readFile("subdir2/master-branch-only.js", { encoding: "utf8" }).then(content=> {
                    assert(typeof content === "string");
                });
                let exists1 = fs.exists("subdir2/master-branch-only.js").then(exists=> {
                    assert(exists === true);
                });
                let exists2 = fs.exists("develop-branch-only.js").then(exists=> {
                    assert(exists === false);
                });
                return Promise.all([fileList, file, readFile, exists1, exists2]);
            });
        });

        it("open develop branch in " + dirInfo.name, () => {
            return fsgit.open(dirInfo.gitDir, "develop").then(fs => {
                let fileList = fs.fileList().then(files=> {
                    assert(files.length !== 0);
                    assert(files.filter(file=> file.path === "develop-branch-only.js").length === 1);
                });
                let file = fs.file("subdir/develop-branch-only.js").then(fileInfo=> {
                    assert(fileInfo.type === "blob");
                });
                let readFile = fs.readFile("subdir/develop-branch-only.js", { encoding: "utf8" }).then(content=> {
                    assert(typeof content === "string");
                });
                let exists1 = fs.exists("develop-branch-only.js").then(exists=> {
                    assert(exists === true);
                });
                let exists2 = fs.exists("master-branch-only.js").then(exists=> {
                    assert(exists === false);
                });
                return Promise.all([fileList, file, readFile, exists1, exists2]);
            });
        });

        it("open test-tag tag in " + dirInfo.name, () => {
            return fsgit.open(dirInfo.gitDir, "test-tag").then(fs => {
                let fileList = fs.fileList().then(files=> {
                    assert(files.length !== 0);
                    assert(files.filter(file=> file.path === "subdir/test.txt").length === 1);
                });
                let file = fs.file("subdir/test.txt").then(fileInfo=> {
                    assert(fileInfo.type === "blob");
                });
                let readFile = fs.readFile("subdir/test.txt", { encoding: "utf8" }).then(content=> {
                    assert(typeof content === "string");
                });
                let exists1 = fs.exists("subdir/test.txt").then(exists=> {
                    assert(exists === true);
                });
                let exists2 = fs.exists("master-branch-only.js").then(exists=> {
                    assert(exists === false);
                });
                return Promise.all([fileList, file, readFile, exists1, exists2]);
            });
        });

        it("open specific ref in " + dirInfo.name, () => {
            return fsgit.open(dirInfo.gitDir, "b41735").then(fs => {
                let fileList = fs.fileList().then(files=> {
                    assert(files.length !== 0);
                    assert(files.filter(file=> file.path === "subdir/test.txt").length === 1);
                });
                let file = fs.file("subdir/test.txt").then(fileInfo=> {
                    assert(fileInfo.type === "blob");
                });
                let readFile = fs.readFile("subdir/test.txt", { encoding: "utf8" }).then(content=> {
                    assert(typeof content === "string");
                });
                let exists1 = fs.exists("subdir/test.txt").then(exists=> {
                    assert(exists === true);
                });
                let exists2 = fs.exists("master-branch-only.js").then(exists=> {
                    assert(exists === false);
                });
                return Promise.all([fileList, file, readFile, exists1, exists2]);
            });
        });
    });
});
