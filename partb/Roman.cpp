// TODO: implement romanToInteger in here
#include "Roman.h"

int value(char roman)
{
    switch(roman)
    {
        case 'I':return 1;
        case 'V':return 5;
        case 'X':return 10;
        case 'L':return 50;
        case 'C':return 100;
        case 'D':return 500;
        case 'M':return 1000;
    }
}

int romanToInteger(const string& s)
{
    int sum=0; char prev='%';
    for(int i=(s.length()-1); i>=0; i--)
    {
        if(value(s[i])<sum && (s[i]!=prev))
        {       sum -= value(s[i]);
            prev = s[i];
        }
        else
        {
            sum += value(s[i]);
            prev = s[i];
        }
    }
    return sum;
}