/*
 * =====================================================================
 *
 *       Filename:  test_do_inject.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/21/2012 12:10:34 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */



#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
#include "instrumentor.h"

using namespace std; 

static char test_in_file[] = "data/hello.cc"; 

TEST(DoInject, clean_call)
{
    struct foot_tracer tracer; 
    tracer.file_id = 1; 
    tracer.input.open(test_in_file); 
    tracer.output.open("clean_call"); 

    int ret = do_inject(&tracer, 54); 

    EXPECT_EQ(0, ret); 
}


