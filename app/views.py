#!/usr/bin/env python
#-*- coding:utf-8 -*-

from functools import wraps
import os

from flask import render_template, request, redirect, flash

from app import app
from config import DEF_FORLDER, JUDGE_FOLDER
from judge import judge, delete_tmp_file


def throw_exception(f):
    @wraps(f)
    def call(*args, **kwargs):
        try:
            return f(*args, **kwargs)
        except Exception, e:
            print e
            return unicode(e)
    return call



@app.route('/', methods = ['GET', 'POST'])
@throw_exception
def home():
    if request.method == 'GET':
        return render_template('index.html')
    else:
        codes = request.form['codes'].split('\n')
        delete_tmp_file('code.txt')
        delete_tmp_file('result.txt')
        judge(codes)
        return redirect('/result')

@app.route('/result')
def result():
    result_file = open(os.path.join(JUDGE_FOLDER, 'result.txt') )
    result = [ line.decode('utf-8') for line in result_file]
    result_file.close();
    code_file = open(os.path.join(JUDGE_FOLDER, 'code.txt') )
    tmp = code_file.read().split('\r')
    # print tmp
    codes = [ (i, line) for (i, line) in enumerate(tmp) ]
    code_file.close()
    return render_template('result.html', result = result, codes = codes)



@app.route('/wordsDef')
@throw_exception
def wordsDef():
    word_file = open(os.path.join(DEF_FORLDER, 'wordDefination.txt'),'r')
    words = [line.decode('utf-8').split(' ') for line in word_file]
    # print words
    word_file.close()
    return render_template('wordsDef.html', words = words)

@app.route('/gramDef')
@throw_exception
def gramDef():
    grammar_file = open(os.path.join(DEF_FORLDER, 'SyntaxDefCN.txt'),'r')
    cng = [line.decode('utf-8') for line in grammar_file]
    grammar_file.close()
    grammar_file = open(os.path.join(DEF_FORLDER, 'initGram.txt'),'r')
    grammar = [line.decode('utf-8') for line in grammar_file]
    grammar_file.close()
    return render_template('gramDef.html', cnExplain = cng, grammar = grammar)

@app.route('/closure')
@throw_exception
def closure():
    closure_file = open(os.path.join(DEF_FORLDER, 'Closure.txt'),'r')
    closure = [line.decode('utf-8') for line in closure_file]

    closure_file.close()
    return render_template('closure.html', closure = closure)

@app.route('/LR1Table')
@throw_exception
def LR1Table():
    table_file = open(os.path.join(DEF_FORLDER, 'LR1Table.txt'),'r')
    tmptable = [line[:-2].decode('utf-8') for line in table_file][2:]
    table = tmptable
    table_file.close()
    return render_template('LR1Table.html', table = table)

@app.route('/action')
@throw_exception
def action():
    action_file = open(os.path.join(DEF_FORLDER, 'action.txt'),'r')
    action = [line[:-2].decode('utf-8') for line in action_file]
    action_file.close()
    # print action
    return render_template('action.html', action = action)

@app.route('/goto')
@throw_exception
def goto():
    goto_file = open(os.path.join(DEF_FORLDER, 'goto.txt'),'r')
    goto = [line[:-2].decode('utf-8') for line in goto_file]
    goto_file.close()
    return render_template('goto.html', goto = goto)
