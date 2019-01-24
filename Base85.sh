if [ $# -ne 1 ]; then
    echo "$0: Takes one argument, the name of the AI to compile"
    exit 1
fi
AI_Name=$1 #Get the AI name you passed as a command line parameter
AI_Name="${AI_Name%.*}" #Remove extension in case you passed "V4.cpp" instead of "V4"
if [ -f "$AI_Name".cpp ]; then #If the file exists
	#Make a copy of your code without pragmas to avoid interfering with the -O3/-Ofast compilation
	cp "$AI_Name".cpp "$AI_Name"_nopragma.cpp 
	sed -i '/^#pragma/d' "$AI_Name"_nopragma.cpp

	#Compile your AI with whatever options you like
	clang++-8 "$AI_Name"_nopragma.cpp -o "$AI_Name" -Ofast -flto -std=c++17 -w -s -fno-rtti -fno-math-errno -fno-exceptions -ffunction-sections -fdata-sections -Wl,--gc-sections -Wl,-z,norelro -Wl,--hash-style=gnu

	#strip+UPX to reduce the size of the binary without affecting correctness or performance
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag "$AI_Name"
	upx "$AI_Name" --ultra-brute --no-backup
	Binary_To_Base85 "$AI_Name" #Make the copy-pastable .cpp file containing the base85-encoded binary
	
	#Cleanup
	rm "$AI_Name" #Remove the AI binary
	rm "$AI_Name"_nopragma.cpp #Remove the copy of your code
	xclip -selection c <  "$AI_Name"_Base85.cpp
else
	echo "Could not find $AI_Name.cpp"
fi
