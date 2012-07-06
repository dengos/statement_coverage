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

static char test_in_file[] = "data/template.cc"; 

TEST(InjectTemplate, test_template_function)
{
    struct foot_tracer tracer; 
    tracer.file_id = 1; 
    tracer.input.open(test_in_file); 
    tracer.output.open("inject/template.cc"); 

    char file_name[] = "template.cc"; 
    char ast_file_name[] = "data/template.ast"; 
    tracer.file_name = file_name; 
    tracer.ast_file_name = ast_file_name; 

    unsigned int ret = inject_file(&tracer); 

    EXPECT_EQ(0, ret); 
}

