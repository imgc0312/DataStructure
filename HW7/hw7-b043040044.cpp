#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
using namespace std;
//B043040044 GC 2016//
//作業

clock_t start;                               //start 暫存每次sort開始時間用
void Swap(int& A,int& B);
void Dcopy(int* output,int* input ,int Size);//複製input 於 output Size為陣列大小
void write(fstream& file,int* input,int Size);//將input 寫入file Size為陣列大小
void showTime(int Case,double time);          //在視窗上顯示內容

double selection_sort(int* input,int Size);//選擇排序法 對input陣列排序 Size為陣列大小
double heap_sort(int* input,int Size);//堆排序法 對input陣列排序 Size為陣列大小
void heap_adjust(int* input,const int root,const int Size);//堆排序用
double quick_sort(int* input,const int left,const int right);//選擇排序法 對input陣列排序 Size為陣列大小
int compare (const void * a, const void * b);//qsort用

int main(){
    int N=0;                                        //N為資料量 input檔中第一組數據
    cin >> N;
    int* data=new int[N+1];                         //data為欲排序資料陣列 將輸入存於1~N
    for(int i=1;i<=N;i++){
        cin >> data[i];
    }
    int* temp=new int[N+1];                         //temp複製data內容進行排序
    fstream file;                                   //file為輸出排序結果用檔案指標
    double time=0;                                  //time暫存 費時長 -1表超出時間限制5min/300000ms
    for (int i=0;i<5;i++){
        switch(i){
            case 0:                                 //i=0 selection sort
                file.open("OutputA.txt",ios::out);
                file << "selection sort" << endl;

                Dcopy(temp,data,N);
                start=clock();
                time=selection_sort(temp,N);

                cout << "資料量:" << N << endl;
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

                cout << "資料量:" << N << endl;
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

                cout << "資料量:" << N << endl;
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
                cout << "資料量:" << N << endl;
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
                cout << "資料量:" << N << endl;
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

void Swap(int& A,int& B){                           //交換A和B
    int Temp= A;
    A=B;
    B=Temp;
}
void Dcopy(int* output,int* input ,int Size){       //複製input陣列內容到output陣列 Size為陣列實際可儲存空間大小
    for(int i=0;i<=Size;i++){
        output[i]=input[i];
    }
}
void write(fstream& file,int* input,int Size){      //把排序完的input陣列的1~Size格內容寫入file
    for(int i=1;i<=Size;i++){
        file << input[i] << endl;
    }
}
void showTime(int Case,double time){                //視窗顯示結果用 time為費時(ms)
    switch(Case){
        case 0:
            cout << "方法: A: selection sort" << endl;
            break;
        case 1:
            cout << "方法: B: heap sort" << endl;
            break;
        case 2:
            cout << "方法: C: quick sort" << endl;
            break;
        case 3:
            cout << "方法: D: qsort( )" << endl;
            break;
        case 4:
            cout << "方法: E: (STL) sort( )" << endl;
            break;
    }
    if (time<0)
        cout << "費時: Time Limit Exceeds" << endl;
    else
        cout << "費時: " << time/1000 << endl;
}

double selection_sort(int* input,int Size){         //input為欲排序陣列 Size為資料量
    for(int i=1;i<=Size;i++){
        int j=i;                                    //i存已排序與未排序分界
        for(int k=i+1;k<=Size;k++){                 //j存待排序資料最小者位置
            if(input[k]<input[j])
                j=k;
        }
        Swap(input[i],input[j]);
        if((double)(clock()-start)>300000)
            return -1;
    }
    return (double)(clock()-start);
}

double heap_sort(int* input,int Size){              //input為欲排序陣列 Size為資料量
    for(int i=Size/2;i>=1;i--){                     //i存位置每次堆重整的起始位置
        heap_adjust(input,i,Size);
        if((double)(clock()-start)>300000)
            return -1;
    }

    for(int i=Size;i>=1;i--){                       //i存位置最小值位置(末端)
        Swap(input[1],input[i]);
        heap_adjust(input,1,i-1);
        if((double)(clock()-start)>300000)
            return -1;
    }

    return (double)(clock()-start);
}
void heap_adjust(int* input,const int root,const int Size){//input為欲排序陣列 root為堆重整起始位置 Size為資料量
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

double quick_sort(int* input,const int left,const int right){//input為欲排序陣列 left為資料群最左位置 right為資料群最右位置
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

int compare (const void * a, const void * b){//qsort用
  return ( *(int*)a - *(int*)b );
}
