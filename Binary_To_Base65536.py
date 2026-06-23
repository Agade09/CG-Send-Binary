#!/usr/bin/env python3
# Encode a compiled binary into a self-contained, self-unpacking Python script for CodinGame.
#
# The binary is LZMA-compressed (Python stdlib) and embedded as base65536 text (2 bytes per
# Unicode code point). The emitted script decompresses it back to a plain ELF on disk and runs
# it. There is deliberately NO UPX step: UPX's self-extraction stub segfaults inside CodinGame's
# sandbox, and stdlib LZMA compresses at least as well while leaving the on-disk binary a plain,
# unmodified executable (see README).
import sys
import io
import lzma

assert len(sys.argv) >= 2, "usage: Binary_To_Base65536.py <binary>"

# Maps the second byte (b2) of each 2-byte group to the start of a Unicode block. The byte pair
# (b1, b2) is encoded as the single code point BLOCK_START[b2] + b1. The 68352 entry (b2 == -1)
# encodes a trailing odd byte. These blocks are valid, assignable code points on CodinGame.
B2 = {**{256+i*256:i for i in range(0,6)},**{2048+(i-6)*256:i for i in range(6,13)},**{4096+(i-13)*256:i for i in range(13,213)},**{57344+(i-213)*256:i for i in range(213,256)},**{68352:-1}}

BLOCK_START = {item: key for key, item in B2.items()}


def encode(value):
    stream = io.StringIO()
    length = len(value)
    for x in range(0, length, 2):
        b1 = value[x]
        b2 = value[x + 1] if x + 1 < length else -1
        stream.write(chr(BLOCK_START[b2] + b1))
    return stream.getvalue()


Binary_Name = sys.argv[1]
with open(Binary_Name, "rb") as f:
    raw = f.read()
compressed = lzma.compress(raw, preset=9 | lzma.PRESET_EXTREME)
payload = encode(compressed)

with open(Binary_Name + "_Base65536.py", "w", encoding="utf-8") as out:
    out.write("#!/usr/bin/env python3\n")
    out.write("# -*- coding: utf-8 -*-\n")
    out.write("import os,io,lzma\n")
    out.write("B2=" + str(B2).replace(" ", "") + "\n")
    out.write("def decode(value):\n")
    out.write("    stream=io.BytesIO()\n")
    out.write("    for ch in value:\n")
    out.write("        code_point=ord(ch)\n")
    out.write("        b1=code_point&255\n")
    out.write("        b2=B2[code_point-b1]\n")
    out.write("        stream.write(bytes([b1]) if b2==-1 else bytes([b1,b2]))\n")
    out.write("    return stream.getvalue()\n")
    out.write('binary_data=u"')
    out.write(payload)
    out.write('"\n')
    out.write("open('CG','wb').write(lzma.decompress(decode(binary_data)))\n")
    out.write('os.system("chmod a+x CG")\n')
    out.write('os.system("./CG")\n')
