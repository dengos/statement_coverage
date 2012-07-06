#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Author: dengos
# E_mail: dengos.w@gmail.com
# Company: scut
#

import sys
sys.path.append("../src")

import os
import nose
from inject_tools import *


def test_create_info_obj():
    argvs = ["-I", "-E", "/usr/test/test.cpp", "ast.c"]
    info_obj = InfoObjFactory.get_info_obj(argvs);
    assert len(info_obj.source_files) == 2
    assert info_obj.source_files[0] == "/usr/test/test.cpp"
    assert info_obj.source_files[1] == "ast.c"
    assert len(info_obj.ast_files) == 2
    assert info_obj.ast_files[0] == "test.ast"
    assert info_obj.ast_files[1] == "ast.ast"
    assert len(info_obj.inj_files) == 2
    assert info_obj.inj_files[0] == "/usr/test/"+INJ_PREFIX+"test.cpp"
    assert info_obj.inj_files[1] == INJ_PREFIX+"ast.c"
    assert len(info_obj.view_files) == 2
    assert info_obj.view_files[0] == "/usr/test/"+VIEW_PREFIX+"test.cpp"
    assert info_obj.view_files[1] == VIEW_PREFIX+"ast.c"
    assert info_obj.cmd == "clang++ {0} -I -E {1} {2}"
    assert len(info_obj.map_fname_fid) == 2
    assert len(info_obj.map_fid_inj_lines) == 2
    assert len(info_obj.map_fid_exc_lines) == 2


def test_get_ast_cmd():
    argvs = ["test.cc", "ast.cc"]
    info_obj = InfoObjFactory.get_info_obj(argvs)
    ast_cmd  = info_obj.get_ast_cmd()
    #nose.tools.set_trace()
    assert ast_cmd == "clang++ -emit-ast test.cc ast.cc"


def test_inj_cmd():
    argvs = ["./test/test.cc", "ast.cc"]
    info_obj = InfoObjFactory.get_info_obj(argvs)
    cmds = []
    for inj_cmd in info_obj.get_inj_cmd("test"):
        cmds.append(inj_cmd)
    assert len(cmds) == 2
    assert cmds[0] == "test ./test/test.cc test.ast 0 ./test/"+INJ_PREFIX+"test.cc "+LOG_FILE
    #nose.tools.set_trace()
    assert cmds[1] == "test ast.cc ast.ast 1 "+INJ_PREFIX+"ast.cc "+LOG_FILE


def test_compile_cmd():
    argvs = ["./test/test.cc", "ast.cc"]
    info_obj = InfoObjFactory.get_info_obj(argvs)
    compile_cmd  = info_obj.get_compile_cmd()
    #nose.tools.set_trace()
    assert compile_cmd == "clang++ -o " + TRACE_PROG_NAME + " ./test/"+INJ_PREFIX+"test.cc "+INJ_PREFIX+"ast.cc " + PATH_RECORD_LIB

def test_parse_log_file():
    argvs = ["./test/test.cc"]
    info_obj = InfoObjFactory.get_info_obj(argvs)
    info_obj.log_file = "test.log"
    with open("test.log", "w") as output:
        output.write("File Name: ./test/test.cc\n")
        output.write("File ID: 0\n")
        output.write("Inject Line: 10 20\n")

    for fname, fid, inj_lines in info_obj.parse_log_file():
        assert fname == "./test/test.cc"
        assert fid == 0
        assert inj_lines == [10, 20]


def test_fresh_inj_lines():
    argvs = ["./test/test.cc"]
    info_obj = InfoObjFactory.get_info_obj(argvs)
    info_obj.log_file = "test.log"
    with open("test.log", "w") as output:
        output.write("File Name: ./test/none.cc\n")
        output.write("File Name: ./test/test.cc\n")
        output.write("File ID: 0\n")
        output.write("Inject Line: 10 20\n")

    #nose.tools.set_trace()
    info_obj.fresh_inj_lines()
    assert len(info_obj.map_fid_inj_lines) == 1
    assert len(info_obj.map_fid_inj_lines[0]) == 2
    assert 10 in info_obj.map_fid_inj_lines[0]
    assert 20 in info_obj.map_fid_inj_lines[0]

def test_fresh_exc_lines():
    argvs = ["./test/test.cc"]
    info_obj = InfoObjFactory.get_info_obj(argvs)
    with open("path_record_test", "w") as output:
        output.write("0 20\n")
        output.write("0 54\n")
    info_obj.fresh_exc_lines(["path_record_test"])
    assert len(info_obj.map_fid_exc_lines) == 1
    assert len(info_obj.map_fid_exc_lines[0]) == 2
    assert 20 in info_obj.map_fid_exc_lines[0]
    assert 54 in info_obj.map_fid_exc_lines[0]






