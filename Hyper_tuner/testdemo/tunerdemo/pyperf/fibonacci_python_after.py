# -*- coding: utf-8 -*-
import ctypes
import os
import sys

# 指定动态链接库
current_path = sys.argv[0]
so_path = os.path.join(os.path.dirname(current_path), 'fibonacci_after.so')
lib = ctypes.cdll.LoadLibrary(so_path)

# 需要指定返回值的类型，默认是 int
lib.problematic_fibonacci_new.restype = ctypes.c_void_p
lib.problematic_fibonacci_calculate.argtypes = [ctypes.c_void_p, ctypes.c_int]
lib.problematic_fibonacci_calculate.restype = ctypes.c_int
lib.problematic_fibonacci_delete.argtypes = [ctypes.c_void_p]


class ProblematicFibonacci(object):
    def __init__(self):
        # 动态链接对象
        self.obj = lib.problematic_fibonacci_new()

    def calculate(self, n):
        res = lib.problematic_fibonacci_calculate(self.obj, n)
        return res

    def __del__(self):
        # 释放对象
        lib.problematic_fibonacci_delete(self.obj)


# 将 python 类型转换成 c 类型，支持 int 的变量转换
def convert_type(input):
    ctypes_map = {int: ctypes.c_int}
    input_type = type(input)
    if input_type in ctypes_map:
        return ctypes_map[input_type](input)
    else:
        return None


if __name__ == '__main__':
    t = ProblematicFibonacci()
    import time

    print(os.getpid())
    time.sleep(5)
    n = 10000000
    result = t.calculate(convert_type(n))
    print(f"Fibonacci({n}): {result}")
