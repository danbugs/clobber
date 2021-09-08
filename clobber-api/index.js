// if you rename this file, don't forget to change the "server" field in Tarantella.toml
const Module = require("./build/clobber-api");
const express = require("express");
const { Server } = require("ws");
const PORT = process.env.PORT || 3000;

Module["onRuntimeInitialized"] = function () {
  // let clob = Module.allocateUTF8("This is a Clob from Node again!");
  // Module._make_clob(clob);
  // Module._free(clob);

  const server = express()
    .use((_, res) => res.send("Welcome to Clobber!"))
    .listen(PORT, () => console.log(`Listening on ${PORT}`));

  const wss = new Server({ server });
  let clobs = Module._get_clobs();

  wss.on("connection", function (ws) {
    console.log("Client connected!");
    ws.send(Module.UTF8ToString(clobs).trim());

    ws.on("close", function () {
      console.log("Client disconnected!");
    });

    ws.on("message", function (message) {
      console.log(message);
      clobs = Module._get_clobs(); // update clobs
    });
  });
};
