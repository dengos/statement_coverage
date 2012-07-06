/*
 * =====================================================================
 *
 *       Filename:  path_record.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2012 12:57:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <set>
#include <fstream>

#define MAX_FILE_NAME 200


using namespace std; 


static set<pair<int, int> > recorder; 


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  open_path_record
 *  Description:  
 * =====================================================================
 */
    int
__open_path_record__ ( void )
{
    // None
    return 0;
}		
/* -----  end of function open_path_record  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  do_path_record
 *  Description:  
 * =====================================================================
 */
    int
__do_path_record__ ( int source_file_id, int line_num )
{
    recorder.insert(pair<int, int>(source_file_id, line_num)); 
    return 0; 
}		
/* -----  end of function do_path_record  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  close_path_record
 *  Description:  
 * =====================================================================
 */
    int
__close_path_record__ ( void )
{

    int  pid = 0; 
    ofstream logfile; 
    char file_name[MAX_FILE_NAME]; 
    set<pair<int, int> >::iterator iter; 

    pid = getpid(); 
    sprintf(file_name, "path_record_%d", pid); 


    logfile.open(file_name); 
    
    for ( iter = recorder.begin(); iter != recorder.end(); iter++ ) 
    {
        logfile << iter->first << "   " << iter->second << endl; 
//      logfile << (*iter)->first << "  " << (*iter)->second << endl; 
    } 

    logfile.close(); 
    return 0; 
}		
/* -----  end of function close_path_record  ----- */



class PathRecord
{
    public:
    ~PathRecord()
    {
        __close_path_record__(); 
    }
}; 

static PathRecord path_recorder; 
