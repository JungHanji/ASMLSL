#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <locale>
#include <codecvt>
#include <sstream>
#include <map>
#include <memory>
#include <stdexcept>
#include <functional>
#include <MathLib/Vector3.hpp>
#include <MathLib/AddMath.hpp>

using namespace std;

wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

wstring stringToWstring(string src){
    return converter.from_bytes(src);
}

string wstringToString(wstring src){
    return converter.to_bytes(src);
}



std::string pixel(vec3 r){
    std::stringstream ss;
    ss << "\033[48;2;" << abs(clamp(0, 255, round(r.x))) << ";" << abs(clamp(0, 255, round(r.y))) <<";" << abs(clamp(0, 255, round(r.z))) <<"m "<<"\033[0;0m";
    return ss.str();
}

std::wstring wpixel(vec3 r){
    std::wstringstream ss;
    ss << L"\033[48;2;" << abs(clamp(0, 255, round(r.x))) << L";" << abs(clamp(0, 255, round(r.y))) << L";" << abs(clamp(0, 255, round(r.z))) <<L"m "<<L"\033[0;0m";
    return ss.str();
}

std::string rgbBGChar(vec3 r, char c){
    std::stringstream ss;
    ss << "\033[48;2;" << abs(clamp(0, 255, round(r.x))) << ";" << abs(clamp(0, 255, round(r.y))) <<";" << abs(clamp(0, 255, round(r.z))) <<"m"<<c<<"\033[0;0m";
    return ss.str();
}

std::string rgbFRChar(vec3 r, char c){
    std::stringstream ss;
    ss << "\033[38;2;" << abs(clamp(0, 255, round(r.x))) << ";" << abs(clamp(0, 255, round(r.y))) <<";" << abs(clamp(0, 255, round(r.z))) <<"m"<<c<<"\033[0;0m";
    return ss.str();
}

std::wstring wrgbFRChar(vec3 r, wchar_t c){
    std::wstringstream ss;
    ss << L"\033[38;2;" << abs(clamp(0, 255, round(r.x))) << L";" << abs(clamp(0, 255, round(r.y))) << L";" << abs(clamp(0, 255, round(r.z))) << L"m" << c << L"\033[0;0m";
    return ss.str();
}

std::wstring wrgbBGChar(vec3 r, wchar_t c){
    std::wstringstream ss;
    ss << L"\033[48;2;" << abs(clamp(0, 255, round(r.x))) << L";" << abs(clamp(0, 255, round(r.y))) << L";" << abs(clamp(0, 255, round(r.z))) << L"m" << c << L"\033[0;0m";
    return ss.str();
}

std::string rgbFRString(vec3 r, string st){
    std::stringstream ss;
    ss << "\033[38;2;" << abs(clamp(0, 255, round(r.x))) << ";" << abs(clamp(0, 255, round(r.y))) << ";" << abs(clamp(0, 255, round(r.z))) << "m" << st << "\033[0;0m";
    return ss.str();
}

std::wstring wrgbFRString(vec3 r, wstring st){
    std::wstringstream ss;
    ss << L"\033[38;2;" << abs(clamp(0, 255, round(r.x))) << L";" << abs(clamp(0, 255, round(r.y))) << L";" << abs(clamp(0, 255, round(r.z))) << L"m" << st << L"\033[0;0m";
    return ss.str();
}

std::wstring wrgbBGString(vec3 r, wstring st){
    std::wstringstream ss;
    ss << L"\033[48;2;" << abs(clamp(0, 255, round(r.x))) << L";" << abs(clamp(0, 255, round(r.y))) << L";" << abs(clamp(0, 255, round(r.z))) << L"m" << st << L"\033[0;0m";
    return ss.str();
}

std::wstring wrgbBGFRChar(vec3 bg, vec3 fr, wchar_t c){
    return wrgbBGString(bg, wrgbFRChar(fr, c));
}

std::wstring wrgbBGFRString(vec3 bg, vec3 fr, wstring s){
    return wrgbBGString(bg, wrgbFRString(fr, s));
}



string to_string(char ch){
    string o;o+=ch;
    return {o};
}

wstring to_wstring(wchar_t ch){
    wstring o;o+=ch;
    return {o};
}

bool replaceFirst(std::string& str, std::string from, std::string to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

bool replaceFirstW(std::wstring& str, std::wstring from, std::wstring to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void replaceAll(std::string& str, std::string from, std::string to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void replaceAllw(std::wstring& str, std::wstring from, std::wstring to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::wstring::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void replaceVInterSpaces(vector<string> &a, string interspace = " ", string toReplace = ""){for(string &elem : a){replaceAll(elem, interspace, toReplace);}}
void replaceInterSpaces(string &a, string interspace = " ", string toReplace = ""){replaceAll(a, interspace, toReplace);}
void replaceVInterSpacesW(vector<wstring> &a, wstring interspace = L" ", wstring toReplace = L""){for(wstring &elem : a){replaceAllw(elem, interspace, toReplace);}}
void replaceInterSpacesW(wstring &a, wstring interspace = L" ", wstring toReplace = L""){replaceAllw(a, interspace, toReplace);}

template<class TString>
TString repeat(TString part, int num){
    TString out;for(int i = 0; i < num; i++){out += part;}
    return {out};
}

template<class TString> 
TString slice(TString str, int aInd = 0, int bInd = INT_MAX){
    if(bInd == INT_MAX) {return str.substr(aInd-1);}
    else if(bInd <= 0) {return str.substr(aInd, str.size()+bInd);}
    else {return str.substr(aInd, bInd);}
}

template<class c>
void print(c cc, bool needEnd = true){needEnd ? cout<<cc<<endl : cout<<cc<<' ';}

template<class kc>
bool in(vector<kc> v, kc i){
    for(kc k : v){
        if(k == i){
            return true;
        }
    }
    return false;
}

template<class vc>
int index(vector<vc> vecs, vc key){
    int ind = 0;
    for(vc vkey : vecs){
        if(vkey == key) return ind;
        ind++;
    }

    return -1;
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args){
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }

    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf(new char[ size ]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

struct BoolWRP{
    bool val;
};

template<class kc, class vc>
class dict{
    public:
    std::vector<kc> keys;
    std::vector<vc> values;
    dict(){
        ;
    }

    int size(){
        return values.size();
    }

    void set(kc key, vc val){
        if(in(keys, key)){
            values.at(index(keys, key)) = val;
        } else {
            keys.push_back(key);
            values.push_back(val);
        }
    }

    bool in_key(kc key){
        return in(keys, key);
    }

    bool in_val(vc val){
        return in(values, val);
    }

    vc &get(kc key){
        return values[index(keys, key)];
    }

    vc &operator[](kc key){
        if(!in(keys, key)) set(key, vc());
        return values[index(keys, key)];
    }
};

template<class kc, class vc>
class paar{
    public:
    kc key;
    vc value;
    vc *vpointer;
    bool nvalue = false;

    paar(){}

    paar(kc k, vc v){
        key=k;
        value=v;
        vpointer = &value;
    }

    void setv(vc val){
        value=val;
        vpointer = &value;
    }

    void setk(kc key_){
        key=key_;
    }

    vc &get(){
        return value;
    }

    vc *getPointer(){
        return vpointer;
    }

    void print(){
        cout<<"key="<<key<<" | value="<<value<<endl;
    }

    bool operator==(paar<kc, vc> &other){
        return key==other.key && value==other.value;
    }
};

template<class alpha>
class Ideoma{
    public:
    
    alpha a, b;
    
    Ideoma(alpha a_, alpha b_){
        a = a_;
        b = b_;
    }

    Ideoma(){
        a = alpha();
        b = alpha();
    }
};

template<class kc, class vc>
paar<kc, vc> &getPaarByName(vector<paar<kc, vc>> vd, kc name){
    for(paar<kc, vc> &p : vd){
        if(p.key == name){
            return p;
        }
    }
    cout<<"NPOS: "<<name<<endl;
    //return npos
}

template<class kc, class vc>
int paarIndex(vector<paar<kc, vc>> vd, paar<kc, vc> paar){
    for(int i = 0; i < vd.size(); i++){
        if(vd[i] == paar){
            return i;
        }
    }
    return -1;
}

template<class kc, class vc>
int paarIndexByName(vector<paar<kc, vc>> vd, kc name){
    for(int i = 0; i < vd.size(); i++){
        if(vd[i].key == name){
            return i;
        }
    }
    return -1;
}

template<class kc, class vc>
bool keyInPaars(vector<paar<kc, vc>> paars, kc key){
    for(paar<kc, vc> p : paars){
        if(p.key == key){
            return true;
        }
    }
    return false;
}

template<class f, class s>
vector<f> getKeysVPairs(vector<pair<f,s>> vc){
    vector<f> o; for(pair<f,s> &el : vc){o.push_back(el.first);}
    return {o};
}

template<class kc, class vc>
vector<reference_wrapper<vc>> getOnlyVals(vector<paar<kc, vc>> vd){
    vector<reference_wrapper<vc>> vals;
    for(paar<kc, vc> p : vd){
        vals.push_back(reference_wrapper<vc>(p.value));
    }
    return {vals};
}

template<class k>
k &getLast(vector<k> v){
    return v[v.size()-1];
}

template<class cs>
void printv(vector<cs> vs, bool qoutes = false, cs end=' '){
    for(cs s: vs){
        if(qoutes) cout<<(cs)"'"<<s<<(cs)"'"<<end;
        else cout<<s<<end;
    }
    cout<<endl;
}

vector<string> getFileLines(std::string fileName, char sep = '\n'){
    ifstream ogfile(fileName);
    if(!ogfile.is_open()){
        cerr<<"[ERROR] getFileLines: cant open file\n";
    }
    string line;
    vector<string> lines;
    while (!ogfile.eof()){
        getline(ogfile,line, sep);
        if(line.size()>=1)lines.push_back(line);
    }

    return {lines};
}

vector<wstring> getFileLinesW(std::string fileName, wchar_t sep = L'\n'){
    wifstream ogfile(fileName);
    if(!ogfile.is_open()){
        cerr<<"[ERROR] getFileLinesW: cant open file\n";
    }
    wstring line;
    vector<wstring> lines;
    while (!ogfile.eof()){
        getline(ogfile,line, sep);
        if(line.size()>=1)lines.push_back(line);
    }

    return {lines};
}

template<class T>
void writeToFile(string fileName, vector<T> lines, char mode = 'w', char sep = '\n'){
    switch (mode){
        case 'w':{
            std::ofstream out(fileName);
            if(out.is_open()){
                for(T &el :lines){
                    out << el << sep;
                }
            } else {
                cerr<<"[ERROR] writeToFile (mode=w): cant open file\n";
            }
            out.close();
            break;
        };
        case 'a':{
            std::ofstream out(fileName, ios::app);
            if(out.is_open()){
                for(T &el :lines){
                    out << el << sep;
                }
            } else {
                cerr<<"[ERROR] writeToFile (mode=a): cant open file\n";
            }
            out.close();
            break;
        };
    }
}


void writeToFileW(string fileName, vector<wstring> lines, char mode = 'w', wchar_t sep = L'\n', wstring end = L"\n"){
    switch (mode){
        case 'w':{
            std::wofstream out(fileName);
            if(out.is_open()){
                for(wstring &el :lines){
                    out << el << sep;
                }
                out<<end;
            } else {
                cerr<<"[ERROR] writeToFile (mode=w): cant open file\n";
            }
            out.close();
            break;
        };
        case 'a':{
            std::wofstream out(fileName, ios::app);
            if(out.is_open()){
                for(wstring &el :lines){
                    out << el << sep;
                }
                out<<end;
            } else {
                cerr<<"[ERROR] writeToFile (mode=a): cant open file\n";
            }
            out.close();
            break;
        };
    }
}

template<class T>
vector<T> delElems(vector<T> vc, int ind){
    
}

vector<int> toIntAll(vector<string> vc){
    vector<int> o;for(string &el : vc){o.push_back(::stoi(el));}
    return {o};
}

vector<string> split(string line, char delim = ' '){
    if(line.find(delim) != string::npos){
        vector<string> elems{""};
        bool isSpace = true;
        
        for(char ch : line){
            if(isSpace && ch!=delim || !isSpace && ch!=delim){
                elems[elems.size()-1]+=ch;
            }
            isSpace = ch == delim;
            if (isSpace) elems.push_back("");
        }

        return {elems};
    }

    return {line};
}

template<class T>
vector<vector<T>> splitVec(vector<T> vc, T elemSep){
    vector<vector<T>> out;
    bool appending = false;
    for(T &el : vc){
        if(el == elemSep){
            out.push_back(vector<T>());
            appending = true;
        } else if(appending){
            out.back().push_back(el);
        }
    }
    return {out};
}

template<class T>
vector<T> sliceVec(vector<T> vc, int inda, int indb){
    /*
    0 1 2 3 4 5 6
          ^ ^     
    4 7-4 = 2
    */
    return vector<T>(vc.begin() + inda, vc.end() + indb);
}

vector<wstring> wsplit(wstring line, wchar_t delim = L' '){
    if(line.find(delim) != wstring::npos){
        vector<wstring> elems{L""};
        bool isSpace = true;
        
        for(wchar_t ch : line){
            if(isSpace && ch!=delim || !isSpace && ch!=delim){
                elems[elems.size()-1]+=ch;
            }
            isSpace = ch == delim;
            if (isSpace) elems.push_back(L"");
        }

        return {elems};
    }

    return {line};
}


string join(vector<string> vs, char udel = ' '){
    string o;
    for(string s : vs){
        o+=s;
        o+=udel;
    }

    return {o};
}

string remove(string str, vector<char> vchs){
    string out;
    for(char ch : str){
        if(!in(vchs, ch)){
            out+=ch;
        }
    }
    return {out};
}

bool isInteger(std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
   char * p;
   strtol(s.c_str(), &p, 10);
   return (*p == 0);
}

template<class elT>
class SmartPointer{
    public:

    elT *pointer = nullptr;

    SmartPointer(elT &elem){
        pointer = &elem;
    }

    SmartPointer(elT *elem){
        pointer = elem;
    }

    void setElement(elT &elem){
        pointer = &elem;
    }

    elT *getElement(){
        return pointer;
    }

    bool isValid(){
        return (pointer);
    }

    ~SmartPointer(){
        delete pointer;
    }

    SmartPointer(){}
};

//"\033[48;2;255;255;255m \033[0;0m";
//"\033[48, 2, 255, 255, 255m, 

vec3 fromWstringPixel(wstring swpixel){
    vector<wstring> o = wsplit(swpixel, L';');
    if(o.size()>=5) return {::stoi(o[2]), ::stoi(o[3]), ::stoi(slice(o[4], 0, -2))};
    else return {0, 0, 0};
}