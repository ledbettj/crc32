#!/usr/bin/env python3
import sys

class CRC32:
    def __init__(self):
        self.table = []
        self.value = None

        for i in range(256):
            v = i
            for j in range(8):
                v = (0xEDB88320 ^ (v >> 1)) if(v & 1) == 1 else (v >> 1)
            self.table.append(v)

    def start(self):
        self.value = 0xffffffff

    def update(self, buf):
        for c in buf:
            self.value = self.table[(self.value ^ c) & 0xFF] ^ (self.value >> 8)

    def finalize(self):
        return self.value ^ 0xffffffff

crc32 = CRC32()

for file_name in sys.argv[1:]:
    f = open(file_name, 'rb')

    crc32.start()
    while True:
        buf = f.read(1024 * 1024)
        if buf == b'':
            break
        crc32.update(buf)

    print("%s: %08X" % (file_name, crc32.finalize()))

    f.close()
