#ifndef _MURMURCLASS_H_
#define _MURMURCLASS_H_

/******************************************************************************
 * Author: Pratyush Kumar
 * Last Updated: June 19, 2017
 * Purpose: Implementation of template based Murmurhash class
 *****************************************************************************/

#include <iostream>
#include <string>
#include <tuple>
#include <utility>
#include <functional>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "MurmurHash3.h"

// HashT = type of hash (uint64_t, uint32_t), Arguments = Tuple of keys
template <typename HashT, typename... Arguments>
class MurmurClass {
  public:
    MurmurClass() : bytesArraySize(0) {}
    void apply(HashT hash, Arguments... args) {
        tuple_ = tupleLambda(args...); // calling lambda function to construct keys tuple
        unpackArgs(tuple_); // unpack tuple
        if (!bytesArraySize) { // if no key provided
            std::cerr << "Nothing to hash!" << std::endl;
        } else {
            unsigned char *inKeyBuffer = &bytesArray[0];
            //srand((unsigned)time(0));
            //uint32_t seed = rand(); // random seed based on time
            uint32_t seed = 1;
            MurmurHash3_x64_128((void *)inKeyBuffer, bytesArraySize, seed, (void *)hash); // update hash
            // can use MurmurHash3_x86_32  / MurmurHash3_x86_128 depending on system
        }
    }

  private:
    std::tuple<Arguments...> tuple_; // to store keys
    static const unsigned int numParam = sizeof...(Arguments);
    std::vector<unsigned char> bytesArray; // to store keys byte wise
    size_t bytesArraySize; // keeps track of size of byte array
    std::function<std::tuple<Arguments...>(Arguments... args)> tupleLambda = [](Arguments... args)  {
        return std::make_tuple(args...); // lambda function returns tuple
    };

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type
    unpackArgs(std::tuple<Tp...>& t) {} // unpack function base case

    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    unpackArgs(std::tuple<Tp...>& t) { // unpack function
        auto currentItem = std::get<I>(t);
        auto prevBytesArraySize = bytesArraySize;
        bytesArraySize += sizeof(currentItem);
        bytesArray.resize(bytesArraySize);
        std::memcpy(bytesArray.data() + prevBytesArraySize, &currentItem, sizeof(currentItem)); // copy to array byte wise
        unpackArgs<I + 1, Tp...>(t);
    }
};


#endif