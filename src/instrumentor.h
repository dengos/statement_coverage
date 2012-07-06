/*
 * =====================================================================
 *
 *       Filename:  instrumentor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2012 11:51:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */

#include <vector>
#include <fstream>
#include <clang-c/Index.h>

#define MAX_FILE_NAME 200
#define MAX_LINE 200



struct foot_tracer {
    char        *source_file_name; 
    char        *ast_file_name; 
    char        *log_file_name; 
    int         file_id; 
    CXFile      cx_file; 
    unsigned    pre_inj_line; 
    std::vector<unsigned> instr_lines; 
    std::ifstream input; 
    std::ofstream output; 
    foot_tracer():
        source_file_name(NULL), 
        ast_file_name(NULL), 
        log_file_name(NULL), 
        cx_file(NULL), 
        file_id(0), 
        pre_inj_line(1)
    {}; 
}; 

enum InjectType {
    InjectType_FuncDecl = 1, 
    InjectType_PathRecord, 
    InjectType_BeginMain, 
    InjectType_EndMain, 
    InjectType_BeginCompStmt, 
    InjectType_EndCompStmt
};				

int get_file_id ( char *file_name ); 

int do_inject ( 
        struct foot_tracer *tracer, unsigned line, enum InjectType); 

int inject_file ( struct foot_tracer *tracer ); 

