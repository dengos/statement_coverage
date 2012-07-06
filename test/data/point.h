/*
 * =====================================================================
 *
 *       Filename:  point.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/22/2012 04:10:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */

#ifndef __POINT__
#define __POINT__

class Point
{
private:
    int x, y;

public:
    Point();
    Point( int, int );

    void SetX( int );           //  设置 X 值
    void SetY( int );           //  设置 Y 值
    void SetXY( int, int );     //  同时设置 X Y 值

    int  GetX()        const;   //   获得 X 值
    int  GetY()        const;   //   获得 Y 值

    void Print()       const;   //   打印点信息
};


#endif
