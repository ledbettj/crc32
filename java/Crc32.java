import java.io.FileInputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

public class Crc32 {

    private int value;
    private int[] table = new int[256];

    private static final int SEED = 0xEDB88320;

    public Crc32() {

        for(int i = 0; i < 256; ++i) {
            int v = i;
            for(int j = 0; j < 8; ++j) {
                v = (v & 1) == 1 ? (SEED ^ (v >>> 1)) : (v >>> 1);
            }
            this.table[i] = v;
        }
    }

    public void start() {
        value = 0xFFFFFFFF;
    }

    public void update(byte[] bytes, int length) {
        for(int i = 0; i < length; ++i) {
            this.value = this.table[((this.value ^ bytes[i]) & 0xFF)] ^ (this.value >>> 8);
        }
    }


    public int result() {
        return (this.value ^ 0xFFFFFFFF);
    }

    public static void main(String[] args) throws IOException {
        Crc32 crc  = new Crc32();
        byte[] buf = new byte[1024 * 1024];
        int len;

        for (String filename : args) {
            FileInputStream fs = null;
            try {
                fs = new FileInputStream(filename);
                crc.start();
                while((len = fs.read(buf)) > 0) {
                    crc.update(buf, len);
                }

                System.out.format("%s: %08x\n", filename, crc.result());

            } catch (FileNotFoundException ex) {

            } finally {
                if (fs != null) {
                    fs.close();
                }
            }
        }

    }
}


