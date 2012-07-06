/*
 * =====================================================================
 *
 *       Filename:  test_path_record.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2012 09:20:55 AM
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
#include "../src/path_record.h"


using namespace std; 


TEST(PathRecord, open_close_record)
{
    __open_path_record__(); 
    __close_path_record__(); 
}

TEST(PathRecord, do_path_record)
{
    __open_path_record__(); 
    __do_path_record__(0, 1); 
    __do_path_record__(0, 10); 
    __do_path_record__(0, 1); 
    __close_path_record__(); 
    // verify
    ifstream logfile; 

    int pid = getpid();
    char file_name[200];
    sprintf(file_name, "path_record_%d", pid); 

    logfile.open(file_name); 
    int file_id, line_num; 
    logfile >> file_id >> line_num; 
    EXPECT_EQ(0, file_id); EXPECT_EQ(1, line_num); 
    logfile >> file_id >> line_num; 
    EXPECT_EQ(0, file_id); EXPECT_EQ(10, line_num); 
    
    int c = logfile.get(); 
    EXPECT_EQ(EOF, logfile.get()); 
}




