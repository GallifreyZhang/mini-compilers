import os
import re

"""
LR语法分析时，若原始文法中不存在空产生式，
即使存在左递归文法，求First集不受影响，忽略
左递归产生式即可。
"""

class Production():
    def __init__(self):
        pass