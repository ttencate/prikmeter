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
