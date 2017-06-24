# Bigstream-Murmur #

## Usage ##
	$ make clean
	$ make

This will compile the classes and the test case. Then run the test case:
> `$ ./testMurmur`

The parameters to the class can be adjusted in the test case and re-compiled.
Class `MurmurClass` object initiated with hash type, followed by key types.
The `apply()` method is called with hash buffer and keys as parameter.
Example:
> `MurmurClass<uint32_t, int, char, std::string> murmurObj;`
> `murmurObj.apply(hashBuffer, 1, 'a', stringItem);`
> `std::cout << hashBuffer[0] << hashBuffer[1];`

## Notes ##
1. At each step of unpacking parameters/ keys, we update the byte array to store the byte representation of the parameter. This array grows at each call to unpack function, while keeping track of array size.
2. The Murmur hashing algorithm is given by Austin Appleby (https://github.com/aappleby/smhasher/tree/master/src)
3. The hashing class is not portable in the sense that the byte ordering is not consistent across different machines (big endian, little endian not considered). It would take considerably longer to work on byte addressing for each type, but the current program would work fine on a single system for non-cryptographic use.
4. I have avoided the use of C++14 features, which limited the use of library functions like make_index_sequence useful for tuple unpacking. Alternatively, I used stack-overflow answer for tuple iterations (https://stackoverflow.com/questions/1198260/iterate-over-tuple).
5. Works for C++11 - gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-11)
6. Currently hashes for x64_128 system but can be changed for other systems in the test case.
7. Random seed based on time. Program can be run multiple times to get different hashes based on seed. - currently commented out
8. Class definition and implementation kept at one place due to its ease working with templates

## Updates ##
1. Updated with string and c-string to correctly convert them to their byte representation by overloading variadic templates.

## Testing ##
Currently tested for:
1. No keys
2. Only ints
3. Ints, char, long, double, float, long long, char *, std::string
4. char * and std::string unpacking and their various combinations
