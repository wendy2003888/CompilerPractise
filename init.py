#!/usr/bin/env python
#-*- coding:utf-8 -*-

import os

from app import db
from app.models import User, Team
from config import ADMIN_NAME, ADMIN_PASSWORD

if os.path.exists('app' + os.sep + 'data' + os.sep + 'data.db'):
    print 'delete old db file'
    os.remove('app' + os.sep + 'data' + os.sep + 'data.db')

print 'create new db file'
db.create_all()
print 'add admin'
db.session.add(User(ADMIN_NAME, ADMIN_PASSWORD))
db.session.commit()
db.session.close()
