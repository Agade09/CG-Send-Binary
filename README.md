# CG-Send-Binary
Convert your locally compiled binary into a self-unpacking Python script you can paste into Codingame.com <br>
**Flagged as a cheat during contests since Wondev Woman**

## What it does:
* LZMA-compresses the binary and encodes it as base65536 text (2 bytes per Unicode code point)
* Outputs a `.py` file that, on the CG servers, decompresses the binary back to disk and runs it

## How to use:
* Compile your AI as a Linux binary
* Run `./Binary_To_Base65536.py MyBinary`
* Paste the generated `MyBinary_Base65536.py` as your program on Codingame (select Python)

`Base65536.sh` is an **example** end-to-end script (compile → strip → encode); edit the compiler
and flags to taste. It needs `xclip` only for the optional copy-to-clipboard step.

## Caution:
* Compile against a glibc **no newer** than CG's, or the binary won't find its runtime symbols.
  CG currently runs glibc 2.36; building on a much newer distro can pull in symbols it lacks.
* Do **not** compile with `-march=native` on a recent CPU. CG supports up to **AVX2**; `native`
  emits newer instructions (AVX-VNNI/GFNI/VAES/...) that crash on CG. Use `-march=x86-64-v3`.
* **No UPX.** UPX's self-extraction stub segfaults inside CG's sandbox. Compression is handled
  by stdlib LZMA in the wrapper, so the binary that runs on CG is a plain, unmodified ELF.

## Recommendations:
* Strip the binary (the example script does this) to shrink the compressor input and stay under
  Codingame's ~100k code-size limit.

## Acknowledgements:
* Original idea is from [[CPC]rOut](https://www.codingame.com/forum/t/neural-network-ressources/1667/17) on Codingame.com
