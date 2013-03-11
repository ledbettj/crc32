#!/usr/bin/env ruby

class CRC32
  SEED = 0xEDB88320

  def initialize
    @table = 256.times.map do |i|
      8.times.reduce(i){ |c, _| c & 1 == 1 ? SEED ^ (c >> 1) : c >> 1 }
    end
  end

  def crc(input)
    start
    update(input)
    finalize
  end

  def start
    @value = 0xffffffff
  end

  def update(input)
    @value = input.each_byte.reduce(@value) do |c, ch|
      @table[(c ^ ch) & 0xFF] ^ (c >> 8)
    end
  end

  def finalize
    @value ^ 0xffffffff
  end
end

crc32 = CRC32.new

ARGV.each do |file_name|
  File.open(file_name, 'r') do |f|
    crc32.start
    while buf = f.read(1024 * 1024)
      crc32.update(buf)
    end
    puts "#{file_name}: %08X" % crc32.finalize
  end rescue nil
end
