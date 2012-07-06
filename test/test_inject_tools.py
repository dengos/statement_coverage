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


def test_parse_argv():
    #nose.tools.set_trace()
    argvs = ["-I", "test.cc"]
    tools = InjectTools(argvs)
    assert(tools.info_obj.source_files[0] == "test.cc")
    assert(tools.info_obj.cmd == "clang++ {0} -I {1}")


def test_generate_ast_file():
    argvs = ["./data/test.cc"]
    os.system("rm -f test.ast")
    tools = InjectTools(argvs)
    tools.generate_ast_files()
    assert(os.path.exists("test.ast"))

def test_inject_source_files():
    argvs = ["./data/test.cc"]
    os.system("rm -f ./data/__inj_test.cc")
    tools = InjectTools(argvs)
    tools.generate_ast_files()
    tools.inject_source_files()
    assert(os.path.exists("./data/__inj_test.cc"))

def test_compile_inject_files():
    argvs = ["./data/test.cc"]
    tools = InjectTools(argvs)
    os.system("rm -f " + TRACE_PROG_NAME)
    tools.generate_ast_files()
    tools.inject_source_files()
    tools.compile_inject_files()
    assert(os.path.exists(TRACE_PROG_NAME))

def test_run_tracer():
    argvs = ["./data/test.cc"]
    tools = InjectTools(argvs)
    tools.generate_ast_files()
    tools.inject_source_files()
    tools.compile_inject_files()
    tools.run_tracer()

def test_report():
    argvs = ["./data/test.cc"]
    tools = InjectTools(argvs)
    tools.generate_ast_files()
    tools.inject_source_files()
    tools.compile_inject_files()
    tools.run_tracer()
    tools.report()
    assert(os.path.exists("./data/"+VIEW_PREFIX+"test.cc"))













