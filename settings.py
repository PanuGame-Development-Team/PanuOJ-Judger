
CONFIG = {
    'SQLALCHEMY_DATABASE_URI': 'mysql+pymysql://root:root@localhost:3306/PojJudger',
    'SQLALCHEMY_TRACK_MODIFICATIONS': False,
    "SQLALCHEMY_ENGINE_OPTIONS": {
        "pool_size": 10,
        "pool_recycle": 1800,
        "pool_pre_ping": True,
    }
}
DEFAULT_IMAGE = "pojenv"
DEFAULT_MEMORY = 256000
DEFAULT_RUNTIME = 1000
