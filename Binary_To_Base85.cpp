#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

constexpr char en85[]{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~"};
char de85[256];

constexpr char half1[]=
"#pragma GCC optimize(\"Ofast,inline,omit-frame-pointer,unroll-loops\")\n"
"#include<algorithm>\n"
"#include<fstream>\n"
"using namespace std;\n"

"constexpr char en85[]{\"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~\"};\n"
"char de85[256];\n"
"const string Base85_Binary{\"";

constexpr char half2[]="\"};"

"string decode_85(const string &base85_string){"
	"string out;"
	"size_t str_ptr{0};"
	"while(str_ptr<base85_string.size()){"
		"unsigned int acc{0};"
		"for(int i=0;i<5;++i){"
			"unsigned char ch=base85_string[str_ptr++];"
			"int de{de85[ch]};"
			"acc=acc*85+de;"
		"}"
		"string sub;"
		"for(int i=0;i<4;++i){"
			"int val=acc%256;"
			"acc/=256;"
			"sub+=static_cast<char>(val);"
		"}"
		"reverse(sub.begin(),sub.end());"
		"out+=sub;"
	"}"
	"return out;"
"}"

"int main(){"
	"for(int i=0;i<85;i++){"
		"int ch{en85[i]};"
		"de85[ch]=i;"
	"}"
	"const string Program_Binary{decode_85(Base85_Binary)};"
	"ofstream Program_File(\"Binary\",ios::binary | ios::trunc);"
	"Program_File.write(Program_Binary.c_str(),Program_Binary.size());"
	"Program_File.close();"
	"system(\"chmod a+x Binary\");"
	"system(\"./Binary\");"
"};";

string decode_85(const string &base85_string){
	string out;
	size_t str_ptr{0};
	while(str_ptr<base85_string.size()){
		unsigned int acc{0};
		for(int i=0;i<5;++i){
			unsigned char ch=base85_string[str_ptr++];
			int de{de85[ch]};
			acc=acc*85+de;
		}
		string sub;
		for(int i=0;i<4;++i){
			int val=acc%256;
			acc/=256;
			sub+=static_cast<char>(val);
		}
		reverse(sub.begin(),sub.end());
		out+=sub;
	}
	return out;
}

string encode_85(const vector<unsigned char> &data){
	string out;
	int data_ptr{0};
	while(data_ptr<data.size()){
		unsigned int acc{0};
		int bytes=min(static_cast<size_t>(4),data.size()-data_ptr);
		for(int i=0;i<bytes;++i){
			unsigned char ch=data[data_ptr++];
			acc+=pow(256,3-i)*ch;
		}
		string sub;
		for(int i=0;i<5;++i){
			int val=acc%85;
			acc/=85;
			sub+=en85[val];
		}
		reverse(sub.begin(),sub.end());
		out+=sub;
	}
	return out;
}

int main(int argc,char** argv){
	if(argc<2){
		cerr << "Program takes at least one argument, the name of the program to convert to base 85" << endl;
	}
	const string Program_Name{argv[1]};
	ifstream Program_File(Program_Name,ios::binary | ios::ate);
	if(!Program_File){
		cerr << "Couldn't open " << Program_Name << endl;
		return 0;
	}
	const streamsize binary_size{Program_File.tellg()};
	Program_File.seekg(0,ios::beg);
	vector<unsigned char> Program_Binary(binary_size);
	Program_File.read(reinterpret_cast<char*>(Program_Binary.data()),binary_size);
	for(int i=0;i<85;i++){
		int ch{en85[i]};
		de85[ch]=i;
	}
	const string base85_Program_Binary=encode_85(Program_Binary);
	ofstream Program_Base85_File(Program_Name+"_Base85.cpp",ios::trunc);
	Program_Base85_File << half1 << base85_Program_Binary << half2 << endl;

	if(argc>=3){//Append source code as comment
		const string Source_Filename{argv[2]};
		ifstream Source_File(Source_Filename,ios::ate);
		const streamsize source_size{Source_File.tellg()};
		cerr << source_size << endl;
		Source_File.seekg(0,ios::beg);
		vector<char> Source(source_size);
		Source_File.read(reinterpret_cast<char*>(Source.data()),source_size);
		const string source_code(Source.begin(),Source.end());
		Program_Base85_File << "#if 0" << endl;
		Program_Base85_File << source_code << endl;
		Program_Base85_File << "#endif" << endl;
	}
}