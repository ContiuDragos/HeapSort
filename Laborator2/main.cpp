#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"
#define NR_TESTS 5
#define MAX_SIZE 5000
#define STEP_SIZE 100

Profiler p("medie");

void printHeap(int v[], int n)
{
    for(int i=0;i<n;i++)
        printf("%d ",v[i]);
    printf("\n");
}

void swap(int *a, int *b)
{
    int aux=*a;
    *a=*b;
    *b=aux;
}

void heapify(int v[], int n, int i)
{
    Operation opAtrBottomUp=p.createOperation("Bottom-Up atr",n);
    Operation opCompBottomUp=p.createOperation("Bottom-Up comp",n);
    int largest=i;
    int left=2*i+1, right=2*i+2;

    if(left<n && v[left] > v[largest])
        largest=left;

    if(right<n && v[right] > v[largest])
        largest=right;
    opCompBottomUp.count(2);
    if(largest!=i) {
        swap(&v[i], &v[largest]);
        opAtrBottomUp.count(3);

        heapify(v,n,largest);
    }
}

void buildHeap_bottomUp(int v[], int n)
{
    int index=(n/2)-1;

    for(int i=index;i>=0;i--)
        heapify(v,n,i);
}
int parent(int n)
{
    if(n%2==0)
        return (n/2-1);
    return n/2;

}
void heapIncreaseKey(int heap[], int i, int key,int n)
{
    Operation opCompTopDown=p.createOperation("Top-Down comp",n);
    Operation opAtrTopDown=p.createOperation("Top-Down atr", n);
    heap[i]=key;
    opAtrTopDown.count();
    opCompTopDown.count();
    while(i>0 && heap[parent(i)]<heap[i])
    {
        opCompTopDown.count();
        swap(&heap[i],&heap[parent(i)]);
        opAtrTopDown.count(3);
        i=parent(i);
    }
}
void buildHeap_topDown(int v[], int heap[], int n, int heapSize)
{
    int aux=n;
    while(n>=0)
    {
        heapSize++;
        heap[heapSize]=INT_MIN;
        n--;
        heapIncreaseKey(heap,heapSize,v[n],aux);
    }
}
void heapSort(int v[], int n)
{
    int aux=n-1;
    for(int i=aux;i>=1;i--)
    {
        swap(&v[i],&v[0]);
        n--;
        buildHeap_bottomUp(v,n);
    }
}
void demo()
{
    int v[]={5,9,2,7,8,4,6,8,15,27,58,62,11};
    int n= sizeof(v)/ sizeof(v[0]), heapSize=-1;
    int *heap=(int*)calloc(n,sizeof(int));


    buildHeap_bottomUp(v,n);
    printHeap(v,n);
    heapSort(v,n);
    printHeap(v,n);
    /*
    buildHeap_topDown(v,heap,n,heapSize);
    printHeap(heap,n);
    */
}

void perf(int order)
{
    int v[MAX_SIZE],heap[MAX_SIZE];
    int n,heapSize=-1;
    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE)
    {
        for(int i=0;i<NR_TESTS;i++)
        {
            heapSize=-1;
            FillRandomArray(v,n,10,50000,false,order);
            buildHeap_bottomUp(v,n);
            FillRandomArray(v,n,10,50000,false,order);
            buildHeap_topDown(v,heap,n,heapSize);
        }
    }
    p.divideValues("Top-Down atr", NR_TESTS);
    p.divideValues("Top-Down comp", NR_TESTS);
    p.divideValues("Bottom-Up atr", NR_TESTS);
    p.divideValues("Bottom-Up comp", NR_TESTS);

    p.addSeries("Top-Down Total", "Top-Down comp", "Top-Down atr");
    p.addSeries("Bottom-Up Total", "Bottom-Up comp", "Bottom-Up atr");

    p.createGroup("Comp", "Top-Down comp", "Bottom-Up comp");
    p.createGroup("Atr", "Top-Down atr", "Bottom-Up atr");
    p.createGroup("Total", "Top-Down Total", "Bottom-Up Total");
}
int main() {

    demo();
    ///perf(0);
    ///p.showReport();
    return 0;
}
