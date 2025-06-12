from docker import from_env as _docker_from_env
from os import system,mkdir
from os.path import abspath as _abspath
from settings import *
_abs_path = _abspath(__file__).split("/judgelib.py")[0]
def judge(code,language,O2,time_limit,memory_limit,problem_id,testcases):
    client = _docker_from_env()
    if language == "C":
        compilecommand = f"""gcc target.c -o target {"-O2 " if O2 else ""} -lm -w"""
    elif language == "C++":
        compilecommand = f"""g++ target.cpp -o target {"-O2 " if O2 else ""} -lm -w"""
    elif language == "C++11":
        compilecommand = f"""g++ target.cpp -o target -std=c++11 {"-O2 " if O2 else ""} -lm -w"""
    elif language == "C++14":
        compilecommand = f"""g++ target.cpp -o target -std=c++14 {"-O2 " if O2 else ""} -lm -w"""
    elif language == "C++17":
        compilecommand = f"""g++ target.cpp -o target -std=c++17 {"-O2 " if O2 else ""} -lm -w"""
    elif language == "C++20":
        compilecommand = f"""g++ target.cpp -o target -std=c++20 {"-O2 " if O2 else ""} -lm -w"""
    system(f"rm -r sandbox")
    mkdir("sandbox")
    system(f"cp -r testcases/{problem_id}/* sandbox")
    system("cp judge sandbox")
    if language.startswith("C++"):
        with open("sandbox/target.cpp", "w") as f:
            f.write(code)
    elif language == "C":
        with open("sandbox/target.c", "w") as f:
            f.write(code)
    result = client.containers.run(DEFAULT_IMAGE,
                    f"""/sandbox/judge {time_limit} {memory_limit} "{compilecommand}" {testcases} strcmp""",
                    remove=True,volumes={_abs_path + "/sandbox":{"bind":"/sandbox/","mode":"rw"}},
                    working_dir="/sandbox").decode("UTF-8")
    result = result.strip("\n").split("\n")
    status = result.pop(0)
    if status == "OK":
        for i in range(len(result)):
            result[i] = result[i].split(" ")
            result[i][0] = int(result[i][0])
            print(result[i])
            result[i][2] = int(result[i][2])
            result[i][3] = int(result[i][3])
    return status,result