/**
* syntaxTree.h
*/

#ifndef syntaxTree_h
#define syntaxTree_h

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>

using namespace std;

struct MidCode
{
    int id;
    string data[4];
    MidCode(int x = 0, string a = "-", string b = "-", string c = "-", string d = "-");
    void print();
};

class syntaxTree;

class syntaxTree {
public:
    struct TreeNode
    {
        string label;
        int pos, size;
        TreeNode *fa;
        //属性信息
        string name, type, senten;
        long long val;
        int line, place, width, tr, fl;
        
        vector<TreeNode*> child;
        TreeNode(string lb, int p = 0, int sz = 0, vector<TreeNode*> ch = vector<TreeNode*>(0), int ln = -1);
        void setFa(TreeNode *f);
        void jump(int t, int f);
        void emit(string s, int t, int f);
    };
    
    int labels;
    TreeNode *root;
    vector<vector<TreeNode*> > v;

    syntaxTree();
    int newLabel();
    void print(TreeNode *root);
};

#endif