#pragma once

#define MAX_TELEGRAM_SIZE 4096

class TelegramReader {

  public:
    TelegramReader();

    void reset();
    bool addByte(unsigned char b);

    bool isEmpty() const { return !size; }
    bool justStarted() const;
    bool hasError() const { return error; }
    bool isComplete() const { return complete; }
    unsigned char const *getBuffer() const { return buffer; }
    unsigned int getSize() const { return size; }

  private:
    unsigned char buffer[MAX_TELEGRAM_SIZE];
    unsigned int size;
    bool error;
    bool insideTelegram;
    bool atStartOfLine;
    bool seenCr;
    bool inChecksumLine;
    bool complete;

};
