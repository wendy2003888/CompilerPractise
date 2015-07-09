#!/usr/bin/env python
#-*- coding:utf-8 -*-

import os

SECRET_KEY = 'dark flame master'

BASE_DIR = os.getcwd()
COMPILER_ADDR = BASE_DIR + os.sep + 'Compiler'
DEF_FORLDER = COMPILER_ADDR + os.sep + 'def'
JUDGE_FOLDER = COMPILER_ADDR + os.sep +'tmpJudgeFile'
QUEUE_SIZE = 5