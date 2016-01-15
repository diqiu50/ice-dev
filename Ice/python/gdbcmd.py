#!/usr/bin/python           
# Filename: gdbcmd.py

import gdb
import string 
import re

class PrintStackTracer(gdb.Command):
    "print thread stack tracer!"
    def __init__(self):
        super(PrintStackTracer, self).__init__("thrds", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True);

    def invoke(self, arg, from_tty):
        argv = [];
        argc = 0;

        if (arg != ""):
            argv = arg.split(' ');
            argc = len(argv);

        _max = 1000000;
        _min = 1;
        if (argc == 1):
            _max = int(argv[0]);
        elif (argc == 2):
            _max = int(argv[0]);
            _min = int(argv[1]);

        p = re.compile('^(\*| ) (\d{1,5})', re.MULTILINE);
        str = gdb.execute("info thread", False, True);
        tidarray = "";
        for tid in p.findall(str):
            id = int(tid[1])
            if (id <= _max and id >= _min):
                tidarray += tid[1] + " "
        #print tidarray;
        print(gdb.execute("thread apply " + tidarray + " bt", False, True));


class InitZykie(gdb.Command):
    def __init__(self):
        super(InitZykie, self).__init__("initzykie", gdb.COMMAND_SUPPORT, gdb.COMPLETE_NONE, True);

    def invoke(self, arg, from_tty):
        str = gdb.execute("info sources", False, True);
        strs = str.split("\r\n")
        filelists = strs[-1];
        #print (filelists[0:300]);
        
        p = re.compile('(/.+tup/mnt/@tupjob-\d{4,5}/).+');
        s = set();
        for line in filelists.split(","):
            mt = p.search(line);
            if mt:
                s.add(mt.group(1));

        for dir in s:
            cmd = "set substitute-path " + dir + " /"
            #print(cmd)
            str = gdb.execute(cmd, False, True);

PrintStackTracer();
InitZykie();
