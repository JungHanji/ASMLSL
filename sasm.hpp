#pragma once
#include <AdtClasses/AdtClasses.hpp>

vector<string> asm_cmds{
    "reg", "exc", "arr", "mov", "set", "jmp", "jbp", "jnb", "sum", 
    "sub", "div", "mlt", "pwr", "psh", "pop", "pek", "brk", "ret", 
    "gip", "got", "inp", "out", "fpush", "tmp", "inc", "dcr", "stv", "get", //27
    "check", "pget", "pset", "paddr", "addr", "dref", "ptr", // 34
    "sget", "sset", "lpp" // 37
};

class Stack{
    public:

    map<string, int> vars;
    map<string, int> statics;
    map<string, int> regs;
    map<string, int> ptrs;
    vector<int> stack;

    int reg(string name){vars[name] = 0; return 0;}
    void set(string name, int i){vars[name] = i;}
    int get(string name){return vars[name];}

    void push(string name){stack.push_back(vars[name]);}
    void push(int val){stack.push_back(val);}
    void sset(int pos, int val){stack[pos] = val;}
    int pop(){int t = stack.back();stack.pop_back();return {t};}
    int peek(){return stack.back();}
    
    void pushreg(string name, int n){regs[name]=n;}
    int popreg(string name){int t = regs[name];regs.erase(name);return {t};}
    int peekreg(string name){return regs[name];}

    void ptrreg(string name){ptrs[name] = -1;}
    void ptrset(string name, int addr){ptrs[name] = addr;}
    int ptrGetAddr(string name){return ptrs[name];}
    int ptrGetValByAddr(string name, int pls_indx = 0){return stack[ptrs[name] + pls_indx];}

    void regSt(string name, int val){if(statics.find(name) == statics.end()){statics[name] = val;}}
    void setSt(string name, int val){statics[name] = val;}
    int getSt(string name){return statics[name];}

    void clear(){
        vars.clear();
        regs.clear();
        ptrs.clear();
        stack.clear();
    }

    void out(){
        int indx = 0;
        
        print("_stack: ", false);
        for(int ee : stack){
            print(ee, false);
            indx++;
        }
        cout<<endl;
        print("_vars: ", true);
        indx = 0;
        for(auto ee : vars){
            cout<<"\t";
            print(ee.first + " : " + to_string(vars[ee.first]));
            indx++;
        }
        cout<<endl;
        print("_statics: ", true);
        indx = 0;
        for(auto ee : statics){
            cout<<"\t";
            print(ee.first + " : " + to_string(vars[ee.first]));
            indx++;
        }
        cout<<endl;
    }

    Stack(){
        ;
    }
};

map<string, Stack> global_stacks;
Stack *this_stack, sysStack, *globalStack;

void pushVarOrInt(vector<string> &splline, int ind){
    if(!isInteger(splline[ind])){
        this_stack->push(splline[ind]);
    } else {
        this_stack->push(::stoi(splline[ind]));
    }
}

void runLine(vector<string> splline, string &part, string &parentPart, int &line, map<string, int> &parts, bool &endedProg){
    switch(index(asm_cmds, splline[0])){
        case 0: { //reg
            for(int i = 1; i < splline.size();i++){this_stack->reg(splline[i]);}
            break;
        }
        case 29:{ // pget
            this_stack->push(this_stack->ptrGetValByAddr(splline[1]));
            break;
        }
        case 30:{ // pset
            for(int i=1;i<splline.size();i++){
                this_stack->ptrset(splline[i], this_stack->stack.size()-i);
            }
            break;
        }
        case 31:{ // paddr
            for(int i=1;i<splline.size();i++){
                this_stack->ptrset(splline[i], this_stack->stack.size()-i);
            }
            break;
        }
        case 32:{ // addr
            this_stack->push(this_stack->stack.size()-1);
            break;
        }
        case 33:{ // dref
            pushVarOrInt(splline, 1);
            this_stack->push(this_stack->stack[this_stack->pop()]);
            break;
        }
        case 34:{ // ptr
            for(int i=1;i<splline.size();i++)
                this_stack->ptrset(splline[i], this_stack->stack.size()-i);
            break;
        }

        case 35:{ // sget
            this_stack->push(this_stack->getSt(splline[1]));
            break;
        }

        case 36:{ // sset
            for(int i = 1; i < 2;i++){
                pushVarOrInt(splline, i+1);
                this_stack->setSt(splline[i], this_stack->pop());
            }
            break;
        }

        case 26:{ //stv
            pushVarOrInt(splline, 1+1);
            this_stack->regSt(splline[1], this_stack->pop());
            break;
        }
        case 28:{ //check
            this_stack->out();
            break;
        }
        case 1: { // exc
            int a, b;
            if(this_stack->statics.find(splline[1]) == this_stack->statics.end()) {pushVarOrInt(splline, 1);}
            else {this_stack->push(this_stack->getSt(splline[1]));}
            if(this_stack->statics.find(splline[3]) == this_stack->statics.end()) {pushVarOrInt(splline, 3);}
            else {this_stack->push(this_stack->getSt(splline[3]));}
            b = this_stack->pop();
            a = this_stack->pop();
            if(splline[2] == ">")      {this_stack->push(a > b);}
            else if(splline[2] == "<") {this_stack->push(a < b);}
            else if(splline[2] == "!="){this_stack->push(a != b);}
            else if(splline[2] == "=="){this_stack->push(a == b);}
            else if(splline[2] == "<="){this_stack->push(a <= b);}
            else if(splline[2] == ">="){this_stack->push(a >= b);}

            if(splline[4] == "&" && !this_stack->peek()){
                line += 1;
            }
            break;
        }
        case 2: { // arr
            this_stack->out();
            this_stack->ptrset(splline[1], (this_stack->stack.size() != 0 ? this_stack->stack.size()-1 : 0));
            pushVarOrInt(splline, 2);
            int size = this_stack->pop();
            for(int i = 0; i < size; i++) {this_stack->push(-1);}
            this_stack->out();
            break;
        }
        case 27:{ // get
            pushVarOrInt(splline, 2);
            this_stack->push(this_stack->ptrGetValByAddr(splline[1], this_stack->pop()));
            break;
        }
        case 3: { // mov
            for(int i = 1; i < splline.size()-1;i+=2){
                pushVarOrInt(splline, i+1);
                this_stack->set(splline[i], this_stack->pop());
            }
            break;
        }
        case 4: { // set
            pushVarOrInt(splline, 3);
            pushVarOrInt(splline, 2);
            int a = this_stack->pop(), b = this_stack->pop();
            this_stack->sset(this_stack->ptrGetAddr(splline[1]) + a, b);
            break;
        }
        case 22:{ // fpush
            
            if(global_stacks.find("$TEMP_STACK:"+splline[1]) == global_stacks.end()) {
                global_stacks["$TEMP_STACK:"+splline[1]] = Stack();
                sysStack.pushreg("needArgs", true);
            }
            for(int i=2;i<splline.size();i++){
                pushVarOrInt(splline, i);
                global_stacks["$TEMP_STACK:"+splline[1]].push(this_stack->pop());
            }
            break;
        }
        case 23:{ // tmp
            for(int i = 1; i < splline.size();i++){
                this_stack->reg(splline[i]);
                this_stack->set(splline[i], this_stack->pop());
            }
            break;
        }
        case 24:{ // inc
            if(this_stack->vars.find(splline[1]) != this_stack->vars.end()){
                for(int i = 1;i<splline.size();i++){
                    this_stack->set(splline[i], this_stack->get(splline[i])+1);
                }
            } else {
                for(int i = 1;i<splline.size();i++){
                    this_stack->setSt(splline[i], this_stack->getSt(splline[i])+1);
                }
                
            }
            break;
        }
        case 25:{ // dcr
            if(this_stack->vars.find(splline[1]) != this_stack->vars.end()){
                for(int i = 1;i<splline.size();i++){
                    this_stack->set(splline[i], this_stack->get(splline[i])-1);
                }
            } else {
                for(int i = 1;i<splline.size();i++){
                    this_stack->setSt(splline[i], this_stack->getSt(splline[i])-1);
                }
            }
            break;
        }
        case 37:{
            line = parts[part];
            break;
        }
        case 5: { // jmp
            parentPart = part;
            part = splline[1];
            sysStack.pushreg("returnTo" + parentPart, line);
            if(sysStack.peekreg("needArgs")){
                int size = global_stacks["$TEMP_STACK:"+part].stack.size();
                for(int i=0;i<size;i++) global_stacks[part].push(global_stacks["$TEMP_STACK:"+part].pop());
                global_stacks.erase("$TEMP_STACK:"+part);
            }
            line = parts[part];
            this_stack = &global_stacks[part];
            sysStack.pushreg("needArgs", false);
            break;
        }
        case 6: { // jbp
            if(this_stack->pop()) {
                parentPart = part; 
                part = splline[1];
                sysStack.pushreg("returnTo" + parentPart, line); 
                if(sysStack.peekreg("needArgs")){
                    int size = global_stacks["$TEMP_STACK:"+part].stack.size();
                    for(int i=0;i<size;i++) global_stacks[part].push(global_stacks["$TEMP_STACK:"+part].pop());
                    global_stacks.erase("$TEMP_STACK:"+part);
                }
                line = parts[part];
                this_stack = &global_stacks[part];
            }
            sysStack.pushreg("needArgs", false);
            break;
        }
        case 7: { // jnb
            if(!this_stack->pop()) {
                parentPart = part; 
                part = splline[1];
                sysStack.pushreg("returnTo" + parentPart, line);
                if(sysStack.peekreg("needArgs")){
                    int size = global_stacks["$TEMP_STACK:"+part].stack.size();
                    for(int i=0;i<size;i++) global_stacks[part].push(global_stacks["$TEMP_STACK:"+part].pop());
                    global_stacks.erase("$TEMP_STACK:"+part);
                }
                line = parts[part];
                this_stack = &global_stacks[part];
            }
            sysStack.pushreg("needArgs", false);
            break;
        }
        case 8: { // sum
            int a = this_stack->get(splline[1]);
            pushVarOrInt(splline, 2);
            int b = this_stack->pop(); 
            this_stack->push(a+b);
            break;
        }
        case 9: { // sub
            int a = this_stack->get(splline[1]);
            pushVarOrInt(splline, 2);
            int b = this_stack->pop();
            this_stack->push(a-b); 
            break;
        }
        case 10: { // div
            int a = this_stack->get(splline[1]);
            pushVarOrInt(splline, 2);
            int b = this_stack->pop();
            this_stack->push(a/b); 
            break;
        }
        case 11: { // mlt
            int a = this_stack->get(splline[1]);
            pushVarOrInt(splline, 2);
            int b = this_stack->pop();
            this_stack->push(a*b); 
            break;
        }
        case 12: { // pwr
            int a = this_stack->get(splline[1]);
            pushVarOrInt(splline, 2);
            int b = this_stack->pop();
            this_stack->push(pow(a,b)); 
            break;
        }
        case 13: { // psh
            pushVarOrInt(splline, 1);
            break;
        }
        case 14: { // pop
            for(int i = 1; i < splline.size();i++){
                if(splline.size()==1){this_stack->pop();} 
                else {this_stack->set(splline[i], this_stack->pop());}
            }
            break;
        }
        case 15: { // pek
            this_stack->set(splline[1], this_stack->peek());
            break;
        }
        case 16: { // brk
            part = parentPart;
            line = sysStack.popreg("returnTo" + part);
            this_stack = &global_stacks[part];
            break;
        }
        case 17: { // ret
            if(splline[1][0] != '$'){
                
                if(this_stack->statics.find(splline[1]) == this_stack->statics.end()) {pushVarOrInt(splline, 1);}
                else {this_stack->push(this_stack->getSt(splline[1]));}
                global_stacks[parentPart].push(this_stack->pop());
                this_stack->clear();
                part = parentPart;
                line = parts[part]+1;

                this_stack = &global_stacks[part];
            } else {
                auto temp = sliceVec(splline, 1, 0);
                temp[0] = slice(temp[0], 1, 0);
                //printv<string>(temp, true, " ");
                int tline = line; string a = part, b = parentPart, c;
                runLine(temp, a, b, tline, parts, endedProg);
                global_stacks[parentPart].push(this_stack->pop());
                this_stack->clear();
                part = parentPart;
                line = parts[part]+1;
                
                this_stack = &global_stacks[part];
            }
            
            line = sysStack.popreg("returnTo" + parentPart);
            break;
        }
        case 18: { // gip
            this_stack->set(splline[1], globalStack->get(splline[2]));
            break;
        }
        case 19: { // got
            globalStack->push(this_stack->get(splline[1]));
            break;
        }
        case 20: { // inp
            for(int i=1;i<splline.size();i++){
                int t;cin>>t;this_stack->set(splline[i], t);
            }
            break;
        }
        case 21: { // out
            if(splline.size()!=1){
                if(this_stack->vars.find(splline[1]) != this_stack->vars.end()){
                    cout<<"var >> "<<this_stack->get(splline[1])<<endl;
                } else if(this_stack->statics.find(splline[1]) != this_stack->statics.end()){
                    cout<<"static >> "<<this_stack->getSt(splline[1])<<endl;
                } else {
                    cout<<"string >> "<<join(sliceVec(splline, 1, 0))<<endl;
                }
            } else {
                cout<<"pop >> "<<this_stack->peek()<<endl;
            }
            break;
        }
        case -1:{
            if(splline[0]==";"){
                if(part!="main"){
                    line = sysStack.popreg("returnTo" + parentPart);
                }
                if(part == "main"){endedProg = true;}
                part = parentPart;
                this_stack = &global_stacks[part];
            } else {
                print(rgbFRString({255, 0, 0}, "[COMPILER] ERROR: command " + splline[0] + " is not defined"));
            }
        }
    }
}

void runCode(vector<string> lines){
    sysStack.pushreg("needArgs", false);
    bool ended = false;
    string part = "main", parentPart = "main";
    map<string, int> lineIndexesOfParts;
    //vector<string> loops;

    int tindx = 0;
    for(string line : lines){
        //if(line[0]=='.' && split(line, '.')[1]!="loop"){
            lineIndexesOfParts[slice(line, 1, -2)] = tindx;
            global_stacks[slice(line, 1, -2)] = Stack();
        //} else if(line[0]=='.' && split(line, '.')[1]=="loop"){
        //    loops.push_back(slice(split(line, '.')[2], 0, -1));
        //    lineIndexesOfParts[slice(split(line, '.')[2], 0, -1)] = tindx;
        //    global_stacks[slice(split(line, '.')[2], 0, -1)] = Stack();
        //}
        
        tindx++;
    }

    int indxLine = index<string>(lines, ".main:");

    if(indxLine==-1){
        print(rgbFRString({255, 0, 0}, "[COMPILER] ERROR: the start point didnt found"));
        throw -1;
    }
    this_stack = &global_stacks["main"];
    indxLine++;

    while(!ended){
        string line = lines[indxLine];
        line = remove(line, {'\t'});
        if(line.size()>=1){
            if(line[0]!='@'){
                runLine(split(line), part, parentPart, indxLine, lineIndexesOfParts, ended);
            }
            indxLine++;
        }
    }
};