// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <cctype>
// #include <string>
// #include <vector>

#include "lexer.h"

int keys, opts, cmps, rels, seps, types;
const char keywords[][10] = {"for", "while", "do", "if", "else", "void", "int", "double", "float", "char", "const", "return", "main", "include"};
const char type[][10] = {"int", "double", "float", "char"};
const char operators[][2] = {"+", "-", "*", "/", "%", "&", "|", "^", "~", "!", "="};
const char cmp_operators[][3] = {"<", "<=", ">", ">=", "==", "!="};
const char rel_operators[][3] = {"&&", "||"};
const char sep_operators[][3] = {",", ";", "(", ")", "{", "}"};
const char label[][20]={"art", "cmp", "rel", "sep"};


Word::Word(int ln, string lb, string nm, long long v)
{
    line = ln, label = lb, name = nm, val = v;
}

void Word::print()
{
    cout<<"line: "<<line<<" label: "<<label<<" name: "<<name<<" val: "<<val<<endl;
}

// Lexer Lexer;   //运行该文件时候debug用

Lexer::Lexer()
{
    line = 0;
    words.clear();
    initial();
}

void Lexer::initial()  //初始化行号和关键字个数
{
    line = 1;
    keys = 14;
    opts = 11;
    cmps = 6;
    rels = 2;
    seps = 6;
    types = 4;
}

char Lexer::remove_blank(char ch)  //移去空格，缩进，并计算行号
{
    while(ch == ' ' || ch == '\t')
    {
        ch = getchar();
    }
    if(ch == '\n' || ch == '\r')
    {
        line++;
        ch = getchar();
    }
    return ch;
}

char Lexer::number(char ch)    //分析数字
{
    string num;
    while(isdigit(ch))
    {
        num += ch;
        ch = getchar();
    }
    // token = Word(line, "num", num, char2num(num) );
    words.push_back( Word(line, "num", num, char2num(num) ) );
    return ch;
}

long long Lexer::char2num(string num)    //将字符串转换成数字
{
    long long number = 0;
    for(int i = 0; i < num.length(); i++)
    {
        number *= 10;
        number += num[i] - '0';
    }
    return number;
}

bool Lexer::check_key(string s)    //检查是否是关键字
{
    for(int i = 0; i < keys; i++)
    {
        if(s == keywords[i] )
            return true;
    }
    return false;
}

char Lexer::word(char ch)  //分析单词
{
    string word;
    while(isalpha(ch))
    {
        word += ch;
        ch = getchar();
    }
    if(check_key(word))
    {
        for(int i = 0; i < types; i++)
        {
            if(word == type[i])
            {
                // token = Word(line, "type", word);
                // token.type = word;
                words.push_back( Word(line, "type", word) );
                return ch;
            }
        }
        // token = Word(line, word, word);
        words.push_back( Word(line, word , word) );
    }
    else
    {
        // token = Word(line, "id", word);
        words.push_back( Word(line, "id", word) );
    }
    return ch;
}

bool Lexer::judgeOpt(string s,int k)  //判断是否是运算符等
{
    if(k == 0)
    {
        for(int i = 0;i < opts; i++)
        {
            if(s.compare(operators[i])==0)
                return true;
        }
    }
    else if(k == 1)
    {
        for(int i = 0; i < cmps; i++)
        {
            if(s.compare(cmp_operators[i])==0)
                return true;
        }
    }
    else if(k == 2)
    {
        for(int i = 0;i < rels; i++)
        {
            if(s.compare(rel_operators[i])==0)
                return true;
        }

    }
    else if(k == 3)
    {
        for(int i = 0; i < seps; i++)
        {
            if(s.compare(sep_operators[i])==0)
                return true;
        }

    }
    return false;
}

char Lexer::others(char ch)    //分析单词和数字以外的字符
{
    string others;
    others += ch;
    ch = getchar();
    if(others[0] == '<' || others[0] == '>' || others[0] == '=' || others[0] == '!')
    {
        if(ch == '=')
        {
            others += ch;
            ch = getchar();
        }
    }
    else if( others[0] == '&' && ch == '&')
    {
        others += ch;
        ch = getchar();
    }
    else if( others[0] == '|' && ch == '|')
    {
        others += ch;
        ch = getchar();
    }
    for(int i = 0; i < 4; i++)
    {
        if(judgeOpt(others,i))
        {
            if(i == 1 || i == 2)
            {
                // token = Word(line, label[i], others);
                words.push_back( Word(line, label[i], others) );
            }
            else 
            {
                // token = Word(line, others, others);
                words.push_back( Word(line, others, others) );
            }
            return ch;
        }
    }
    cout<<" 不能识别的字符 "<<endl;
    exit(0);
}

void Lexer::runLexer()
{
    ch = getchar();
    while(ch!=EOF)
    {
        if(isdigit(ch))
        {
            ch = number(ch);
        }
        else if(isalpha(ch))
        {
            ch = word(ch);
        }
        else if(isspace(ch))
        {
            ch = remove_blank(ch);
        }
        else
        {
            ch = others(ch);
        }
    }
}

void Lexer::print()
{
    for(int i = 0; i < words.size(); i++)
    {
        cout<<words[i].line<<" "<<words[i].label<<" "<<words[i].name<<" "<<words[i].val<<endl;
    }
}

// int main()
// {
//     // freopen("in.txt","r",stdin);    //读入文件
//     // Lexer.runLexer();
//     // Lexer.print();
//     return 0;
// }