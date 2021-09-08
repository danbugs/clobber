// if you rename this file, don't forget to change the "server" field in Tarantella.toml
const Module = require("./build/clobber-api");
const express = require("express");
const { Server } = require("ws");
const PORT = process.env.PORT || 3000;

Module["onRuntimeInitialized"] = function () {
  const server = express()
    .use((_, res) => res.send("Welcome to Clobber!"))
    .listen(PORT, () => console.log(`Listening on ${PORT}`));

  const wss = new Server({ server });

  wss.on("connection", function (ws) {
    console.log("Client connected!");
    let clobs = Module._get_clobs();
    ws.send(Module.UTF8ToString(clobs).trim());

    ws.on("close", function () {
      console.log("Client disconnected!");
    });

    ws.on("message", function (message) {
      let new_clob = Module.allocateUTF8(message + "");
      Module._make_clob(new_clob);
      Module._free(new_clob);
      clobs = Module._get_clobs();
      ws.send(Module.UTF8ToString(clobs).trim());
      console.log(Module.UTF8ToString(clobs).trim());
    });
  });
};
