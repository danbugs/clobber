// if you rename this file, don't forget to change the "server" field in Tarantella.toml
const Module = require('./build/clobber-server');

Module['onRuntimeInitialized'] = function() {    
    let clob = Module.allocateUTF8("This is a Clob from Node again!");
    Module._make_clob(clob);
    Module._free(clob);
    let clobs = Module._get_clobs();
    console.log(Module.UTF8ToString(clobs).trim().split('\n'));
}