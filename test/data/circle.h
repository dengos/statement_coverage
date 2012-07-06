/*
 * =====================================================================
 *
 *       Filename:  circle.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/22/2012 04:11:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */


#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "point.h"

class Circle
{
    private:
        int   radius;
        Point centre;

    public:
        Circle();
        Circle( int );
        Circle( int, Point );
        Circle( int, int, int );

        void SetRadius( int );
        void SetCentre( int, int );
        void SetCentre( Point );

        int   GetRadius()      const;
        Point GetCentre()      const;

        void Print()          const;

        void   Relation( Circle& );
};


#endif
