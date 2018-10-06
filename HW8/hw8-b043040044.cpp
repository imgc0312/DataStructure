#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;
// �d�T�� B043040044
// 2017/01/07
// �@�~8  huffman
struct ASCIfreq{                                    //ASCIfreq �s�S�w�r���W�v
    unsigned char ASCI;                             //ASCIfreq.ASCI �r���s��
    int freq;                                       //ASCIfreq.freq �r���W�v
};

class NODE{                                         //NODE �Ω��huffman tree
public:
    NODE ();
    NODE(ASCIfreq t);
    void adjust();                                  //�p��Ӹ`�I�W�v

    unsigned char ASCI;                             //NODE.ASCI�Ӹ`�I�r���s��
    int freq;                                       //NODE.freq �Ӹ`�I�W�v
    int data_freq;                                  //NODE.data_freq �Ӹ`�I�r���W�v
    NODE* right;                                    //NODE.right ���V�k�l��`�I
    NODE* left;                                     //NODE.left ���V���l��`�I
};

void C(fstream& in,fstream& out);                   //C() ���Y��
void list_freq(vector<ASCIfreq>& inlist,ASCIfreq input);        //list_freq()�C�X�W�v��ñƧ�
NODE* Huffman(string* codelist,vector<ASCIfreq>& inputlist);    //Huffman() �s�@huffman��
void swap_LR(NODE** left,NODE** right);                         //swap_LR() �l�𥪥k�ﱼ
void Huff_sort(vector<NODE*>& trees);                           //Huffman() �s�@huffman�� �`�I�Ƨ�
int Huff_list(string* codelist,int& num,string code,NODE* root);//Huff_list() ��X�s�X��
int list_cmp (const void * a, const void * b);                  //list_cmp() �Ƨǽs�X���
int compression(const string* codelist,const int list_size,fstream& in,string& out_file);//compression() ���Y�ɮפ��e


void burn(NODE*);                                               //burn()�N��M��

void U(fstream& in,fstream& out);                   //U() �����Y��
void build_tree_path(NODE** r_root,char i);                     //build_tree_path() �إ�huffman��
void build_tree_edge(NODE* root,int i);                         //build_tree_path() �إ�huffman��
void Decompression(NODE* tree,fstream& in,fstream& out,int bits);//Decompression() �����Y�ɮפ��e
int char_search(NODE** r_root,char code_bit);                   //char_search() �q�𤤿�X�r��

int main(int argc,char* argv[]){
    if (argc<5){
        cout << "too few argument" << endl;
        return 0;
    }
    double start = clock();
    fstream fin;                                                //fin �ɮ׿�J
    fstream fout;                                               //fout ��X�ɮ�

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

    if (strcmp(argv[1],"-u")==0)                               //����
        U(fin,fout);
    else if(strcmp(argv[1],"-c")==0)                           //���Y
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
/*---------------------------------------���Y--------------------------------------*/
void C(fstream& in,fstream& out){                           //in����J��, out��X��
    int in_byte=0,out_byte=0;                               //in_byte,out_byte�p���J��X�ɮת��j�p
    int frequence[256];                                     //frequence�s�U�r���X�{�W�v
    vector<ASCIfreq> ASCIlist;                              //ASCIlist �s�r���W�v��

    for(int i=0;i<256;i++)
        frequence[i]=0;
    char tt;                                                //tt ��J�μȦs�r��
    unsigned char target_char;                              //target_char �ؼЦr��
    while(!in.eof() && in.get(tt)){         //���v�έp
        target_char=(unsigned char)tt;
        in_byte++;
        frequence[target_char]++;
    }

    for (int i=0;i<256;i++){                //�W�v�Ƨ�
        if(frequence[i]){
            ASCIfreq t;
            t.ASCI=i;
            t.freq=frequence[i];
            list_freq(ASCIlist,t);
        }
    }
    string codelist[ASCIlist.size()];                       //codelist �s�s�X��
    NODE* Huffman_tree;                                     //Huffman_tree ���Vhuffman��
    Huffman_tree=Huffman(codelist,ASCIlist);             //�إ߽s�X���codelist

    in.clear();                                          //�M�z�ɮ׿�J��T
    in.seekg(0,ios::beg);                                //�^��J���ɮ׶}�Y

    string out_file;                                        //out_file �s�����Y�������e
    int bits;                                               //bits �s��l�s�Xbit��
    bits = compression(codelist,ASCIlist.size(),in,out_file);

    out << bits;

    for(int i=0;i<ASCIlist.size();i++){     //�s�X��g�J�}�Y
        out<<('=');
        out<<((unsigned char)codelist[i][0]);
        out_byte+=2;
        for(int j=1;codelist[i][j]!=0;j++){
            out << ((unsigned char)codelist[i][j]);
            out_byte++;
        }
    }
    out_byte+=out_file.length();
    double rate_compression = (double)out_byte/(double)in_byte;//rate_compression �p�����Y�v
    cout << "���Y�e:" << in_byte << " bytes" << endl;
    cout << "���Y��:" << out_byte << " bytes" << endl;
    cout << "���Y�v:" << rate_compression << endl;
    out <<'#' << in_byte << '/' << out_byte << '/' <<rate_compression << '#';

    for(int i=0;i<out_file.length();i++)
        out << ((unsigned char)out_file[i]);

    burn(Huffman_tree);
}

void list_freq(vector<ASCIfreq>& inlist ,ASCIfreq input){//�s�@�W�v�� inlist�W�v�� ,input �s�W�r���W�v
    int i;
    for(i=0;i<inlist.size();i++){
        if ((inlist.at(i)).freq > input.freq){
            break;
        }
    }
    inlist.insert(inlist.begin()+i,input);
}

NODE* Huffman(string* codelist,vector<ASCIfreq>& inputlist){//codelist���s�X�� inputlist �W�v��
    vector<NODE*> trees;                                    //trees �s��huffman��θ`�I
    for(int i=0;i<inputlist.size();i++){
        NODE * tree=new NODE (inputlist.at(i));
        trees.push_back(tree);
    }
    if (trees.size()==1){                               //���J�ɮ׶Ȧ��@�ئr��
        NODE* ntree=new NODE;
        (*ntree).left=trees.at(0);
        trees.at(0)=(*ntree).left;
    }
    while(trees.size()>1){                              //����X�ָ`�I �ܶȳѤ@��
        NODE* ntree=new NODE;                               //ntree���V�X�֥X���s��
        unsigned char ASCI_1=(*(trees.at(0))).ASCI;         //ASCI_1�Ȧs���l��r���s��
        unsigned char ASCI_2=(*(trees.at(1))).ASCI;         //ASCI_2�Ȧs�k�l��r���s��
        (*ntree).left=trees.at(0);
        (*ntree).right=trees.at(1);

        if(ASCI_1 < ASCI_2){                                //�s�`�I�r���s�� ��ASCI_1,ASCI_2�����p��
            (*ntree).ASCI=ASCI_1;
        }
        else{
            (*ntree).ASCI=ASCI_2;
            swap_LR(& ((*ntree).left),& ((*ntree).right));  //�`�I�s�����p�̬����l��
        }

        (*ntree).adjust();

        trees.erase(trees.begin());
        trees.at(0)=ntree;                                  //�s���\�btrees���̫e

        Huff_sort(trees);
    }
    int num=0;                                              //�q��إ߽s�X��
    Huff_list(codelist,num,"",trees[0]);
    qsort(codelist,inputlist.size(),sizeof(string),list_cmp);//�Ƨǽs�X��
    return trees[0];
}

void swap_LR(NODE** left,NODE** right){                     //left ���V ����V�쥪�l��`�I right ���V ����V��k�l��`�I
    NODE* temp;                                             //temp�Ȧs�`�I���V
    temp=(*left);
    (*left)=(*right);
    (*right)=temp;
}

void Huff_sort(vector<NODE*>& trees){                       //��s�𴡤Jtrees�����T��m trees�shuffman��θ`�I
    int pos;                                                //pos���P�_��trees����m
    int t_freq=(*(trees[0])).freq;                          //t_freq�Ȧs�s���W�v
    unsigned char t_ASCI=(*(trees[0])).ASCI;                //t_ASCI���s���W�v
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

int Huff_list(string* codelist,int& num,string code,NODE* root){//�إ߽s�X�� codelist���s�X�� num���s�X�ռ� code�s�X root���V�B�z�`�I
    if (root){

        int i,j;                                                //�����k�l��s�X i,j�Ҭ�0��`�I����
        i = Huff_list(codelist,num,code+"0",(*root).left) ;
        j = Huff_list(codelist,num,code+"1",(*root).right) ;
        if  (i==0 && j==0){
            string thecode;                                     //thecode �s�r���s���νs�X �[�J�s�X��
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

int list_cmp (const void * a, const void * b){//�Ƨǽs�X���
  return (int)((*(string*)a)[0] - (*(string*) b)[0]);
}

int compression(const string* codelist,const int list_size,fstream& in,string& out_file){
            //���Y codelist�s�X�� list_size�s�X��j�p in��J�� out��X��
    string file_code;                                       //file_code �Ȧs��J�ഫ�s�X
    int bits=0;                                             //bits �s�ɮ׽s�Xbit��
    char target_char=0;                                     //target_char �Ȧs��J�r��
    unsigned char out_char=0;                               //out_char �Ȧs��X�r��

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
    for(int code_pos=0,i=0;code_pos<file_code.length();code_pos++,i++){//code_pos �ثe�B�zfile_code��m i��r���i�{
        out_char += (int)(file_code[code_pos]-'0') * pow(2,7-i);
        if(i==7){                                                      //�C8bit�@�r�� �g�Jout_file
            out_file += out_char;
            out_char=0;
            i=-1;
        }
        else if(code_pos==file_code.length()-1){                        //�Ѿlbit �g�Jout_file
            out_file += out_char;
        }
    }
    return bits;
}

/*---------------------------------------���Y--------------------------------------*/
void burn(NODE* root){                                  //root���R�����V�`�I
    if(root){
        burn((*root).left);
        burn((*root).right);
        delete root;
    }
}
/*---------------------------------------�����Y--------------------------------------*/
void U(fstream& in,fstream& out){                                       //in����J�� out��X��
    int bits;                                                           //bits�s�����s�Xbit��
    char trash;                                                         //trash�ȦsŪ�J�r��
    int target_ASCI=256;                                                //target_ASCI�ȦsŪ�J�r���s��
    in >> bits;
    if(! in >> bits){
        cout << "error int"<<endl;
        return;
    }
    NODE* tree=new NODE;                                                //tree���Vhuffman��
    NODE* temp_root=tree;                                               //temp_root���V���B�z�`�I
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

void build_tree_path(NODE** r_root,char i){                             //r_root���V ���V���ק�`�I i�P�_�ާ@
    NODE** p_root;                                                      //p_root���V ���V�s�ظ`�I
    NODE* root=(*r_root);                                               //root ���V���ק�`�I
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

void build_tree_edge(NODE* root,int i){                                 //root���V�ؼи`�I i���ؼЦr���s��
    if(i>=0 && i<256 )
        (*root).ASCI=i;
    else
        cout << "ASCI error" << i << endl;
}

void Decompression(NODE* tree,fstream& in,fstream& out,int bits){       //in����J�� out����X�� bits����J�s�Xbit��
    if(in && out && tree){
        char trash;                                                     //trash �Ȧs��J�r��
        string code;                                                    //code �Ȧs�٭�s�X

        while(!in.eof() && in.get(trash)){
            int codeing=(unsigned char)trash;                           //codeing �B�z��J��

            for(int i=0;i<8;i++){
                if(codeing>=pow(2,7-i)){
                    code+='1';
                    codeing-=pow(2,7-i);
                }
                else
                    code+='0';
            }
        }

        NODE* temp_root=tree;                                           //temp_root�Ȧs�ثe���`�I��m
        int output;                                                     //output�s�𤤧�쪺�r���s��
        for(int doing_bit=0 ; doing_bit < bits ; doing_bit++){
            output=char_search(&temp_root,code[doing_bit]);
            if(output>=0 && output <256){
                out << (unsigned char)output;
                temp_root=tree;
            }
        }
    }
}

int char_search(NODE** r_root,char code_bit){                           //r_root���V ���V����`�I code_bit�P�_�ާ@
    NODE* root=(*r_root);                                               //root ���V����`�I

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
/*---------------------------------------�����Y--------------------------------------*/
