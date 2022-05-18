#ifndef UTIL_H
#define UTIL_H
class QString;
#include <type_traits>
namespace util{
  constexpr int int32_max = 2147483647;
  constexpr long long int64_max = 9223372036854775807;
  QString millisecondsToTimeString(long);
  int timeStringToMilliseconds(const QString&);
  inline constexpr int countDigits(long);
  QString numberWithCommas(long);
  void SendMessage(const QString&);  
  void SendMessage(int num);
  void SendMessage(double num);
  void SendMessage(bool b);
  template<class T>
  constexpr inline T abs(T num){
    return num > 0 ? num : -num;
  }
  template<class T>
  constexpr inline T clamp(T num, T _min, T _max){
    if(num > _max)
      num = _max;
    else if(num < _min)
      num = _min;

    return num;
  }

  template<class IT, class Func>
  IT min_element(IT first, IT last, Func func)
  {
      if (first == last)
        return last;

      IT smallest = first;
      ++first;
      for (; first != last; ++first) {
          if (func(*first, *smallest)) {
              smallest = first;
          }
      }
      return smallest;
  }
  template<class IT, class Func>
  IT max_element(IT first, IT last, Func func)
  {
    if (first == last)
      return last;

    IT largest = first;
    ++first;
    for (; first != last; ++first) {
      if (func(*largest, *first)) {
        largest = first;
      }
    }
    return largest;
  }
  template<class IT, class Func, class Vec>
  inline IT copy_to_vector(IT first, IT last, Vec& vec, Func func){
    for(; first != last; ++first)
      if(func(*first))
        vec.push_back(*first);
  }
}
#endif // UTIL_H
