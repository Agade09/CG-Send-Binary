if [ $# -lt 1 ]; then
    echo "$0: Takes at least one argument, the name of the AI to compile"
    exit 1
fi
Compiler=g++-10
AI_Name=$1 #Get the AI name you passed as a command line parameter
AI_Name="${AI_Name%.*}" #Remove extension in case you passed "V4.cpp" instead of "V4"
if [ -f "$AI_Name".cpp ]; then #If the file exists
	#Make a copy of your code without pragmas to avoid interfering with the -O3/-Ofast compilation
	cp "$AI_Name".cpp "$AI_Name"_nopragma.cpp 
	sed -i '/^#pragma/d' "$AI_Name"_nopragma.cpp

	#Compile your AI with whatever options you like
	"$Compiler" "$AI_Name"_nopragma.cpp -o "$AI_Name" -Ofast -march=native -std=c++17 -w -s -flto -fno-rtti -fno-math-errno -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-z,norelro -Wl,--hash-style=gnu #-fno-exceptions

	#strip+UPX to reduce the size of the binary without affecting correctness or performance
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag "$AI_Name"
	upx "$AI_Name" --ultra-brute --no-backup
	Binary_To_Base65536.py "$AI_Name" "$AI_Name".cpp #Make the copy-pastable .cpp file containing the base85-encoded binary
    Base65536_Filename="$AI_Name"_Base85.cpp
    #Base65536_Size=$(wc -c <"$Base65536_Filename")
    If0_Line=$(sed -n '/#if 0/=' $Base65536_Filename)
    let If0_NextLine=$If0_Line+2
    if [[ $If0_Line -ne 0 ]]; then
        Compiler_Version=$("$Compiler" --version | head -n 1)
        sed -i "$If0_NextLine"' s/^/\/\/Compiled with '"$Compiler_Version"'\n/' $Base65536_Filename
    fi
	
	#Cleanup
	rm "$AI_Name" #Remove the AI binary
	rm "$AI_Name"_nopragma.cpp #Remove the copy of your code
	xclip -selection c <  "$AI_Name"_Base65536.py #Puts the copy-pastable .cpp file into your clipboard for quicker copy pasting
else
	echo "Could not find $AI_Name.cpp"
fi
