#include "graph.h"
#include "Queue.h"
#include "OPP.h"
#include "Basic Search Tree.h"
#include "Symmetry.h"


int main(int argc, char *argv[])
{
   if(argc!=4)
     {
         printf("Input Error: main.c InputFile.txt outputFile.txt graphFile.viz\n");
         exit(0);
     }

    system("color f2");
    unsigned long numOfVer;
    unsigned long numOfColor;

    CGraph *graph=readFileFormat(&numOfColor,&numOfVer,argv);

    CGraph *graphB=makeACopy(graph,numOfVer);

    printf("\n\nCreate Initial OP for the Graph\n\n");
    OP initialOPP=(OP)malloc(sizeof(struct DOP));
    initialOPP->Top=buildOPP(graph,numOfVer,numOfColor);
    initialOPP->Bottom=buildOPP(graphB,numOfVer,numOfColor);

    initialOPP->numOfVer=numOfVer;
    initialOPP->numOfColor=numOfColor;

    initialOPP->graphTop=makeACopy(graph,numOfVer);
    initialOPP->graphBottom=makeACopy(graphB,numOfVer);
    // TestOPP(initialOPP,numOfVer);
    printf("\n\n");


    printf("Start Refinement of the OP\n\n\n");
    clock_t start = clock();
    initialOPP->Top=Refinement(graph,&numOfColor,numOfVer,initialOPP->Top);
    printf("Top\n");
    Trigger(initialOPP->Top,numOfVer);
    initialOPP->Bottom=Refinement(graphB,&numOfColor,numOfVer,initialOPP->Bottom);
    printf("Bottom\n");
    Trigger(initialOPP->Bottom,numOfVer);
    printf ( "\n\nRefinement Time :%f\n", ( (double)clock() - start ) / CLOCKS_PER_SEC );

    initialOPP->graphTop=makeACopy(graph,numOfVer);
    initialOPP->graphBottom=makeACopy(graphB,numOfVer);


    printf("Start finding Symmetries \n\n\n");
    clock_t startS = clock();

    ///Basic Search Tree
    Tree T=createTree(initialOPP);
    T=AutoCreate(T,initialOPP,initialOPP->graphTop,initialOPP->graphBottom);
    printf ( "\n\nSymmetries found in  Time :%f\n", ( (double)clock() - startS ) / CLOCKS_PER_SEC );
    printS();


    return 0;
}

//Create Automaticallay a Basic Search Tree and Find Symmetries
Tree AutoCreate(Tree T,OP P,CGraph* top_graph, CGraph* bottom_graph)
{

    bool swap_result;

    if(Stop(T->element))
      {

         swap_result = symmatryTest(P -> Top, P -> Bottom, P->graphTop, P -> graphBottom, P ->numOfVer );

          if(swap_result)   {

                sym_num++;
            printf(/*"Permutation Result"*/"\n\n\n");
            Trigger(P->Top,P->numOfVer);
            Trigger(P->Bottom,P->numOfVer);

          }

           // printf("\nsym_num = %ld\n", sym_num);

          return NULL;


      }else {
               unsigned long index=getFirstNonSingleTonIndex(P);
               T->numOfChildren=getNumOfChildren(P,index);
         //        printf("Index %ld , Children Number %d\n",index,T->numOfChildren);
               T->Children=(Child*)malloc(sizeof(struct treeNode)*T->numOfChildren);
               int i;
               OP PC=NULL;

               for(i=0;i<T->numOfChildren;i++)
                 {
                    free(PC);
                    PC=CopyOPS(P);
                   // printf("Maping %ld-->%ld\n",PC->Top->VertexArray[index],PC->Bottom->VertexArray[index+i]);
                    PC=MapingVertices(PC,PC->Top->VertexArray[index],PC->Bottom->VertexArray[index+i]);
                     //Trigger(PC->Top,PC->numOfVer);
                   // Trigger(PC->Bottom,PC->numOfVer);
                   //PC->Top=Refinement(PC->graphTop,&PC->numOfColor,PC->numOfVer,PC->Top);
                   //PC->Bottom=Refinement(PC->graphTop,&PC->numOfColor,PC->numOfVer,PC->Bottom);
                    T->Children[i]=createTreeNode(PC,P->Top->VertexArray[index],P->Bottom->VertexArray[index+i]);

                     AutoCreate(T->Children[i],PC, top_graph, bottom_graph);
                 }
                 return T;
            }
}

