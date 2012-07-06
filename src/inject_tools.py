#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# Author: dengos
# E_mail: dengos.w@gmail.com
# Company: scut
#


import sys
import re
import os
import fnmatch


CMD_PREFIX = "clang++ {0}"
INJ_PREFIX = "__inj_"
VIEW_PREFIX = "__view_"
REC_PREFIX = "path_record_"
AST_FLAG = "-emit-ast"
TRACE_PROG_NAME = "__tracer"
LOG_FILE = "inject.log"
PATH_RECORD_LIB = "path_record.o"

LABEL_BLK = " "
LABEL_INJ = "-"
LABEL_EXC = "+"

class InjectInformation:
    def __init__(self, source_files, ast_files, inj_files, view_files, cmd):
        self.source_files = source_files
        self.ast_files = ast_files
        self.inj_files = inj_files
        self.view_files = view_files
        self.log_file = LOG_FILE
        self.cmd = cmd
        self.map_fname_fid = {}
        self.map_fid_inj_lines = {}
        self.map_fid_exc_lines = {}
        for idx in range(len(source_files)):
            source_file_name = source_files[idx]
            if not self.map_fname_fid.has_key(source_file_name):
                self.map_fname_fid[source_file_name] = idx
                self.map_fid_inj_lines[idx] = set()
                self.map_fid_exc_lines[idx] = set()
    # end of __init__


    def get_ast_cmd(self):
        ast_cmd = self.cmd.format(AST_FLAG, *self.source_files)
        return ast_cmd
    # end of get_ast_cmd

    def get_inj_cmd(self, prog_name):
        for i in range(len(self.source_files)):
            inj_cmd = prog_name + " "
            inj_cmd += self.source_files[i] + " "
            inj_cmd += self.ast_files[i] + " "
            inj_cmd += str(self.map_fname_fid[self.source_files[i]]) + " "
            inj_cmd += self.inj_files[i] + " "
            inj_cmd += self.log_file
            yield inj_cmd
    # end of get_inj_cmd

    def get_compile_cmd(self):
        compile_cmd = self.cmd.format("-o " + TRACE_PROG_NAME, *self.inj_files)
        compile_cmd += " " + PATH_RECORD_LIB
        return compile_cmd
    # end of get_compile_cmd

    def parse_log_file(self):
        log = open(self.log_file, "r")
        lines = log.readlines()
        idx = 0
        while idx < len(lines):
            current_line = lines[idx]
            head_match = re.match("File Name:", current_line)
            if head_match is None:
                idx += 1
                continue
            # match a log record
            extend_line = reduce(lambda x, y: x+y, lines[idx:idx+3])
            log_record_match = re.match("File Name: (.+)\nFile ID: (.+)\nInject Line: (.+)\n", extend_line)
            if log_record_match is None:
                idx += 1
                continue
            fname, fid, inj_lines = log_record_match.groups()
            fname = fname.strip()
            fid = int(fid)
            inj_lines = map(int, inj_lines.split())
            yield fname, fid, inj_lines
            #file_name = log_record_match.group(0).strip()
            #file_id = int(log_record_match.group(1))
            #inj_lines = map(int, log_record_match.group(2).split())
            #yield file_name, file_id, inj_lines
            idx += 3
        log.close()
    # end of parse_log_file

    def fresh_inj_lines(self):
        for fname, fid, inj_lines in self.parse_log_file():
            # check the consistence between fname and fid
            if fid != self.map_fname_fid[fname]:
                print "Unexpected file " + fname
                continue
            map(self.map_fid_inj_lines[fid].add, inj_lines)
    # end of fresh_inj_lines

    def fresh_exc_lines(self, records):
        for fname in records:
            with open(fname, "r") as record:
                for line in record:
                    fid, line_num = map(int, line.split())
                    if not self.map_fid_exc_lines.has_key(fid):
                        print "Unexpected File ID: " + str(fid)
                        continue
                    self.map_fid_exc_lines[fid].add(line_num)
    # end of fresh_exc_lines


class InfoObjFactory:
    @staticmethod
    def get_info_obj(argvs):
        cmd = CMD_PREFIX
        source_files = []
        ast_files = []
        inj_files = []
        view_files = []
        prog = re.compile("((.+?/)*)(.*)(\.cpp|\.cc|\.c)")
        for arg in argvs:
            match_obj = prog.match(arg)
            if match_obj is None:
                cmd += " " + arg
                continue
            path, tmp, fname, postfix = match_obj.groups()
            source_files.append(arg)
            ast_files.append(fname + ".ast")
            inj_files.append(path + INJ_PREFIX + fname + postfix)
            view_files.append(path + VIEW_PREFIX + fname + postfix)
            cmd += " {" + str(len(source_files)) + "}"
        info_obj = InjectInformation(source_files, ast_files, inj_files, view_files, cmd)
        return info_obj

class InjectTools:

    def __init__(self, argvs):
        self.info_obj = InfoObjFactory.get_info_obj(argvs)

    def generate_ast_files(self):
        ast_cmd = self.info_obj.get_ast_cmd()
        os.system(ast_cmd)


    def inject_source_files(self):
        inj_prog_name = "./instrumentor "
        os.system("rm -f " + self.info_obj.log_file)
        for inj_cmd in self.info_obj.get_inj_cmd(inj_prog_name):
            os.system(inj_cmd)
        self.info_obj.fresh_inj_lines()
    # end of inject_source_files

    def compile_inject_files(self):
        compile_cmd = self.info_obj.get_compile_cmd()
        os.system(compile_cmd)
    # end of compile_inject_files

    def run_tracer(self):
        old_records = set()
        for path_record in os.listdir("."):
            if fnmatch.fnmatch(path_record, REC_PREFIX + "*"):
                old_records.add(path_record)
        os.system("./" + TRACE_PROG_NAME)
        fresh_records = set()
        for path_record in os.listdir("."):
            if fnmatch.fnmatch(path_record, REC_PREFIX + '*'):
                fresh_records.add(path_record)
        fresh_records = fresh_records.difference(old_records)
        self.info_obj.fresh_exc_lines(fresh_records)
    # end of run_tracer

    def report(self):
        print "Path Coverage Report"
        print " ==  ==  ==  ==  ==  ==  ==  == =="
        exc_total = 0
        inj_total = 0
        connect_obj = lambda str_sum, str_next: str_sum + " " + str_next
        for idx in range(len(self.info_obj.source_files)):
            fname = self.info_obj.source_files[idx]
            fid = self.info_obj.map_fname_fid[fname]
            inj_lines = sorted(self.info_obj.map_fid_inj_lines[fid])
            exc_lines = sorted(self.info_obj.map_fid_exc_lines[fid])
            print "File Name: " + fname
            print "Inject Lines: " + reduce(connect_obj, map(str, inj_lines))
            print "Execute Lines: " + reduce(connect_obj, map(str, exc_lines))
            print "Coverage Rate: {0:.2%}".format(float(len(exc_lines))/float(len(inj_lines)))
            print " ==  ==  ==  ==  ==  ==  ==  == =="
            exc_total += len(exc_lines)
            inj_total += len(inj_lines)
            self.view(fname, self.info_obj.view_files[idx], inj_lines, exc_lines)

        print "Total Coverage Rate: {0:.2%}".format(float(exc_total) / float(inj_total))
    # end of report

    def view(self, source_fname, view_fname, inj_lines, exc_lines):
        finput  = open(source_fname, "r")
        foutput = open(view_fname, "w")

        lines = finput.readlines()
        inj_lines = set(inj_lines)
        exc_lines = set(exc_lines)
        for idx in range(len(lines)):
            if idx+1 in inj_lines:
                if idx+1 in exc_lines:
                    foutput.write(LABEL_EXC + " " + lines[idx])
                else:
                    foutput.write(LABEL_INJ + " " + lines[idx])
            else:
                foutput.write(LABEL_BLK + " " + lines[idx])
        foutput.close()
        finput.close()
    # end of view



def main():
    # the first argv is "./inject_tools.py"
    if len(sys.argv) < 2:
        print 'inject_tool: error: no input files'

    # argv[0] is "inject_tools.py"
    tool = InjectTools(sys.argv[1:])
    print "Generate AST Files"
    tool.generate_ast_files()
    print "Inject Source Files"
    tool.inject_source_files()
    print "Compile...."
    tool.compile_inject_files()
    print "Runing Target Program..."
    tool.run_tracer()
    print "Generate Report..."
    tool.report()
    print "End"



if __name__ == "__main__":
    main()







