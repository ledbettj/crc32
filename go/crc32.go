package main

import (
	"os"
	"fmt"
)

type Crc32 struct {
	value uint32
	table[256] uint32
}

func (self *Crc32) Init() {
	var v uint32

	for i := 0; i < 256; i++ {
		v = uint32(i)
		for j := 0; j < 8; j++ {
			if (v & 1 == 1) {
				v = (0xedb88320 ^ (v >> 1))
			} else {
				v >>= 1
			}
		}
		self.table[i] = v
	}
}

func (self *Crc32) Start() {
	self.value = 0xffffffff
}

func (self *Crc32) Finalize() (uint32) {
	return self.value ^ 0xffffffff
}

func (self *Crc32) Update(data []byte) {
	for i := 0; i < len(data); i++ {
		self.value = self.table[(self.value ^ uint32(data[i])) & 0xFF] ^ (self.value >> 8)
	}
}

func main() {
	var args = os.Args[1:]
	var crc  = &Crc32{}

	crc.Init()

	for i := 0; i < len(args); i++ {
		fp, err := os.Open(args[i])
		if (err == nil) {
			crc.Start()
			data := make([]byte, 1024 * 1024)

			for {
				count, err := fp.Read(data)
				if (err != nil || count == 0) {
					break
				}
				crc.Update(data[:count])
			}
			fmt.Printf("%s: %08X\n", args[i], crc.Finalize())
			fp.Close()
		}
	}

}