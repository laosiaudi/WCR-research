/*********mZ***************************************************************
	> File Name: query.cpp
	> Author: Laosi
	> Mail: laosiaudi@gmail.com 
	> Created Time: 2013年08月19日 星期一 20时31分22秒
 ************************************************************************/

#include<iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

struct edge{
    int node1,node2;
    int label;
};

struct indexNode{
    int num;
    int label;
    //int node1;
    //int node2;
    int root;
    indexNode *left;
    indexNode *right;
    int component;
};

int father[5000000];
int cmp(const void *a,const void *b){
    struct edge *aa = (edge *)a;
    struct edge *bb = (edge *)b;
    return ((aa->label > bb->label)?1:-1);
}

int find(int v){    //find the root of a vertex
    int t;
    t = v;
    while(father[t] !=  t)
        t = father[t];

    return t;
}


bool isConnect(int v1,int v2){
    int x = find(v1);
    int y = find(v2);
    return x == y;
}


int depth[1000000];
int top = 0;
int cp;

indexNode node[10000000];
void dfs_(int c,int f,int dep,bool edge,indexNode nodeE[]){
   if(edge == false){
        node[c].component = cp;    
        depth[c] = dep;
        return;
    }



    for(int i = 0;i <= 1;i ++){
        if(!i){
             if ((nodeE[c].left)->label != 0)
                dfs_((nodeE[c].left)->num,c,dep+1,true,nodeE);
             else
                dfs_((nodeE[c].left)->num,c,dep+1,false,nodeE);
         }
         else{
            if ((nodeE[c].right)->label != 0)
                dfs_((nodeE[c].right)->num,c,dep+1,true,nodeE);
             else
                dfs_((nodeE[c].right)->num,c,dep+1,false,nodeE);
         }
         top ++;
    }
    
}

edge E[5000000];
edge mst[5000000];
indexNode nodeE[5000000];
int main(int argc, char* argv[])
{
    ifstream fin_;
    ifstream fin_2;
    ofstream fout_;
    ofstream fout_2;
    ofstream fout_3;
	fin_.open(argv[1],ios::in);
    fin_2.open(argv[2],ios::in);
    fout_.open(argv[3],ios::out);
    //fout_2.open("Result_hour_number_1.txt",ios::out);
    fout_3.open(argv[4],ios::out);

    int numOfNodes,numOfEdges;

    fin_ >> numOfNodes >> numOfEdges;


    int i = 0;
    while(i <= numOfEdges - 1){
        fin_ >> E[i].node1 >> E[i].node2 >> E[i].label;
        i ++;

    }
    qsort(E,numOfEdges,sizeof(E[0]),cmp);
//---------------------MST----------------------------

    for(int i = 0;i <= numOfNodes;i ++)
        father[i] = i;
    int index1 = 0,index2 = 0;
    int v1,v2;

    while(index1 < numOfEdges && index2 < numOfNodes - 1){
        if (index1 == 0){
            mst[index2].node1 = E[index1].node1;
            mst[index2].node2 = E[index1].node2;
            mst[index2].label = E[index1].label;
            v1 = find(E[index1].node1);
            v2 = find(E[index1].node2);
            if (v1 > v2)
                father[v1] = v2;
            else
                father[v2] = v1;
            index2 ++;

        }
        else{
            v1 = find(E[index1].node1);
            v2 = find(E[index1].node2);
            if (v1 != v2){
                if (v1 > v2)
                    father[v1] = v2;
                else
                    father[v2] = v1;
                mst[index2].node1 = E[index1].node1;
                mst[index2].node2 = E[index1].node2;
                mst[index2].label = E[index1].label;
                index2 ++;
                
            }
        }
        index1 ++;

    }

    cout << "Building mst finished" << endl;
    fout_3 << "The index size is :" << (index2 + numOfNodes)*sizeof(indexNode) << "bytes" << endl;
//------------------------------------------------------


//-----------to build edge index tree-------------------
   clock_t sindex = clock();
   for (int i = 1;i <= numOfNodes;i ++){
        node[i].label = 0;
        node[i].left = NULL;
        node[i].right = NULL;
        node[i].root = -1;
        node[i].num = i;
    }
    for (int i = 0;i < index2;i ++)
        nodeE[i].root = -1;

    for (int i = 0;i < index2;i ++){
        nodeE[i].label = mst[i].label;
        
        if (node[mst[i].node1].root == -1)
        {
            nodeE[i].left = &node[mst[i].node1];
            node[mst[i].node1].root = i;
        }
        else
        {
            int r = node[mst[i].node1].root;
            int h;
            while (r != -1){
                h = r;
                r = nodeE[r].root;
            }

            nodeE[i].left = &nodeE[h];
            nodeE[h].root = i;
        }

        if (node[mst[i].node2].root == -1)
        {
            nodeE[i].right = &node[mst[i].node2];
            node[mst[i].node2].root = i;
        }
        else
        {
            int r = node[mst[i].node2].root;
            int h;
            while (r != -1){
                h = r;
                r = nodeE[r].root;
            }
            nodeE[i].right = &nodeE[h];
            nodeE[h].root = i;
        }
        nodeE[i].num = i;
        nodeE[i].component = -1;
        
 
     }
     clock_t eindex = clock();
     double indexTime = (double)(eindex-sindex)/CLOCKS_PER_SEC;
     fout_3 << "The index construction time is :" << indexTime << endl;
    cout << "Building edge index tree finished" <<endl;
//------------------tree building succeed--------------------
   
//-----------------doing query-------------------------------
//pre-dealing with the tree (dfs)
   /*int no1,no2;
    dfs(index2-1,-1,0,true,nodeE);
    makeRmqIndex(top,depth);
    cout << nodeE[lca(6,2)].label;//rmq出问题，估计是边和节点的下标没有处理好*/

    //对上面的解决方法，先dfs一遍，记下每个node的深度，最后用O（n）的方法回溯求lca
    cp = 1;
    for (int i = 0;i < index2;i ++){
        if (nodeE[i].root == -1)
        {   
            dfs_(i,-1,0,true,nodeE);
            cp ++;
        }
    }
   cout << "Start querying........." << endl;
   int num;
   double QueryTime = 0;
   fin_2 >> num;
   int qn1;
   int qn2;
   int dis;

   for(int i = 1;i <= num;i ++){
    fin_2 >> qn1 >> qn2 >> dis;
   
    clock_t start = clock();
    if  (node[qn1].component != node[qn2].component || node[qn1].root == -1 || node[qn2].root == -1){
       //if (!isConnect(qn1,qn2)){
       fout_ << "No" << endl;
       clock_t end = clock();
       QueryTime += (double)(end - start)/CLOCKS_PER_SEC;
       //fout_2 << "-1" << endl;
    }
    else{
    int no1 =qn1,no2 = qn2;
    clock_t end;
    if (depth[no1] > depth[no2]){
        int diff = depth[no1] - depth[no2];
        indexNode *newNode  = &node[no1];
        int nNum = newNode->root;
        for (int i = 1;i < diff;i ++){
            nNum = nodeE[nNum].root;
        }

        int nNum2 = node[no2].root;
        nNum = nodeE[nNum].root;
        bool f = true;
        while(nNum2 != nNum){
            if (nodeE[nNum].label > dis || nodeE[nNum2].label > dis){
                f = false;
                break;
            }
            nNum2 = nodeE[nNum2].root;
            nNum = nodeE[nNum].root;
            
        }

        //fout_2 << nodeE[nNum].label << endl;
        if (nodeE[nNum].label <= dis && f){
            end = clock();
            fout_ << "Yes" << endl;
        }
        else{
            end = clock();
            fout_ << "No" << endl;
        }
    }
    else if (depth[no1] == depth[no2]){
        int nNum = node[no1].root;
        int nNum2 = node[no2].root;
        
        bool f = true;
        while(nNum2 != nNum){
            if (nodeE[nNum].label > dis || nodeE[nNum2].label > dis){
                f = false;
                break;
            }
            nNum2 = nodeE[nNum2].root;
            nNum = nodeE[nNum].root;
        }
       // fout_2 << nodeE[nNum].label << endl;
    
        if (nodeE[nNum].label <= dis && f){
            end = clock();
            fout_ << "Yes" << endl;
        }
        else{
             end = clock();
            fout_ << "No" << endl;
        }
    }
    else{

        int diff = depth[no2] - depth[no1];

        indexNode *newNode  = &node[no2];
        int nNum = newNode->root;
        for (int i = 1;i < diff;i ++){
            nNum = nodeE[nNum].root;
        }

        int nNum2 = node[no1].root;
        nNum = nodeE[nNum].root;
        bool f = true;
        while(nNum2 != nNum){

            if (nodeE[nNum].label > dis || nodeE[nNum2].label > dis){
                f = false;
                break;
            }
            nNum2 = nodeE[nNum2].root;
            nNum = nodeE[nNum].root;
        }

        //fout_2 << nodeE[nNum].label << endl;
        
        if (nodeE[nNum].label <= dis && f){
            end = clock();
            fout_ << "Yes" << endl;
        }
        else{
            end = clock();
            fout_ << "No" << endl;
        }
    }
   QueryTime += (double)(end - start)/CLOCKS_PER_SEC;   

}
}
  
  
   cout << "Query done! Please check all the files containing the result!" << endl;
   cout << "Time :" << QueryTime << "s" <<endl;
   fout_3 << "All the processing query time is :" << QueryTime <<"s" << endl;

   return 0;

}
