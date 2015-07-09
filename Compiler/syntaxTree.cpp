/**
* syntaxTree.cpp
*/

#include "syntaxTree.h"


MidCode::MidCode(int x, string a, string b, string c, string d)
{
    id = x, data[0] = a, data[1] = b, data[2] = c, data[3] = d;
}

void MidCode::print()
{
    cout<<"L"<<id<<"\t";
    for(int i = 0; i < 4; i++)
        cout<<" "<<data[i];
    cout<<endl;
}
syntaxTree::TreeNode::TreeNode(string lb, int p, int sz, vector<TreeNode*> ch, int ln)
{
    label = lb, pos = p, size = sz, line = ln, child = ch, val = 0, place = tr = fl = -1;
}

syntaxTree::syntaxTree()
{
    labels = 0;
    root = NULL;
}


int syntaxTree::newLabel()
{
    return labels++;
}

void syntaxTree::TreeNode::setFa(TreeNode *f)
{
    fa = f;
}


// void syntaxTree::TreeNode::jump(string s, int t, int f)
// {
// }

void syntaxTree::TreeNode::emit(string s, int t, int f)
{
    cout<<"L"<<t<<" "<<s<<endl;
}

void syntaxTree::print(TreeNode *root)
{
    //levelOrder,层次遍历
    if( !root )
        return;
    v.clear();
    queue<pair<TreeNode*, int> > Q;
    Q.push( make_pair(root, 0) );
    pair<TreeNode*, int> x;
    TreeNode *tmp;
    int pre = 0, i;
    while( !Q.empty() )
    {
        x = Q.front(), Q.pop();
        tmp = x.first;
        if( !tmp->child.empty() )
        {
            for(int i = 0; i < tmp->child.size(); i++)
            {
                Q.push( make_pair(tmp->child[i], x.second + 1) );
            }
        }
        if(x.second != pre)
        {
            cout<<endl;
            pre = x.second;
            i = 0;
        }
        for(; i < tmp->pos; i++)
        {
            cout<<"-";
            if(tmp->pos - i > 1)
                cout<<"\t";
        }
        if( tmp->label[0] >= 'A' && tmp->label[0] <= 'Z')
            cout<<tmp->label;
        if( tmp->label != "num" && !(tmp->label[0] >= 'A' && tmp->label[0]  <= 'Z'))
            cout<<tmp->name;
        // if( tmp->label == "num" || (tmp->label[0] >= 'A' && tmp->label[0] <= 'Z') || tmp->label == "id")   
        //     cout<<tmp->val;
        if( tmp->label == "num")   
            cout<<"("<<tmp->val<<")";
        // if(tmp->label == "id")
        //     cout<<"("<<tmp->type<<")";
        // if( tmp->label != "key" && tmp->label != "type" && tmp->label != "cmp" && tmp->label != "rel")
        // cout<<"("<<tmp->val<<")";
        // if(tmp->label != "num" && tmp->label != "type")
        //     cout<<"("<<tmp->type<<")";
        cout<<"\t";
    }
    cout<<endl;
}
