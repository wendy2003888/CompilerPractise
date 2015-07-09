/**
* parser.h
*/

#ifndef parser_h
#define parser_h

#include "lexer.h"
#include "syntaxTree.h"

#include <cstring>
#include <cctype>
#include <algorithm>
#include <stack>
#include <set>
#include <fstream>

using namespace std;

struct Rule
{
    int id;    //规则编号
    string lelm;  //leftElement左边的非终结符
    vector<string> relm;  //rightElement右侧的规则符号

    Rule(int i = 0, string lel = "");
    void print();

    bool operator < (const Rule &x) const;
    bool operator == (const Rule &x) const;
    bool operator != (const Rule &x) const;
};

class Grammar;

class Grammar {
public:
    const string ept; //empty  "#"

    class Item { 
    public:
        struct State
        {
            int dot;
            string ahead;
            State(int d = 0, string s = "#")
            {
                dot = d, ahead = s;
            }

            bool operator < (const State &x) const 
            {
                if(dot != x.dot)
                    return dot < x.dot;
                return ahead < x.ahead;
            }

            bool operator == (const State &x) const 
            {
                return dot == x.dot && ahead == x.ahead;
            }

            bool operator != (const State &x) const 
            {
                if(dot == x.dot)
                    return ahead != x.ahead;
                return dot != x.dot;
            }
        };

        set<pair<Rule, State> > itemSet; //增广文法项目

        bool operator < (const Item &x) const;
        bool operator == (const Item &x) const;
        bool operator != (const Item &x) const;
        void print();
    };
    

    set<string> vn;
    set<string> vt;
    string startSym;
    vector<Rule> rules;
    map<string, set<string> > first; //firstSet
    Item item;
    vector<set<pair<Rule, Item::State> > > closure; //项目集族
    map<int, map<string, int> > go_to;
    map<int, map<string, pair<char, int> > > action;

    Grammar();
    void initSyntex();  //初始化语法分析所用Vn, Vt, First集和LR1表
    map<string, set<string> > geneFirst();  //生成first集
    bool isVt(string s);  //判断是否是终结符
    bool hasNotVancancy(string s);  //判断是否空规则
    void initClosure();  //初始化项目集规范簇
    void initTable(int x);  //初始化每个规范簇的表
    set<string> getFirst(string s, string ahead);  //在生成项目集规范簇时取得后面字符和检查字符的first集
    void geneSet();  //生成项目集闭包
    void geneClosure();  //生成项目集规范簇，用于事先的debug
    int findSet(set<pair<Rule, Item::State> > s);  //检查当前生成的项目集闭包是否已经存在
    bool eqlSet(const set<pair<Rule, Item::State> > &a, const set<pair<Rule, Item::State> > &b);  //判断2个项目集闭包是否相等
    void buildLR1table();  //生成项目集规范簇的同时生成goto和action表
    int getRuleNum(Rule r);  //找到需要规约时候的规则号，用于生成action表
    void print();  //打印与输出first集，
};

#endif