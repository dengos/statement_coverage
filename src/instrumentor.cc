/*
 * =====================================================================
 *
 *       Filename:  instrumentor.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2012 09:22:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dengos (), dengos.w@gmail.com
 *        Company:  scut
 *
 * =====================================================================
 */



#include <clang-c/Index.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#include "instrumentor.h"


using namespace std; 

static char FUNC_DECL[] 
    = "extern int __do_path_record__(int, int);"; 
static char INJECT_CODE_PATTERN[] = "__do_path_record__(%d, %u); "; 
static char BEGIN_MAIN[] = "__open_path_record__(); "; 
static char END_MAIN[] = "__close_path_record__(); "; 
static char BEGIN_COMPOUND_STMT[] = "{"; 
static char END_COMPOUND_STMT[] = "}"; 

static map<string, int> file_map; 


static CXChildVisitResult visit_ast_cursor ( 
        CXCursor cursor, CXCursor parent, CXClientData client_data ); 


struct ast_query {
    bool ans; 
    ast_query():ans(false){}; 
}; 
/* ----------  end of struct query  ---------- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  get_file_id
 *  Description:  
 * =====================================================================
 */
    int
get_file_id ( char *file_name )
{
    map<string, int>::iterator iter; 
    string key = string(file_name); 

    iter = file_map.find(key); 
    if (iter == file_map.end())
    {
        file_map.insert(pair<string, int>(key, file_map.size()+1)); 
        return file_map.size(); 
    }

    return iter->second;
}		
/* -----  end of function get_file_id  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  is_trace_file
 *  Description:  
 * =====================================================================
 */
    int
is_trace_file ( struct foot_tracer *tracer, CXCursor cursor )
{
    int is_true = 0; 
    unsigned line = 0; 

    CXFile cx_file; 

    CXSourceLocation location
        = clang_getCursorLocation(cursor); 

    clang_getSpellingLocation(location, &cx_file, &line, NULL, NULL); 

    is_true = cx_file == tracer->cx_file; 

    return is_true;
}		
/* -----  end of function is_trace_file  ----- */

/* 
 * ===  FUNCTION  ======================================================
 *         Name:  get_cursor_line
 *  Description:  
 * =====================================================================
 */
    unsigned
get_cursor_line ( CXCursor cursor )
{
    unsigned line = 0; 
    CXSourceLocation location
        = clang_getCursorLocation(cursor); 

    clang_getSpellingLocation(location, NULL, &line, NULL, NULL); 

    return line;
}		
/* -----  end of function get_cursor_line  ----- */


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  do_inject
 *  Description:  
 * =====================================================================
 */
    int
do_inject ( struct foot_tracer *tracer, unsigned line, 
        enum InjectType inj_type)
{
    static char line_buf[MAX_LINE]; 
    char *code = NULL; 


    /* you don't want to inject the same line again */
    if (inj_type == InjectType_PathRecord 
            && line <= tracer->pre_inj_line)
        return 0;

    for ( ; tracer->pre_inj_line < line; tracer->pre_inj_line++ ) 
    {
        if (tracer->input.getline(line_buf, MAX_LINE))
            tracer->output << line_buf << endl; 
        else
            return 0;   /* reach the end of input file */ 
    }

    switch(inj_type)
    {
        case InjectType_FuncDecl:
            code = FUNC_DECL; 
            break; 
        case InjectType_PathRecord:
            tracer->instr_lines.push_back(line); 
            sprintf(line_buf, 
                    INJECT_CODE_PATTERN, tracer->file_id, line); 
            code = line_buf; 
            break; 
        case InjectType_BeginMain:
            code = BEGIN_MAIN; 
            break; 
        case InjectType_EndMain:
            code = END_MAIN; 
            break; 
        case InjectType_BeginCompStmt:
            code = BEGIN_COMPOUND_STMT; 
            break; 
        case InjectType_EndCompStmt:
            code = END_COMPOUND_STMT; 
            break; 
        default:
            cerr << "Undefined InjectType!" << endl; 
            return 1; 
    }

    tracer->output << code << endl; 
    return 0;
}		
/* -----  end of function do_inject  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  complete_inject
 *  Description:  
 * =====================================================================
 */
    int
complete_inject ( struct foot_tracer *tracer )
{
    /* saddly, can't re-enter */
    static char line_buf[MAX_LINE]; 

    while ( !tracer->input.eof() ) 
    {
        tracer->input.getline(line_buf, MAX_LINE); 
        tracer->output << line_buf << endl;  
    }

    return 0;
}		
/* -----  end of function complete_inject  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  has_compound_stmt
 *  Description:  
 * =====================================================================
 */
    CXChildVisitResult
has_compound_stmt ( CXCursor cursor, 
        CXCursor parent, CXClientData client_data )
{
    CXCursorKind kind = clang_getCursorKind(cursor); 

    if (kind == CXCursor_CompoundStmt)
    {
        struct ast_query *query = (struct ast_query *)client_data; 
        query->ans = true; 
    }

    return CXChildVisit_Continue; 
}		
/* -----  end of function has_compound_stmt  ----- */


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  inject_expression
 *  Description:  
 * =====================================================================
 */
    CXChildVisitResult
inject_expression ( CXCursor cursor,
        CXCursor parent, struct foot_tracer *tracer )
{
    CXCursorKind parent_kind = clang_getCursorKind(parent); 

    switch(parent_kind)
    {
        case CXCursor_Constructor:
          /* class A{ A():a(0){}} */
        case CXCursor_IfStmt:
        case CXCursor_SwitchStmt:
        case CXCursor_WhileStmt:
        case CXCursor_DoStmt:
        case CXCursor_ForStmt:
            return CXChildVisit_Continue; 
        default:
            break; 
    }

    unsigned line = get_cursor_line(cursor); 
    do_inject(tracer, line, InjectType_PathRecord); 

    return CXChildVisit_Continue;
}		
/* -----  end of function inject_expression  ----- */





/* 
 * ===  FUNCTION  ======================================================
 *         Name:  inject_unique_statement
 *  Description:  
 * =====================================================================
 */
    CXChildVisitResult
inject_unique_statement ( CXCursor cursor, struct foot_tracer *tracer)
{
    CXChildVisitResult ret = CXChildVisit_Recurse; 
    struct ast_query query; 

    clang_visitChildren(cursor, has_compound_stmt, &query); 

    if (!query.ans)
    {
        /* if/for/while/do_while statement don't have sub compound
         * statement.
         * */
        ret = CXChildVisit_Continue; 

        unsigned start_line = get_cursor_line(cursor); 

        unsigned end_line = 0;  

        CXSourceRange range
            = clang_getCursorExtent(cursor); 
        CXSourceLocation location
            = clang_getRangeEnd(range); 
        clang_getSpellingLocation(
                location, NULL, &end_line, NULL, NULL); 

        CXCursorKind kind = clang_getCursorKind(cursor);  
        /* special case: do { .. } while */
        if (kind == CXCursor_DoStmt)    end_line--; 

        if (start_line > end_line)
        {
            /* Do Notice that:
             * !! if you write if else statement like that:
             *  ==>   if ( .. ) ..; 
             *  ==>   else ..; 
             *  or even more complecate
             *  ==>   if ( .. ) ..; 
             *  ==>   else if ( .. ) ..; 
             *  ==>   else ..; 
             *  My inject program won't have any idea of where to 
             *  inject the tracing code to record the path of your
             *  program.
             *  After spending several time to work on this problem, 
             *  I realize that to deal with this kind of special case, 
             *  my code will become long and dirty, which i can't bear.
             *  However, this kind of poor coding style shouldn't be
             *  encourage, so why not just put {} around you if-else
             *  statement, and save everyone's time. ^ ^
             * */

            /* when the last statment laid on the same line as
             * if/for/while/do_while, conside they are just one
             * super statment. otherwise...
             * */
            unsigned inj_line = end_line; 
            do_inject(tracer, inj_line, InjectType_BeginCompStmt); 
            do_inject(tracer, inj_line, InjectType_PathRecord); 
            /* inj_line+1 => push last statement to output */
            do_inject(tracer, inj_line+1, InjectType_EndCompStmt); 
        }
        /* else already inject by inject_statement */
    }

    return ret;
}		
/* -----  end of function inject_unique_statement  ----- */


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  inject_statement
 *  Description:  
 * =====================================================================
 */
    CXChildVisitResult
inject_statement ( CXCursor cursor, 
        CXCursor parent, struct foot_tracer *tracer )
{
    unsigned line = 0; 

    CXCursorKind kind = clang_getCursorKind(cursor);  
    switch(kind)
    {
        case CXCursor_CompoundStmt:
        case CXCursor_CaseStmt:
        case CXCursor_DefaultStmt:
            return CXChildVisit_Recurse; 
        case CXCursor_NullStmt:
            return CXChildVisit_Continue; 
        default:
            break; 
    }

    line = get_cursor_line(cursor); 
    do_inject(tracer, line, InjectType_PathRecord); 

    CXChildVisitResult ret = CXChildVisit_Recurse; 
    switch(kind)
    {
        case CXCursor_IfStmt:
        case CXCursor_WhileStmt:
        case CXCursor_DoStmt:
        case CXCursor_ForStmt:
            ret = inject_unique_statement(cursor, tracer); 
        default:
            break; 
    }

    return ret; 
}		
/* -----  end of function inject_statement  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  inject_declaration
 *  Description:  
 * =====================================================================
 */
    CXChildVisitResult
inject_declaration (CXCursor cursor, 
        CXCursor parent, struct foot_tracer *tracer )
{
    CXChildVisitResult ret = CXChildVisit_Continue; 
    CXCursorKind kind = clang_getCursorKind(cursor); 
    switch(kind)
    {
        case CXCursor_StructDecl:
        case CXCursor_UnionDecl:
        case CXCursor_ClassDecl:
        case CXCursor_FunctionDecl:
        case CXCursor_CXXMethod:
        case CXCursor_Namespace:
        case CXCursor_Constructor:
        case CXCursor_Destructor:
        case CXCursor_FunctionTemplate:
        case CXCursor_ClassTemplate:
            if (is_trace_file(tracer, cursor))
                ret = CXChildVisit_Recurse; 
            break; 
        default:
            break; 
    }

    return ret; 
}		
/* -----  end of function inject_declaration  ----- */


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  visit_ast_cursor
 *  Description:  
 * =====================================================================
 */
CXChildVisitResult
visit_ast_cursor ( CXCursor cursor, \
        CXCursor parent, CXClientData client_data )
{
    /* uncomplete */
    CXChildVisitResult ret = CXChildVisit_Continue; 

    CXCursorKind kind = clang_getCursorKind(cursor); 

    if (clang_isExpression(kind))
    {
        ret = inject_expression(
                cursor, parent, (struct foot_tracer *)client_data); 
    }
    else if (clang_isStatement(kind))
    {
        ret = inject_statement(
                cursor, parent, (struct foot_tracer *)client_data); 
    }
    else if (clang_isDeclaration(kind))
    {
        ret = inject_declaration(
                cursor, parent, (struct foot_tracer *)client_data); 
    }
    /* tempory, don't deal with CXCursorKind other than these three */

    return ret; 
}		
/* -----  end of function visit_ast_cursor  ----- */


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  inject_file
 *  Description:  
 * =====================================================================
 */
    int
inject_file ( struct foot_tracer *tracer )
{
    CXIndex index = clang_createIndex(1, 1); 
    CXTranslationUnit trans_unit 
        = clang_createTranslationUnit(index, tracer->ast_file_name); 


    CXCursor top_cursor = clang_getTranslationUnitCursor(trans_unit); 

    tracer->cx_file = clang_getFile(trans_unit, tracer->source_file_name); 

    do_inject(tracer, 1, InjectType_FuncDecl); 

    unsigned int ret
        = clang_visitChildren(top_cursor, visit_ast_cursor, tracer); 

    /* temporary put here */
    complete_inject(tracer); 
    /* temporary put here */

    clang_disposeTranslationUnit(trans_unit); 
    clang_disposeIndex(index);  
    return ret;
}		
/* -----  end of function inject_file  ----- */



/* 
 * ===  FUNCTION  ======================================================
 *         Name:  log_inject_info
 *  Description:  
 * =====================================================================
 */
int
log_inject_info ( struct foot_tracer *tracer )
{
    ofstream logfile; 

    logfile.open(tracer->log_file_name, ios::app); 

    logfile << "File Name: " << tracer->source_file_name << endl; 
    logfile << "File ID: " << tracer->file_id << endl; 
    logfile << "Inject Line: "; 

    vector<unsigned>::iterator iter = tracer->instr_lines.begin();

    for ( ; iter != tracer->instr_lines.end(); iter++ ) 
    {
        logfile << *iter << " "; 
    }
    logfile << endl << endl; 

    return 0;
}		
/* -----  end of function log_inject_info  ----- */


#ifdef GTEST
/* 
 * */

#else


/* 
 * ===  FUNCTION  ======================================================
 *         Name:  main
 *  Description:  
 * =====================================================================
 */
    int
main (int argc, char *argv[])
{
    if (argc < 6)
    {
        cerr << "Instrumentor: expect argc bigger than 6, but only got "
             <<  argc << endl; 
        return EXIT_FAILURE; 
    }

    char *source_file_name = argv[1]; 
    char *ast_file_name = argv[2]; 
    char *file_id       = argv[3]; 
    char *inj_file_name = argv[4]; 
    char *log_file_name = argv[5]; 
 
    struct foot_tracer tracer; 
    tracer.source_file_name = source_file_name; 
    tracer.ast_file_name = ast_file_name; 
    tracer.log_file_name = log_file_name; 
    sscanf(file_id, "%d", &tracer.file_id); 
    tracer.input.open(source_file_name); 
    tracer.output.open(inj_file_name); 

    unsigned ret = inject_file(&tracer); 

    /* log the information */
    log_inject_info(&tracer); 
    cout << argv[0] << ": file " << source_file_name << "inject complete!" << endl; 

    return EXIT_SUCCESS;
}				
/* ----------  end of function main  ---------- */


#endif
