编译原理 —— 一个编译器

词法分析器        √
语法分析器        √
    使用自底向上的LR(1)分析方法，允许左递归，能解决SLR(1)解决不了的冲突
    能自动生成非终结符集， 终结符集， First集， 项目集规范簇 和 LR(1)分析表
    判断字符串是否合法, 生成语法树
语义分析          √
中间代码生成      √   
    生成三元式或者四元式
中间代码解释器＊


Compiler
Lexer              √
Parser             √
    Use LR(1) which is a downn to top approach
    Can generate Vn Set, Vt Set, First Set, Clousure and LR(1) Table automatically
    Can judge if string is valid to the Syntax, generate syntax tree
Semantic analysis  √
    (Syntax-directed translation)
Code generation    √
    Tree or four configurations sequences
Code optimization *

运行build.sh编译，再运行./main < in1.txt运行程序
可以替换in1.txt成 in2.txt in3.txt

test文件夹里式测试样例用于Parser的测试
1-5测试first集，项目集规范簇 和 LR(1)表
6-7测试判断字符串是否合法的程序

definition.txt是语法定义文件
格式: !! 重要， 重要， 重要 !!

A->B | a B
B->b | #

每个规则中间用一个空格 ＋ "|" ＋ 一个空格隔开, 每条规则的项目中间用空格分开 
例如B-> a B | c A id [这个相当于规则 B->aB和B->cAid (id是终结符)]
每一个规则最左边的非终结符和第一个定义规则用 "->"连接，中间不要出现空格
!!!!!!!!!!!!!终结符用小写字母或者符号!!!!!!!!!!!!!!
"#"代表空规则, 也可以用其他字符，但是要改parser.cpp里Class Grammar中的ept;
按照格式修改defnition.txt，
修改掉parser.cpp文件里的print函数和main函数注释，
运行程序会自动产生所要生成的LR(1)表


"definition.txt" is a file declares the Syntax definiton.
Format: !! VERT IMPORTANT !!

A->B | a B
B->b | #

Please use " | " (a space + '|' + a space) to seperate different rule of a Vn item, 
use " " (a space) to seperate Vt and Vn of a rule item. e.g. B-> a B | c A id [that means B->aB, B->cAid (id is a Vt)]
Use "->" to link the left Vn item of a rule and the first rule item (!! do not add any space !!). 
!!!!!!!!!!!!! Vt item should be lower case letters or signs !!!!!!!!!!!!!!
"#" means empty rule, you can also change it both in "definition.txt" and "parser.cpp",
change the "ept" in "Grammar class" of "parse.cpp".
Please edit the "defnition.txt" as the format above,
then delete the comment in function "print()" and "main" of "parser.cpp",
and run the program then get the LR(1) table.

Author: wendy2003888