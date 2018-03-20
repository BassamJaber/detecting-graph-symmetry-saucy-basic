
#define false 0
#define true 1

typedef int bool;



bool symmatryTest(OPP, OPP, CGraph*, CGraph*, unsigned long);
bool swapAndTest(int, int, CGraph*, CGraph*);
void QuickSort(unsigned long*, int, int);
Tree AutoCreate(Tree ,OP ,CGraph*, CGraph*);

void printS();


bool symmatryTest(OPP top, OPP bottom, CGraph* graphTop, CGraph* graphBottom, unsigned long vertices_number)	{


	unsigned long i;
	int first_vertex;
	int second_vertex;
    Position temp;


    CGraph* top_graph = makeACopy(graphTop, vertices_number);
    CGraph* bottom_graph = makeACopy(graphBottom, vertices_number);




    Position* edges_temps;

    edges_temps = (Position*)malloc(vertices_number * sizeof(struct node));


            //saving the edges of each vertex
    for(i = 0; i < vertices_number; i++)    {

        edges_temps[i] = bottom_graph[i].H;

    }

/*

for(i = 0; i < vertices_number; i++)    {

     temp = edges_temps[i] -> next;
      while(temp != NULL)
      {
            printf("%ld --> %ld\n",i,temp -> vertex);
            temp = temp->next;
      }
    }

*/

    for(i = 0; i < vertices_number; i++)    {

            bottom_graph[bottom -> VertexArray[i]].H = edges_temps[top -> VertexArray[i]];

    }

/*

    for(i = 0; i < vertices_number; i++)    {

            temp = bottom_graph[i].H -> next;
            while(temp != NULL)
      {
            printf("%ld --> %ld\n",i,temp -> vertex);
            temp = temp->next;
      }

    }


*/



        //swapping edge by edge
    for(i = 0; i < vertices_number; i++)    {

        temp = bottom_graph[i].H -> next;

        while(temp != NULL) {
      //      printf("\n%ld mapped to", temp -> vertex);
                temp -> vertex =  bottom -> VertexArray[top -> Position[temp -> vertex]];
    //        printf(" %ld in vertex %ld\n",temp -> vertex, i);
                temp = temp -> next;

        }   //while ends here
    }   // i for ends here


	for(i = 0; i < vertices_number; i++)	{

		first_vertex = i;	//search first_vertex in the top_graph
		second_vertex = i;	//search second_vertex in the bottom_graph

//		printf("\nsending vertex %ld\n",i);

		if(!swapAndTest(first_vertex, second_vertex, top_graph, bottom_graph))
			return false;

	}	//i for ends here

    return true;

}	//symmatryTest ends here



bool swapAndTest(int first_vertex, int second_vertex, CGraph* top_graph, CGraph* bottom_graph)	{




	unsigned long* first_array;	//array of the edges of the first_vertex in the top_graph
	unsigned long* second_array;	//array of the edges in the second_vertex in the bottom_graphh

	int first_vertex_edges_counter;		//number of the edges of the first_vertex
	int second_vertex_edges_counter;	//number of the edges of the second counter
	int i;					//loop variable
	Position temp;


		/*allocating memory for the array of edges*/
   	first_array = (unsigned long*)malloc(66*sizeof(unsigned long*));
	second_array = (unsigned long*)malloc(66*sizeof(unsigned long*));



	first_vertex_edges_counter = 0;
	second_vertex_edges_counter = 0;




	temp = top_graph[first_vertex].H -> next;

	for(i = 0; temp != NULL; i++)	{

        if(temp -> vertex == second_vertex) {
            i--;
            temp = temp -> next;
            continue;
        }


		first_array[i] = temp -> vertex;
		first_vertex_edges_counter++;
		temp = temp -> next;
	}

/*
	for(i = 0; i < first_vertex_edges_counter; i++)
        printf("%ld", first_array[i]);
    printf("\n");
*/

	temp = bottom_graph[second_vertex].H -> next;

	for(i = 0; temp != NULL; i++)	{

        if(temp -> vertex == first_vertex)  {
            i--;
            temp = temp -> next;
            continue;
        }


		second_array[i] = temp -> vertex;
		second_vertex_edges_counter++;
		temp = temp -> next;


	}
/*
	for(i = 0; i < second_vertex_edges_counter; i++)
        printf("%ld", second_array[i]);
    printf("\n");

*/
	if(first_vertex_edges_counter != second_vertex_edges_counter)
		return false;



	QuickSort(first_array, 0, first_vertex_edges_counter - 1);
	QuickSort(second_array, 0, second_vertex_edges_counter - 1);

/*
    printf("\nnow\n");
	for(i = 0; i < first_vertex_edges_counter; i++)
        printf("%ld", first_array[i]);
    printf("\n");

  for(i = 0; i < second_vertex_edges_counter; i++)
        printf("%ld", second_array[i]);
    printf("\n");


*/
	for(i = 0; i < first_vertex_edges_counter /*any edges number of the two arrays*/; i++)	{

		if(first_array[i] != second_array[i])
			return false;

	}	//testing edges ends here

	return true;



}//swapAndTest ends here








void QuickSort(unsigned long* array, int left, int right)	{

    int pivot, l_hold, r_hold;

    l_hold = left;
    r_hold = right;
    pivot = array[left];
    while (left < right)    {

        while ((array[right] >= pivot) && (left < right))
        right--;
        if (left != right)  {

            array[left] = array[right];
            left++;
        }


        while ((array[left] <= pivot) && (left < right))
            left++;

        if (left != right)  {

            array[right] = array[left];
            right--;
        }

    }

    array[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;

    if (left < pivot)
        QuickSort(array, left, pivot-1);
    if (right > pivot)
        QuickSort(array, pivot+1, right);


}//QuickSort ends here

unsigned long sym_num = 0;

void printS()
{
    printf("Symmetries Found %ld\n",sym_num);
}

