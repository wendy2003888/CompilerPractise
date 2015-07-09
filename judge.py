#!/usr/bin/python
# -*- coding: utf-8 -*-

import os, subprocess
import codecs
from config import COMPILER_ADDR, JUDGE_FOLDER
import sys  

reload(sys)  
sys.setdefaultencoding('utf8')

def delete_tmp_file(filename):
    os.system(' '.join(['rm', os.path.join(JUDGE_FOLDER, filename) ] ) )

def run(filename):
    compile_cmd =  './main < ' + os.path.join(JUDGE_FOLDER, 'code.txt')
    # print compile_cmd
    p = subprocess.Popen(compile_cmd, shell=True, cwd=COMPILER_ADDR, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output,err =  p.communicate()
    if p.returncode == 0:
        return (True, output)
    else:
        return (False, err)

def judge(codes):
    tmpfile = codecs.open(os.path.join(JUDGE_FOLDER, 'code.txt'), 'w', encoding='utf8')
    for line in codes:
        tmpfile.write(line)
    tmpfile.close()
    result = run('code.txt')
    resultfile = codecs.open(os.path.join(JUDGE_FOLDER, 'result.txt'), 'w',encoding='utf8')
    resultfile.write(result[1]) 
    resultfile.close()
    if result[0] == True:
        return True
    return False