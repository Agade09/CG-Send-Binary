if [ $# -lt 1 ]; then
    echo "$0: Takes at least one argument, the name of the AI to compile"
    exit 1
fi
Compiler=g++-12 #clang++ #June 2026: CG crashes with gcc-13
AI_Name=$1 #Get the AI name you passed as a command line parameter
AI_Name="${AI_Name%.*}" #Remove extension in case you passed "V4.cpp" instead of "V4"
if [ -f "$AI_Name".cpp ]; then #If the file exists
	#Make a copy of your code without pragmas to avoid interfering with the -O3/-Ofast compilation
	cp "$AI_Name".cpp "$AI_Name"_nopragma.cpp
	sed -i '/^#pragma/d' "$AI_Name"_nopragma.cpp

	#Compile your AI. NOTE: do NOT use -march=native -- on a recent CPU it emits instructions
	#(AVX-VNNI/GFNI/VAES/...) that CodinGame's older CPUs cannot run, which crashes the bot.
	#CodinGame supports up to AVX2, so x86-64-v3 (AVX2+FMA+BMI2) is the safe, fast target.
	"$Compiler" "$AI_Name"_nopragma.cpp -o "$AI_Name" -Ofast -march=native -std=c++17 -w -s -flto -fno-rtti -fno-math-errno -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-z,norelro -Wl,--hash-style=gnu #-fno-exceptions

	#Strip to shrink the input to the compressor. NOTE: no UPX -- its self-extraction stub
	#segfaults inside CodinGame's sandbox. Binary_To_Base65536.py LZMA-compresses instead.
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag "$AI_Name"

	#Make the copy-pastable self-unpacking .py (LZMA-compressed, base65536-encoded)
	./Binary_To_Base65536.py "$AI_Name"

	#Cleanup
	rm "$AI_Name" #Remove the AI binary
	rm "$AI_Name"_nopragma.cpp #Remove the copy of your code
	xclip -selection c <  "$AI_Name"_Base65536.py #Puts the copy-pastable .py file into your clipboard for quicker copy pasting
else
	echo "Could not find $AI_Name.cpp"
fi
