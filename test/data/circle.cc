/*
 * =====================================================================
 *
 *       Filename:  circle.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/22/2012 04:12:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */


//下面这些包含在文件 Circle.cpp 中

#include <iostream>
#include <cmath>

#include "circle.h"
#include "point.h"

using namespace std;

Circle::Circle ()
{
}

Circle::Circle( int a )
{
    radius= a;
}

Circle::Circle( int a, int b, int c )
{
    radius= a;

    centre.SetXY( b, c);
}

Circle::Circle( int a, Point b )
{
    radius= a;
    centre= b;
}

void Circle::SetCentre( int a, int b )
{
    centre.SetXY( a, b );
}

void Circle::SetCentre( Point p )
{
    centre= p;
}

void Circle::SetRadius( int a )
{
    radius= a;
}

void Circle::Print() const
{
    cout << "半径为 " << radius << endl;
    centre.Print();
}

int Circle::GetRadius() const
{
    return radius;
}

Point Circle::GetCentre() const
{
    return centre;
}

void Circle::Relation ( Circle& t )
{
    double dis= sqrt( (double)(t.GetCentre().GetX()- centre.GetX())* (t.GetCentre().GetX()- centre.GetX())+
            (t.GetCentre().GetY()- centre.GetY())* (t.GetCentre().GetY()- centre.GetY()) );

    double r1= radius+ t.GetRadius();
    double r2= abs( radius- t.GetRadius() );

    if( dis== r1 || r2== dis  ) cout<< "这两个圆相切" << endl;
    else if( dis> r1 ) cout << "两圆相离" << endl;
    else if( dis< r2 ) cout << "两圆包含" << endl;
    else  cout << "两圆相交" << endl;
}


