#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <inttypes.h>

const static int ONE_MB = 1024 * 1024;

class Crc32 {
private:
  const static uint32_t seed;
  std::vector<uint32_t> table;
  uint32_t value;
public:
  Crc32();

  uint32_t crc(const std::vector<uint8_t>& bytes);
  void     start();
  void     update(const std::vector<uint8_t>& bytes);
  uint32_t finalize();
};

const uint32_t Crc32::seed = 0xEDB88320;

Crc32::Crc32() : value(0)
{
  uint32_t v;
  table.resize(256);

  for(size_t i = 0; i < 256; ++i) {
    v = i;
    for(size_t j = 0; j < 8; ++j) {
      v = (v & 1) ? (seed ^ (v >> 1)) : (v >> 1);
    }
    table[i] = v;
  }
}

uint32_t Crc32::crc(const std::vector<uint8_t>& bytes)
{
  start();
  update(bytes);
  return finalize();
}

void Crc32::start()
{
  value = 0xFFFFFFFF;
}

void Crc32::update(const std::vector<uint8_t>& bytes)
{
  for(std::vector<uint8_t>::const_iterator i = bytes.begin();
      i != bytes.end();
      i++) {
    value = table[(value ^ *i) & 0xFF] ^ (value >> 8);
  }
}

uint32_t Crc32::finalize()
{
  return value ^ 0xFFFFFFFFul;
}


int main(int argc, char* argv[])
{
  Crc32 crc;
  std::vector<uint8_t> buf;

  buf.resize(ONE_MB);


  for(int i = 1; i < argc; ++i) {
    crc.start();
    std::ifstream f(argv[i], std::ios::binary);

    while(f.read((char*)&buf[0], ONE_MB)) {
      crc.update(buf);
    }

    buf.resize(f.gcount());
    crc.update(buf);

    f.close();
    std::cout << argv[i] << ": " << std::setw(8) << std::setfill('0') <<
      std::hex << crc.finalize() << std::endl;
  }
}
