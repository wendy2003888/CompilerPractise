#include "semantic.h"

using namespace std;
Lexer Lexer;
Grammar G;
syntaxTree synTree;
Semantic S;


int main()
{
    // G.print();  
    // freopen("in1.txt","r",stdin);    //读入文件

    // freopen("out.txt","w",stdout);  //写入文件
    Lexer.runLexer();
    // Lexer.print();
    if( S.start(Lexer.words, synTree, G) )
    {
        // synTree.print(synTree.root);
        S.emitMidCode(synTree);
    }
    return 0;
}