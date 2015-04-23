use std::fs::File;
use std::path::Path;
use std::env;
use std::thread::{spawn,JoinHandle};
use std::io::Read;

pub struct Crc32 {
    table: [u32; 256],
    value: u32
}

static CRC32_INITIAL:u32 = 0xedb88320;

impl Crc32 {

    fn new() -> Crc32 {
        let mut c = Crc32 { table: [0; 256], value: 0xffffffff };

        for i in 0..256 {
            let mut v = i as u32;
            for _ in 0..8 {
                v = if v & 1 != 0 {
                    CRC32_INITIAL ^ (v >> 1)
                } else {
                    v >> 1
                }
            }
            c.table[i] = v;
        }

        c
    }

    fn start(&mut self) {
        self.value = 0xffffffff;
    }

    fn update(&mut self, buf: &[u8]) {
        for &i in buf {
            self.value = self.table[((self.value ^ (i as u32)) & 0xFF) as usize] ^ (self.value >> 8);
        }
    }

    fn finalize(&mut self) -> u32 {
        self.value ^ 0xffffffffu32
    }

    fn crc(&mut self, buf: &[u8]) -> u32 {
        self.start();
        self.update(buf);
        self.finalize()
    }
}

pub fn main() {
    let args = env::args();
    let mut children = vec![];
        
    for arg in args.skip(1) {
        let mut buf = [0u8 ; 1024];
        let mut crc = Crc32::new();
        let a = arg.clone();
        let r = spawn(move || {
            let path = Path::new(&a);
            let disp = path.display();

            let mut file = match File::open(&path) {
                Ok(file) => file,
                Err(e) => {
                    println!("{}: {}", disp, e);
                    return;
                }
            };

            crc.start();

            while match file.read(&mut buf) {
                Ok(len) => {
                    crc.update(&buf[0..len]);
                    len > 0
                },
                Err(_) => false
            } { /* do nothing */ };

            println!("{}: {:X}", disp, crc.finalize());
        });
        children.push(r);
    }

    for child in children {
        child.join();
    }

}
