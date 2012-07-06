/*
 * =====================================================================
 *
 *       Filename:  hello.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2012 07:53:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */


#include <stdlib.h>
#include <stdio.h>
#include <iostream>


using namespace std; 


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  hello
 *  Description:  
 * =====================================================================
 */
    void
hello ( void )
{
    printf("test hello\n");
    return ;
}		
/* -----  end of function hello  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  main
 *  Description:  
 * =====================================================================
 */
    int
main (int argc, char *argv[])
{
    int i, j; 

    for ( i = 0; i < 1; i++ ) 
    {

        for ( j = 0; j < 1; j++ ) 
        {
            hello(); 
            cout << "bye" << endl; 
        }
    }

    i = 1; 
    if (i == 0)
    {
        printf("if statement\n");
    }

    return EXIT_SUCCESS;
}				
/* ----------  end of function main  ---------- */


