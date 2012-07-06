/*
 * =====================================================================
 *
 *       Filename:  point.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/22/2012 04:11:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */

#include <iostream>

using namespace std;

#include "point.h"

Point::Point()
{
}

Point::Point( int a, int b ):
x(a), y(b)
{
}

void Point::SetX( int a )
{
    x= a;
}

void Point::SetY( int a )
{
    y= a;
}

void Point::SetXY( int a, int b )
{
    x= a, y= b;
}

int Point::GetX() const
{
    return x;
}


int Point::GetY() const
{
    return y;
}

void Point::Print() const
{
    cout << "X 坐标为 " << x << endl;
    cout << "y 坐标为 " << y << endl;
}

