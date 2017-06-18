#ifndef _MURMURCLASS_H_
#define _MURMURCLASS_H_

template<typename... Arguments>
class MurmurClass {
  private:
    static const size_t numParam = sizeof...(Arguments);
  public:
    apply();
};


#endif