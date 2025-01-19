#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "project2023.h"


int main(){

  // Description of Graph G
  int number_vertices = 11; //6;
  int minimum_time = 1;
  int maximum_time = 7;

  struct graph *graph = new_graph(number_vertices, minimum_time, maximum_time);

  add_edge(graph,0,1);
  add_edge(graph,1,8);
  add_edge(graph,1,9);
  add_edge(graph,2,1);
  add_edge(graph,3,0);
  add_edge(graph,3,2);
  add_edge(graph,4,0);
  add_edge(graph,4,1);
  add_edge(graph,4,5);
  add_edge(graph,5,6);
  add_edge(graph,5,7);
  add_edge(graph,6,7);
  add_edge(graph,8,7);
  add_edge(graph,8,10);
  add_edge(graph,9,10);


  struct task **adjacencyList = graph->adjacency_list;
  printf("\n");
  printf("Presentation of the selected graph G: \n");

  printf("\n");

  printf("Structure of the graph G using the adjacency list view : \n");

  printAdjacencyList(adjacencyList,number_vertices);
  printf("\n");
  printf("\n");

  int *time = malloc(number_vertices*sizeof(int));

  printf("Time of each task : \n");
  for (int i = 0; i < number_vertices ; i++){
  struct task task = graph->all_tasks[i];
  int task_time = task.time;
  time[i] = task_time;
  printf("Task ID : %d, Task's Time : %d \n" ,i,task_time );
}
  printf("\n");


  printf("Part 1 : Topological Ordering of the directed acyclic Graph G \n");
  printf("\n");
  topological_sort(graph);
  Kahn_toposort(graph);
  printf("\n");
  printSeparator();


  printf("Part 2 : Now having unlimited worker, here is the unique topological ordering for the graph G \n");


  printf("\n");
  printf("\n");

  printf("Here is a breakdown on when each task starts : \n" );
  printf("\n");


  printf("\n Using unlimited workers, the time to complete all the tasks is now : %d\n",parallele_task_scheduling(graph));
  printf("\n");

  printf("Using only one worker, we would complete all the task in %d units of time.",time_sum(graph,time));
  free_mem(graph);
  free(time);

  printf("\n");
  printSeparator();
  int number_vertices_by_user;
  int number_edges_by_user;
  int min_time_by_user;
  int max_time_by_user;
  int choice_of_alg;

  printf("Part 3 : Random Graph Creation and evaluation of the algorithms. \n");
  printf("\n");
  printf("\n");

  printf("Now we are going to create a random graph : \n");
  printf("Please specify the number of tasks : \n");
  scanf("%d", &number_vertices_by_user);
  printf("Please specify the lower limit of task's time : \n");
  scanf("%d", &min_time_by_user);

  printf("Please specify the upper limit of task's time : \n");
  scanf("%d", &max_time_by_user);

  if (min_time_by_user > max_time_by_user){
    printf("The minimum time is greater than the maximum time.\n");
    return 0;
  }

  printf("Please specify the number of edges : \n");
  scanf("%d", &number_edges_by_user);

  if(number_edges_by_user > (number_vertices_by_user*(number_vertices_by_user-1)/2)){
    printf("The number of edges entered is greater than limit allowed for this graph.\n");
    printf("The limit being %d \n",(number_vertices_by_user*(number_vertices_by_user-1)/2) );

  }

  else{
    printf("Would you prefer to use the truly random version (type 0) or the less random but faster version (type 1) ? \n");
    scanf("%d",&choice_of_alg);
    if (choice_of_alg ==0){
    printf("Thank you for your choice (Truly Random). \n");
    printf("Kindly wait as the workers are diligently performing their tasks as quickly as possible.\n");
    printf("\n");


    struct graph *new_graph = create_true_random_graph(number_vertices_by_user, number_edges_by_user, min_time_by_user, max_time_by_user);
    printf("Presentation of the random graph : \n");

    printf("\n");

    printf("Structure of the random graph using the adjacency list view : \n");
    printAdjacencyList(new_graph->adjacency_list, number_vertices_by_user);
    printf("\n");
    printf("\n");

    int *time = malloc(number_vertices_by_user*sizeof(int));

    printf("Time of each task : \n");
    for (int i = 0; i < number_vertices_by_user ; i++){
    struct task task = new_graph->all_tasks[i];
    int task_time = task.time;
    time[i] = task_time;
    printf("Task ID : %d, Task's Time : %d \n" ,i,task_time );
  }
    printf("\n");
    printf("\n");
    topological_sort(new_graph);
    Kahn_toposort(new_graph);
    printf("\n");

    printSeparator();
    printf("Now having unlimited worker, here is the unique topological ordering for the graph G \n");


    printf("\n");
    printf("\n");

    printf("Here is a breakdown on when each task starts : \n" );
    printf("\n");


    printf("\nUsing unlimited workers, the time to complete all the tasks is now : %d\n",parallele_task_scheduling(new_graph));
    printf("\n");
    printf("Using only one worker, we would complete all the task in %d units of time.",time_sum(new_graph,time));
    printf("\n");

    free_mem(new_graph);
    free(time);
  }
  if (choice_of_alg ==1){
  printf("Thank you for your choice (Less Random). \n");
  printf("Kindly wait as the workers are diligently performing their tasks as quickly as possible.\n");
  printf("\n");


  struct graph *new_graph = create_less_random_graph(number_vertices_by_user, number_edges_by_user, min_time_by_user, max_time_by_user);
  printf("Presentation of the random graph : \n");

  printf("\n");

  printf("Structure of the random graph using the adjacency list view : \n");
  printAdjacencyList(new_graph->adjacency_list, number_vertices_by_user);
  printf("\n");
  printf("\n");

  int *time = malloc(number_vertices_by_user*sizeof(int));

  printf("Time of each task : \n");
  for (int i = 0; i < number_vertices_by_user ; i++){
  struct task task = new_graph->all_tasks[i];
  int task_time = task.time;
  time[i] = task_time;
  printf("Task ID : %d, Task's Time : %d \n" ,i,task_time );
}
  printf("\n");
  printf("\n");
  topological_sort(new_graph);
  Kahn_toposort(new_graph);
  printf("\n");

  printSeparator();
  printf("Now having unlimited worker, here is the unique topological ordering for the graph G \n");


  printf("\n");
  printf("\n");

  printf("Here is a breakdown on when each task starts : \n" );
  printf("\n");


  printf("\nUsing unlimited workers, the time to complete all the tasks is now : %d\n",parallele_task_scheduling(new_graph));
  printf("\n");
  printf("Using only one worker, we would complete all the task in %d units of time.",time_sum(new_graph,time));
  printf("\n");

  free_mem(new_graph);
  free(time);
}
  }

  return 0;

}
