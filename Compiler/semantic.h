/**
* semantic.h
*/

#ifndef semantic_h
#define semantic_h

#include "parser.h"


class Semantic {
public:
    map<string, string> typeTable;
    map<string, syntaxTree::TreeNode*> placeTable;
    map<int, MidCode> Code;

    Semantic();
    bool semantic(vector<Word> input, syntaxTree &synTree, Grammar G);  //语义分析程序
    string num2Char(int a);  //将数字转换成字母
    void backpatch(syntaxTree::TreeNode &a, int place, int flag);  //拉链回填
    void emitMidCode(syntaxTree synTree);  //输出4元式
    bool start(vector<Word> input, syntaxTree &synTree, Grammar G);  //开始语法语义分析并生成语法树和中间代码(四元式)
};

#endif