
#include "semantic.h"

Semantic::Semantic()
{
    Code.clear();
}

string Semantic::num2Char(int a)
{
    string num = "";
    bool flag = 0;
    if(a < 0)
    {
        flag = 1;
        a *= -1;
    }
    else if(a == 0)
        num.insert(num.begin(), '0');
    while(a)
    {
        int tmp = a % 10;
        num.insert(num.begin(), tmp + '0');
        a /= 10;
    }
    if(flag)
        num.insert(num.begin(), '-');
    num.insert(num.begin(), 'L');
    return num;
}

void Semantic::backpatch(syntaxTree::TreeNode &a, int place, int flag) //回填
{
    if(flag == 1)
    {
        Code[a.tr - 2].data[3] = num2Char(place);
        a.tr = place;
        // cout<<a.tr<<" "<<place<<" "<<Code[a.place].id<<" "<<Code[a.tr].data[3]<<endl;
    }
    else
    {
        // cout<<a.fl<<" "<<place<<" "<<Code[a.fl].data[1]<<endl;
        Code[a.fl - 2].data[1] = num2Char(place);
        a.fl = place;
    }
}

bool Semantic::semantic(vector<Word> input, syntaxTree &synTree, Grammar G)
{ 
    stack<int> status;
    stack<string> symble;
    stack<long long> semantic;
    vector<syntaxTree::TreeNode*>v(0);
    syntaxTree::TreeNode *node = new syntaxTree::TreeNode("#", 0, 1), *last;
    int a[300]; string b[300];
    pair<char, int> tmp;
    string s, top;
    int x, tail = -1;
    Word word;
    while( !status.empty() )
        status.pop();
    while( !symble.empty() )
        symble.pop();
    status.push(0), v.push_back(node);
    a[++tail] = 0;

    // ofstream out;
    // out.open("./test/test7_outStack.txt");
    while(input.size() > 1 || !symble.empty() )
    {
        if(input.empty() )
            s = "#";
        else
        {    
            word = input.front(); 
            s = word.label;
        }
        x = status.top();
        tmp = G.action[x][s]; 

        // cout<<s<<" "<<x<<" "<<tmp.first<<" "<<tmp.second<<" "<<G.action[x][s].first<<" "<<G.action[x][s].second<<endl; 
        if(tmp.first == 'A')  //接受
        {
            // out<<"ACC"<<endl;
            cout<<"ACC"<<endl;
            node = v.back();
            vector<syntaxTree::TreeNode*> ch(0); ch.push_back(node);
            synTree.root = new syntaxTree::TreeNode("AC", 0, 1, ch);
            node->setFa(synTree.root);
            v.pop_back(), status.pop(), symble.pop();
            tail--;
            // out.close();
            return true;
        }
        else if(tmp.first == 'S')  //移进
        {
            syntaxTree::TreeNode *pre = v.back();
            node  = new syntaxTree::TreeNode(s, pre->pos + pre->size, 1);
            // void *ss, *tmpv;
            // ss = &word.name, tmpv = &word.val;
            if(s == "type")
                node->type = word.name;
            if(s == "num")
                node->val = word.val;
            node->name = word.name, node->line = word.line;
            v.push_back(node);
            status.push(tmp.second), symble.push(s), semantic.push(word.val);
            a[++tail] = tmp.second, b[tail] = s;
            input.erase(input.begin() );
        }
        else if(tmp.first == 'r')   //规约
        {
            vector<syntaxTree::TreeNode*> ch(0);
            node = new syntaxTree::TreeNode(G.rules[tmp.second].lelm);

            if(node->label == "M")  //main程序M->int main ( ) { P return ; }
            {
                for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                {
                    top = G.rules[tmp.second].relm[i];
                    if(symble.top() == top )
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size, node->line = last->line;
                        last->setFa(node);
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        cout<<"Line: "<<word.line<<"缺少 "<<top<<" "<<word.label<<endl;
                        return false;
                    }
                }
                node->place = ch[6]->place;
                Code[synTree.labels] = MidCode(synTree.newLabel() );  //所有P结束后最后一个结束语句什么也不做
            }
            else if(node->label == "P")  //程序P->DS
            {
                for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                {
                    top = G.rules[tmp.second].relm[i];
                    if(symble.top() == top )
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        cout<<"Line: "<<word.line<<"非法的程序定义 "<<top<<" "<<word.label<<endl;
                        return false;
                    }
                }
                node->place = ch[0]->place, node->line = ch[1]->line;
            }
            else if(node->label == "D")  //声明语句
            {
                for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                {
                    top = G.rules[tmp.second].relm[i];
                    if(symble.top() == top )
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        if(top == "id")  // D->D type id ; | type id ;
                        {
                            last->type = v[v.size() - 2]->type;
                            typeTable[last->name] = last->type;
                            placeTable[last->name] = last;
                            last->place = synTree.newLabel();
                            Code[last->place] = MidCode(last->place, "declare", last->type, "-", last->name);
                        }
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        if(top == "type")
                            cout<<"Line: "<<word.line<<" 缺少类型 "<<word.label<<endl;
                        else if(top == ";")
                            cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                        return false;
                    }
                }
                node->line = ch[ch.size() - 2]->line;
                if(ch.size() == 3)
                    node->place = ch[1]->place;
                else
                    node->place = ch[0]->place;
            }
            else if(node->label == "S")  //语句
            {
                if(G.rules[tmp.second].relm.size() == 1)  //S->A | FR
                {
                    last = v.back();
                    ch.insert(ch.begin(),  last);
                    node->size += last->size;
                    last->setFa(node);
                    node->place = last->place, node->line = last->line;
                    v.pop_back(), status.pop(), symble.pop();
                    tail--;
                }
                else if(G.rules[tmp.second].relm.size() == 3)  //S-> { L }
                {
                    if(symble.top() == "}" )
                    {
                        for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                        {
                            top = G.rules[tmp.second].relm[i];
                            if(symble.top() == top )
                            {
                                last = v.back();
                                last->setFa(node);
                                node->size += last->size;
                                ch.insert(ch.begin(),  last);
                                if(top == "L")
                                {
                                    
                                    if(last->place == -1)
                                    {    
                                        last->place = synTree.newLabel();
                                        Code[last->place] = MidCode(last->place);
                                    }
                                    node->place = last->place, node->line = ch[1]->line;
                                }
                                v.pop_back(), status.pop(), symble.pop();
                                tail--;       
                            }
                            else
                            {
                                if(top == "}" || top == "{")
                                    cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                                return false;
                            }
                        }
                    }   
                }
                else if(G.rules[tmp.second].relm[0] == "while")  //S->while ( B ) { L }
                {
                    for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                    {
                        top = G.rules[tmp.second].relm[i];
                        if(symble.top() == top )
                        {
                            last = v.back();
                            last->setFa(node);
                            node->size += last->size;
                            ch.insert(ch.begin(),  last);
                            v.pop_back(), status.pop(), symble.pop();
                            tail--;       
                        }
                        else
                        {
                            if(top == "if" || top == "(" || top == ")" || top == "else")
                                cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                            else if(top == "B")
                                cout<<"Line: "<<word.line<<" 条件控制语句缺少条件 "<<" "<<word.label<<endl;
                            else
                                cout<<"Line: "<<word.line<<" 条件控制语句缺少执行语句 "<<" "<<word.label<<endl;
                            return false;
                        }
                    }
                    // cout<<ch[2]->label<<" "<<ch[2]->place<<" "<<ch[4]->label<<" "<<ch[4]->place<<endl;
                    backpatch(*ch[2], ch[5]->place, 1);
                    node->place = ch[2]->place, node->line = ch[6]->line;
                    Code[synTree.labels] = MidCode(synTree.newLabel(), "goto", num2Char(node->place) );
                    backpatch(*ch[2], synTree.labels, 0);
                }
                else
                {
                    if(G.rules[tmp.second].relm.size() == 7)  //S->if ( B ) { L }
                    {
                        for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                        {
                            top = G.rules[tmp.second].relm[i];
                            if(symble.top() == top )
                            {
                                last = v.back();
                                last->setFa(node);
                                node->size += last->size;
                                ch.insert(ch.begin(),  last);
                                v.pop_back(), status.pop(), symble.pop();
                                tail--;       
                            }
                            else
                            {
                                if(top == "if" || top == "(" || top == ")" || top == "}")
                                    cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                                else if(top == "B")
                                    cout<<"Line: "<<word.line<<" 条件控制语句缺少条件 "<<" "<<word.label<<endl;
                                else
                                    cout<<"Line: "<<word.line<<" 条件控制语句缺少执行语句 "<<" "<<word.label<<endl;
                                return false;
                            }
                        }
                        // cout<<ch[2]->label<<" "<<ch[2]->place<<" "<<ch[4]->label<<" "<<ch[4]->place<<endl;
                        backpatch(*ch[2], ch[5]->place, 1);
                        backpatch(*ch[2], synTree.labels, 0);
                        node->place = ch[2]->place, node->line = ch[6]->line;
                    }
                    else  //B->if ( B ) { L } else { L }
                    {
                        for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                        {
                            top = G.rules[tmp.second].relm[i];
                            if(symble.top() == top )
                            {
                                last = v.back();
                                last->setFa(node);
                                node->size += last->size;
                                ch.insert(ch.begin(),  last);
                                v.pop_back(), status.pop(), symble.pop();
                                tail--;       
                            }
                            else
                            {
                                if(top == "if" || top == "(" || top == ")" || top == "{" || top == "}" || top == "else")
                                    cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                                else if(top == "B")
                                    cout<<"Line: "<<word.line<<" 条件控制语句缺少条件 "<<" "<<word.label<<endl;
                                else
                                    cout<<"Line: "<<word.line<<" 条件控制语句缺少执行语句 "<<" "<<word.label<<endl;
                                return false;
                            }
                        }
                        // cout<<ch[2]->label<<" "<<ch[2]->place<<" "<<ch[4]->label<<" "<<ch[4]->place<<endl;
                        backpatch(*ch[2], ch[5]->place, 1);
                        if(ch[9]->place == -1)
                            ch[9]->place = synTree.labels;
                        backpatch(*ch[2], ch[9]->place, 0);
                        node->place = ch[2]->place, node->line = ch[9]->line;
                    }
                }
            }
            else if(node->label == "FR")  //for ( A ; B ; F1 F2
            {
                for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                {
                    top = G.rules[tmp.second].relm[i];
                    if(symble.top() == top )
                    {
                        last = v.back();
                        last->setFa(node);
                        node->size += last->size;
                        ch.insert(ch.begin(),  last);
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                        return false;
                    }
                }
                node->place = ch[2]->place, node->line = ch[7]->line;
                // cout<<ch[6]->label<<" "<<ch[6]->place<<" "<<ch[4]->label<<" "<<ch[4]->place<<endl;
                backpatch(*ch[4], ch[7]->place, 1);  //B真执行 F2->L }的L语句
                backpatch(*ch[4], synTree.labels, 0);  //B假跳出循环
                backpatch(*ch[7], ch[6]->place, 0);  //L结束返回第二个A
                backpatch(*ch[6], ch[4]->place, 0);  //第二个A结束返回判断语句
            }
            else if(node->label == "F1")  //F1->A ) {
            {
                for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                {
                    top = G.rules[tmp.second].relm[i];
                    if(symble.top() == top )
                    {
                        last = v.back();
                        last->setFa(node);
                        node->size += last->size;
                        ch.insert(ch.begin(),  last);
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                        return false;
                    }
                }
                node->place = ch[0]->place, node->line = ch[2]->line;
                Code[synTree.labels] = MidCode(synTree.newLabel(), "goto");  //返回循环判断
                node->fl = synTree.labels + 1; //回填时候fl位置－2
            }
            else if(node->label == "F2")  //F2->L }
            {
                for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                {
                    top = G.rules[tmp.second].relm[i];
                    if(symble.top() == top )
                    {
                        last = v.back();
                        last->setFa(node);
                        node->size += last->size;
                        ch.insert(ch.begin(),  last);
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                        return false;
                    }
                }
                node->place = node->tr = ch[0]->place, node->line = ch[1]->line;
                Code[synTree.labels] = MidCode(synTree.newLabel(), "goto");
                node->fl = synTree.labels + 1; //回填时候fl位置－2
            }
            else if(node->label == "A")  //Assign赋值语句// A->id = E
            {
                for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                {
                    top = G.rules[tmp.second].relm[i];
                    if(symble.top() == top )
                    {
                        last = v.back();
                        last->setFa(node);
                        node->size += last->size;
                        ch.insert(ch.begin(),  last);
                        if(top == "E") 
                        {
                            node->val = last->val;
                        }
                        else if(top == "id")  
                        {
                            if(typeTable.find(last->name) == typeTable.end() )
                            {
                                cout<<"Line: "<<word.line<<" 未声明的变量 "<<last->name<<endl;
                                return false;
                            }
                            last->val = placeTable[last->name]->val = node->val;
                            last->type = placeTable[last->name]->type, last->place = placeTable[last->name]->place;
                        }
                        // cout<<node->label<<" "<<last->label<<" "<<node->size<<" "<<last->size<<endl;
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                        return false;
                    }
                }
                node->place = ch[2]->place, node->line = ch[2]->line;
                Code[synTree.labels] =  MidCode(synTree.newLabel(), ch[ch.size() - 3]->name, ":=", "-", num2Char(ch[ch.size() - 1]->place) );
            }
            else if(node->label == "L")  //语句序列
            {
                if(G.rules[tmp.second].relm.size() == 1)  
                {
                    if(symble.top() == "S") //L->S
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        node->place = last->place, node->line = last->line;
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;
                    }
                    //L->#
                }
                else  //L->S ; L
                {
                    for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                    {
                        top = G.rules[tmp.second].relm[i];
                        if(symble.top() == top )
                        {
                            last = v.back();
                            ch.insert(ch.begin(),  last);
                            node->size += last->size;
                            last->setFa(node);
                            v.pop_back(), status.pop(), symble.pop();
                            tail--;       
                        }
                        else
                        {
                            cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                            return false;
                        }
                    }
                    node->place = ch[0]->place, node->line = ch[2]->line;
                }
            }
            else if(node->label == "B")  //Bool表达式语句
            {
                if(G.rules[tmp.second].relm.size() == 1)  //B->C
                {
                    if(symble.top() == "C" )
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        node->place = last->place, node->tr = last->tr, node->fl = last->fl, node->line = last->line;
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                }
                else if(G.rules[tmp.second].relm.size() == 2)  //B->! B  暂时有问题 生成LR1表会冲突所以语言定义删除了下面的回填没改
                {
                    for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                    {
                        top = G.rules[tmp.second].relm[i];
                        if(symble.top() == top )
                        {
                            last = v.back();
                            ch.insert(ch.begin(),  last);
                            node->size += last->size;
                            last->setFa(node);
                            v.pop_back(), status.pop(), symble.pop();
                            tail--;       
                        }
                        else
                        {
                            cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                            return false;
                        }
                    }
                    node->place = ch[1]->place, node->line = ch[1]->line;
                    node->tr = ch[1]->fl, node->fl = ch[1]->tr;
                }
                else  //B->B rel C
                {
                    for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                    {
                        top = G.rules[tmp.second].relm[i];
                        if(symble.top() == top )
                        {
                            last = v.back();
                            ch.insert(ch.begin(),  last);
                            node->size += last->size;
                            last->setFa(node);
                            if(top == "T")
                            {
                                node->val = last->val;
                            }
                            else if(top == "E")  
                            {
                                node->val = last->val - node->val;
                            }
                            v.pop_back(), status.pop(), symble.pop();
                            tail--;       
                        }
                        else
                        {
                            if(top == "-")
                                cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                            else
                                cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                            return false;
                        }
                    }
                    if(ch[1]->name == "&&") // B && C
                    {
                        backpatch(*ch[0], ch[2]->place, 1); //第一个条件true进入第二个条件开始
                        backpatch(*ch[0], max(ch[0]->fl, ch[2]->fl), 0); //回填第一个语句false时候
                        node->tr = ch[2]->tr;  //更改trueplace为c的true;
                        node->place = ch[0]->place, node->fl = ch[0]->fl;
                    }
                    else if(ch[1]->name == "||")  //B || C
                    {
                        backpatch(*ch[0], max(ch[0]->tr, ch[2]->tr), 1); //回填第一个语句true时候
                        backpatch(*ch[0], ch[2]->place, 0); //回填语句的goto到C开始
                        node->fl = ch[2]->fl;  //更改falseplace为c的false;
                        node->place = ch[0]->place, node->tr = ch[0]->tr;
                    }
                    node->line = ch[2]->line;
                }
            }
            else if(node->label == "C")  //Bool表达式语句
            {
                if(G.rules[tmp.second].relm.size() == 1)  //C->id
                {
                    if(symble.top() == "id" )
                    {
                        last = v.back();
                        if(typeTable.find(last->name) == typeTable.end() )
                        {
                            cout<<"Line: "<<word.line<<" 未声明的变量 "<<last->name<<endl;
                            return false;
                        }
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        last->place = placeTable[last->name]->place, node->line = last->line;
                        node->place = synTree.newLabel();
                        node->tr = synTree.labels + 1; //labels++ 
                        Code[node->place] =  MidCode(node->place , "if", ch[0]->name + ch[1]->name + ch[2]->name, "goto", num2Char(node->tr)) ; //true
                        Code[synTree.labels] =  MidCode(synTree.newLabel(), "goto", num2Char(synTree.labels + 1)) ;  //false
                        node->fl = synTree.labels + 1; //labels++ 
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                }
                else  //c->id cmp id | id cmp num
                {
                    for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                    {
                        top = G.rules[tmp.second].relm[i];
                        if(symble.top() == top )
                        {
                            last = v.back();
                            if(top == "id")
                            {
                                if(typeTable.find(last->name) == typeTable.end() )
                                {
                                    cout<<"Line: "<<word.line<<" 未声明的变量 "<<last->name<<endl;
                                    return false;
                                }
                                last->place = placeTable[last->name]->place;
                            }
                            ch.insert(ch.begin(),  last);
                            node->size += last->size;
                            last->setFa(node);
                            v.pop_back(), status.pop(), symble.pop();
                            tail--;       
                        }
                        else
                        {
                            cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                            return false;
                        }
                    }
                    node->place = synTree.newLabel(), node->line = ch[2]->line; 
                    node->tr = synTree.labels + 1; //labels++ 
                    Code[node->place] =  MidCode(node->place , "if", ch[0]->name + ch[1]->name + ch[2]->name, "goto", num2Char(node->tr)) ; //true
                    Code[synTree.labels] =  MidCode(synTree.newLabel(), "goto", num2Char(synTree.labels + 1)) ;  //false
                    node->fl = synTree.labels + 1; //labels++ 
                }
            }
            else if(node->label == "E")  //表达式语句
            {
                if(G.rules[tmp.second].relm.size() == 1)  //E->T
                {
                    if(symble.top() == "T" )
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        node->val = last->val;
                        node->place = last->place, node->line = last->line;
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                }
                else
                {
                    node->place = synTree.newLabel();
                    if(G.rules[tmp.second].relm[1] == "+")  //E + T
                    {
                        for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                        {
                            top = G.rules[tmp.second].relm[i];
                            if(symble.top() == top )
                            {
                                last = v.back();
                                ch.insert(ch.begin(),  last);
                                node->size += last->size;
                                last->setFa(node);
                                if(top == "T")
                                {
                                    node->val = last->val;
                                }
                                else if(top == "E")  
                                {
                                    node->val = last->val + node->val;
                                }
                                v.pop_back(), status.pop(), symble.pop();
                                tail--;       
                            }
                            else
                            {
                                if(top == "+")
                                    cout<<"Line: "<<word.line<<" 缺少 "<<top<<" "<<word.label<<endl;
                                else
                                    cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                                return false;
                            }
                        }
                        node->line = ch[2]->line;
                        Code[node->place] = MidCode(node->place,  node->label, num2Char(ch[0]->place), "+", num2Char(ch[2]->place) );
                    }
                    else  //E - T
                    {
                        for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                        {
                            top = G.rules[tmp.second].relm[i];
                            if(symble.top() == top )
                            {
                                last = v.back();
                                ch.insert(ch.begin(),  last);
                                node->size += last->size;
                                last->setFa(node);
                                if(top == "T")
                                {
                                    node->val = last->val;
                                }
                                else if(top == "E")  
                                {
                                    node->val = last->val - node->val;
                                }
                                v.pop_back(), status.pop(), symble.pop();
                                tail--;       
                            }
                            else
                            {
                                cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                                return false;
                            }
                        }
                        node->line = ch[2]->line;
                        Code[node->place] = MidCode(node->place,  node->label, num2Char(ch[ch.size() - 3]->place), "-", num2Char(ch[ch.size() - 1]->place) );
                    }
                }
            }
            else if(node->label == "T")  //乘除法语句
            {
                if(G.rules[tmp.second].relm.size() == 1)  //T->F 
                {
                    if(symble.top() == "F" )
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        node->val = last->val;
                        node->place = last->place, node->line = last->line;
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else
                    {
                        cout<<"Line: "<<word.line<<" 错误 "<<word.label<<endl;
                        return false;
                    }
                }
                else
                {
                    node->place = synTree.newLabel();
                    if(G.rules[tmp.second].relm[1] == "*")  //T * F
                    {
                        for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                        {
                            top = G.rules[tmp.second].relm[i];
                            if(symble.top() == top )
                            {
                                last = v.back();
                                ch.insert(ch.begin(),  last);
                                node->size += last->size;
                                last->setFa(node);
                                if(top == "F")
                                {
                                    node->val = last->val;
                                }
                                else if(top == "T")  
                                {
                                    node->val = last->val * node->val;
                                }
                                v.pop_back(), status.pop(), symble.pop();
                                tail--;       
                            }
                            else
                            {
                                cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                                return false;
                            }
                        }
                        node->line = ch[2]->line;
                        Code[node->place] = MidCode(node->place,  node->label, num2Char(ch[ch.size() - 3]->place), "*", num2Char(ch[ch.size() - 1]->place) );
                    }
                    else  //T / F
                    {
                        for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                        {
                            top = G.rules[tmp.second].relm[i];
                            if(symble.top() == top )
                            {
                                last = v.back();
                                ch.insert(ch.begin(),  last);
                                node->size += last->size;
                                last->setFa(node);
                                if(top == "F")
                                {
                                    node->val = last->val;
                                }
                                else if(top == "T")  
                                {
                                    node->val = last->val / node->val;
                                }
                                v.pop_back(), status.pop(), symble.pop();
                                tail--;       
                            }
                            else
                            {
                                cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                                return false;
                            }
                        }
                        node->line = ch[2]->line;
                        Code[node->place] = MidCode(node->place,  node->label, num2Char(ch[ch.size() - 3]->place), "/", num2Char(ch[ch.size() - 1]->place) );
                    }
                }
            }
            else if(node->label == "F")  //因子
            {
                if(G.rules[tmp.second].relm.size() == 1)  //F->id | num
                {
                    if(symble.top() == "num" )
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        node->val = last->val, node->line = last->line;
                        node->place = synTree.newLabel();
                        Code[node->place] = MidCode(node->place, "num", node->label, ":=",ch[0]->name);
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;       
                    }
                    else if(symble.top() == "id")
                    {
                        last = v.back();
                        ch.insert(ch.begin(),  last);
                        node->size += last->size;
                        last->setFa(node);
                        node->val = last->val, node->line = last->line;
                        if(typeTable.find(last->name) == typeTable.end() )
                        {
                            cout<<"Line: "<<word.line<<" 未声明的变量 "<<last->name<<endl;
                            return false;
                        }
                        node->val = placeTable[last->name]->val;
                        node->type = placeTable[last->name]->type, node->place = placeTable[last->name]->place;
                        v.pop_back(), status.pop(), symble.pop();
                        tail--;
                    }
                }
                else  //F->( E )
                {
                    for(int i = G.rules[tmp.second].relm.size() - 1; i >= 0 ; i--)
                    {
                        top = G.rules[tmp.second].relm[i];
                        if(symble.top() == top )
                        {
                            last = v.back();
                            ch.insert(ch.begin(),  last);
                            node->size += last->size;
                            last->setFa(node);
                            if(top == "F")
                            {
                                node->val = last->val;
                            }
                            else if(top == "T")  
                            {
                                node->val = last->val / node->val;
                            }
                            v.pop_back(), status.pop(), symble.pop();
                            tail--;       
                        }
                        else
                        {
                            cout<<"Line: "<<word.line<<" 表达式错误 "<<top<<" "<<word.label<<endl;
                            return false;
                        }
                    }
                    node->place = ch[1]->place, node->line = ch[2]->line;
                }
            }
            if(G.go_to[status.top()][G.rules[tmp.second].lelm] != -1)
            {
                // cout<<status.top()<<" "<<G.rules[tmp.second].lelm<<" "<<go_to[status.top()][G.rules[tmp.second].lelm]<<endl;
                last = v.back();
                node->pos = last->pos + last->size;
                node->child = ch;
                status.push(G.go_to[status.top()][G.rules[tmp.second].lelm] ),  symble.push(G.rules[tmp.second].lelm);
                // cout<<" "<<go_to[status.top()][G.rules[tmp.second].lelm]<<" "<<status.top()<<endl;
                v.push_back(node);
                a[++tail] = status.top(), b[tail] = G.rules[tmp.second].lelm;
            }
            else
            {
                cout<<"Line: "<<word.line<<" 错误 "<<word.label<<endl;
                G.rules[tmp.second].print();
                return false;
            }
        }
        else
        {
            cout<<"Line: "<<word.line<<" 非法语句 "<<" "<<word.label<<endl;
            if(v.size() == 0)
                cout<<"\t必须声明主函数 void main { return; }"<<endl;
            else if(v.back()->label == "P")
                cout<<"\tLine: "<<v.back()->line<<" 缺少return"<<endl;
            else if(v.back()->label == "D")
                cout<<"\tLine: "<<v.back()->line<<" 声明结束不能直接结束程序，需要有程序语句"<<endl;
            else if(v.back()->label == "id")
                cout<<"\tLine: "<<v.back()->line<<" 缺少 ; "<<endl;
            else if(v.back()->label == "S" && v.back()->child.size() == 1)
                cout<<"\tLine: "<<v.back()->line<<" 缺少 ; "<<endl;
            else if(v.back()->label == "B" && (v[v.size() - 3]->label == "if" || v[v.size() - 3]->label == "while") )
                cout<<"\tLine: "<<v.back()->line<<" 缺少 ) "<<endl;
            else if(v.back()->label == "L" && v[v.size() - 1]->label == "{" )
                cout<<"\tLine: "<<v.back()->line<<" 缺少 } "<<endl;
            else if(v.back()->label == "if" || v.back()->label == "while" || v.back()->label == "for")
                cout<<"\tLine: "<<v.back()->line<<" 缺少 ( "<<endl;
            else if(v.back()->label == "else")
                cout<<"\tLine: "<<v.back()->line<<" 缺少 { "<<endl;
            else if(v.back()->label == "A" && v[v.size() - 1]->label == "(")
                cout<<"\tLine: "<<v.back()->line<<" 缺少 ; "<<endl;
            else if(v.back()->label == "B" && v[v.size() - 1]->label == ";")
                cout<<"\tLine: "<<v.back()->line<<" 缺少 ; "<<endl;
            else if(v.back()->label == "A" && v[v.size() - 1]->label == ";")
                cout<<"\tLine: "<<v.back()->line<<" 缺少 ) "<<endl;
            else if(v.back()->label == ")")
                cout<<"\tLine: "<<v.back()->line<<" 缺少 { "<<endl;
            else if(v.back()->label == "C")
                cout<<"\tLine: "<<v.back()->line<<" 布尔语句不符合语言规则"<<endl;
            else
                cout<<"非法符号"<<endl;
            return false;
        }
        // cout<<node->label<<" "<<node->pos<<" "<<node->size<<" "<<node->child.size()<<endl;
        //print for debug
        // if(out)
        // {
        //     for(int i = 0; i <= tail; i++)
        //     {
        //         out<<a[i]<<" ";
        //     }
        //     out<<"\t\t";
        //     for(int i = 0; i <= tail; i++)
        //     {
        //         out<<b[i]<<" ";
        //     }
        //     out<<endl;
        // }
        // for(int i = 0; i <= tail; i++)
        // {
        //     cout<<a[i]<<" ";
        // }
        // cout<<"\t\t";
        // for(int i = 0; i <= tail; i++)
        // {
        //     cout<<b[i]<<" ";
        // }
        // cout<<endl;
    }
    return true;
}

void Semantic::emitMidCode(syntaxTree synTree)
{
    for(int i = 0; i < synTree.labels; i++)
    {
        Code[i].print();
    }
}

/**
*  开始语法语义分析并生成中间代码
*/
bool Semantic::start(vector<Word> input, syntaxTree &synTree, Grammar G)
{
    input.push_back( Word(-1, "#", "#") );
    return semantic(input, synTree, G);
    // ifstream in;
    // in.open("./test/test7_string.txt");
    // in.open("in.txt");
    // if(in)
    // {
    //     string s; char ss[100];
    //     while(getline(in, s))
    //     {
    //         queue<string>input;
    //         while(!input.empty())
    //             input.pop();
    //         int i, j;
    //         for(i = 0, j = 0; i < s.length(); i++, j++)
    //         {
    //             if(s[i] == ' ')
    //             {
    //                 std::size_t l = s.copy(ss, j, i - j); //size_t copy (char* s, size_t len, size_t pos = 0) const;
    //                 ss[l] = '\0';
    //                 input.push(ss);
    //                 j = -1;
    //             }
    //         }
    //         std::size_t l = s.copy(ss, j, i - j);
    //         ss[l] = '\0';
    //         input.push(ss);
    //         input.push(G.ept);
    //         // cout<<s<<endl;
    //         // while(!input.empty() )
    //         // {
    //         //     cout<<input.front()<<endl;
    //         //     input.pop();
    //         // }
    //         G.lr1Driver(input);
    //     }
    //     in.close();
    // }
}