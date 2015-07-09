/**
* lexer.h
*/

#ifndef lexer_h
#define lexer_h

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;


struct Word
{
    string label, name;
    int line, type;
    long long val;
    Word(int ln = -1, string lb = "", string nm = "", long long v = 0);
    void print();
};

class Lexer;

class Lexer {
public:    
    int line;
    char ch;
    vector<Word> words;  //结果存储

    Lexer();
    void initial();    //初始化行号和关键字个数
    char remove_blank(char ch);    //移去空格，缩进，并计算行号
    char number(char ch);    //分析数字
    char word(char ch);    //分析单词
    char others(char ch);    //分析单词和数字以外的字符
    long long char2num(string num);    //将字符串转换成数字
    bool check_key(string s);    //检查是否是关键字
    bool judgeOpt(string s,int k);  //判断是否是运算符等
    void runLexer();  //运行词法分析器
    void print();    //输出结果
};

#endif