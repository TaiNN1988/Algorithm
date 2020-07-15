////////////////////////////////////////////////////////////////////////////
//
//   Project     : Large Integer multiplication
//   Description :
//      Implement Grade-chool Algorithm, karatsuba and test peformance
//
//   Version:
//         1.0(7/2020)
//   Created By: Tai Nguyen(taingocnguyen1988@gmail.com)
////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include<boost/multiprecision/cpp_int.hpp>
#include<stdlib.h>
#include<string>
#include<chrono>
using namespace std;

namespace mp =  boost::multiprecision;

#define largePower10(n) (mp::pow(mp::cpp_int(10), n))

typedef struct
{
    string number1;
    string number2;
    string expectedOutPut;
}DataTest;

/*
 * Caculate large value of power exponent with base 10
*/
mp::cpp_int myPower10(int n)
{
    mp::cpp_int prod = 1;
    for(int i = 0; i < n; i++)
    {
        prod = prod * 10;
    }
    return prod;
}

/*
 * Convert large string value to int
*/
mp::cpp_int convertStringToInt(string& strValue)
{
    mp::cpp_int result = 0;
    int lenght = strValue.length();
    for(int i = 0; i < lenght; i++)
    {
        int temp = strValue[lenght -i -1] - '0';
        result += temp * largePower10((i));
    }
    return result;
}

/*
 * Add two large value
*/
string addString(string& str1, string& str2)
{
    string result;
    int remain = 0;
    char ch ;

    for(int i = str1.length() - 1, j = str2.length() - 1; i >= 0 || j >=0; j--, i--)
    {
        int temp = ((i >= 0)? (str1[i] -'0') : 0) + ((j >= 0)?(str2[j] - '0'):0) + remain;
       ch = temp%10 + '0';

        result.insert(0,1,ch);

        remain = temp/10;
    }

    if(remain == 1)
    {
        ch = '0' + 1;

         result.insert(0,1,ch);
    }

    return  result;
}

/*
 * Count digital number
*/
int countDigitalNumber(mp::cpp_int value)
{
    int count = 0;

    while (value > 0) {
        value = value/10;
        count++;
    }
    return count;
}

/*
 * Stipt value to low and high part
*/
void sliptAt(const mp::cpp_int& value, int pos, mp::cpp_int& low, mp::cpp_int& high)
{
    mp::cpp_int p = largePower10(pos)/*myPower10(pos)*/;
    low = value%p;
    high = value/p;
}

/*
 * Kaatsuba algorithm: mutiple two large integer
*/
mp::cpp_int MultipleKaratsuba(string& strNumber1, string& strNumber2)
{
    int lenght1 = strNumber1.length();
    int lenght2 = strNumber2.length();

    // Return  value when it can not split
    if(lenght1 == 1 || lenght2 == 1)
    {
        mp::cpp_int num1 = convertStringToInt(strNumber1);
        mp::cpp_int num2 = convertStringToInt(strNumber2);
        return num1 * num2;
    }

    int m = lenght1 < lenght2 ? lenght1: lenght2;
    int m2 = floor(m/2);

    string high1 = strNumber1.substr(0, lenght1 - m2);
    string low1 = strNumber1.substr(lenght1 - m2, m2);

    string high2 = strNumber2.substr(0, lenght2 - m2);
    string low2 = strNumber2.substr(lenght2 - m2, m2);

    // Step 1
    mp::cpp_int z0 = MultipleKaratsuba(low1, low2);

    // Step 2
    mp::cpp_int z1 = MultipleKaratsuba(high1, high2);

    // Step 3
    string addLowHigh1 = addString(high1, low1);
    string addLowHigh2 = addString(high2, low2);
    mp::cpp_int z2 = MultipleKaratsuba(addLowHigh1, addLowHigh2);

    return  z1 * largePower10(m2*2) + (z2 - z1 - z0) * largePower10(m2) + z0;
}

/*
 * Kaatsuba algorithm: mutiple two large integer
*/
mp::cpp_int MultipleKaratsuba(const mp::cpp_int& number1,const mp::cpp_int& number2)
{
    // Return  value when it can not split
    if(number1 < 10 || number2< 10)
    {
        return number1 * number2;
    }

    mp::cpp_int low1 = 0, high1 = 0;
    mp::cpp_int low2 = 0, high2 = 0;
    int lenght1 = countDigitalNumber(number1);
    int lenght2 = countDigitalNumber(number2);

    int m = lenght1 < lenght2 ? lenght1: lenght2;
    int m2 = floor(m/2);


    sliptAt(number1, m2, low1, high1);
    sliptAt(number2, m2, low2, high2);

    // Step 1
    mp::cpp_int z0 = MultipleKaratsuba(low1, low2);

    // Step 2
    mp::cpp_int z1 = MultipleKaratsuba(high1, high2);

    // Step 3
    mp::cpp_int z2 = MultipleKaratsuba(low1 + high1, low2 + high2);

    return (z1 * largePower10(m2*2) + (z2 - z1 - z0) * largePower10(m2) + z0);
}

/*
 *  Grade-School algorithm: mutiple two large integer
*/
mp::cpp_int GradeSchoolAlorithm(mp::cpp_int number1, mp::cpp_int number2)
{
    if(number1 < 10 || number2< 10)
    {
        return number1 * number2;
    }

    mp::cpp_int result = 0;

    int nextdigital1 = 0;
    int nextdigital2 = 0;
    mp::cpp_int tempNumber2 = number2;
    int count1 = 0;

    while (number1 > 0) {
         nextdigital1 = int(number1%10);
         int count2 = 0;
          int re = 0;
         mp::cpp_int value = 0;

        while (tempNumber2 > 0) {
            nextdigital2 = int(tempNumber2%10);
            tempNumber2 =tempNumber2/10;

            int tempValue = nextdigital1*nextdigital2;

            if(tempNumber2 == 0)
            {
                // Compute final digital number
                value += (tempValue + re)*largePower10(count2);
            }
            else
            {
                value += (tempValue%10 + re)*largePower10(count2);
            }

            re = tempValue/10;
            count2++;
        }

        result += value*largePower10(count1);

        number1 = number1/10;
        tempNumber2 = number2;
        count1++;
    }
    return result;
}

int main()
{
    // Create data for test
    DataTest dataTest[] =
    {
        /* A     B     A*B */
        {"12", "15", "180"},
        {"123", "153", "18819"},
        {"1234", "15", "18510"},
        {"1234", "5", "6170"},
        {"123456", "7891", "974191296"},
        {"7891", "123456", "974191296"},
        {"31415926", "27182818", "853973398759468"},
        {"3141592653589793", "2718281828459045", "8539734222673565677848730527685"},
        {"3141592653589793238462643383279502884197169399375105820974944592", "2718281828459045235360287471352662497757247093699959574966967627", "8539734222673567065463550869546574495034888535765114961879601127067743044893204848617875072216249073013374895871952806582723184"}
     };
    mp::cpp_int expectedOut = 0;
    mp::cpp_int actualOut1 = 0;
    mp::cpp_int actualOut2 = 0;

    //Excute and Verify
    for(int i = 0; i < int(sizeof(dataTest)/sizeof(DataTest)); i++)
    {
        DataTest item = dataTest[i];

        expectedOut = convertStringToInt(item.expectedOutPut);
        mp::cpp_int number1 = convertStringToInt(item.number1);
        mp::cpp_int number2 = convertStringToInt(item.number2);

        auto started1 = chrono::high_resolution_clock::now();
        actualOut1 = GradeSchoolAlorithm(number1, number2);
        auto done1 = chrono::high_resolution_clock::now();

        auto started2 = chrono::high_resolution_clock::now();
        actualOut2 = MultipleKaratsuba(number1, number2);
        auto done2 = chrono::high_resolution_clock::now();

        cout<<"Test Case: "<<i+1<<endl;
        cout<<"Input valueA:"<<item.number1<<endl;
        cout<<"Input valueB:"<<item.number2<<endl;
        cout<<"Expected ouput:"<<item.expectedOutPut<<endl;

        cout<<"GradeSchoolAlorithm acutal ouput:"<<actualOut1<<endl;
        cout<<"MultipleKaratsuba acutal ouput:"<<actualOut2<<endl;

        cout <<"GradeSchoolAlorithm time(nanoseconds): "<< chrono::duration_cast<std::chrono::nanoseconds>(done1-started1).count()<<endl;
        cout <<"MultipleKaratsuba time(nanoseconds): "<< chrono::duration_cast<std::chrono::nanoseconds>(done2-started2).count()<<endl;


        if(actualOut1 == expectedOut && actualOut2 == expectedOut)
        {
            cout<<"<<<Passed>>>"<<endl;
        }
        else {
             cout<<"<<<Failed>>>"<<endl;
        }
        cout<<endl;
    }

    return 0;
}


mp::cpp_int SimpleKaratsuba(string& strNumber1, string& strNumber2)
{

    int lenght1 = strNumber1.length();
    int lenght2 = strNumber2.length();

    mp::cpp_int result = 0;
    mp::cpp_int supA1 = 0, supA2 = 0;
    mp::cpp_int supB1 = 0, supB2 = 0;

    string supstrNumber11 = strNumber1.substr(0,lenght1/2);
    string supstrNumber12 = strNumber1.substr(lenght1/2,lenght1/2);

    string supstrNumber21 = strNumber2.substr(0,lenght2/2);
    string supstrNumber22 = strNumber2.substr(lenght2/2,lenght2/2);

    //Convert str to int
    for(int i = 0; i < lenght1/2;i++)
    {
        int temp = supstrNumber11[lenght1/2 -1 - i] - '0';
        supA1 += temp * myPower10(i);

        temp = supstrNumber12[lenght1/2 -1 - i] - '0';
        supA2 += temp * myPower10(i);

        temp = supstrNumber21[lenght1/2 -1 - i] - '0';
        supB1 += temp * myPower10(i);

        temp = supstrNumber22[lenght1/2 -1 - i] - '0';
        supB2 += temp * myPower10(i);
    }

    //Step 1 calculate A1*B1 = a*c
    mp::cpp_int a1b1 = supA1*supB1;

    //Step 2 bd = A2*B2
    mp::cpp_int a2b2 = supA2*supB2;

    //Step 3
    mp::cpp_int sum3 = (supA1 +supA2)*(supB1 + supB2);

    //Step4 ad + bc
    mp::cpp_int sum4 = sum3 - a1b1 - a2b2;

    //step5
    result = a1b1*myPower10(lenght1) + sum4*myPower10(lenght1/2) + a2b2;

    return result;
}
