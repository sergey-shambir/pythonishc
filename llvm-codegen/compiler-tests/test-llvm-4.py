#!/usr/bin/env python3

import os.path
import subprocess
import shutil

def get_compiler_path():
    compiler_path = os.path.abspath("../../debug-compiler-theory-samples/llvm_4")
    if not os.path.exists(compiler_path):
        raise ValueError('compiler llvm_4 not found')
    return compiler_path

class Runner:
    def __init__(self, compiler_path):
        self.compiler_path = compiler_path

    def run(self, input_name):
        input_path = os.path.abspath(os.path.join("data", input_name))
        with open(input_path, "r") as input_file:
            print("Running", input_name)
            subprocess.check_call([self.compiler_path], stdin=input_file)
            obj_file_name = os.path.splitext(input_name)[0] + ".o"
            shutil.move("program.o", os.path.join("out", obj_file_name))

def main():
    r = Runner(get_compiler_path())
    r.run("first-space-velocity.txt")
    r.run("if_branching.txt")
    r.run("simple_strings_concat.txt")
    r.run("square.txt")
    r.run("advanced_strings_concat.txt")

if __name__ == "__main__":
    main()
