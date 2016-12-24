#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

static const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const string half1="#include <iostream>\n"
"#include <fstream>\n"
"using namespace std;"
"static const string base64_chars = \"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/\";"
"const string Base64_Binary{\"";

const string half2="\"};"

"static inline bool is_base64(unsigned char c){"
"  return (isalnum(c)||(c=='+')||(c=='/'));"
"}"

"std::string base64_decode(std::string const& encoded_string) {"
	"int in_len = encoded_string.size();"
	"int i = 0,j = 0,in_ = 0;"
	"unsigned char char_array_4[4], char_array_3[3];"
	"std::string ret;"

	"while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {"
		"char_array_4[i++] = encoded_string[in_]; in_++;"
		"if(i==4){"
		  "for (i = 0; i <4; i++)"
		    "char_array_4[i] = base64_chars.find(char_array_4[i]);"

			"char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);"
			"char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);"
			"char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];"

			"for (i = 0; (i < 3); i++){"
				"ret += char_array_3[i];"
			"}"
		  	"i=0;"
		"}"
	"}"

	"if(i){"
		"for (j = i; j <4; j++){"
		  	"char_array_4[j] = 0;"
		"}"

		"for (j = 0; j <4; j++){"
		  "char_array_4[j] = base64_chars.find(char_array_4[j]);"
		"}"

		"char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);"
		"char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);"
		"char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];"

		"for (j = 0; (j < i - 1); j++){"
			"ret += char_array_3[j];"
		"}" 
	"}"
	"return ret;"
"}"

"int main(){"
	"const string Program_Binary{base64_decode(Base64_Binary)};"
	"ofstream Program_File(\"Binary\",ios::binary | ios::trunc);"
	"Program_File.write(Program_Binary.c_str(),Program_Binary.size());"
	"Program_File.close();"
	"system(\"chmod a+x Binary\");"
	"system(\"./Binary\");"
"}";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	string ret;
	int i=0,j=0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	while(in_len--){
	char_array_3[i++] = *(bytes_to_encode++);
		if(i == 3){
			char_array_4[0]=(char_array_3[0] & 0xfc) >> 2;
			char_array_4[1]=((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2]=((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3]=char_array_3[2] & 0x3f;
			for(i=0;i<4;i++){
				ret += base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}
	if(i){
		for(j = i; j < 3; j++){
		  	char_array_3[j] = '\0';
		}
		char_array_4[0]=(char_array_3[0] & 0xfc) >> 2;
		char_array_4[1]=((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2]=((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3]=char_array_3[2] & 0x3f;
		for(j=0;j<i+1;++j){
	  		ret += base64_chars[char_array_4[j]];
		}
		while(i++<3){
		  	ret += '=';
		}
	}

	return ret;
}

std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0,j = 0,in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if(i==4){
		  for (i = 0; i <4; i++)
		    char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++){
				ret += char_array_3[i];
			}
		  	i = 0;
		}
	}

	if(i){
		for (j = i; j <4; j++){
		  	char_array_4[j] = 0;
		}

		for (j = 0; j <4; j++){
		  char_array_4[j] = base64_chars.find(char_array_4[j]);
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++){
			ret += char_array_3[j];
		} 
	}
	return ret;
}

int main(int argc,char** argv){
	if(argc!=2){
		cerr << "Program takes one argument, the name of the program to convert to base 64" << endl;
	}
	string Program_Name{argv[1]};
	ifstream Program_File(Program_Name,ios::binary | ios::ate);
	if(!Program_File){
		cerr << "Couldn't open " << Program_Name << endl;
		return 0;
	}
	streamsize size = Program_File.tellg();
	Program_File.seekg(0,ios::beg);
	vector<char> Program_Binary(size);
	Program_File.read(Program_Binary.data(),size);
	const string base64_Program_Binary=base64_encode(reinterpret_cast<unsigned char*>(Program_Binary.data()),size);
	ofstream Program_Base64_File(Program_Name+"_Base64.cpp",ios::trunc);
	Program_Base64_File << half1 << base64_Program_Binary << half2 << endl;
}
