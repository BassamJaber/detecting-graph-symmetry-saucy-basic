typedef struct QueueNode *ptrQ;

struct QueueNode
{
    unsigned long element;
    ptrQ next;
};
typedef ptrQ PositionQ;

struct QueueInfo
{
    unsigned long numOfNodes;
    PositionQ lastNode;  ///Represent The Last Node OF The Queue (for Fast Dequeue)
    ptrQ next;           ///Represent The First Node Of the Queue(for Fast InQueue)
};
typedef struct QueueInfo *PQ;
typedef PQ Queue;

int isEmptyQueue(Queue);
PositionQ createNodeQ(unsigned long);
Queue createQueue();
void InQueue(Queue, unsigned long);
unsigned long DeQueue(Queue);
void printQueue(Queue);

void printQueue(Queue Q)
{
    if(isEmptyQueue(Q))
      {
          printf("Queue Empty !\n");
      }else {
               PositionQ Temp;
               Temp=Q->next;
               while(Temp!=NULL)
                 {
                   printf("%ld ",Temp->element);
                   Temp=Temp->next;
                 }
                  printf("\n\n");
            }
}

//DeQueue , Removing From The First of The List
unsigned long DeQueue(Queue Q)
{
  if(Q==NULL)
    {
        printf("Error:No Queue Exists\n\n");
        exit(0);
    }else{
          if(isEmptyQueue(Q))
            {
             printf("Error:Queue Already Empty !\n\n");
             return 0;
            }else{
                   PositionQ Temp;
                   Temp=Q->next;   //The Last element in The Queue we Want to Remove
                   Q->next=Temp->next;
                   Q->numOfNodes--;
                   return Temp->element;
                 }
         }
}
//Insert From the Last , Remove From First FIFO
void InQueue(Queue Q, unsigned long element)
{
    if(Q==NULL)
      {
          printf("Error:Queue Does Not Exist\n\n");
          exit(0);
      }else{
           PositionQ P=createNodeQ(element);
             if(isEmptyQueue(Q))
               {
                   Q->next=P;
                   Q->lastNode=P;
                   P->next=NULL;
                   Q->numOfNodes++;
               }else {
                        Q->lastNode->next=P;
                        Q->lastNode=P;
                        P->next=NULL;
                        Q->numOfNodes++;
                     }
           }
}

//Test if The Queue is Empty Or Not
//return 1 if Empty return 0 if not
int isEmptyQueue(Queue Q)
{
   if(Q==NULL)
      {
          printf("Error:No Queue\n\n");
          exit(0);
      }else {
              if(Q->next==NULL &&Q->numOfNodes==0)
                 return 1;
              else
                 return 0;
            }
}
//Create And initialize the Queue For Use
Queue createQueue()
{
    Queue Q=(Queue)malloc(sizeof(struct QueueInfo));
    if(Q==NULL)
       {
           printf("Error:No Enough Memory!\n\n");
           exit(0);
       }else{
              Q->lastNode=NULL;
              Q->next=NULL;
              Q->numOfNodes=0;
              return Q;
            }
}
//create a Node For the Queue
PositionQ createNodeQ(unsigned long element)
{
    PositionQ P;
    P=(PositionQ)malloc(sizeof(struct QueueNode));
    if(P==NULL)
      {
          printf("Error:No Enough Memory!\n\n");
          exit(0);
      }else {
             P->element=element;
             P->next=NULL;
             return P;
            }
}
