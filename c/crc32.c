#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct crc32 {
  uint32_t table[256];
  uint32_t value;
} crc32_t;

#define CRC32_INITIAL 0xedb88320

void crc32_init(crc32_t* crc) {
  uint32_t v;

  for(int i = 0; i < 256; ++i) {
    v = i;

    for(int j = 0; j < 8; ++j) {
      v = (v & 1) ? (CRC32_INITIAL ^ (v >> 1)) : (v >> 1);
    }
    crc->table[i] = v;
  }
}

void crc32_update(crc32_t* c, uint8_t* buf, size_t len) {
  for(size_t i = 0; i < len; ++i) {
    c->value = c->table[(c->value ^ buf[i]) & 0xFF] ^ (c->value >> 8);
  }
}

void crc32_start(crc32_t* c) {
  c->value = 0xfffffffful;
}

uint32_t crc32_finalize(crc32_t* c) {
  return c->value ^ 0xfffffffful;
}

uint32_t crc32_crc(crc32_t* c, uint8_t* buf, size_t len) {
  crc32_start(c);
  crc32_update(c, buf, len);
  return crc32_finalize(c);
}


int main(int argc, char* argv[]) {
  crc32_t c;
  size_t   buf_size = 1024 * 1024;
  uint8_t* buf = malloc(buf_size);
  ssize_t  len;

  if (!buf) {
    perror("error allocating memory");
    exit(1);
  }

  crc32_init(&c);

  for(int i = 1; i < argc; ++i) {
    FILE* fp = fopen(argv[i], "rb");
    crc32_start(&c);

    while((len = fread(buf, 1, buf_size, fp)) > 0) {
      crc32_update(&c, buf, len);
    }
    printf("%s: %08X\n", argv[i], crc32_finalize(&c));

    fclose(fp);
  }

  free(buf);

  return 0;
}

