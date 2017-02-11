#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

constexpr char en85[]{"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~"};
char de85[256];

constexpr char half1[]=
"#pragma GCC optimize(\"Ofast,inline,omit-frame-pointer,unroll-loops\")\n"
"#include <fstream>\n"
"using namespace std;\n"

"constexpr char en85[]{\"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~\"};\n"
"char de85[256];\n"
"const string Base85_Binary{\"";

constexpr char half2[]="\"};"

"string decode_85(const string &base85_string){"
	"string out;"
	"size_t str_ptr{0};"
	"while(str_ptr<base85_string.size()-1){"
		"int cnt{min(static_cast<size_t>(4),base85_string.size()-str_ptr-1)};"
		"unsigned int acc{0};"
		"for(int i=0;i<4;++i){"
			"unsigned char ch=base85_string[str_ptr++];"
			"int de{de85[ch]-1};"
			"acc=acc*85+de;"
		"}"
		"unsigned char ch{base85_string[str_ptr++]};"
		"int de{de85[ch]-1};"
		"if (0xffffffff / 85 < acc || 0xffffffff - de < (acc*=85)){"
		    "throw(0);"
		"}"
		"acc+=de;"
		"for(int i=0;i<cnt;++i){"
			"acc = (acc << 8) | (acc >> 24);"
			"out+=static_cast<char>(acc);"
		"}"
	"}"
	"return out;"
"}"

"int main(){"
	"for(int i=0;i<85;i++){"
		"int ch{en85[i]};"
		"de85[ch]=i+1;"
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
	while(str_ptr<base85_string.size()-1){
		int cnt=min(static_cast<size_t>(4),base85_string.size()-str_ptr-1);
		unsigned int acc{0};
		for(int i=0;i<4;++i){
			unsigned char ch=base85_string[str_ptr++];
			int de{de85[ch]-1};
			acc=acc*85+de;
		}
		unsigned char ch=base85_string[str_ptr++];
		int de{de85[ch]-1};
		if (0xffffffff / 85 < acc || 0xffffffff - de < (acc*=85)){
		    throw(0);
		}
		acc+=de;
		for(int i=0;i<cnt;++i){
			acc = (acc << 8) | (acc >> 24);
			out+=static_cast<char>(acc);
		}
	}
	return out;
}

string encode_85(const vector<unsigned char> &data){
	string out;
	int data_ptr{0};
	while(data_ptr<data.size()){
		unsigned int acc{0};
		for(int cnt=24;cnt>=0;cnt-=8){
			unsigned int ch=data[data_ptr++];
			acc|=ch<<cnt;
			if(data_ptr==data.size()){
				break;
			}
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
	if(argc!=2){
		cerr << "Program takes one argument, the name of the program to convert to base 85" << endl;
	}
	string Program_Name{argv[1]};
	ifstream Program_File(Program_Name,ios::binary | ios::ate);
	if(!Program_File){
		cerr << "Couldn't open " << Program_Name << endl;
		return 0;
	}
	streamsize size = Program_File.tellg();
	Program_File.seekg(0,ios::beg);
	vector<unsigned char> Program_Binary(size);
	Program_File.read(reinterpret_cast<char*>(Program_Binary.data()),size);
	for(int i=0;i<85;i++){
		int ch{en85[i]};
		de85[ch]=i+1;
	}
	const string base85_Program_Binary=encode_85(Program_Binary);
	ofstream Program_Base85_File(Program_Name+"_Base85.cpp",ios::trunc);
	Program_Base85_File << half1 << base85_Program_Binary << half2 << endl;
}