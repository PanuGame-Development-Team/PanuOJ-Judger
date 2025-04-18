from flask import *
from constances import *
from settings import *
from model import *
from lib import *
from multiprocessing import Process
app = Flask(__name__)
for config in CONFIG:
    app.config[config] = CONFIG[config]
db.init_app(app)
@app.route('/',methods=["POST"])
def index():
    if not lin(["problem_id","record_id","time","memory","language","O2","code","tests"],request.form) or not lin(["testcases"],request.files):
        return jsonify({"status":"ERROR","message":"missing parameters"})
    testcases = request.files["testcases"]
    tests = int(request.form["tests"])
    problem_id = request.form["problem_id"]
    record_id = request.form["record_id"]
    time = int(request.form["time"])
    memory = int(request.form["memory"])
    language = request.form["language"]
    O2 = bool(int(request.form["O2"]))
    code = request.form["code"]
    filename = f"/tmp/{uuid4().hex}.zip"
    testcases.save(filename)
    ZipFile(filename).extractall(f"testcases/{problem_id}")
    Process(target=judge,args=(code,language,O2,time,memory,problem_id,tests,record_id,app)).start()
    return jsonify({"status":"OK"})
@app.route('/record/<int:record_id>',methods=["GET"])
@app.route('/record/<int:record_id>/',methods=["GET"])
def result(record_id):
    rec = Record.query.get(record_id)
    if rec == None:
        return jsonify({"status":"ERROR","message":"record not found or judging not finished"})
    return jsonify({"status":"OK","result":rec.result,"memory":rec.memory,"runtime":rec.runtime,"detail":loads(rec.detail)})
@app.route('/heartbeat',methods=["GET"])
@app.route('/heartbeat/',methods=["GET"])
def heartbeat():
    return jsonify({"status":"OK"})
if __name__ == "__main__":
    app.run(host=HOST,port=PORT)