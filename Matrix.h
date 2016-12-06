//matrix.h
//class matrix

#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <ctime>
//#include "etree.h"
using namespace std;

class matrix{
public:
	/*
	VAL   : value of non-zero element
	I     : row-index of non-zero element          -----COO format
	J     : colomn-index of non-zero element
	*/
	matrix(string name);                     //read input store in COO format
	void convertfullCSC();           //convert COO to CSR format
	int getrow(){ return m; }
	void converthalfCSC();
	void generateetree();
	void lfactorstructure();
	void cholesky();
	template <typename T> void output(const vector <T> &array);
	~matrix();

	
	struct AdjListNode{int dest;struct AdjListNode* next;};
	struct AdjList{struct AdjListNode *head;};
	struct Etree{int V;struct AdjList *array;};
	
	struct AdjListNode* newAdjListNode(int dest)
	{
    	struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    	newNode->dest=dest;
    	newNode->next = NULL;
    	return newNode;
	}

	struct Etree* createEtree(int V)
	{
    	struct Etree* etree = (struct Etree*) malloc(sizeof(struct Etree));
    	etree->V = V;
    	etree->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    	for (int i = 0; i < V; ++i)
        	etree->array[i].head = NULL;
    	return etree;
	}
 

void addEdge(struct Etree* etree, int src, int dest)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = etree->array[src].head;
    etree->array[src].head = newNode;
}

void checkfillin(struct Etree* etree)
{
	vector <int>::const_iterator c1;
	vector <int>::const_iterator c2;
	int duplicate=0;
	for(int i=0;i<etree->V;i++)
	{
		struct AdjListNode* n1 = etree->array[i].head;
		while(n1&&n1->next)
		{
			for(c1=xx.begin(),c2=yy.begin();c1!=xx.end()&&c2!=yy.end();++c1,++c2)
			{
				if(*c1==n1->dest&&*c2==n1->next->dest)
				{	duplicate=1;break;}
			}
			if(duplicate==0)
			{
				xx.push_back(n1->dest);
				yy.push_back(n1->next->dest);
			}
			n1=n1->next;
			duplicate=0;
		}
	}
	output(xx);cout<<xx.size()<<endl;
	output(yy);cout<<yy.size()<<endl;
	cout<<"checkfillin step one FINISH"<<endl;
	duplicate=0;
	for(int i=0;i<etree->V;i++)
	{
		struct AdjListNode* n1 = etree->array[i].head;
		struct AdjListNode* temp = n1;
		while(n1&&n1->next)
		{
			for(c1=xx.begin(),c2=yy.begin();c1!=xx.end()&&c2!=yy.end();++c1,++c2)
			{
				if(*c1==temp->dest&&*c2==n1->next->dest)
				{	duplicate=1;break;}
			}

			if(duplicate==0)
			{
				xx.push_back(temp->dest);
				yy.push_back(n1->next->dest);
			}
			n1=n1->next;
			duplicate=0;
		}
	}
	cout<<"checkfillin step two FINISH"<<endl;
	output(xx);cout<<xx.size()<<endl;
	output(yy);cout<<yy.size()<<endl;
}

void printEtree(struct Etree* etree)
{
    int v;
    for (v = 0; v < etree->V; ++v)
    {
        struct AdjListNode* pCrawl = etree->array[v].head;
        printf("\n Adjacency list of vertex %d\n ", v);
        while (pCrawl)
        {
            printf("%d ->", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("%d",v);
    }
    cout<<endl;
}

private:                   //input bcsstk27 is pattern-only sparse matrix (symmetric structure store entries only on or below the main diagonal)
	int m, n, hnz, nz;     // m: row  n:colomn  hnz: non-zero element in input data   nz: total non-zero element in matrix
	int *I, *IA;              //IA: row-index of non-zero element after converting to CSR
	int *J, *JA;
	double *val, *VAL,*result;
	int *ia;
	int count1;          //number of non-zero on the main diagonal
	int *counter;
	int *col_ptr;
	vector <int> xx;
	vector <int> yy;
};

#endif