#include "telegram_reader.h"

#include "catch2/catch.hpp"

void addBytes(TelegramReader &tr, char const *bytes) {
  while (*bytes) {
    tr.addByte(static_cast<unsigned char>(*bytes));
    bytes++;
  }
}

TEST_CASE("TelegramReader resets correctly", "[telegram_reader]") {
  TelegramReader tr;
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE(tr.isEmpty());

  addBytes(tr, "/foo bar\r\nbla bla");
  REQUIRE_FALSE(tr.isEmpty());

  tr.reset();
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE(tr.isEmpty());
}

TEST_CASE("TelegramReader reads full telegram", "[telegram_reader]") {
  TelegramReader tr;

  addBytes(tr, "/");
  REQUIRE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());

  addBytes(tr, "m");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());

  addBytes(tr, "eter_id\r\n\r\n");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());

  addBytes(tr, "!");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());

  addBytes(tr, "abcd\r");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());

  addBytes(tr, "\n");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());
}

TEST_CASE("TelegramReader ignores leading partial telegram before telegram", "[telegram_reader]") {
  TelegramReader tr;

  addBytes(tr, "hello world\r\n!1234\r\n");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE(tr.isEmpty());

  addBytes(tr, "/");
  REQUIRE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());
}

TEST_CASE("TelegramReader ignores leading junk before telegram", "[telegram_reader]") {
  TelegramReader tr;

  addBytes(tr, "hello world\r\nbla bla\r\n");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE(tr.isEmpty());

  addBytes(tr, "/");
  REQUIRE(tr.justStarted());
  REQUIRE_FALSE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());
}

TEST_CASE("TelegramReader ignores trailing junk after telegram", "[telegram_reader]") {
  TelegramReader tr;

  addBytes(tr, "/meter_id\r\nreading1\r\n!abcd\r\n");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());
  unsigned int size = tr.getSize();

  addBytes(tr, "some more stuff here\r\nnewlines\r\n/headers\r\n!abcd\r\n");
  REQUIRE_FALSE(tr.justStarted());
  REQUIRE(tr.isComplete());
  REQUIRE_FALSE(tr.hasError());
  REQUIRE_FALSE(tr.isEmpty());
  REQUIRE(tr.getSize() == size);
}
