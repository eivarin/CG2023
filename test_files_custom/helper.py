import re
import sys
import os

comment_detector = re.compile(r'(?<=<!--\s)(?:\.\/)?generator .*(?=\s-->)')

f = open(sys.argv[1], 'r')
full_file = f.read()

os.system("make clean")
os.system("make")

command:str
for command in comment_detector.findall(full_file):
    try:
        # print(command)
        if(command.startswith("./")):
            os.system(command)
        else:
            os.system("./"+command)
    except:
        print('No such command')

os.system(f"./engine {sys.argv[1]}")