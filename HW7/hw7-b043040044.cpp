#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;
//B043040044 ��T�u�{108�� �d�T�� 2016//
//�@�~

clock_t start;                               //start �Ȧs�C��sort�}�l�ɶ���
void Swap(int& A,int& B);
void Dcopy(int* output,int* input ,int Size);//�ƻsinput �� output Size���}�C�j�p
void write(fstream& file,int* input,int Size);//�Ninput �g�Jfile Size���}�C�j�p
void showTime(int Case,double time);          //�b�����W��ܤ��e

double selection_sort(int* input,int Size);//��ܱƧǪk ��input�}�C�Ƨ� Size���}�C�j�p
double heap_sort(int* input,int Size);//��ƧǪk ��input�}�C�Ƨ� Size���}�C�j�p
void heap_adjust(int* input,const int root,const int Size);//��Ƨǥ�
double quick_sort(int* input,const int left,const int right);//��ܱƧǪk ��input�}�C�Ƨ� Size���}�C�j�p
int compare (const void * a, const void * b);//qsort��

int main(){
    int N=0;                                        //N����ƶq input�ɤ��Ĥ@�ռƾ�
    cin >> N;
    int* data=new int[N+1];                         //data�����ƧǸ�ư}�C �N��J�s��1~N
    for(int i=1;i<=N;i++){
        cin >> data[i];
    }
    int* temp=new int[N+1];                         //temp�ƻsdata���e�i��Ƨ�
    fstream file;                                   //file����X�Ƨǵ��G���ɮ׫���
    double time=0;                                  //time�Ȧs �O�ɪ� -1��W�X�ɶ�����5min/300000ms
    for (int i=0;i<5;i++){
        switch(i){
            case 0:                                 //i=0 selection sort
                file.open("OutputA.txt",ios::out);
                file << "selection sort" << endl;

                Dcopy(temp,data,N);
                start=clock();
                time=selection_sort(temp,N);

                cout << "��ƶq:" << N << endl;
                showTime(i,time);
                write(file,temp,N);
                file.close();
                break;
            case 1:                                 //i=1 heap sort
                file.open("OutputB.txt",ios::out);
                file << "heap sort" << endl;

                Dcopy(temp,data,N);
                start=clock();
                time=heap_sort(temp,N);

                cout << "��ƶq:" << N << endl;
                showTime(i,time);
                write(file,temp,N);
                file.close();
                break;
            case 2:                                 //i=2 quick sort
                file.open("OutputC.txt",ios::out);
                file << "quick sort" << endl;

                Dcopy(temp,data,N);
                start=clock();
                time=quick_sort(temp,1,N);

                cout << "��ƶq:" << N << endl;
                showTime(i,time);
                write(file,temp,N);
                file.close();
                break;
            case 3:                                 //i=3 qsort
                file.open("OutputD.txt",ios::out);
                file << "C qsort()" << endl;

                Dcopy(temp,data,N);
                start=clock();
                qsort(&(temp[1]),N,sizeof(int),compare);
                time=(double)(clock()-start);
                cout << "��ƶq:" << N << endl;
                showTime(i,time);
                write(file,temp,N);
                file.close();
                break;
            case 4:                                 //i=4 c++sort
                file.open("OutputE.txt",ios::out);
                file << "C++ sort()" << endl;

                Dcopy(temp,data,N);
                start=clock();
                sort(&(temp[1]),&(temp[N+1]));
                time=(double)(clock()-start);
                cout << "��ƶq:" << N << endl;
                showTime(i,time);
                write(file,temp,N);
                file.close();
                break;
        }
    }
    delete[] data;
    delete[] temp;
    return 0;
}

void Swap(int& A,int& B){                           //�洫A�MB
    int Temp= A;
    A=B;
    B=Temp;
}
void Dcopy(int* output,int* input ,int Size){       //�ƻsinput�}�C���e��output�}�C Size���}�C��ڥi�x�s�Ŷ��j�p
    for(int i=0;i<=Size;i++){
        output[i]=input[i];
    }
}
void write(fstream& file,int* input,int Size){      //��Ƨǧ���input�}�C��1~Size�椺�e�g�Jfile
    for(int i=1;i<=Size;i++){
        file << input[i] << endl;
    }
}
void showTime(int Case,double time){                //������ܵ��G�� time���O��(ms)
    switch(Case){
        case 0:
            cout << "��k: A: selection sort" << endl;
            break;
        case 1:
            cout << "��k: B: heap sort" << endl;
            break;
        case 2:
            cout << "��k: C: quick sort" << endl;
            break;
        case 3:
            cout << "��k: D: qsort( )" << endl;
            break;
        case 4:
            cout << "��k: E: (STL) sort( )" << endl;
            break;
    }
    if (time<0)
        cout << "�O��: Time Limit Exceeds" << endl;
    else
        cout << "�O��: " << time/1000 << endl;
}

double selection_sort(int* input,int Size){         //input�����Ƨǰ}�C Size����ƶq
    for(int i=1;i<=Size;i++){
        int j=i;                                    //i�s�w�ƧǻP���ƧǤ���
        for(int k=i+1;k<=Size;k++){                 //j�s�ݱƧǸ�Ƴ̤p�̦�m
            if(input[k]<input[j])
                j=k;
        }
        Swap(input[i],input[j]);
        if((double)(clock()-start)>300000)
            return -1;
    }
    return (double)(clock()-start);
}

double heap_sort(int* input,int Size){              //input�����Ƨǰ}�C Size����ƶq
    for(int i=Size/2;i>=1;i--){                     //i�s��m�C���ﭫ�㪺�_�l��m
        heap_adjust(input,i,Size);
        if((double)(clock()-start)>300000)
            return -1;
    }

    for(int i=Size;i>=1;i--){                       //i�s��m�̤p�Ȧ�m(����)
        Swap(input[1],input[i]);
        heap_adjust(input,1,i-1);
        if((double)(clock()-start)>300000)
            return -1;
    }

    return (double)(clock()-start);
}
void heap_adjust(int* input,const int root,const int Size){//input�����Ƨǰ}�C root���ﭫ��_�l��m Size����ƶq
    int Temp=input[root];
    int i;
    for(i=2*root;i<=Size;i*=2){
        if(i<Size && input[i]<input[i+1])
            i++;
        if(Temp >= input[i])
            break;
        input[i/2]=input[i];
    }
    input[i/2]=Temp;
}

double quick_sort(int* input,const int left,const int right){//input�����Ƨǰ}�C left����Ƹs�̥���m right����Ƹs�̥k��m
    if(left < right){
        int i= left;
        int j= right +1;
        int pivot=input[left];
        do{
            do i++;
                while(input[i] < pivot);
            do j--;
                while(input[j] > pivot);
            if(i<j)
                Swap(input[i],input[j]);
        }while(i<j);
        Swap(input[left],input[j]);

        quick_sort(input,left,j-1);
        quick_sort(input,j+1,right);

        if((double)(clock()-start)>300000)
            return -1;
    }
    return (double)(clock()-start);
}

int compare (const void * a, const void * b){//qsort��
  return ( *(int*)a - *(int*)b );
}
