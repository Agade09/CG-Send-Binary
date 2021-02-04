#!/usr/bin/env python
import sys
import io
from six import indexbytes, unichr

assert len(sys.argv)>=1

B2 = {**{256+i*256:i for i in range(0,6)},**{2048+(i-6)*256:i for i in range(6,13)},**{4096+(i-13)*256:i for i in range(13,213)},**{57344+(i-213)*256:i for i in range(213,256)},**{68352:-1}}

BLOCK_START = {item:key for key,item in B2.items()}

#TEST
for key in B2:
	for i in range(256):
		try:
			unichr(key+i)
		except:
			print(key+i,"is invalid unicode point")


def encode(value):
    stream = io.StringIO()
    length = len(value)
    for x in range(0, length, 2):
        b1 = indexbytes(value, x)
        b2 = indexbytes(value, x + 1) if x + 1 < length else -1
        code_point = BLOCK_START[b2] + b1
        stream.write(unichr(code_point))
    return stream.getvalue()

Binary_Name = sys.argv[1]
Binary_file = open(Binary_Name, "rb")
binary_data = Binary_file.read()
Binary_file.close()
binary_data = encode(binary_data)

Reconstruction_File = open(Binary_Name+"_Base65536.py",'w')
Reconstruction_File.write("#!/usr/bin/env python\n")
Reconstruction_File.write("# -*- coding: utf-8 -*-\n")
Reconstruction_File.write("import os\n")
Reconstruction_File.write("import io\n")
Reconstruction_File.write("from six import int2byte\n")
Reconstruction_File.write("B2 = "+"{{".format()+str(B2)[1:-1].replace(' ','')+"}}".format()+"\n")
Reconstruction_File.write("def decode(value):\n")
Reconstruction_File.write("    stream = io.BytesIO()\n")
Reconstruction_File.write("    done = False\n")
Reconstruction_File.write("    for ch in value:\n")
Reconstruction_File.write("        code_point = ord(ch)\n")
Reconstruction_File.write("        b1 = code_point & ((1 << 8) - 1)\n")
Reconstruction_File.write("        b2 = B2[code_point - b1]\n")
Reconstruction_File.write("        b = int2byte(b1) if b2 == -1 else int2byte(b1) + int2byte(b2)\n")
Reconstruction_File.write("        if len(b) == 1:\n")
Reconstruction_File.write("            done = True\n")
Reconstruction_File.write("        stream.write(b)\n")
Reconstruction_File.write("    return stream.getvalue()\n")
Reconstruction_File.write('binary_data=u"')
Reconstruction_File.write(binary_data)
Reconstruction_File.write('"\n')
Reconstruction_File.write("binary_data=decode(binary_data)\n")
Reconstruction_File.write('binary_file=open("CG","wb")\n')
Reconstruction_File.write('binary_file.write(binary_data)\n')
Reconstruction_File.write('binary_file.close()\n')
Reconstruction_File.write('os.system("chmod a+x CG")\n')
Reconstruction_File.write('os.system("./CG")\n')