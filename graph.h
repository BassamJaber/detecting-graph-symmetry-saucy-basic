#include <stdio.h>
#include <stdlib.h>
#include <time.h>

///------------------------------------------///
///        Structures for The graph          ///
///------------------------------------------///

typedef struct node *ptr;
struct node
{
    unsigned long vertex;
    ptr next;
};
typedef ptr Position;
typedef ptr Head;

struct vertex
{
  unsigned long color ;
  unsigned long VertexNumber;
  Head H;
};
typedef struct vertex CGraph;
///------------------------------------------///
///         Functions for The graph          ///
///------------------------------------------///

FILE* openFileRead(char *);
FILE* openFileWrite(char *);
Head creatList();
Position createNode(unsigned long);
void insert(Head, Position);
void printGraph(CGraph [], unsigned long );
void initializeGraph(CGraph [],unsigned long);
void makeGraphivFile(CGraph[],unsigned long,char*);
void printStatistics(FILE*,CGraph [], unsigned long,unsigned long);
CGraph* readFileFormat(unsigned long*, unsigned long*,char*[]);
CGraph* makeCopy(CGraph*, unsigned long);

///------------------------------------------///
///         Functions Implementation         ///
///------------------------------------------///

//make a Copy from a Graph for Special Use
CGraph* makeACopy(CGraph* graph, unsigned long numOfVer)
{
    CGraph *graph2;
    graph2=(CGraph*)malloc(sizeof(struct vertex)*numOfVer);
    initializeGraph(graph2,numOfVer);

    unsigned long i;
    Position P,PC;

    for(i=0;i<numOfVer;i++)
    {
       graph2[i].color=graph[i].color;
       P=graph[i].H->next;
       while(P!=NULL)
       {
        PC=createNode(P->vertex);
        insert(graph2[i].H,PC);
        P=P->next;
       }
    }
    return graph2;
}

CGraph* readFileFormat(unsigned long *numColor, unsigned long *Size,char*argv[])
{
    clock_t start = clock();  ///Start Measuring the File Reading Time

    FILE *input;
    input=openFileRead(argv[1]);          //Open the File and Test if Exist or not !

    unsigned long  numOfVer,numOfEdge,numOfColor;     //Local Variables to store # of (Vertex , Edge, Color)
    fscanf(input,"%ld",&numOfVer);
  if(numOfVer==0)
    {
        printf("The Graph Have No Vertices ! please Check the Input File \n");
        exit(0);
    }
    fscanf(input,"%ld",&numOfEdge);
    if(numOfEdge==0)
    {
        printf("The Graph Have No Edges ! please Check the Input File \n");
        exit(0);
    }
    fscanf(input,"%ld",&numOfColor);
  if(numOfColor==0)
    {
        printf("The Graph Have No Edges ! please Check the Input File \n");
        exit(0);
    }

    CGraph *graph;
    graph=(CGraph*)malloc(sizeof(struct vertex)*numOfVer);
    initializeGraph(graph,numOfVer);

    unsigned long* colorRange;
    if(numOfColor>1)
      colorRange=(unsigned long*)malloc(sizeof(unsigned long)*(numOfColor-1));     //store the ranges of Vertex colors
    else
       {
           colorRange=(unsigned long*)malloc(sizeof(unsigned long));
           colorRange[0]=-1;
       }
    unsigned long i;

    //scan the index of the color from the file and save them in an array
    for(i=0;i<numOfColor-1;i++)
       {
           fscanf(input,"%ld",&colorRange[i]);
       }

    int j=0 , k=0;                           //k is color counter (0 first color 1 second etc..)
                                             //j is Conter for index for ColorRange Array
    for(i=0;i<numOfVer;i++)
    {
        if(i<colorRange[j] && j<numOfColor-1)
          {
              graph[i].color=k;
          }else if(j<numOfColor-1)
                {
                  k++;                       //if reach to the range increase the color
                  j++;                       // and go to the next range
                  graph[i].color=k;
                }else {                       //the last elements after the last range given the last color
                        graph[i].color=k;
                      }
    }

  ///Start Reading The Edges Between Vertices

    int sourceV,destV;                       //source Vertex and destination Vertix
    Position P;                              //Temporary Variable to Store Vertex

    fscanf(input,"%d",&sourceV);
    while(!feof(input))
    {
        fscanf(input,"%d",&destV);

        P=createNode(destV);
        insert(graph[sourceV].H,P);    //insert the Edge for the Source Vertex

        P=createNode(sourceV);
        insert(graph[destV].H,P);      //insert Edge For Destination Vertex

        fscanf(input,"%d",&sourceV);
    }
    fclose(input);
    /// The Time For Reading Input File
    printf ( "\n\nReading Time :%f\n\nfile Read Done.!\n\n", ( (double)clock() - start ) / CLOCKS_PER_SEC );

  //  printGraph(graph,numOfVer);           //print graph  Edges and Colors of Edges to OutPutScreen

    clock_t startOut = clock();  ///Start Measuring the File Writing Time

    // print The Input Data to outPutFile To Make Sure We Read them Well !
   // printf("\n\nEnter The Output File Name\t \"Hint:outputFormat.txt\"\n");
   // scanf("%s",fileName);         //scan the name of Input Data

    /// Print Output into File

    FILE *output;
    output=openFileWrite(argv[2]);          //Open the File and Test if Exist or not !

    fprintf(output,"%ld %ld %ld ",numOfVer,numOfEdge,numOfColor);


      for(i=0;i<numOfColor-1;i++)
          fprintf(output,"%ld ",colorRange[i]);
    fprintf(output,"\n");

    for(i=0;i<numOfVer;i++)
    {
      P=graph[i].H->next;
      while(P!=NULL)
      {
          if(i<P->vertex)
            fprintf(output,"%ld %ld\n",i,P->vertex);
          P=P->next;
      }
    }

    fprintf(output,"\n\nThe Vertex Numbers And Colors\n\n");
    for(i=0;i<numOfVer;i++)
    {
        fprintf(output,"Index # %ld Has Color # %ld\n",i,graph[i].color);
    }

 printf("Go And Check %s File For OutPut\n\n",argv[2]);

 printStatistics(output,graph,numOfVer,numOfColor);
    fclose(output);

    printf ( "\n\nwriting Time :%f\n\n", ( (double)clock() - startOut ) / CLOCKS_PER_SEC );

    makeGraphivFile(graph,numOfVer,argv[3]);

    *Size=numOfVer;
    *numColor=numOfColor;
    return graph;
}

//Function used to print Statistics about the Graph into output File
void printStatistics(FILE*output,CGraph graph[], unsigned long Size , unsigned long numC)
{
  fprintf(output,"Color Degree For Each Vertex\n\n");

  unsigned long  *colorCounter=(unsigned long *)malloc (sizeof(unsigned long)*numC);             //used as a multipe counter for all colors ( reset for each vertex)

   unsigned long k;
  // reset color counter
   for(k=0;k<numC;k++)
       colorCounter[k]=0;
  Position P;                         //to Move throgh the list of Edges
  unsigned long i;                              // to move through the Vertex
  unsigned long j;                              // counter to print the Color Counter

  for(i=0;i<Size;i++)
  {
      P=graph[i].H->next;
      while(P!=NULL)
      {
         colorCounter[graph[P->vertex].color]++;          //sorry for this ! ^__^
         //for Each vertex we go through the list of connected vetreces and get each one color
         //then increase the counter of the index of that Color
         P=P->next;      //go to the next element
      }
      //after we finish Each vertex we should print it out to the File then reset Counter
     fprintf(output,"\n\nThe Vertex # %ld Color Degree As Follows\n\n",i);
     for(j=0;j<numC;j++)
        fprintf(output,"The Color # %ld repeated %ld Times \n",j,colorCounter[j]);

     //now reset color counter
     for(j=0;j<numC;j++)
        colorCounter[j]=0;
  }

}

void makeGraphivFile(CGraph graph[], unsigned long numOfVer,char*fileName)
{
    /*
    printf("\nEnter The output File Name To Make Graphviz Code\t \"Hint:graph.viz\"\n");
    char fileName[20];
    scanf("%s",fileName);         //scan the name of Input Data
*/
    FILE *outGraph;
    outGraph=openFileWrite(fileName);          //Open the File and Test if Exist or not !

    fprintf(outGraph,"graph G\t{\n\tnode [style = solid];\n");

    unsigned long i;
    char*color;
	for(i=0;i<numOfVer; i++)
	   {
		switch(graph[i].color)
        {
            case 0:color="Black";
                   break;
            case 1:color="Blue";
                   break;
            case 2:color="Gray";
                   break;
            case 3:color="Yellow";
                   break;
            case 4:color="Green";
                   break;
            case 5:color="Red";
                   break;
            case 6:color="Orange";
                   break;
            default:color="Brown";
                   break;
        }
		fprintf(outGraph,"%ld [color = %s];\n",i,color);
	   }

    Position P;
  	for(i=0; i < numOfVer; i++)
	{
      P=graph[i].H->next;
        while(P!=NULL)
	      {
            if(i<P->vertex)
               fprintf(outGraph,"\t%ld -- %ld;\n",i,P->vertex);
               P = P->next;
		  }
    }
	fprintf(outGraph,"\n}");
	fclose(outGraph);

	printf("Write Graph VIz Done\n");
}
//Function Used To initialize the Graph to Fill It
void initializeGraph(CGraph graph[],unsigned long Size)
{
    int i;
    for(i=0;i<Size;i++)
     {
       graph[i].H=creatList();    //initialize the Linked List
       graph[i].VertexNumber=i;
       graph[i].color=0;          //default color
     }
}

//Function that Print The Edges and Colors of Each Vertex OF Colored Graph
void printGraph(CGraph graph[], unsigned long Size)
{
    unsigned long i;
    Position P;
    for(i=0;i<Size;i++)
    {
      P=graph[i].H->next;
      while(P!=NULL)
      {
          if(i<P->vertex)
            printf("%ld --> %ld\n",i,P->vertex);
          P=P->next;
      }
    }

    printf("\n\nThe Vertex Numbers And Colors\n\n");
    for(i=0;i<Size;i++)
    {
        printf("Index # %ld Has Color # %ld\n",i,graph[i].color);
    }
}
//insert Node to the Linked List first Position
void insert(Head H , Position P)
{
    if(H==NULL)
    {
        printf("Error: List Does Not Exist\n");
        exit(0);
    }else {
            P->next=H->next;
            H->next=P;
          }
}

//Create A Node And Return A Pointer For The Created Node ( Vertex)
Position createNode(unsigned long Vertex)
{
    Position P;
    P=(Position)malloc(sizeof(struct node));
    if(P==NULL)
    {
      printf("Error:\"Memory FULL\"");
      exit(0);
    }else{
           P->vertex=Vertex;
           P->next=NULL;
         }
  return P;
}

//function That Initialize The Head Of The Linked List ( PS. Head Don't Contain Data )
Head creatList()
{
    Head H=(Head)malloc(sizeof(struct node));
    if(H==NULL)
      {
          printf("Error:\"Memory FULL\"");
          exit(0);
      }else{
             H->vertex=0;
             H->next=NULL;
           }
    return H;
}

//Function That Open A Certain File To Read And Return A Pointer to the File
FILE* openFileRead(char *fileName)
{
    FILE*input;
    input=fopen(fileName,"r");
    if(input==NULL)
       {
           printf("File Does Not Exists\n");
           exit(0);
       }else {
               return input;
             }
}
//Function That Open A Certain File To write And Return A Pointer to the File
FILE* openFileWrite(char *fileName)
{
    FILE*input;
    input=fopen(fileName,"w");
    if(input==NULL)
       {
           printf("File Exists\n");
           exit(0);
       }else {
               return input;
             }
}
