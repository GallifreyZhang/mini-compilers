import os
import re

"""
LR语法分析时，若原始文法中不存在空产生式，
即使存在左递归文法，求First集不受影响，忽略
左递归产生式即可。
"""

class Production():
    """
    文法符号定义为二元组：
    (symbol, type)，symbol为文法符号，type为终结符或非终结符
    产生式定义：
    Production类是一个产生式集，每个对象的产生式左部相同；
    左部是一个二元组，右部是一个二元组串数组
    """
    def __init__(self, left ,right):
        self.left = left
        self.right = right 


class Grammar:
    """
    ProductionGroup为所有的文法，是一个数组
    每一项为一个二元组，
    第一个元素是左部的文法符号；
    第二个元素是Production对象
    """
    
    def __init__(self):
        self.START = ("P", "NT")
        self.variable_list = [
            ("P", "NT"),
            ("D", "NT"),
            ("L", "NT"),
            ("S", "NT"),
            ("C", "NT"),
            ("E", "NT"),
            ("T", "NT"),
            ("F", "NT")
        ]
        self.terminal_list = [
            ("id", "T"),
            (";", "T"),
            ("int", "T"),
            ("float", "T"),
            ("(", "T"),
            (")", "T"),
            ("<", "T"),
            (">", "T"),
            ("==", "T"),
            ("+", "T"),
            ("-", "T"),
            ("*", "T"),
            ("/", "T"),
            ("=", "T"),
            ("int10", "T"),
            ("else", "T"),
        ]
        self.start_production = [("P", "NT"), [("D", "NT"), ("S", "NT")]]
        self.ProductionGroup = {}
        left = ("P", "NT") # 开始符号
        right = [
            [("D", "NT"), ("S", "NT")],
        ]
        # self.ProductionGroup.append(left, Production(left, right))
        self.ProductionGroup[left] = right

        left = ("D", "NT")
        right = [
            [("L", "NT"), ("id", "T"), (";", "T"), ("D", "NT")],
            [("", "NULL")],
        ]
        self.ProductionGroup[left] = right

        left = ("L", "NT")
        right = [
            [("int", "T")],
            [("float", "T")]
        ]
        self.ProductionGroup[left] = right 

        left = ("S", "NT")
        right = [
            [("id", "T"), ("=", "T"), ("E", "NT"), (";", "T")],
            [("if", "T"), ("(", "T"), ("C", "NT"), (")", "T"), ("S", "NT")],
            [("if", "T"), ("(", "T"), ("C", "NT"), (")", "T"), ("S", "NT"), ("else", "T"), ("S", "NT")],
            [("while", "T"), ("(", "T"), ("C", "NT"), (")", "T"), ("S", "NT")],
            [("S", "NT"), (";", "T"), ("S", "NT")]
        ]
        self.ProductionGroup[left] = right

        left = ("C", "NT")
        right = [
            [("E", "NT"), (">", "T"), ("E", "NT")],
            [("E", "NT"), ("<", "T"), ("E", "NT")],
            [("E", "NT"), ("==", "T"), ("E", "NT")],
        ]
        self.ProductionGroup[left] = right

        left = ("E", "NT")
        right = [
            [("E", "NT"), ("+", "T"), ("T", "NT")],
            [("E", "NT"), ("-", "T"), ("T", "NT")],
            [("T", "NT")]
        ]
        self.ProductionGroup[left] = right

        left = ("T", "NT")
        right = [
            [("F", "NT")],
            [("T", "NT"), ("*", "T"), ("F", "NT")],
            [("T", "NT"), ("/", "T"), ("F", "NT")],
        ]
        self.ProductionGroup[left] = right

        left = ("F", "NT")
        right = [
            [("(", "T"), ("E", "NT"), (")", "T")],
            [("id", "T")],
            [("int10", "T")]
        ]
        self.ProductionGroup[left] = right


    def get_first_set(self):
        """
        求First集，First集是集合类型的数据结构
        如果文法符号是终结符，则其First集是其本身；
        若文法符号是非终结符，则其First集是产生式右部第一个符号；
        若第一个符号的产生式含有空产生式，则将第二个符号放入First集，
        以此类推
        处理空产生式的First集
        """
        self.FirstSetGroup = {}
        variable_proc = set()
        for t in self.terminal_list:
            self.FirstSetGroup[t] = set()
            self.FirstSetGroup[t].add(t)
        for v in self.variable_list:
            self.FirstSetGroup[v] = set()
            for item in self.ProductionGroup[v]:
                for j in item:
                    if v == j: # 左递归的情形，跳过这一项
                        break;
                    self.FirstSetGroup[v].add(j)
                    if j[1] == "NT":
                        variable_proc.add(j)
                        if (("", "NUll")) not in self.ProductionGroup[j]:
                            break
                    elif j[1] == "T":
                        break
                    # 继续循环的情况是首非终结符含有空产生式
        # 不能完全处理空产生式，只是针对该实验的文法，空产生式是直接推导的，不存在间接产生的情况
        # 假设左递归式的First集处理中不出现空产生式
        tmp = variable_proc.copy()
        null_symbol = set()
        null_symbol.add(("", "NULL"))
        while variable_proc != set():
            variable_proc = tmp.copy()
            for v in variable_proc:
                """
                variable_proc中的元素都是出现在某些First集中的非终结符
                如果这个非终结符的First集中没有非终结符，即与variable_proc的交集为空 
                则将这个非终结符从variable_proc中删去；
                并且将First集中含有该非终结符的元素用该非终结符的First集替代
                """
                if self.FirstSetGroup[v].isdisjoint(tmp):
                    tmp.discard(v)
                    for i in self.variable_list:
                        if v in self.FirstSetGroup[i]:
                            self.FirstSetGroup[i].discard(v)
                            self.FirstSetGroup[i] = self.FirstSetGroup[i] | (self.FirstSetGroup[v] - null_symbol)
                            # for j in self.FirstSetGroup[v]:
                            #     if j != ("", "NULL"):
                            #         self.FirstSetGroup[i].add(j)


    def get_follow_set(self):
        """
        假设在求一个非终结符的Follow集时，该非终结符后的非终结符不存在
        可推导出空产生式的情况；
        开始符号的Follow集初始化为{$}
        对于产生式A->aBb
        结束时机：
        第一遍循环结束后，根据后继符号的First集求Follow集已经结束
        Follow集的更新只能靠产生式最右符号的Follow集更新
        """
        self.FollowSetGroup = {}
        for v in self.variable_list:
            self.FollowSetGroup[v] = set()
        self.FollowSetGroup[self.START].add(("$", "END"))

        # 遍历所有产生式，初步产生Follow集
        for key in self.ProductionGroup.keys():
            productions = self.ProductionGroup[key]
            for production in productions:
                prod_len = len(production)
                for i in range(prod_len-1):
                    if production[i][1] == "NT":
                        self.FollowSetGroup[production[i]] = self.FollowSetGroup[production[i]] | self.FirstSetGroup[production[i+1]]
                if production[prod_len-1][1] == "NT":
                    self.FollowSetGroup[production[prod_len-1]] = self.FollowSetGroup[production[prod_len-1]] | self.FollowSetGroup[key]     

        # 更新Follow集，需设置一个Follow集不再变化标志flag
        flag = True
        while flag:
            flag = False
            for key in self.ProductionGroup.keys():
                productions = self.ProductionGroup[key]
                for production in productions:
                    prod_len  =len(production)
                    if production[prod_len-1][1] == "NT":
                        if self.FollowSetGroup[production[prod_len-1]] != self.FollowSetGroup[production[prod_len-1]] | self.FollowSetGroup[key]:
                            self.FollowSetGroup[production[prod_len-1]] = self.FollowSetGroup[production[prod_len-1]] | self.FollowSetGroup[key]  
                            flag = True    
                            
    def get_item_set(self):
        """
        SLR(1)的项目集闭包 
        项目集以及项目的定义：
        对于每一个产生式，项的定义是一个二元组：
        第一个元素是该产生式，第二个元素是点在产生式中的位置，
        第一个位置是0，第二个位置是产生式右部的长度
        项目集的数据结构是一个列表，列表的每一个元素是一个项目
        项目集族由一个列表和一个GO字典组成
        列表的下标就是项目集的标号
        列表的元素就是项目集

        GO字典对应着寻找有效活前缀的DFA
        字典的key是一个二元组：第一个元素是当前状态，第二个元素是读入的符号
        value是一个数字，表示进入的状态

        扩展文法重新定义文法的结构，文法全部放入一个列表
        列表的每一项都是一个产生式项，每个产生式项都是一个三元组
        第一个是文法符号；第二个是个文法符号的列表；第三个是点在产生式中的位置
        """
        self.ItemSetGroup = {} # {int : [[[],int], [[],int],]}
        self.ExtGrammarGroup = []
        self.ExtGrammarGroup.append([("P'", "NT"), [("P", "NT")]])
        # # 生成扩展文法项
        # # TODO: 空产生式的扩展
        for v in self.variable_list:
            productions = self.ProductionGroup[v]
            for production in productions:
                prod_len = len(production)
                self.ExtGrammarGroup.append([v, production])
        
        J = 0
        self.ItemSetGroup[0] = []
        ext_start_production = [self.start_production, 0]
        self.ItemSetGroup[0].append(ext_start_production)
        for item in self.ItemSetGroup[0]:
            res = self.get_closure(item)
            for r in res:
                if r not in self.ItemSetGroup[0]:
                    self.ItemSetGroup[0].append(r)

        self.GO = {}
        self.GO[0] = {}

        # for item in self.ItemSetGroup[0]:
        #     ext_prod = item[0]
        #     if item[1] < len(ext_prod[1]):
        #         if ext_prod[1][item[1]] not in self.GO[0].keys():
        #             J += 1
        #             self.GO[0][ext_prod[1][item[1]]] = J
        #             self.ItemSetGroup[J] = []
        #             self.ItemSetGroup[J].append([item[0], item[1]+1])
        #         else:
        #             if [item[0], item[1]+1] not in ItemSetGroup[J]:
        #                 self.ItemSetGroup[J].append([item[0], item[1]+1])

        j = 0
        while j <= J:
            self.GO[j] = {}
            for item in self.ItemSetGroup[j]:
                ext_prod = item[0]
                if item[1] < len(ext_prod[1]):
                    if ext_prod[1][item[1]] not in self.GO[j].keys():
                        J += 1
                        self.GO[j][ext_prod[1][item[1]]] = J
                        self.ItemSetGroup[J] = []
                        self.ItemSetGroup[J].append([item[0], item[1]+1])
                    else:
                        if [item[0], item[1]+1] not in self.ItemSetGroup[J]:
                            self.ItemSetGroup[J].append([item[0], item[1]+1])
            j += 1

        self.status_count = j

        # self.ItemSetGroup.append(set())
        # # 将扩展文法的第一项放入第一个项目集中
        # self.ItemSetGroup[0].add(0)
        # ext_len = len(self.ExtGrammarGroup)
        # for i in self.ItemSetGroup[0]:
        #     # 计算闭包
        #     ext_gram = self.ExtGrammarGroup[i]
        #     gram_len = len(ext_gram[1])
        #     if ext_gram[2] < gram_len:
        #         if ext_gram[1][ext_gram[2]][1] == "NT":
        #             rights = self.ProductionGroup[ext_gram[1][ext_gram[2]]]    
        #             for right in rights:
        #                 j = self.ExtGrammarGroup.index([ext_gram[1][ext_gram[2]], right, 0])
        #                 self.ItemSetGroup[0].add(j)
        

    def get_closure(self, production):
        """
        传入参数production只是一个产生式
        只求直接闭包：对于res中的新产生式不求闭包，
        交由上层调用函数解决
        """
        # production : [[(), [(),()]], i]
        res = [production]
        right_part = production[0][1]
        prod_len = len(right_part)
        if production[1] < prod_len and right_part[production[1]][1] == "NT":
            var_symbol = right_part[production[1]]
            for ext_prod in self.ExtGrammarGroup:
                if var_symbol == ext_prod[0]:
                    if [ext_prod, 0] not in res:
                        res.append([ext_prod, 0])
        return res

    def get_slr_table(self):
        """
        建立action表和goto表
        """
        self.action_table = {}
        self.goto_table = {}
        for i in range(self.status_count):
            self.action_table[i] = {}
            self.goto_table[i] = {}
            for v in self.variable_list:
                if v in self.GO[i].keys():
                    self.goto_table[i][v] = self.GO[i][v]

            for item in self.ItemSetGroup[i]:
                right_part = item[0][1]
                if item[1] < len(right_part):
                    symbol = right_part[item[1]]
                    if right_part[item[1]][1] == "T":
                        if right_part[item[1]] in self.GO[i].keys():
                            self.action_table[i][right_part[item[1]]] = "S" + str(self.GO[i][right_part[item[1]]])
                    elif symbol[1] == "NT":
                        pass
                elif item[1] == len(right_part):
                    var_symbol = item[0][0]
                    for terminal in self.FollowSetGroup[var_symbol]:
                        self.action_table[i][terminal] = "r" + str(self.ExtGrammarGroup.index(item[0]))

            if [self.ExtGrammarGroup[0][0], 1] in self.ItemSetGroup[i]:
                self.action_table[i][("$", "END")] = "acc"
            

if __name__ == "__main__":
    G = Grammar()
    G.get_first_set()
    G.get_follow_set()
    G.get_item_set()
    G.get_slr_table()
    pass