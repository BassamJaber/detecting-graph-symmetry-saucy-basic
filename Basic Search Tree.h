///------------------------------------------///
///    Structures for Basic Search Tree      ///
///------------------------------------------///
struct DOP
{
    OPP Top;
    OPP Bottom;
    CGraph *graphTop;          //Graph Colored Depending on the Permutation
    CGraph *graphBottom;
    unsigned long numOfVer;
    unsigned long numOfColor;
};

typedef struct DOP *OP;

typedef struct treeNode *Treeptr;
struct treeNode
{
    OP element;             //specific element in the Tree
    int numOfChildren;      //number of Children to a Node
    Treeptr *Children;      //Array of Pointers to Childrens
    unsigned long TopMap;   //Top Vertex of Maping
    unsigned long BottomMap;//Bottom Vertex of Maping
    int root;               //to differ between Root Node and Non Root or leaf
};
typedef Treeptr Tree;
typedef Treeptr Child;

///------------------------------------------///
///    Functions  for Basic Search Tree      ///
///------------------------------------------///

Tree createTree(OP);
Child createTreeNode(OP,unsigned long, unsigned long);
int isSingleton(OP);
int Stop(OP);
int getNumOfChildren();
//Tree AutoCreate(Tree,OP);
unsigned long getFirstNonSingleTonIndex(OP);
OP MapingVertices(OP,unsigned long,unsigned long);
void printTree(Tree);
OP SwapVertex(OP,unsigned long,unsigned long);
OP CopyOPS(OP);
///------------------------------------------///
///  Implementation for Basic Search Tree    ///
///------------------------------------------///

//Create a Basic Search Tree that Contain Only a root
Tree createTree(OP P)
{
    Tree T =(Tree)malloc(sizeof(struct treeNode));
    if(T==NULL)
     {
         printf("Error :Segmentation Fault,No Memory\n");
         exit(0);
     }else{
            T->Children=NULL;
            T->numOfChildren=0;
            T->element=P;
            T->root=1;
            return T;
          }
}

//Create a Basic Search Tree Node
Child createTreeNode(OP P,unsigned long TopMap, unsigned long BottomMap)
{
    Child C=(Child)malloc(sizeof(struct treeNode));
    if(C==NULL)
      {
          printf("Error:Segmentation Fault , No Memory\n");
          exit(0);
      }else{
            C->Children=NULL;
            C->numOfChildren=0;
            C->element=P;
            C->TopMap=TopMap;
            C->BottomMap=BottomMap;
            C->root=0;  //a  Non Root or a Leaf
            return C;
           }
}

//make Copy from OP
OP CopyOPS(OP P)
{
    OP PC=(OP)malloc(sizeof(struct DOP));
    PC->Top=CopyOPP(P->Top,P->numOfVer);
    PC->Bottom=CopyOPP(P->Bottom,P->numOfVer);
    PC->graphTop=makeACopy(P->graphTop,P->numOfVer);
    PC->graphBottom=makeACopy(P->graphBottom,P->numOfVer);
    PC->numOfColor=P->numOfColor;
    PC->numOfVer=P->numOfVer;

    return PC;
}

//Maping the OP and return a New OP
OP MapingVertices(OP P,unsigned long TopV,unsigned long BottomV)
{
  P->numOfColor++;
  P->graphTop[TopV].color=(P->numOfColor);
  P->graphBottom[BottomV].color=(P->numOfColor);
  P=SwapVertex(P,TopV,BottomV);
  P->Top=updateOP(P->graphTop,P->Top,P->numOfVer);
  P->Bottom=updateOP(P->graphBottom,P->Bottom,P->numOfVer);

return P;
}
//Swap two Vertex in the OPP
OP SwapVertex(OP P,unsigned long TopV,unsigned long BottomV)
{
  // if So it's Already Mapped No Need To Swap ^_^
  if(P->Top->Position[TopV]==P->Bottom->Position[BottomV])
    {
        return P;
    }
  unsigned long temp=P->Top->Position[TopV];
  unsigned long temp2=P->Bottom->Position[BottomV];
  unsigned long tempV;
  unsigned long t=P->Top->Position[TopV];
  if((temp2>t)&&(temp2<t+getCellSize(P->Top,TopV)))
  {
     tempV=P->Bottom->VertexArray[temp];
     P->Bottom->VertexArray[temp]=BottomV;
     P->Bottom->VertexArray[temp2]=tempV;
  }

  return P;
}
//get the index of the First N
unsigned long getFirstNonSingleTonIndex(OP P)
{
  unsigned long i=0;
  while(getCellSize(P->Top,P->Top->VertexArray[i])==1)
  {
      i++;
  }

return i;
}


//Stop Function when create a Tree when A Specific Rule to Stop when Reach to a Specific Leaf
int Stop(OP P)
{
    return (isSingleton(P)?1:0);
}

//Determine if the Ordered Partition Cells All Singleton or not
//Singleton return 1, not Singleton return 0
int isSingleton(OP P)
{
  unsigned long i;
  for(i=0;i<P->numOfVer;i++)
  {
    if(getCellSize(P->Top,P->Top->VertexArray[i])!=1)
       {
         return 0;
       }
  }
  return 1;
}

//Determine the Number of Children for a Specefic cell
//and It's Equal to it's Size (
int getNumOfChildren(OP P,unsigned long Index)
{
  return(getCellSize(P->Bottom,P->Bottom->VertexArray[Index]));
}



