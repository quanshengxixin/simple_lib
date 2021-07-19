import os
import struct
import numpy as np

def pcm2txt(file_path, out_file_path, bitdepth):
    out_fid = open(out_file_path, "w")

    with open(file_path, "rb") as fd:
        while True:
            item = fd.read(bitdepth // 8)
            if item == b"":
                break
            if bitdepth == 32:
                data = struct.unpack("I", item)
                out_fid.write("0x%08X\n" % data)
            elif bitdepth == 16:
                data = struct.unpack("H", item)
                out_fid.write("0x%04X\n" % data)

    out_fid.close()


def txt2pcm(file_path, out_file_path, bitdepth):
    
    if bitdepth == 32:
        data = np.loadtxt(file_path, dtype=np.uint32, converters = {_ : lambda s: int(s,16) for _ in range(1)})
    elif bitdepth == 16:
        data = np.loadtxt(file_path, dtype=np.uint16, converters = {_ : lambda s: int(s,16) for _ in range(1)})

    out_fid = open(out_file_path, "wb")
    out_fid.write(data)
    out_fid.close()


if __name__ == "__main__":
    bitdepth = 32
    file_name = "44100_32bit_2ch.pcm"
    file_path = os.path.abspath(os.path.join(".", file_name))

    out_file_name = os.path.splitext(file_name)[0] + '.txt'
    out_file_path = os.path.abspath(os.path.join(".", out_file_name))
    
    pcm2txt(file_path, out_file_path, bitdepth)

    
    bitdepth = 16
    file_name = "44100_16bit_2ch.txt"
    file_path = os.path.abspath(os.path.join(".", file_name))

    out_file_name = os.path.splitext(file_name)[0] + '.pcm'
    out_file_path = os.path.abspath(os.path.join(".", out_file_name))
    
    txt2pcm(file_path, out_file_path, bitdepth)

