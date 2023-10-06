#include "asmlsl.hpp"

Stack mainStack;

int main(int argc, const char **argv){
    system("cls");
    vector<string> cmds; for(int i=1;i<argc;i++){cmds.push_back(argv[i]);}

    globalStack = &mainStack;
    mainStack.set("testVar", 4200);
    
    runCode(getFileLines(cmds[0]));
    cout<<"\n<-END\n";
}