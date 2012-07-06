/*
 * =====================================================================
 *
 *       Filename:  test_inject_expr.cc
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

TEST(InjectExpr, clean_call)
{
    struct foot_tracer tracer; 
    tracer.file_id = 1; 
    tracer.input.open(test_in_file); 
    tracer.output.open("inject/hello.cc"); 

    char file_name[] = "data/hello.ast"; 
    tracer.source_file_name = "data/hello.cc"; 
    tracer.ast_file_name = "data/hello.ast"; 
    tracer.log_file_name = "data/inject.log"; 

    unsigned int ret = inject_file(&tracer); 

    EXPECT_EQ(0, ret); 
}

