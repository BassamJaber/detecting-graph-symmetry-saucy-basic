
///------------------------------------------///
///           Structures for OPP             ///
///------------------------------------------///
struct cellOPP
{
    unsigned long Size;
    int EmptyFlag;
};

typedef struct cellOPP CellOPP;

struct OrderedPartitionPair
{
  unsigned long * VertexArray ;
  unsigned long *Position ;
  unsigned long *CellFront;
  CellOPP * CellSize;
};
typedef struct OrderedPartitionPair *OPP;

typedef unsigned long Inducing;
typedef unsigned long Target;

///------------------------------------------///
///         Functions for The OPP            ///
///------------------------------------------///

void freeOPP(OPP);
OPP createEmptyOPP(unsigned long );
unsigned long getCellSize(OPP ,unsigned long);
void TestOPP(OPP,unsigned long);
void Trigger(OPP ,unsigned long );
unsigned long ColorRelativeVertexDegree(CGraph*  ,unsigned long ,unsigned long );
unsigned long getCellFront(CGraph*,OPP ,unsigned long,unsigned long );
OPP buildOPP(CGraph*,unsigned long,unsigned long);
OPP refineCELL(CGraph* ,unsigned long*,unsigned long,Queue,OPP,Target,Inducing);
OPP updateOP(CGraph*,OPP, unsigned long);
Queue findTargetCellList(CGraph*,OPP,unsigned long,unsigned long,Inducing);
OPP Refinement(CGraph*,unsigned long *,unsigned long , OPP );
int searchQueue(Queue, Target);
void RemoveTargetFromQlist(Queue,Target);
void removeRefinedFromTarget(Queue,Queue);
OPP CopyOPP(OPP, unsigned long);

///------------------------------------------///
///         Functions Implementaions         ///
///------------------------------------------///
OPP Refinement(CGraph*graph,unsigned long *numOfColor,unsigned long numOfVer, OPP P)
{
  Queue Q=createQueue();       //contain the Inducing Cells
  Queue list=createQueue();    //contain the Resulting Targeting cells after Refine Cell
  Queue TargetList=createQueue();   //contain the Target cells that connected to specific inducing cell
  Queue Refined=createQueue();  //contain the Cells that need No Refinment to remove them from Target List
  unsigned long i;
  unsigned long temp;

  //fill All cells of the Initial Partition Into the Queue
  for(i=0;i<numOfVer;i++)
     {
         if(P->CellSize[i].EmptyFlag==1)
           {
                 InQueue(Q,i);
           }
     }
      //printf("Initial Inducing Cells Queue\n");
      //printQueue(Q);

  Inducing I;
  Target T;
  unsigned long CSize;

  while(!isEmptyQueue(Q))
   {
      // printf("Start Of Loop\n");
       I=DeQueue(Q);
      // printf("Start Of the Refinement,Inducing Cell %ld\n",I);
       TargetList=findTargetCellList(graph,P,numOfVer,*numOfColor,I);

       if(!searchQueue(TargetList,I))
          {
              InQueue(TargetList,I);
          }
      // printf("Find Target List Related and Connected to this Inducing Cell \n");
      // printQueue(TargetList);


       //remove Refined Cells From Target Cells
          if(!isEmptyQueue(Refined)&& !isEmptyQueue(TargetList))
             {
                 removeRefinedFromTarget(Refined,TargetList);
                // printf("Target List after Removing Refined Cells\n");
                // printQueue(TargetList);
             }

       while(!isEmptyQueue(TargetList))
         {
          //  printf("Start Splitting Target Cells By Inducing Cell\n");
            T=DeQueue(TargetList);
          //  printf("Target Cell %ld , Inducing Cell %ld\n ",T,I);
            //This is the Error i was Sending Index not Vertex !
            CSize=getCellSize(P,P->VertexArray[T]);
            if(CSize>1)
            {
             P=refineCELL(graph,&*numOfColor,numOfVer,list,P,T,I);
             //Trigger(P,numOfVer);

             if(!isEmptyQueue(Q)&& searchQueue(Q,T))
                {
                   RemoveTargetFromQlist(Q,T);
                }
            }else{
              //     printf("No Refinment : Singleton Cell\n");
                   InQueue(Refined,T);   //singleton cell no refinement
                 }

            if(!isEmptyQueue(list)&&(list->numOfNodes>1))
                {
                  while(!isEmptyQueue(list))
                        InQueue(Q,DeQueue(list));
                }else if(!isEmptyQueue(list))
                       {
                    //    printf("No Refinement\n");
                        temp=DeQueue(list);  //noRefinement
                       }
               //printf("Queue Contains After the Splitting Target Cell\n");
              //  printQueue(Q);
         }
   }
   //printf("Partition Equitable\n");
return P;
}

//remove refined Cells from Target List to Refine ( make the Operation Terminate)
void removeRefinedFromTarget(Queue refined,Queue TargetList)
{
   PositionQ P;
   P=refined->next;
   while(P!=NULL)
     {
         if(searchQueue(TargetList,P->element))
             RemoveTargetFromQlist(TargetList,P->element);  //remove each element in the Refined Queue from Target Queue
         P=P->next;
     }
}

//Remove Target Cell from the Queue
void RemoveTargetFromQlist(Queue Q,Target T)
{
 PositionQ P,R;
 P=Q->next;   //heading
 R=Q->next;   //Previous

 int flag=1;  //to make Head 1 Step Forward

 while(P!=NULL)
   {
       if(P->element==T &&R!=P)
         {
             R->next=P->next;
             free(P);
             P=NULL;
         }else if(P->element==T &&R==P)
                {
                    Q->next=P->next;
                    free(P);
                    P=NULL;
                }
         if(P!=NULL)
           P=P->next;
         if(flag==0)
           {
             R=R->next;
           }
            flag=0;
   }
   //We Removed the Node from the List
   // now Modify the Info About the Queue to make it Stable S)
 Q->numOfNodes--;

 PositionQ L=Q->next;
  if(L==NULL)
    Q->lastNode=L;
   else{
       while(L->next!=NULL)
           {
              L=L->next;
           }
          Q->lastNode=L;
       }

}
//Serach the Queue For a specefic Cell
int searchQueue(Queue Q , Target T)
{
    if(Q==NULL)
      {
          printf("Segmentation Fault : Queue Does Not Exist\n");
          exit(0);
      }

    PositionQ P=Q->next;
    while(P!=NULL )
    {
        if(P->element==T)
           return 1;
        else
           P=P->next;
    }
    return 0;
}
//Make A list of Connected Cells to a Specefic Inducing cell
Queue findTargetCellList(CGraph*graph,OPP P,unsigned long numOfVer,unsigned long numOfColor,Inducing I)
{
  Queue TargetList=createQueue();
  //first find all Cells in the Partition
  // then reduce the number of Cells to only Cells that have connection
  unsigned long i,j;
  for(i=0;i<numOfVer;i++)
    {
        if(P->CellSize[i].EmptyFlag==1)
           InQueue(TargetList,i);
    }

   //the Target List Now Contains all the Cells of the Partition Inculding Inducing Cell
   //now eleminate Some Cells that have no Connection to I
   Queue Result=createQueue();// will hold the cells that have connection with I

   unsigned long index;
   unsigned long Iindex;
   unsigned long size;
   unsigned long Isize;
   int connection;
   unsigned long vertex;
   Position PO;

   Iindex=I;
   Isize=getCellSize(P,P->VertexArray[Iindex]);
   //we PoP cells from Target if there is a Connection Push it into Result
   //test if cells have connections to I or not
   while(!isEmptyQueue(TargetList))
     {
         index=DeQueue(TargetList);     //get the first Target cell to test
         vertex=P->VertexArray[index];
         size=getCellSize(P,P->VertexArray[index]);
         i=0,j=0;
         connection=0;    //flag indicate if there is a connection to break loops

         while(i<Isize && connection==0)
         {
           while(j<size &&connection==0 && i<Isize)
              {
                 PO=graph[P->VertexArray[Iindex+i]].H->next;  //indicate the Vertex of the first Index of Inducing CELL
                 vertex=P->VertexArray[index+j];
                 while(PO!=NULL && connection==0)
                    {
                        if(PO->vertex==vertex)     //if connection found
                           {
                               connection=1;           //end all loops
                               InQueue(Result,index);
                           }
                           PO=PO->next;
                    }
                    j++;
              }
              j=0;
              i++;
         }

     }

  return Result;

}
//Refine Cell
OPP refineCELL(CGraph*graph ,unsigned long *numOfColor,unsigned long numOfVer,Queue list, OPP P , Target T , Inducing I)
{
  unsigned long targetCellSize=getCellSize(P,P->VertexArray[T]);
  unsigned long *counter=(unsigned long*)malloc(sizeof(unsigned long)*targetCellSize);
  unsigned long InducingCELLColor=graph[P->VertexArray[I]].color;
  unsigned long i;
  unsigned long index=T;

  //find the Color Relative Degree for Each Vertex in Target Cell
  for(i=0;i<targetCellSize;i++)
     {
         counter[i]=ColorRelativeVertexDegree(graph,P->VertexArray[index],InducingCELLColor);
         index++;
     }

  // ok now i will divide the vertices in Target Partintion Depending on there Degrees
  //Update the Target Cell and OP for the new Cells , Put there front Index in a List
  //and return a Finer Partition and the List of Cell results from Target Cell
  //.. then we are Done

  //at first i made a new Array that will hold a new Sequence for the Vertices for the Target Cell
  //once i filled it i upgraded the OP

  //this Array will hold the new Partition before upgrading the OP
  unsigned long*finerPartition=(unsigned long*)malloc(sizeof(unsigned long)*targetCellSize);
  unsigned long j=0,k;
  unsigned long InducingCellSize=getCellSize(P,P->VertexArray[I]);
  unsigned long nextC=*numOfColor-1;
  int flag=0;  //test for Colors
  int change=0;//flag indicate that if we have new Cell or not to change Color
  int first=1;

  for(k=0;k<=InducingCellSize;k++)  // number of Repeatition
    {
     for(i=0;i<targetCellSize;i++)// search for Vertex with degree 0 ,1 ,2
      {

        if(counter[i]==k)
          {
              if(first==1)
                {
                  InQueue(list,T+j);
                  first=0;
                }
              finerPartition[j++]=P->VertexArray[T+i];  //give us the Vertex Number fro the Original Partition
              change=1;
              if(flag!=0)
                {
                    graph[P->VertexArray[T+i]].color=nextC;
                }
          }
      }
      first=1;
      if(j!=0 &&change==1)
      {
       flag=1;
       nextC++;
       change=0;
      }

    }
    //Update the Position Array with Finer Partition
    for(i=0;i<targetCellSize;i++)
       {
           P->VertexArray[T+i]=finerPartition[i];
       }

     P=updateOP(graph,P,numOfVer);
    *numOfColor=nextC;
     return P;
}

//update the OP Arrays after Spliting the Target cell
OPP updateOP(CGraph* graph,OPP P , unsigned long numOfVer )
{
    unsigned long i;
    //put in the index referenced by the Vertex number the Same index
    //Invers of Vertex Array
    for(i=0;i<numOfVer;i++)
       {
           P->Position[P->VertexArray[i]]=i;
       }
    //find the Cell Frony by starting from the index where the Vertex
    // in the vertex array ( and Go back until we find a Cell with Different Color)
    //then we return the next index ( The Front Index of the cell Containing that Vertex)
    for(i=0;i<numOfVer;i++)
    {
       P->CellFront[i]=getCellFront(graph,P,i,numOfVer);
    }
      //Set all Flags To Zero (means all Cell are Empty ) flag=0 represent '-'
   for(i=0;i<numOfVer;i++)
     P->CellSize[i].EmptyFlag=0;

 unsigned long begin=0,counter=0,size=numOfVer;
 unsigned long beginColor=graph[P->VertexArray[0]].color;
 i=1;
 while(i<size)
  {
    if((graph[P->VertexArray[i]].color==beginColor) && (i<size))
       {
           counter++;
           i++;
       }else {
              P->CellSize[begin].EmptyFlag=1;
              P->CellSize[begin].Size=counter;
              counter=0;
              begin=i;
              beginColor=graph[P->VertexArray[i]].color;
              i++;
             }
  }
  P->CellSize[begin].EmptyFlag=1;
  P->CellSize[begin].Size=counter;

return P;
}

//after changing some Vetrices Colors due to Cell RefineMent we Rebuilding the OPP for the New Graphs
OPP buildOPP(CGraph*graph,unsigned long numOfVer,unsigned long numOfColor)
{

    OPP P=createEmptyOPP(numOfVer);
    unsigned long i , j,k=0;

    //Fill the Vertex Array Depending On The Colors Start Searching
    //for the Vertices with Color 0 then 1 etc..
    //and  fill the indeces numbers in Vertex array
    for(j=0;j<numOfColor;j++)
       {for(i=0;i<numOfVer;i++)
          {
            if(graph[i].color==j)
             {
                P->VertexArray[k]=graph[i].VertexNumber;
                k++;
             }
          }
       }

    //put in the index referenced by the Vertex number the Same index
    //Invers of Vertex Array
    for(i=0;i<numOfVer;i++)
       {
           P->Position[P->VertexArray[i]]=i;
       }

    //find the Cell Frony by starting from the index where the Vertex
    // in the vertex array ( and Go back until we find a Cell with Different Color)
    //then we return the next index ( The Front Index of the cell Containing that Vertex)
    for(i=0;i<numOfVer;i++)
    {
       P->CellFront[i]=getCellFront(graph,P,i,numOfVer);
    }
      //Set all Flags To Zero (means all Cell are Empty ) flag=0 represent '-'
   for(i=0;i<numOfVer;i++)
     P->CellSize[i].EmptyFlag=0;

 unsigned long begin=0,counter=0,size=numOfVer;
 unsigned long beginColor=graph[P->VertexArray[0]].color;
 i=1;
 while(i<size)
  {
    if((graph[P->VertexArray[i]].color==beginColor) && (i<size))
       {
           counter++;
           i++;
       }else {
              P->CellSize[begin].EmptyFlag=1;
              P->CellSize[begin].Size=counter;
              counter=0;
              begin=i;
              beginColor=graph[P->VertexArray[i]].color;
              i++;
             }
  }
  P->CellSize[begin].EmptyFlag=1;
  P->CellSize[begin].Size=counter;

return P;
}

unsigned long getCellFront(CGraph*graph,OPP P,unsigned long vertex,unsigned long NumOfVer)
{
  unsigned long index=P->Position[vertex];  //the index of the Vertex in Vertex Array
  unsigned long c=graph[vertex].color;      //the Color of the vertex
  unsigned long v=vertex;
  unsigned long tempc=graph[v].color;
  while(index !=-1 && tempc==c)
     {
         index-=1;
         if( index!=-1 )
          {
            v=P->VertexArray[index];
            tempc=graph[v].color;
          }
     }
    index++;
 return index;
}

unsigned long ColorRelativeVertexDegree(CGraph* graph,unsigned long vertex,unsigned long CellColor)
{
  unsigned long  counter=0;
  Position P;
  P=graph[vertex].H->next;
  while(P!=NULL)
   {
      if(graph[P->vertex].color==CellColor)
         counter++;
       P=P->next;
   }
   return counter;
}

//print Out Vertex Array AND Size Array
void Trigger(OPP graphOPP,unsigned long size)
{
  unsigned long i;
  //printf("\nResult Partition\n");
  for(i=0;i<size;i++)
      if(graphOPP->CellSize[i].EmptyFlag)
         printf("%c %ld ",179,graphOPP->VertexArray[i]);
         else
             printf("%ld ",graphOPP->VertexArray[i]);
      printf("\n\n");
}
//Test the OPP and print out all the 4 tracing Arrays
void TestOPP(OPP graphOPP,unsigned long size)
{
  unsigned long i;
    printf("\n\nVertex Array \n\n");
  for(i=0;i<size;i++)
      printf("%ld |",i);
      printf("\n");
  for(i=0;i<size;i++)
      printf("%ld |",graphOPP->VertexArray[i]);

  printf("\n\nPosition Array\n\n");
  for(i=0;i<size;i++)
      printf("%ld |",i);
      printf("\n");
  for(i=0;i<size;i++)
      printf("%ld |",graphOPP->Position[i]);

  printf("\n\nTest Cell Front\n\n");
  for(i=0;i<size;i++)
      printf("%ld |",i);
      printf("\n");
  for(i=0;i<size;i++)
      printf("%ld |",graphOPP->CellFront[i]);

  //Test Cell Front
  printf("\n\nTest Cell  Size\n\n");
  for(i=0;i<size;i++)
      printf("%ld |",i);
      printf("\n");
  for(i=0;i<size;i++)
      if(graphOPP->CellSize[i].EmptyFlag)
         printf("%ld |",graphOPP->CellSize[i].Size);
         else
             printf("- |");

  printf("\n");
  for(i=0;i<size;i++)
         printf("Vertex %ld in a cell that have Size of %ld\n",i,getCellSize(graphOPP,i));

}
//calculate the Cell Size of a Partition
unsigned long getCellSize(OPP graph,unsigned long vertex)
{
    if(graph->CellSize[graph->CellFront[vertex]].EmptyFlag!=0)
        return(1+(graph->CellSize[graph->CellFront[vertex]].Size));
        else
        return -1;
}

//create and initialize and Empty OPP
OPP createEmptyOPP(unsigned long size)
{
 OPP graphOPP=(OPP)malloc(sizeof(struct OrderedPartitionPair));

 graphOPP->VertexArray=(unsigned long*)malloc(sizeof(unsigned long)*size);
 if(graphOPP->VertexArray==NULL)
   {
       printf("Segmentation Fault Error :No Memory\n");
       exit(0);
   }
 graphOPP->Position=(unsigned long*)malloc(sizeof(unsigned long)*size);
 if(graphOPP->Position==NULL)
    {
       printf("Segmentation Fault Error :No Memory\n");
       exit(0);
    }
 graphOPP->CellFront=(unsigned long*)malloc(sizeof(unsigned long)*size);
  if(graphOPP->CellFront==NULL)
    {
       printf("Segmentation Fault Error :No Memory\n");
       exit(0);
    }
 graphOPP->CellSize=(CellOPP*)malloc(sizeof(struct cellOPP)*size);
  if(graphOPP->CellSize==NULL)
    {
       printf("Segmentation Fault Error :No Memory\n");
       exit(0);
    }
 return graphOPP;
}
//make A Copy From OPP
OPP CopyOPP(OPP P , unsigned long numOfVer)
{
  OPP Copy=createEmptyOPP(numOfVer);
  unsigned long i;
  for(i=0;i<numOfVer;i++)
  {
      Copy->VertexArray[i]=P->VertexArray[i];
      Copy->Position[i]=P->Position[i];
      Copy->CellFront[i]=P->CellFront[i];
      Copy->CellSize[i].EmptyFlag=P->CellSize[i].EmptyFlag;
      Copy->CellSize[i].Size=P->CellSize[i].Size;
  }
  return Copy;
}
//free the OPP from the Memory
void freeOPP(OPP P)
{
    if(P==NULL)
      {
          printf("Error:Ordered Partition Does Not Exist");
      }else {
                free(P->VertexArray);
                free(P->Position);
                free(P->CellFront);
                free(P->CellSize);
                free(P);
            }
}
