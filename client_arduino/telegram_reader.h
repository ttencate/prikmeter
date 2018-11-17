#pragma once

#define MAX_TELEGRAM_SIZE 4096

class TelegramReader {

  public:
    TelegramReader();

    void reset();
    bool addByte(byte b);

    bool isEmpty() const { return !size; }
    bool justStarted() const;
    bool hasError() const { return error; }
    bool isComplete() const { return complete; }
    byte const *getBuffer() const { return buffer; }
    unsigned int getSize() const { return size; }

  private:
    byte buffer[MAX_TELEGRAM_SIZE];
    unsigned int size;
    bool error;
    bool insideTelegram;
    bool atStartOfLine;
    bool seenCr;
    bool inChecksumLine;
    bool complete;

};
