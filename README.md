Prikmeter
=========

Prikmeter is a hobby project to fetch and store readings from my electricity
meter, a Dutch "smart meter". It consists of three parts:

* A hardware component based on a [ESP8266](http://esp8266.net/)
  System-on-Chip. This goes in your meter cabinet. Specs are in the
  [`hardware/`](hardware) subdirectory.

* A firmware component written in C++ using the
  [ESP8266 core for Arduino](https://github.com/esp8266/Arduino). Its code is in
  the [`arduino_client/`](arduino_client) directory. (There is also a
  discontinued [`nodemcu_client/`](nodemcu_client) which uses NodeMCU and Lua.)

* A webserver running [Node.js](https://nodejs.org/) and
  [Express](http://expressjs.com/), which stores the data in a
  [PostgreSQL](https://www.postgresql.org/) database.
