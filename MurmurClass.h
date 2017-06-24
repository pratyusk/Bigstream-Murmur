#ifndef _MURMURCLASS_H_
#define _MURMURCLASS_H_

/******************************************************************************
 * Author: Pratyush Kumar
 * Last Updated: June 24, 2017
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
#include <type_traits>
#include <typeinfo>
#include "MurmurHash3.h"

// to check if the type is std::string or not
template<class T>
struct is_str : std::integral_constant<
                bool,
                std::is_same<std::string, typename std::decay<T>::type>::value> {};

// to check if the type is c-string or not
template<class T>
struct is_c_str : std::integral_constant<
                  bool,
                  std::is_same<char const *, typename std::decay<T>::type>::value ||
                  std::is_same<char *, typename std::decay<T>::type>::value> {};


// HashT = type of hash (uint64_t, uint32_t), Arguments = Tuple of keys
template <typename HashT, typename... Arguments>
class MurmurClass {
  public:
    MurmurClass() : bytesArraySize(0) {}
    void apply(HashT hash, Arguments... args) {
        tuple_ = tupleLambda(args...); // calling lambda function to construct keys tuple
        std::cout << "number of arguments: " << numParam << std::endl;
        unpackArgs(tuple_); // unpack tuple
        if (!bytesArraySize) { // if no key provided
            std::cerr << "Nothing to hash!" << std::endl;
        } else {
            unsigned char *inKeyBuffer = &bytesArray[0];
            uint32_t seed = 1; // can be changed
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

    // case for no keys
    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<sizeof...(Tp) == 0, void>::type
    unpackArgs(std::tuple<Tp...>& t) {}

    // unpack function entry case; get the first item and pass to next template
    template<std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp), void>::type
    unpackArgs(std::tuple<Tp...>& t) {
        auto firstItem = std::get<I>(t);
        unpackArgs<I + 1, decltype(firstItem), Tp...>(t, firstItem);
    }

    // unpack function middle case (not string or c-string)
    template<std::size_t I = 0, typename Ti, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp)
                                       && !(is_str<Ti>::value)
                                       && !(is_c_str<Ti>::value), void>::type
    unpackArgs(std::tuple<Tp...>& t, Ti prevItem) {
        auto currentItem = std::get<I>(t);
        auto prevBytesArraySize = bytesArraySize;
        std::cout << "I: " << I << std::endl;
        std::cout << "currentItem: " << prevItem << std::endl;
        std::cout << "currentItem size: " << sizeof(prevItem) << std::endl;
        bytesArraySize += sizeof(prevItem);
        bytesArray.resize(bytesArraySize);
        std::memcpy(bytesArray.data() + prevBytesArraySize, &prevItem, sizeof(prevItem)); // copy to array byte wise
        unpackArgs<I + 1, decltype(currentItem), Tp...>(t, currentItem);
    }

    // unpack function middle case (string)
    template<std::size_t I = 0, typename Ti, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp)
                                       && (is_str<Ti>::value)
                                       && !(is_c_str<Ti>::value), void>::type
    unpackArgs(std::tuple<Tp...>& t, Ti prevItem) {
        auto currentItem = std::get<I>(t);
        auto prevBytesArraySize = bytesArraySize;
        std::cout << "I: " << I << std::endl;
        std::cout << "currentItem: " << prevItem << std::endl;
        std::cout << "currentItem size: " << prevItem.length() + 1 << std::endl;
        bytesArraySize += prevItem.length() + 1; // +1 to include the null character
        bytesArray.resize(bytesArraySize);
        // strcpy(bytesArray.data() + prevBytesArraySize, prevItem.c_str());  // copy to array each character byte wise
        std::memcpy(bytesArray.data() + prevBytesArraySize, prevItem.c_str(), strlen(prevItem.c_str()) + 1); // copy to array byte wise
        unpackArgs<I + 1, decltype(currentItem), Tp...>(t, currentItem);
    }

    // unpack function middle case (c-string)
    template<std::size_t I = 0, typename Ti, typename... Tp>
    inline typename std::enable_if<I < sizeof...(Tp)
                                       && !(is_str<Ti>::value)
                                       && (is_c_str<Ti>::value), void>::type
    unpackArgs(std::tuple<Tp...>& t, Ti prevItem) {
        auto currentItem = std::get<I>(t);
        auto prevBytesArraySize = bytesArraySize;
        std::cout << "I: " << I << std::endl;
        std::cout << "currentItem: " << prevItem << std::endl;
        std::cout << "currentItem size: " << strlen(prevItem) + 1 << std::endl;
        bytesArraySize += strlen(prevItem) + 1; // +1 to include the null character
        bytesArray.resize(bytesArraySize);
        std::memcpy(bytesArray.data() + prevBytesArraySize, prevItem, strlen(prevItem) + 1); // copy to array byte wise
        unpackArgs<I + 1, decltype(currentItem), Tp...>(t, currentItem);
    }

    // unpack function last item (not string or c-string)
    template<std::size_t I = 0, typename Ti, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp)
                                       && !(is_str<Ti>::value)
                                       && !(is_c_str<Ti>::value), void>::type
    unpackArgs(std::tuple<Tp...>& t, Ti lastItem) {
        auto prevBytesArraySize = bytesArraySize;
        std::cout << "I: " << I << std::endl;
        std::cout << "lastItem: " << lastItem << std::endl;
        std::cout << "lastItem size: " << sizeof(lastItem) << std::endl;
        bytesArraySize += sizeof(lastItem);
        bytesArray.resize(bytesArraySize);
        std::memcpy(bytesArray.data() + prevBytesArraySize, &lastItem, sizeof(lastItem)); // copy to array byte wise
        // for (auto i = 0; i < bytesArraySize; i++) {
        //     std::cout << bytesArray[i] << std::endl;
        // }
        // std::cout << "bytesArraySize matches : " << (bytesArraySize == bytesArray.size()) << std::endl;
    }

    // unpack function last item (string)
    template<std::size_t I = 0, typename Ti, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp)
                                       && (is_str<Ti>::value)
                                       && !(is_c_str<Ti>::value), void>::type
    unpackArgs(std::tuple<Tp...>& t, Ti lastItem) {
        auto prevBytesArraySize = bytesArraySize;
        std::cout << "I: " << I << std::endl;
        std::cout << "lastItem: " << lastItem << std::endl;
        std::cout << "lastItem size: " << lastItem.length() + 1 << std::endl;
        bytesArraySize += lastItem.length() + 1; // +1 to include the null character
        bytesArray.resize(bytesArraySize);
        std::memcpy(bytesArray.data() + prevBytesArraySize, lastItem.c_str(), strlen(lastItem.c_str()) + 1); // copy to array byte wise
        // for (auto i = 0; i < bytesArraySize; i++) {
        //     std::cout << bytesArray[i] << std::endl;
        // }
        // std::cout << "bytesArraySize matches : " << (bytesArraySize == bytesArray.size()) << std::endl;
    }

    // unpack function base case; process last item (c-string)
    template<std::size_t I = 0, typename Ti, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp)
                                       && !(is_str<Ti>::value)
                                       && (is_c_str<Ti>::value), void>::type
    unpackArgs(std::tuple<Tp...>& t, Ti lastItem) {
        auto prevBytesArraySize = bytesArraySize;
        std::cout << "I: " << I << std::endl;
        std::cout << "lastItem: " << lastItem << std::endl;
        std::cout << "lastItem size: " << strlen(lastItem) + 1 << std::endl;
        bytesArraySize += strlen(lastItem) + 1; // +1 to include the null character
        bytesArray.resize(bytesArraySize);
        std::memcpy(bytesArray.data() + prevBytesArraySize, lastItem, strlen(lastItem) + 1); // copy to array byte wise
        // for (auto i = 0; i < bytesArraySize; i++) {
        //     std::cout << bytesArray[i] << std::endl;
        // }
        // std::cout << "bytesArraySize matches : " << (bytesArraySize == bytesArray.size()) << std::endl;
    }
};


#endif