#include "MurmurClass.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

int main() {
	int inA = 1;
	int inB = 2;
	int inC = 3;
	char inD = 's';
	long long int inE = std::numeric_limits<long long int>::max();
	std::string inF = "string input";
	const char * inG = "char * input"; // input keys
	uint64_t *hash = new uint64_t[2]; // to get hashed output
	MurmurClass<uint64_t*, int,int,int,char,long long int, std::string, const char *> murmurObj; // initiate object
	murmurObj.apply(hash, inA, inB, inC, inD, inE, inF, inG); // apply method, updates hash
	std::cout << "Keys are : " << std::endl;
	std::cout << "\t" << inA << " " << inB << " " << inC << " "
			  << inD << " " << inE << " " << inF << " " << inG << std::endl;
	std::cout << std::endl;
	std::cout << "Murmurhash is : " << std::endl;
	std::cout << "\t" << hash[0] << hash[1] << std::endl;
	std::cout << std::endl;
	std::cout << "When Key is empty : " << std::endl;
	MurmurClass<uint64_t*> murmurEmptyObj;
	murmurEmptyObj.apply(hash);
	delete [] hash;
	return 0;
}