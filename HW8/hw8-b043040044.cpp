#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;
// 吳俊忻 B043040044
// 2017/01/07
// 作業8  huffman
struct ASCIfreq{                                    //ASCIfreq 存特定字元頻率
    unsigned char ASCI;                             //ASCIfreq.ASCI 字元編號
    int freq;                                       //ASCIfreq.freq 字元頻率
};

class NODE{                                         //NODE 用於建huffman tree
public:
    NODE ();
    NODE(ASCIfreq t);
    void adjust();                                  //計算該節點頻率

    unsigned char ASCI;                             //NODE.ASCI該節點字元編號
    int freq;                                       //NODE.freq 該節點頻率
    int data_freq;                                  //NODE.data_freq 該節點字元頻率
    NODE* right;                                    //NODE.right 指向右子樹節點
    NODE* left;                                     //NODE.left 指向左子樹節點
};

void C(fstream& in,fstream& out);                   //C() 壓縮時
void list_freq(vector<ASCIfreq>& inlist,ASCIfreq input);        //list_freq()列出頻率表並排序
NODE* Huffman(string* codelist,vector<ASCIfreq>& inputlist);    //Huffman() 製作huffman樹
void swap_LR(NODE** left,NODE** right);                         //swap_LR() 子樹左右對掉
void Huff_sort(vector<NODE*>& trees);                           //Huffman() 製作huffman樹 節點排序
int Huff_list(string* codelist,int& num,string code,NODE* root);//Huff_list() 輸出編碼表
int list_cmp (const void * a, const void * b);                  //list_cmp() 排序編碼表用
int compression(const string* codelist,const int list_size,fstream& in,string& out_file);//compression() 壓縮檔案內容


void burn(NODE*);                                               //burn()將樹清除

void U(fstream& in,fstream& out);                   //U() 解壓縮時
void build_tree_path(NODE** r_root,char i);                     //build_tree_path() 建立huffman樹
void build_tree_edge(NODE* root,int i);                         //build_tree_path() 建立huffman樹
void Decompression(NODE* tree,fstream& in,fstream& out,int bits);//Decompression() 解壓縮檔案內容
int char_search(NODE** r_root,char code_bit);                   //char_search() 從樹中輸出字元

int main(int argc,char* argv[]){
    if (argc<5){
        cout << "too few argument" << endl;
        return 0;
    }
    double start = clock();
    fstream fin;                                                //fin 檔案輸入
    fstream fout;                                               //fout 輸出檔案

    if(strcmp(argv[2],"-i")==0 && strcmp(argv[4],"-o")==0){
        fin.open(argv[3], ios::in |ios::binary);
        fout.open(argv[5], ios::out |ios::binary);
    }
    else if(strcmp(argv[2],"-o")==0 && strcmp(argv[4],"-i")==0){
        fin.open(argv[5], ios::in |ios::binary);
        fout.open(argv[3], ios::out |ios::binary);
    }
    else{
        cout << "argv[2] argv[4] error" << endl;
        return 0;
    }

    if(!fin){
        cout << argv[3] << "not find" << endl;
        return 0;
    }
    if(!fout){
        cout << argv[5] << "creat error" << endl;
        return 0;
    }

    if (strcmp(argv[1],"-u")==0)                               //解壓
        U(fin,fout);
    else if(strcmp(argv[1],"-c")==0)                           //壓縮
        C(fin,fout);
    else{
        cout << "argv[1] error" << endl;
        return 0;
    }

    fin.close();
    fout.close();
    cout << "cost time:" << ((double)clock()-start)/1000 << " s" << endl;
    return 0;
}

/*-------------------------------------------NODE--------------------------------*/
NODE::NODE(){
    ASCI=0;
    freq=0;
    data_freq=0;
    right=0;
    left=0;
}
NODE::NODE(ASCIfreq t){
    ASCI=t.ASCI;
    freq=t.freq;
    data_freq=t.freq;
    right=0;
    left=0;
}
void NODE::adjust(){
    freq=data_freq;
    if(right)
        freq+=(*right).freq;
    if(left)
        freq+=(*left).freq;
}

/*----------------------------------------main----------------------------------------*/
/*---------------------------------------壓縮--------------------------------------*/
void C(fstream& in,fstream& out){                           //in為輸入檔, out輸出檔
    int in_byte=0,out_byte=0;                               //in_byte,out_byte計算輸入輸出檔案的大小
    int frequence[256];                                     //frequence存各字元出現頻率
    vector<ASCIfreq> ASCIlist;                              //ASCIlist 存字元頻率表

    for(int i=0;i<256;i++)
        frequence[i]=0;
    char tt;                                                //tt 輸入用暫存字元
    unsigned char target_char;                              //target_char 目標字元
    while(!in.eof() && in.get(tt)){         //平率統計
        target_char=(unsigned char)tt;
        in_byte++;
        frequence[target_char]++;
    }

    for (int i=0;i<256;i++){                //頻率排序
        if(frequence[i]){
            ASCIfreq t;
            t.ASCI=i;
            t.freq=frequence[i];
            list_freq(ASCIlist,t);
        }
    }
    string codelist[ASCIlist.size()];                       //codelist 存編碼表
    NODE* Huffman_tree;                                     //Huffman_tree 指向huffman樹
    Huffman_tree=Huffman(codelist,ASCIlist);             //建立編碼表於codelist

    in.clear();                                          //清理檔案輸入資訊
    in.seekg(0,ios::beg);                                //回輸入的檔案開頭

    string out_file;                                        //out_file 存放壓縮完的內容
    int bits;                                               //bits 存原始編碼bit數
    bits = compression(codelist,ASCIlist.size(),in,out_file);

    out << bits;

    for(int i=0;i<ASCIlist.size();i++){     //編碼表寫入開頭
        out<<('=');
        out<<((unsigned char)codelist[i][0]);
        out_byte+=2;
        for(int j=1;codelist[i][j]!=0;j++){
            out << ((unsigned char)codelist[i][j]);
            out_byte++;
        }
    }
    out_byte+=out_file.length();
    double rate_compression = (double)out_byte/(double)in_byte;//rate_compression 計算壓縮率
    cout << "壓縮前:" << in_byte << " bytes" << endl;
    cout << "壓縮後:" << out_byte << " bytes" << endl;
    cout << "壓縮率:" << rate_compression << endl;
    out <<'#' << in_byte << '/' << out_byte << '/' <<rate_compression << '#';

    for(int i=0;i<out_file.length();i++)
        out << ((unsigned char)out_file[i]);

    burn(Huffman_tree);
}

void list_freq(vector<ASCIfreq>& inlist ,ASCIfreq input){//製作頻率表 inlist頻率表 ,input 新增字元頻率
    int i;
    for(i=0;i<inlist.size();i++){
        if ((inlist.at(i)).freq > input.freq){
            break;
        }
    }
    inlist.insert(inlist.begin()+i,input);
}

NODE* Huffman(string* codelist,vector<ASCIfreq>& inputlist){//codelist為編碼表 inputlist 頻率表
    vector<NODE*> trees;                                    //trees 存放huffman樹及節點
    for(int i=0;i<inputlist.size();i++){
        NODE * tree=new NODE (inputlist.at(i));
        trees.push_back(tree);
    }
    if (trees.size()==1){                               //當輸入檔案僅有一種字元
        NODE* ntree=new NODE;
        (*ntree).left=trees.at(0);
        trees.at(0)=(*ntree).left;
    }
    while(trees.size()>1){                              //持續合併節點 至僅剩一樹
        NODE* ntree=new NODE;                               //ntree指向合併出的新樹
        unsigned char ASCI_1=(*(trees.at(0))).ASCI;         //ASCI_1暫存左子樹字元編號
        unsigned char ASCI_2=(*(trees.at(1))).ASCI;         //ASCI_2暫存右子樹字元編號
        (*ntree).left=trees.at(0);
        (*ntree).right=trees.at(1);

        if(ASCI_1 < ASCI_2){                                //新節點字元編號 為ASCI_1,ASCI_2中較小者
            (*ntree).ASCI=ASCI_1;
        }
        else{
            (*ntree).ASCI=ASCI_2;
            swap_LR(& ((*ntree).left),& ((*ntree).right));  //節點編號較小者為左子樹
        }

        (*ntree).adjust();

        trees.erase(trees.begin());
        trees.at(0)=ntree;                                  //新樹擺在trees中最前

        Huff_sort(trees);
    }
    int num=0;                                              //從樹建立編碼表
    Huff_list(codelist,num,"",trees[0]);
    qsort(codelist,inputlist.size(),sizeof(string),list_cmp);//排序編碼表
    return trees[0];
}

void swap_LR(NODE** left,NODE** right){                     //left 指向 原指向原左子樹節點 right 指向 原指向原右子樹節點
    NODE* temp;                                             //temp暫存節點指向
    temp=(*left);
    (*left)=(*right);
    (*right)=temp;
}

void Huff_sort(vector<NODE*>& trees){                       //把新樹插入trees中正確位置 trees存huffman樹及節點
    int pos;                                                //pos為判斷中trees的位置
    int t_freq=(*(trees[0])).freq;                          //t_freq暫存新樹的頻率
    unsigned char t_ASCI=(*(trees[0])).ASCI;                //t_ASCI維新樹頻率
    for(pos=1 ; pos<trees.size() ; pos++){
        if (t_freq < (*(trees[pos])).freq )
            break;
        else if (t_freq == (*(trees[pos])).freq){
            if(t_ASCI < (*(trees[pos])).ASCI)
                break;
        }
    }
    trees.insert(trees.begin()+pos , trees[0]);
    trees.erase(trees.begin());
}

int Huff_list(string* codelist,int& num,string code,NODE* root){//建立編碼表 codelist為編碼表 num為編碼組數 code編碼 root指向處理節點
    if (root){

        int i,j;                                                //往左右子樹編碼 i,j皆為0表節點末端
        i = Huff_list(codelist,num,code+"0",(*root).left) ;
        j = Huff_list(codelist,num,code+"1",(*root).right) ;
        if  (i==0 && j==0){
            string thecode;                                     //thecode 存字元編號及編碼 加入編碼表
            thecode+=(*root).ASCI;
            thecode+= code;
            codelist[num]=thecode;
            num++;
            return 1;
        }
    }
    else
        return 0;
    return 2;
}

int list_cmp (const void * a, const void * b){//排序編碼表用
  return (int)((*(string*)a)[0] - (*(string*) b)[0]);
}

int compression(const string* codelist,const int list_size,fstream& in,string& out_file){
            //壓縮 codelist編碼表 list_size編碼表大小 in輸入檔 out輸出檔
    string file_code;                                       //file_code 暫存輸入轉換編碼
    int bits=0;                                             //bits 存檔案編碼bit數
    char target_char=0;                                     //target_char 暫存輸入字元
    unsigned char out_char=0;                               //out_char 暫存輸出字元

    while(!in.eof() && in.get(target_char) ){
        for(int i=0 ; i<list_size ; i++){
            if (codelist[i][0] == target_char){
                for(int j=1 ; codelist[i][j]!=0 ; j++){
                    if(codelist[i][j]=='1')
                        file_code+='1';
                    else
                        file_code+='0';
                    bits++;
                }
                break;
            }
        }
    }
    for(int code_pos=0,i=0;code_pos<file_code.length();code_pos++,i++){//code_pos 目前處理file_code位置 i轉字元進程
        out_char += (int)(file_code[code_pos]-'0') * pow(2,7-i);
        if(i==7){                                                      //每8bit一字元 寫入out_file
            out_file += out_char;
            out_char=0;
            i=-1;
        }
        else if(code_pos==file_code.length()-1){                        //剩餘bit 寫入out_file
            out_file += out_char;
        }
    }
    return bits;
}

/*---------------------------------------壓縮--------------------------------------*/
void burn(NODE* root){                                  //root欲刪除指向節點
    if(root){
        burn((*root).left);
        burn((*root).right);
        delete root;
    }
}
/*---------------------------------------解壓縮--------------------------------------*/
void U(fstream& in,fstream& out){                                       //in為輸入檔 out輸出檔
    int bits;                                                           //bits存解壓編碼bit數
    char trash;                                                         //trash暫存讀入字元
    int target_ASCI=256;                                                //target_ASCI暫存讀入字元編號
    in >> bits;
    if(! in >> bits){
        cout << "error int"<<endl;
        return;
    }
    NODE* tree=new NODE;                                                //tree指向huffman樹
    NODE* temp_root=tree;                                               //temp_root指向欲處理節點
    while(!in.eof() && in.get(trash)){
        if (trash == '='){
            if(target_ASCI<256)
                build_tree_edge(temp_root,target_ASCI);
            temp_root=tree;
            in.get(trash);
            target_ASCI=(unsigned char)trash;
        }
        else if(trash=='1' || trash=='0' ){
            build_tree_path(&temp_root,trash);
        }
        else if (trash=='#'){
            if(target_ASCI<256)
                build_tree_edge(temp_root,target_ASCI);
            temp_root=tree;
            while(in.get(trash) && trash!='#')
                ;
            break;
        }
        else{
            cout << "error get"<< trash <<endl;
            return;
        }
    }
    Decompression(tree,in,out,bits);
    burn(tree);
}

void build_tree_path(NODE** r_root,char i){                             //r_root指向 指向欲修改節點 i判斷操作
    NODE** p_root;                                                      //p_root指向 指向新建節點
    NODE* root=(*r_root);                                               //root 指向欲修改節點
    if(i=='0'){
        p_root=&((*root).left);
        root=*p_root;
    }
    else if(i=='1'){
        p_root=&((*root).right);
        root=*p_root;
    }
    else{
        cout << "build error"<<endl;
        return;
    }
    (*r_root)=root;
    if(root==0){
        (*r_root)=new NODE;
        (*p_root)=(*r_root);
    }
}

void build_tree_edge(NODE* root,int i){                                 //root指向目標節點 i為目標字元編號
    if(i>=0 && i<256 )
        (*root).ASCI=i;
    else
        cout << "ASCI error" << i << endl;
}

void Decompression(NODE* tree,fstream& in,fstream& out,int bits){       //in為輸入檔 out為輸出檔 bits為輸入編碼bit數
    if(in && out && tree){
        char trash;                                                     //trash 暫存輸入字元
        string code;                                                    //code 暫存還原編碼

        while(!in.eof() && in.get(trash)){
            int codeing=(unsigned char)trash;                           //codeing 處理輸入用

            for(int i=0;i<8;i++){
                if(codeing>=pow(2,7-i)){
                    code+='1';
                    codeing-=pow(2,7-i);
                }
                else
                    code+='0';
            }
        }

        NODE* temp_root=tree;                                           //temp_root暫存目前找到節點位置
        int output;                                                     //output存樹中找到的字元編號
        for(int doing_bit=0 ; doing_bit < bits ; doing_bit++){
            output=char_search(&temp_root,code[doing_bit]);
            if(output>=0 && output <256){
                out << (unsigned char)output;
                temp_root=tree;
            }
        }
    }
}

int char_search(NODE** r_root,char code_bit){                           //r_root指向 指向欲找節點 code_bit判斷操作
    NODE* root=(*r_root);                                               //root 指向欲找節點

    if(root){
        if(code_bit=='0')
            root=(*root).left;
        else if(code_bit=='1')
            root=(*root).right;
        else
            return -1;

        (*r_root)=root;

        if(root==0){
            cout << "tree error"<<endl;
            return -1;
        }

        if((*root).left==0)
            if((*root).right==0)
                return (*root).ASCI;
    }
    else{
        cout << "root error"<<endl;
        return -1;
    }
    return 256;
}
/*---------------------------------------解壓縮--------------------------------------*/
