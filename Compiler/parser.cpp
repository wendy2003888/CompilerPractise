/**
* parser.cpp
*/

#include "parser.h"

Rule::Rule(int i, string lel)
{
    id = i, lelm = lel, relm.clear();
}

void Rule::print()
{
    cout<<id<<" "<<lelm<<" -> ";
    for(int i = 0; i < relm.size(); i++)
    {
        cout<<relm[i]<<" ";
    }
    cout<<endl;
}

bool Rule::operator < (const Rule &x) const
{
    if(lelm == x.lelm)
    {
        for(int i = 0, j = 0; i < relm.size() && j < x.relm.size(); i++, j++)
        {
            if(relm[i] != x.relm[i])
                return relm[i] < x.relm[i];
        }            
        return relm.size() < x.relm.size(); 
    }
    return id < x.id;
}

bool Rule::operator == (const Rule &x) const
{
    if(lelm == x.lelm)
    {
        if(relm.size() == x.relm.size())
        {
            for(int i = 0; i < relm.size(); i++)
            {
                if(relm[i] != x.relm[i])
                    return false;
            }
            return true;
        }
        return false;
    }
    return false; 
}

bool Rule::operator != (const Rule &x) const
{
    if(lelm == x.lelm)
    {
        if(relm.size() == x.relm.size())
        {
            for(int i = 0; i < relm.size(); i++)
            {
                if(relm[i] != x.relm[i])
                    return true;
            }
            return false;
        }
        return true;
    }
    return true; 
}

bool Grammar::Item::operator < (const Item &x) const
{
    if(itemSet.size() == x.itemSet.size())
    {
        for(set< pair<Rule, State> >::iterator it = itemSet.begin(), jt = x.itemSet.begin();
                                it != itemSet.end(); it++, jt++) 
        {
                    if(it->first != jt->first)
                        return it->first < jt->first;
                    if(it->second != jt->second)
                        return it->second < jt->second;
        }
        return false;
    }
    return itemSet.size() < x.itemSet.size();           
}

bool Grammar::Item::operator == (const Item &x) const
{
    if(itemSet.size() == x.itemSet.size())
    {
        for(set< pair<Rule, State> >::iterator it = itemSet.begin(), jt = x.itemSet.begin();
                                it != itemSet.end(); it++, jt++) 
        {
                    if(it->first != jt->first)
                        return false;
                    if(it->second != jt->second)
                        return false;
        }
        return true;
    }
    return false;           
}

bool Grammar::Item::operator != (const Item &x) const
{
    if(itemSet.size() == x.itemSet.size())
    {
        for(set< pair<Rule, State> >::iterator it = itemSet.begin(), jt = x.itemSet.begin();
                                it != itemSet.end(); it++, jt++) 
        {
                    if(it->first != jt->first)
                        return true;
                    if(it->second != jt->second)
                        return true;
        }
        return false;
    }
    return true;           
}

void Grammar::Item::print()
{
    set<pair<Rule, Item::State> >::iterator it;
    for(it = itemSet.begin(); it != itemSet.end(); it++)
    {
        Rule tmp = it->first;
        int dot = it->second.dot;

        cout<<tmp.lelm<<" -> ";
        for(int i = 0; i < tmp.relm.size(); i++)
        {
            if(dot == i)
                    cout<<"@";
            cout<<tmp.relm[i]<<" ";
        }
        if(dot == tmp.relm.size() )
            cout<<"@";
        cout<<", "<<it->second.ahead<<endl;
    }
    cout<<endl;
}

Grammar::Grammar():ept("#")
{
    rules.clear();
    closure.clear();
    rules.push_back(Rule(0, "$"));
    initSyntex();
}

bool Grammar::isVt(string s)
{
    if(s == ept)
        return true;
    if(s.length() == 0)
        return false;
    for(set<string>::iterator it = vt.begin(); it != vt.end(); it++)
    {
        if(s.compare(*it) == 0 )
            return true;
    }
    return false;
}

bool Grammar::hasNotVancancy(string s)
{
    vector<Rule>::iterator it;
    for(it = rules.begin(); it != rules.end(); it++)
    {
        if(it->lelm == s)
        {
            if(it->relm.size() == 0)
                return false;
            for(int i = 0; i < it->relm.size(); i++)
            {
                if(it->relm[i] == ept)
                    return false;
            }
        }
    }
    return true;
}

map<string, set<string> > Grammar::geneFirst()
{
    map<string, set<string> >first;
    first.clear();
    string tmp;
    first[rules[0].lelm].insert(ept), first[startSym].insert(ept);
    for(int i = 0; i < rules.size(); i++)
    {
        if(rules[i].relm.size() == 0)
            continue;
        tmp = rules[i].relm[0];
        // cout<<tmp<<endl;
        if(isVt(tmp) || tmp == ept)
        {
            first[rules[i].lelm].insert(tmp);
        }
    }

    bool flag = true;
    while(flag)
    {
        flag = false;
        for(int i = 0; i < rules.size(); i++)
        {
            int presize = first[rules[i].lelm].size();
            int j = 0;
            for(j = 0; j < rules[i].relm.size(); j++)
            {
                tmp = rules[i].relm[j];
                if( !isVt(tmp) )
                {
                    for(set<string>::iterator k = first[tmp].begin(); k != first[tmp].end(); k++)
                    {
                        if(*k != ept)
                            first[rules[i].lelm].insert(*k);
                    }
                    // cout<<i<<" "<<first[rules[i].lelm].size()<<" "<<presize<<endl;
                    if(first[rules[i].lelm].size() != presize)
                        flag = true;
                    if(hasNotVancancy(tmp))
                    {
                        break;
                    }
                    
                }
                else
                {
                    first[rules[i].lelm].insert(tmp);
                    // cout<<i<<" "<<first[rules[i].lelm].size()<<"^ "<<presize<<endl;
                    if(first[rules[i].lelm].size() != presize)
                        flag = true;
                    break;
                }
                
            }
            if(j == rules[i].relm.size())
                first[rules[i].lelm].insert(ept);
            if(first[rules[i].lelm].size() != presize)
                flag = true;
        }
    }
    return first;
}

void Grammar::initClosure()
{
    item.itemSet.clear();
    item.itemSet.insert( make_pair(rules[0], Item::State(0, ept) ) );
}

void Grammar::initTable(int x)
{
    set<string>::iterator it;
    for(it = vn.begin(); it != vn.end(); it++)
    {
        go_to[x][*it] = -1;
    }
    for(it = vt.begin(); it != vt.end(); it++)
    {
        action[x][*it] = make_pair('F', -1);
    }
    action[x][ept] = make_pair('F', -1);
}

set<string> Grammar::getFirst(string s, string ahead)
{
    set<string> result;
    set<string>::iterator it;
    bool flag = 1;
    result.clear();
    if(s == ept)
    {
        result.insert(ahead);
        return result;
    }
    if( isVt(s) )
    {
        result.insert(s);
        return result;
    }
    for(it = first[s].begin(); it != first[s].end(); it++)
    {
         // cout<<*it<<endl;
        if(*it != ept)
        {
            result.insert(*it);
            flag = 0;
        }
    }
    if(flag)
        result.insert(ahead);
    return result;
}

bool Grammar::eqlSet(const set<pair<Rule, Item::State> > &a, const set<pair<Rule, Item::State> > &b)
{
    if(a.size() == b.size() )
    {
        for(set<pair<Rule, Item::State> >::iterator i = a.begin(), j = b.begin(); i != a.end(); i++, j++)
        {
            if(i->first != j->first)
                return false;
            else if(i->second != j->second)
                return false;
        }
        return true;
    }
    else
        return false;
}

int Grammar::findSet(set<pair<Rule, Item::State> > s)
{
    if(s.size() == 0)
        return -2;
    for(int i = 0; i < closure.size(); i++)
    {
        if( eqlSet(closure[i], s) )
            return i;
    }
    return -1;
}

void Grammar::geneSet()
{
    int dot;
    string ahead, tmp, next;
    bool flag = 1;
    set<string> fst;
    set<string>::iterator k;
    set<pair<Rule, Item::State> > cl = item.itemSet;
    set<pair<Rule, Item::State> >::iterator it;
    while(flag)
    {
        flag = 0;
        for(it = item.itemSet.begin(); it != item.itemSet.end(); it++)
        {
            dot = it->second.dot, ahead = it->second.ahead;
            if(dot == it->first.relm.size() )
                continue;
            tmp = it->first.relm[dot];
            if(dot == it->first.relm.size() - 1)  //S'->S, T->aBc.D
                next = ept;
            else
                next = it->first.relm[dot + 1];
            if( !isVt(tmp) )
            {
                for(int j = 0; j < rules.size(); j++)
                {
                    if(rules[j].lelm == tmp)
                    {
                        fst = getFirst(next, ahead);  //first(beta, ahead)
                        for(k = fst.begin(); k != fst.end(); k++)
                        {
                            // cout<<j<<" j$\t";
                            // rules[j].print();
                            // // cout<<*k<<" "<<cl.size()<<" ";
                            // cout<<tmp<<" ##"<<next<<" "<<ahead<<" "<<*k<<" "<<cl.size()<<endl;
                            // set<pair<Rule, Item::State> >::iterator pos = cl.find(make_pair(rules[j], Item::State(0, *k) ) );
                            // if(pos != cl.end())
                            // {
                            //     cout<<pos->first.lelm<<" -> ";
                            //     int dot = pos->second.dot;
                            //     for(int p = 0; p < pos->first.relm.size(); p++)
                            //     {
                            //         if(dot == p)
                            //             cout<<"@";
                            //         cout<<pos->first.relm[p]<<" ";
                            //     }
                            //     if(dot == pos->first.relm.size() )
                            //         cout<<"@";
                            //     cout<<", "<<pos->second.ahead<<endl;
                            //     // cout<<pos->second.dot<<" "<<pos->second.ahead<<endl;
                            // }
                            cl.insert( make_pair(rules[j], Item::State(0, *k) ) );
                            // cout<<cl.size()<<endl;
                        }
                    }
                }
            }
        }
        if(item.itemSet.size() != cl.size() )
            flag = 1;
        item.itemSet = cl;
    } 
    // cout<<endl;
    // item.print();   
}


void Grammar::geneClosure()
{
    initClosure();
    geneSet();
    closure.push_back(item.itemSet);

    set<pair<Rule, Item::State> > cl;
    set<pair<Rule, Item::State> >::iterator j;
    pair<Rule, Item::State> tmp;
    string next;
    int dot;
    for(int i = 0; i < closure.size(); i++)
    {
        initTable(i);
        cl = closure[i];
        for(j = cl.begin(); j != cl.end(); j++)
        {
            tmp = *j, dot = tmp.second.dot;
            item.itemSet.clear();
            if(dot < tmp.first.relm.size() && ( tmp.first.relm.size() && tmp.first.relm[0] != ept) ) //A->. empty状况
            {
                next = tmp.first.relm[dot];
                for(set<pair<Rule, Item::State> >::iterator k = cl.begin(); k != cl.end(); k++)
                {
                    if(k->second.dot == k->first.relm.size() )
                        continue;
                    if(k->first.relm[ k->second.dot ] == next)
                        item.itemSet.insert(make_pair(k->first, Item::State(k->second.dot + 1,  k->second.ahead) ) );
                }
            }
            geneSet();
            if( findSet(item.itemSet) == -1)
            {
                // cout<<i<<" "<< xx<<endl;
                // item.print();
                closure.push_back(item.itemSet);
            }
        }
    }
}

int Grammar::getRuleNum(Rule r)
{
    for(int i = 0; i < rules.size(); i++)
    {
        if(rules[i] == r)
            return i;
    }
    return -1;
}

void Grammar::buildLR1table()
{
    set<pair<Rule, Item::State> > cl;
    set<pair<Rule, Item::State> >::iterator j;
    pair<Rule, Item::State> tmp;
    string next;
    int dot;

    initClosure();
    geneSet();
    closure.push_back(item.itemSet);
    initTable(0);

    for(int i = 0; i < closure.size(); i++)
    {
        cl = closure[i];
        for(j = cl.begin(); j != cl.end(); j++)
        {
            tmp = *j, dot = tmp.second.dot;
            item.itemSet.clear();
            if(dot < tmp.first.relm.size() && ( tmp.first.relm.size() && tmp.first.relm[0] != ept ) ) //A->. empty状况
            {
                next = tmp.first.relm[dot];
                for(set<pair<Rule, Item::State> >::iterator k = cl.begin(); k != cl.end(); k++)
                {
                    if(k->second.dot == k->first.relm.size() )
                        continue;
                    if(k->first.relm[ k->second.dot ] == next)
                        item.itemSet.insert(make_pair(k->first, Item::State(k->second.dot + 1,  k->second.ahead) ) );
                }
            }
            else
                next = ept;
            geneSet();
            int J = findSet(item.itemSet);
            if(J == -1)
            {
                closure.push_back(item.itemSet);
                J = closure.size() - 1;
                initTable(J);
            }
            // cout<<i<<next<<" "<<J<<" "<<tmp.second.ahead<<" "<<( !isVt(next) )<<endl;
            if( !isVt(next) )
            {
                if(go_to[i][next] == -1 || go_to[i][next] == J)
                    go_to[i][next] = J; //转入状态j
                else
                    cout<<"generate LR1 tabel conflict. from"<<i<<" goto "<<J<<" "<<go_to[i][next]<<endl;
            }
            else if(next != ept && J >= 0)
            {
                if(action[i][next].second == -1 || action[i][next] == make_pair('S', J) )
                    action[i][next] = make_pair('S', J); //移进符号a和状态j
                else
                    cout<<"generate LR1 tabel conflict. "<<i<<" getin "<<next<<" and "<<J<<" pre "<<action[i][next].first<<" "<<action[i][next].second<<endl;
            }
            else if(next == ept && tmp.first.relm[dot - 1] == startSym)
            {
                if(action[i][next].second == -1 || action[i][next] == make_pair('A', -1) )
                    action[i][next] = make_pair('A', -1);  //ACC
                else
                    cout<<"generate LR1 tabel conflict."<<i<<" getin "<<next<<" at ACC"<<endl;
            }   
            else
            {
                int rulnum = getRuleNum(tmp.first);
                // tmp.first.print(); cout<<" "<<tmp.second.dot<<" "<<tmp.second.ahead<<endl;
                // cout<<" "<<rulnum<<" "<<endl;
                if(action[i][tmp.second.ahead].second == -1 || action[i][tmp.second.ahead] == make_pair('r', rulnum ) )
                    action[i][tmp.second.ahead] = make_pair('r', rulnum ); //规约
                else
                    cout<<"generate LR1 tabel conflict. reduce"<<i<<" "<<next<<" "<<rulnum<<" pre "<<action[i][tmp.second.ahead].first<<" "<<action[i][tmp.second.ahead].second<<endl;
                // tmp.first.print();
            }   
        }
    }
}

void Grammar::print()
{
    ofstream out;
    out.open("./def/initGram.txt");
    if(out)
    {
        out<<"vn : ";
        for(set<string>::iterator it = vn.begin(); it != vn.end(); it++)
        {
            out<<*it<<" ";
        }
        out<<endl;
        out<<"vt : ";
        for(set<string>::iterator it = vt.begin(); it != vt.end(); it++)
        {
            out<<*it<<" ";
        }
        out<<endl;
        for(int i = 0; i < rules.size(); i++)
        {
            out<<rules[i].id<<" "<<rules[i].lelm<<" -> ";
            for(int j = 0; j < rules[i].relm.size(); j++)
            {
                out<<rules[i].relm[j]<<" ";
            }
            out<<endl;
        }
        out.close();
    }

    out.open("./def/firstSet.txt");
    if(out)
    {
        out<<"First : "<<endl;
        map<string, set<string> >::iterator it;
        for(it = first.begin(); it != first.end(); it++)
        {
            out<<it->first<<" : ";
            for(set<string>::iterator j = it->second.begin(); j != it->second.end(); j++)
            {
                out<<*j<<" ";
            }
            out<<endl;
        }
        out.close();
    }
    out.open("./def/Closure.txt");
    if(out)
    {
        out<<"closure:"<<endl;
        set<pair<Rule, Item::State> >::iterator sit;
        for(int i = 0; i < closure.size(); i++)
        {
            out<<i<<" : "<<endl;
            for(sit = closure[i].begin(); sit != closure[i].end(); sit++)
            {
                out<<sit->first.lelm<<" -> ";
                int dot = sit->second.dot;
                for(int j = 0; j < sit->first.relm.size(); j++)
                {
                    if(dot == j)
                        out<<"@";
                    out<<sit->first.relm[j]<<" ";
                }
                if(dot == sit->first.relm.size() )
                    out<<"@";
                out<<", "<<sit->second.ahead<<endl;
            }
        }
        out.close();
    }

    // // out.open("./test/test7out.txt");
    out.open("./def/LR1Table.txt");
    if(out)
    {
        out<<"LR1 Table :"<<endl;
        out<<"\tAction";
        int n = vt.size();
        for(int i = 0; i < n; i++)
        {
            (i == (n - 1) ) ? out<<"\t\tGOTO\n" : out<<"\t" ;
        }
        set<string>::iterator sit;
        out<<"\t";
        for(sit = vt.begin(); sit != vt.end(); sit++)
        {
            out<<*sit<<"\t";
        }
        out<<ept<<"\t";
        for(sit = vn.begin(); sit != vn.end(); sit++)
        {
            out<<*sit<<"\t";
        }
        out<<endl;
        for(int i = 0; i < closure.size(); i++)
        {
            out<<i<<"\t";
            for(sit = vt.begin(); sit != vt.end(); sit++)
            {
                if(action[i][*sit].first != 'F')
                    out<<action[i][*sit].first;
                if(action[i][*sit].second != -1)
                    out<<action[i][*sit].second;
                out<<"\t";
            }
            if(action[i][ept].first != 'F')
                out<<action[i][ept].first;
            if(action[i][ept].second != -1)
                    out<<action[i][ept].second;
            out<<"\t";
            for(sit = vn.begin(); sit != vn.end(); sit++)
            {
                if(go_to[i][*sit] != -1)
                    out<<go_to[i][*sit]<<"\t";
            }
            out<<endl;
        }
        out.close();
    }
    out.open("./def/goto.txt");
    if(out)
    {
        set<string>::iterator sit;
        out<<"\t";
        for(sit = vn.begin(); sit != vn.end(); sit++)
        {
            out<<*sit<<"\t";
        }
        out<<endl;
        for(int i = 0; i < closure.size(); i++)
        {
            out<<i<<"\t";
            for(sit = vn.begin(); sit != vn.end(); sit++)
            {
                if(go_to[i][*sit] != -1)
                    out<<go_to[i][*sit]<<"\t";
            }
            out<<endl;
        }
        out.close();
    }
    out.open("./def/action.txt");
    if(out)
    {
        set<string>::iterator sit;
        out<<"\t";
        for(sit = vt.begin(); sit != vt.end(); sit++)
        {
            out<<*sit<<"\t";
        }
        out<<ept<<"\t"<<endl;
        for(int i = 0; i < closure.size(); i++)
        {
            out<<i<<"\t";
            for(sit = vt.begin(); sit != vt.end(); sit++)
            {
                if(action[i][*sit].first != 'F')
                    out<<action[i][*sit].first;
                if(action[i][*sit].second != -1)
                    out<<action[i][*sit].second;
                out<<"\t";
            }
            if(action[i][ept].first != 'F')
                out<<action[i][ept].first;
            if(action[i][ept].second != -1)
                    out<<action[i][ept].second;
            out<<"\t"<<endl;;
        }
        out.close();
    }
}

/**
*  初始化语法分析所用Vn, Vt, First集和LR1表
*/
void Grammar::initSyntex()
{
    // freopen("./test/test7.txt", "r", stdin);
    ifstream in;
    in.open("definition.txt");
    if(in)
    {
        string s,ss;
        bool flag;
        int id = 0;
        while(getline(in, s) )
        {
            Rule rule;
            ss = "";
            for(int i = 0; i < s.length(); i++)
            {
                if(s[i] == ' ')
                {
                    rule.relm.push_back(ss);
                    if(ss[0] >= 'A' && ss[0] <= 'Z')
                    {
                        vn.insert(ss);
                    }
                    else
                        vt.insert(ss);
                    ss = "";
                    continue;
                }
                if(s[i] == '|')
                {
                    rule.id = ++id;
                    rules.push_back(rule);
                    rule.relm.clear();
                    ss = "";
                    i++; //去除每个｜后面空格
                    continue;
                }
                if(s[i] == '-' && s[i + 1] == '>')
                {
                    if(rules.size() == 1)
                    {
                        startSym = ss;
                        rules[0].relm.push_back(startSym);
                    }
                    vn.insert(ss);
                    rule.lelm = ss;
                    ss = "";
                    i++;
                }
                else
                {
                    ss += s[i];   
                }
            }
            if(ss[0] >= 'A' && ss[0] <= 'Z') //最后一个  
                vn.insert(ss);
            else if(ss != ept)
                vt.insert(ss);
            rule.relm.push_back(ss); 
            rule.id = ++id;
            rules.push_back(rule);  
        }
        first = geneFirst();
        buildLR1table();
        in.close();
            // print();
    }
}

// int main()
// {
//         Grammar G;
//         freopen("out.txt", "w", stdout);
//         G.print();
//         return 0;
// }