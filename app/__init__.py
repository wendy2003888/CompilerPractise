#!/usr/bin/env python
#-*- coding:utf-8 -*-

from flask import Flask

FLASKS_ETTINGS = '../config.py'

app = Flask(__name__)
app.config.from_pyfile(FLASKS_ETTINGS, silent = False)