#include <iostream>
#include "Vector.h"
using namespace std;
int main()
{
    Vector<int> sw;
    sw[0] = 1;
    sw[1] = 2;
    sw[2] = 3;
    sw[3] = 4;

    for (int i = 0; i < 3; i++)
    {
        cout << sw[i] << " ";
    }
}