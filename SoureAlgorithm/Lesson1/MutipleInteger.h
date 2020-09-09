#ifndef MUTIPLEINTEGER_H
#define MUTIPLEINTEGER_H
#include<boost/multiprecision/cpp_int.hpp>
namespace mp =  boost::multiprecision;

mp::cpp_int MultipleKaratsuba(const mp::cpp_int& number1,const mp::cpp_int& number2);
mp::cpp_int GradeSchoolAlorithm(mp::cpp_int number1, mp::cpp_int number2);

void testMutipleInteger();
#endif // MUTIPLEINTEGER_H
