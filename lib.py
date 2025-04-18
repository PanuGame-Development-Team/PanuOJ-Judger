from uuid import uuid4
from zipfile import ZipFile
from model import *
from judgelib import judge as _judge
from json import dumps,loads
def lin(ls1,ls2):
    for i in ls1:
        if not i in ls2:
            return False
    return True
def judge(code,language,O2,time_limit,memory_limit,problem_id,testcases,record_id,app):
    ans = _judge(code,language,O2,time_limit,memory_limit,problem_id,testcases)
    status = ans[0]
    detail = ans[1]
    with app.app_context():
        rec = Record()
        rec.id = record_id
        rec.result = status
        rec.memory = 0 if status != "OK" else max(*[i[2] for i in detail])
        rec.runtime = 0 if status != "OK" else max(*[i[3] for i in detail])
        rec.detail = dumps(detail)
        db.session.add(rec)
        db.session.commit()