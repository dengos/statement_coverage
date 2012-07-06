/*
 * =====================================================================
 *
 *       Filename:  gtest_main.c
 *
 *    Description:  
 *
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  google
 *        Company:  google
 *
 * =====================================================================
 */



#include "gtest/gtest.h"

/* 
 * ===  FUNCTION  ======================================================
 *         Name:  main
 *  Description:  
 * =====================================================================
 */
    int
main (int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}				
/* ----------  end of function main  ---------- */

