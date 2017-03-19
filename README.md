# CG-Send-Binary
Convert your locally compiled binary to a C++ code/text format that you can use on Codingame.com

## What it does:
* Converts the binary to base85 text encoding
* Outputs a .cpp file with code to recreate the binary file on the CG servers and run it

## How to use:
* Compile this tool 
* Compile your AI as a linux binary
* Run this tool on the linux binary
* Use the "_Base85.cpp" file as your program on Codingame

## Caution:
* If you compile your AI with a different version of the compiler to what CG is using the binary may not run due to looking for a non-existing version of the runtime libraries

## Recommendations:
* Strip the binary
* Compress it with [UPX](https://upx.github.io/)

before using this tool, to reduce the chance of hitting the 100ko code size limit of Codingame.

The included Base85.sh shell script is meant as an **example** of using this tool. It will not work on your machine if your not on linux, your PATH is not correct and/or you have not installed UPX.

## Credits:
* Original idea is from [[CPC]rOut](https://www.codingame.com/forum/t/neural-network-ressources/1667/17) on Codingame.com
