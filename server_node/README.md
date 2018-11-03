Server
======

This is the server part, using Node.js with the Express HTTP framework and the
Knex query builder. It's all pretty standard stuff if you're familiar with
these.

It heavily uses the new `async`/`await` syntax, so it requires a recent Node
version (I'm using 8.6.0).

For storage, PostgreSQL is used. In testing, this can be swapped out for a
SQLite single-file store.

There is a lot of code out there with similar purposes, for example [DSMR
Reader](https://github.com/dennissiemensma/dsmr-reader) by Dennis Siemensma,
but as this is a hobby project, I just like to roll my own.

Running
-------

Development mode:

    npm run run_watch

Production mode:

    npm run run

Testing
-------

Continuously:

    npm run test_watch

Once:

    npm run test

SSL certificates
----------------

The Arduino client checks the certificate fingerprint, but is too dumb to
verify an entire certificate chain
(https://github.com/nodemcu/nodemcu-firmware/issues/1707). We could try
checking at least the fingerprint of the presented certificate, but
certificates change frequently (especially those from Let's Encrypt) and there
is no way to remotely update the fingerprint that the client expects.

For those reasons, it's better to host the uploads on a separate domain which
uses a self-signed certificate with a long lifetime, whose fingerprint you can
embed into the client. Generate such a cert using a command like this:

    openssl req -x509 -nodes -days 36500 -newkey rsa:2048 \
        -keyout upload.my-server.example.com.key \
        -out upload.my-server.example.com.crt

To output its fingerprint:

    openssl x509 -in upload.my-server.example.com.crt -noout -fingerprint
