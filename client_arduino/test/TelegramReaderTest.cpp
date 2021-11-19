#include <unity.h>

#include "TelegramReader.h"

void addBytes(TelegramReader &tr, char const *bytes) {
  while (*bytes) {
    tr.addByte(static_cast<unsigned char>(*bytes));
    bytes++;
  }
}

void testTelegramReaderReset() {
  TelegramReader tr;
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_TRUE(tr.isEmpty());

  addBytes(tr, "/foo bar\r\nbla bla");
  TEST_ASSERT_FALSE(tr.isEmpty());

  tr.reset();
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_TRUE(tr.isEmpty());
}

void testTelegramReaderReadFull() {
  TelegramReader tr;

  addBytes(tr, "/");
  TEST_ASSERT_TRUE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());

  addBytes(tr, "m");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());

  addBytes(tr, "eter_id\r\n\r\n");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());

  addBytes(tr, "!");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());

  addBytes(tr, "abcd\r");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());

  addBytes(tr, "\n");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_TRUE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());
}

void testTelegramReaderIgnoresLeadingPartialTelegram() {
  TelegramReader tr;

  addBytes(tr, "hello world\r\n!1234\r\n");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_TRUE(tr.isEmpty());

  addBytes(tr, "/");
  TEST_ASSERT_TRUE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());
}

void testTelegramReaderIgnoresLeadingGarbage() {
  TelegramReader tr;

  addBytes(tr, "hello world\r\nbla bla\r\n");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_TRUE(tr.isEmpty());

  addBytes(tr, "/");
  TEST_ASSERT_TRUE(tr.justStarted());
  TEST_ASSERT_FALSE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());
}

void testTelegramReaderIgnoresTrailingGarbage() {
  TelegramReader tr;

  addBytes(tr, "/meter_id\r\nreading1\r\n!abcd\r\n");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_TRUE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());
  unsigned int size = tr.getSize();

  addBytes(tr, "some more stuff here\r\nnewlines\r\n/headers\r\n!abcd\r\n");
  TEST_ASSERT_FALSE(tr.justStarted());
  TEST_ASSERT_TRUE(tr.isComplete());
  TEST_ASSERT_FALSE(tr.hasError());
  TEST_ASSERT_FALSE(tr.isEmpty());
  TEST_ASSERT_TRUE(tr.getSize() == size);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(testTelegramReaderReset);
  RUN_TEST(testTelegramReaderReadFull);
  RUN_TEST(testTelegramReaderIgnoresLeadingPartialTelegram);
  RUN_TEST(testTelegramReaderIgnoresLeadingGarbage);
  RUN_TEST(testTelegramReaderIgnoresTrailingGarbage);
  UNITY_END();
}
