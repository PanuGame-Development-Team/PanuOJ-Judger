from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
db = SQLAlchemy()
class Record(db.Model):
    id = db.Column(db.Integer,primary_key=True)
    result = db.Column(db.String(10))
    submit_time = db.Column(db.DateTime,default=datetime.now)
    runtime = db.Column(db.Integer)
    memory = db.Column(db.Integer)
    detail = db.Column(db.Text)