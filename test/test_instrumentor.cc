/*
 * =====================================================================
 *
 *       Filename:  test_inject_file.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2012 11:17:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */


#include <fstream>
#include "gtest/gtest.h"
#include "../src/instrumentor.h"



using namespace std; 

TEST(InjectFile, call_inject_file)
{
    char file_name[] = "data/hello.ast"; 
    struct foot_tracer tracer; 
    tracer.file_name = file_name; 

//    char *argv[] = {
//        "-Wl, rpath=/usr/lib64/llvm:/usr/include/linux"
//    }; 

    unsigned int ret = inject_file(&tracer); 
    EXPECT_EQ(0, ret); 
}


TEST(FileMap, get_file_id)
{
    int file_id = 0; 
    char file_name[] = "data/hello.cpp"; 
    file_id = get_file_id(file_name); 
    EXPECT_EQ(1, file_id); 
    file_id = get_file_id(file_name); 
    EXPECT_EQ(1, file_id); 
}

