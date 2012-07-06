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

static char test_in_file[] = "data/point.cc"; 

//TEST(InjectClass, test_point_class)
//{
//    struct foot_tracer tracer; 
//    tracer.file_id = 1; 
//    tracer.input.open(test_in_file); 
//    tracer.output.open("inject/point.cc"); 
//
//    char file_name[] = "data/point.cc"; 
//    char ast_file_name[] = "data/point.ast"; 
//    tracer.file_name = file_name; 
//    tracer.ast_file_name = ast_file_name; 
//
//    unsigned int ret = inject_file(&tracer); 
//
//    EXPECT_EQ(0, ret); 
//}
//

TEST(InjectClass, test_point_class)
{
    struct foot_tracer tracer; 
    tracer.file_id = 1; 
    tracer.input.open("data/point.cc"); 
    tracer.output.open("inject/point.cc"); 

    char file_name[] = "data/point.cc"; 
    char ast_file_name[] = "data/point.ast"; 
    tracer.source_file_name = file_name; 
    tracer.ast_file_name = ast_file_name; 
    tracer.log_file_name = "inject.log"; 

    unsigned int ret = inject_file(&tracer); 

    EXPECT_EQ(0, ret); 
}

