#include "MurmurClass.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

int main() {
	std::cout << "### test 1 - int, char, long long, std::string, float, const char * ###" << std::endl;
	int inA = 1, inB = 2, inC = 3;
	char inD = 's';
	long long int inE = std::numeric_limits<long long int>::max();
	std::string inF = "string input";
	const char * inG = "char * input"; // input keys
	float inH = 3.14;
	uint64_t *hash1 = new uint64_t[2]; // to get hashed output
	MurmurClass<uint64_t*, int,int,int,char,long long int, const char *, float, std::string> murmurObj1; // initiate object
	murmurObj1.apply(hash1, inA, inB, inC, inD, inE, inG, inH, inF); // apply method, updates hash
	std::cout << "Keys are : " << std::endl;
	std::cout << "\t" << inA << " " << inB << " " << inC << " "
			  << inD << " " << inE << " " << inF << " " << inG << " " << inH << std::endl;
	std::cout << std::endl;
	std::cout << "Murmurhash is : " << std::endl;
	std::cout << "\t" << hash1[0] << hash1[1] << std::endl;
	std::cout << std::endl;

	std::cout << "### test 2 - only const char * ###" << std::endl;
	const char *inA2 = "string generic input";
	uint64_t *hash2 = new uint64_t[2];
	MurmurClass<uint64_t*, const char *> murmurObj2; // initiate object
	murmurObj2.apply(hash2, inA2);
	std::cout << "Keys are : " << std::endl;
	std::cout << "\t" << inA2 << std::endl;
	std::cout << std::endl;
	std::cout << "Murmurhash is : " << std::endl;
	std::cout << "\t" << hash2[0] << hash2[1] << std::endl;
	std::cout << std::endl;

	std::cout << "### test 3 - only std::string ###" << std::endl;
	std::string inA3 = "string generic input";
	uint64_t *hash3 = new uint64_t[2];
	MurmurClass<uint64_t*, std::string> murmurObj3; // initiate object
	murmurObj3.apply(hash3, inA3);
	std::cout << "Keys are : " << std::endl;
	std::cout << "\t" << inA3 << std::endl;
	std::cout << std::endl;
	std::cout << "Murmurhash is : " << std::endl;
	std::cout << "\t" << hash3[0] << hash3[1] << std::endl;
	std::cout << std::endl;

	std::cout << "### test 4 - only const char * and std::string ###" << std::endl;
	std::string inA4 = "string input 2";
	const char *inB4 = "const char * input 2";
	uint64_t *hash4 = new uint64_t[2];
	MurmurClass<uint64_t*, std::string, const char *> murmurObj4; // initiate object
	murmurObj4.apply(hash4, inA4, inB4);
	std::cout << "Keys are : " << std::endl;
	std::cout << "\t" << inA4 << " " << inB4 << std::endl;
	std::cout << std::endl;
	std::cout << "Murmurhash is : " << std::endl;
	std::cout << "\t" << hash4[0] << hash4[1] << std::endl;
	std::cout << std::endl;

	std::cout << "### test 5 - empty key case ###" << std::endl;
	std::cout << "When Key is empty : " << std::endl;
	MurmurClass<uint64_t*> murmurEmptyObj;
	murmurEmptyObj.apply(hash1);
	delete [] hash1;
	delete [] hash2;
	delete [] hash3;
	delete [] hash4;
	return 0;
}