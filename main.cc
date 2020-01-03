#include <stdio.h>
#include "PyCall.h"



int main()
{
    std::vector<double> x = {1.0,2.0,3.0};
    std::vector<double> xx = {11.0,22.0,33.0};

    PyCall pyCall;
    pyCall.subplot(211);
    pyCall.plot(x,"g");
    pyCall.subplot(212);
    pyCall.plot(x, xx,"ro");
    pyCall.show();
}





