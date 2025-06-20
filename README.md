# PanuOJ 判题机后端

## 使用前注意事项
1. **判题机必须只能向前端服务器公开，否则容易成为挖矿病毒的牺牲品！！！**
2. **判题机必须在容器中稳定通过speed_j测试，使用方法：`gcc speed_j.c -o speed_j -O2 -lm -w`，然后`./speed_j`，连续数次判定`PASS`即可。** 本评测机的标准是 `400000000` 次即可通过，洛谷标准为 `533323830`。
3. 判题机现在需要使用标准输入输出。
## 使用方法
### 环境依赖
1. Python 3.12
2. 见 `Pipfile`，Pipenv依赖
3. docker.io，一个稳定的镜像，且包含评测编译环境，具体镜像设置在文件 `settings.py` 中。
### 构建方法
1. 编译 `judge.c`，执行 `gcc judge.c -o judge -O2 -lm -w`。
2. 创建所需要的 `testcases`、`sandbox` 文件夹，执行 `mkdir testcases sandbox`。
3. 创建 `settings.py` 文件，并填写相关配置，参考样板如下：
```python
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
FRONTEND_URL = ""
JUDGER_NAME = "Mercury"
UUID = ""                              # 你的ID，对于前端回调使用。
```
### 运行方法
1. 使用 gunicorn/uwsgi + systemctl 部署。如有必要，可以置一层nginx反向代理。
2. 在前端服务器 `judgerlib.py` 中添加判题机地址。
3. 文件名必须为 `testxx.in` 和 `testxx.ans`，其中 `xx` 为连续正整数，表示第几组测试数据。
## 项目问题
1. 评测时，出于我们未知的问题，可能会定期出现7ms的评测时长波动，不会影响评测结果，但是如果要对时长进行排名，会极大影响排名。
2. 评测机监测内存与运行时长过于吃性能，可能导致评测机性能下降。如果遇到性能瓶颈请考虑加长评测时间限制。