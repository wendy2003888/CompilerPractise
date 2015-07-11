编译原理 —— 一个编译器

词法分析器        √
语法分析器        √
    使用自底向上的LR(1)分析方法，允许左递归，能解决SLR(1)解决不了的冲突
    能自动生成非终结符集， 终结符集， First集， 项目集规范簇 和 LR(1)分析表
    判断字符串是否合法, 生成语法树
语义分析         √
中间代码生成      √   
    生成四元式
中间代码解释器＊   未写

语义分析同时生成四元式有bug

Compiler
Lexer              √
Parser             √
    Use LR(1) which is a downn to top approach
    Can generate Vn Set, Vt Set, First Set, Clousure and LR(1) Table automatically
    Can judge if string is valid to the Syntax, generate syntax tree
Semantic analysis  √
    (Syntax-directed translation)
Code generation    √
     quaternion
Code optimization * not write

搭建了网页的用户界面
python start.py
localhost:8080

Author: wendy2003888
