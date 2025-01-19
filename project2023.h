
#ifndef PROJETC_H_
#define PROJETC_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


//Structure of a task
struct task{
  int id;
  int time;
  struct task *next;
};

//Structure of a graph
struct graph{
  int number_vertices;
  struct task **adjacency_list;
  struct task *all_tasks;
};

//Function to create a new node
struct task *new_task(int id, int time){
  struct task *task_new = malloc(sizeof(struct task));

  task_new->id = id;
  task_new->time = time;
  task_new->next = NULL;

  return task_new;
}

//Function to create a new graph
struct graph *new_graph(int number_vertices, int minimum_time, int maximum_time){
  srand(time(NULL)); // Seed for the random time.

  struct graph *graph_new = malloc(sizeof(struct graph));
  graph_new->number_vertices = number_vertices;

  graph_new->adjacency_list = malloc(number_vertices * sizeof(struct task *)); // allocating memory for an array of pointer where each points to a struct task
  for(int i = 0; i<number_vertices; i++){
    graph_new->adjacency_list[i] = NULL;
  }

  graph_new->all_tasks = malloc(number_vertices * sizeof(struct task));
  for (int i = 0; i < number_vertices; i++) {
      int random_time = rand()%maximum_time + minimum_time;
      graph_new->all_tasks[i] = *new_task(i, random_time);
    }
  return graph_new;
}

//Function to free memory
void free_mem(struct graph *graph){

  for (int i = 0; i < graph->number_vertices; i++) {
    struct task *current = graph->adjacency_list[i];
    struct task *next;
    while (current != NULL) {
      next = current->next;
      free(current);
      current = next;
      }
}
// Free memory for the array of adjacency lists
  free(graph->adjacency_list);
  free(graph->all_tasks);

// Free memory for the graph structure itself
  free(graph);
}


//Function to add edges between vertices
void add_edge(struct graph* graph, int src, int dest){

    struct task task = graph->all_tasks[dest];
    int time = task.time;

    struct task *newtask = new_task(dest, time);
    newtask->next = graph->adjacency_list[src];
    graph->adjacency_list[src] = newtask;
}

//Function to print the adjacency list
void printAdjacencyList(struct task **adjacencyList, int number_vertices) {
    for (int i = 0; i < number_vertices; ++i) {
        printf("Adjacency list for vertex %d: ", i);

        struct task * current = adjacencyList[i];
        while (current != NULL) {
            printf("%d -> ", current->id);
            current = current->next;
        }
        printf("NULL\n");
      }
}

//Function to print the time array
void print_time(struct graph* graph){
  int number_vertices = graph->number_vertices;


  int *time = malloc(number_vertices * sizeof(int));

for (int i = 0; i < number_vertices; i++){
  struct task task = graph->all_tasks[i];
  int task_time = task.time;
  time[i] = task_time;
  printf("%d ",task_time);
}
}
//////////////////////////////////////////////////////////////////////
// First way to do topological sort


// Depth-First Search
void DFS(int task, bool visited[], int order[], int *index, struct graph* graph) {
    visited[task] = true;
    struct task* current = graph->adjacency_list[task];
    while (current != NULL) {
        int neighbor = current->id;
        // If the neighbor has not been visited, recursively call DFS on the neighbor
        if (!visited[neighbor]) {
            DFS(neighbor, visited, order, index, graph);
        }
        // Move to the next neighbor in the adjacency list
        current = current->next;
    }
    // Add the current node to the order array and increment the index
    order[(*index)++] = task;
}

// Function to topological sort by DFS
void topological_sort(struct graph* graph) {
    int number_vertices = graph->number_vertices;

    // Initialize all vertices as not visited
    bool visited[number_vertices];
    for (int i = 0; i < number_vertices; i++) {
        visited[i] = false;
}
    // Initialize the vector of topological order with zero
    int order[number_vertices];
    for (int i = 0; i < number_vertices; i++) {
        order[i] = 0;
}
    // Initialize index for the topological order array
    int index = 0;

    // Iterate through all vertices
    for (int i = 0; i < number_vertices; i++) {
        // If the current vertex is not visited, perform DFS starting from that vertex
        if (!visited[i]) {
            DFS(i, visited, order, &index, graph);
        }
    }

    // Print the topological order
    printf("Topological Order using Depth First Search: ");
    for (int i = number_vertices - 1; i >= 0; i--) {
        printf("%d ", order[i]);
    }
    printf("\n");

}

/////////////////////////////////////////////////////////////////////
// Second way to do topological sort.

struct Queue {
  size_t front;
  size_t length;
  size_t capacity;
  size_t elem_size;
  void *data;
};

// Initialize a queue structure
int queue_init(struct Queue *q, size_t elem_size, size_t capacity){
  q->front = 0;
  q->length = 0;
  q->capacity = capacity;
  q->elem_size = elem_size;
  q->data = malloc(elem_size * capacity);
  return 0;
}

// Free the queue structure
void queue_dispose(struct Queue *q){
 free(q->data);
 free(q);
}

// Enqueue one element in the queue structure
void queue_enqueue(struct Queue *q, int *src){
  int idx = (q->front + q->length);
  void *dst = q->data + (idx * q->elem_size);
  memcpy(dst, src, q->elem_size);
  q->length++;
}

// Dequeue one element from the queue structure
void queue_dequeue(struct Queue *q, int *dest){
  void *src = q->data + q->front * q->elem_size;
  memcpy(dest, src, q->elem_size);
  q->length--;
  q->front = (q->front + 1) % q->capacity;
}

// Function to check if the queue is empty
bool is_empty(const struct Queue *q){
  return (q->length == 0);
}

// Function to create an incoming degree array for every task.
int* indegreelist(struct graph *graph){
  int number_vertices = graph->number_vertices;
  int *indegree_list = malloc(number_vertices*sizeof(int));
  for(int i = 0; i<number_vertices;i++){
    indegree_list[i]=0;
}

  for(int i = 0;i<number_vertices; i++){
    struct task* current = graph->adjacency_list[i];
    while(current !=NULL){
      for(int j = 0;j<number_vertices; j++){
        int id = current->id;
          if (id ==j){
            indegree_list[j]++;
      }
    }
  current = current->next;
}
}
  return indegree_list;
}


// Topological sorting by Kahns Algorithm
void Kahn_toposort(struct graph *graph){
  int number_vertices = graph->number_vertices;

  int *indegree_list = indegreelist(graph);

  struct Queue *q = malloc(sizeof(struct Queue));
  size_t intsize = sizeof(int);
  queue_init(q, intsize, (size_t)number_vertices);

  for(int i = 0; i < number_vertices; i++){
    if (indegree_list[i]==0){
      queue_enqueue(q,&i);
    }
  }

  int index = 0;

  //Initializing order array with zeros
  int order[number_vertices];
  for(int i = 0; i < number_vertices; i++) {
    order[i] = 0;
}

  while (!is_empty(q)){
    int dequeued_task;
    queue_dequeue(q, &dequeued_task);
    order[index++] = dequeued_task;
    struct task *current_task = graph->adjacency_list[dequeued_task];
    while(current_task != NULL){
      int neighbor = current_task ->id;
      indegree_list[neighbor]--;
      if (indegree_list[neighbor] == 0){
        queue_enqueue(q,&neighbor);
    }
    current_task = current_task->next;

  }
}
  // Printing the topological order
  printf("Topological order Using Kahn's Algorithm: ");
  for(int i = 0; i < number_vertices; i++) {
    printf("%d ", order[i]);
}
  printf("\n");
  // Freeing everything
  queue_dispose(q);
  free(indegree_list);
}

// Recursive DFS algorithm to detect cycle used for the random graph creation
bool hasCycleDFS(int u, bool visited[], bool inPath[], struct graph* graph) {
    visited[u] = true;
    inPath[u] = true;

    struct task* current = graph->adjacency_list[u];
    while (current != NULL) {
        int v = current->id;
        // If the adjacent vertex is in the current DFS path, a cycle is detected
        if (inPath[v]) {
            return true;
        }

        // If the adjacent vertex is not visited, continue DFS
        if (!visited[v] && hasCycleDFS(v, visited, inPath, graph)) {
            return true; // Cycle found in the DFS subtree
        }

        current = current->next;
    }

    inPath[u] = false; // Remove the current vertex from the DFS path
    return false;
}

// Find if the Graph has a cycle by using a modified DFS algorithm
bool hasCycle(struct graph* graph) {
    bool visited[graph->number_vertices];
    bool inPath[graph->number_vertices];
    for (int i = 0; i < graph->number_vertices; i++) {
        visited[i] = false;
        inPath[i] = false;
    }

    for (int i = 0; i < graph->number_vertices; i++) {
        if (!visited[i] && hasCycleDFS(i, visited, inPath, graph)) {
            return true;
        }
    }

    return false;
}



//////////////////////////////// Question 2 /////////////////////////////////

// Return a pointer to an array of task with zero indegree
int *zero_indegree_task_id(struct graph *graph, int *indegree, int *completed_task){
    int number_vertices = graph->number_vertices;
    int* selected = malloc((number_vertices +1) * sizeof(int));
    int counter = 0;

    for (int i = 0; i < number_vertices; i++) {
        if (indegree[i] == 0 && !completed_task[i]) { //&& !completed[i]
            selected[counter] = i;
            counter++;
        }
    }
    selected[counter] = -1; // zone tampon.
    return selected;
}

// Update the total time left
int time_sum(struct graph * graph, int *time_array){
  int total_time = 0;
  int number_vertices = graph ->number_vertices;
  for(int i = 0; i < number_vertices; i++){
    total_time = total_time + time_array[i];
  }
  return total_time;
}

//Find the minimum time for the selected task
int min_time(int *selected_task, int *time_array){
  int i = 0;
  int first_task = selected_task[i];
  int minimum_time = time_array[first_task];
  while(selected_task[i+1] != -1){

    if(minimum_time < time_array[selected_task[i+1]]){
      i = i+1;
    }
    else{
      minimum_time = time_array[selected_task[i+1]];
      i = i+1;
    }
  }
  return minimum_time;
}

//Update the time by substracting min time for the selected task
void updated_time(int *time_array, int* selected_task, int min_time){
  int i = 0;
  while(selected_task[i]!= -1){
    int task_id = selected_task[i];
    time_array[task_id] =   time_array[task_id] - min_time;
    i = i+1;
  }

}

// Function to update the indegree of the neighbor of completed task.
void update_indegree_of_completed_task(struct graph *graph, int *time_array, int *selected_task, int *completed_task, int *indegreelist){
  int i = 0;

  while(selected_task[i]!= -1){
    int task_id = selected_task[i];

    if(time_array[task_id]==0){
      completed_task[task_id] = 1;
      // update in degree of neighbor now
      struct task *current_task = graph->adjacency_list[task_id];
      while(current_task != NULL){
        int neighbor = current_task ->id;
        indegreelist[neighbor]--;
        current_task = current_task->next;

    }
  }
  i = i+1;
}
}


// The optimal solution for the case of unlimited workers
int parallele_task_scheduling(struct graph *graph){
  int number_vertices = graph->number_vertices;

  int *indegree_array = indegreelist(graph);

  int *time_array = malloc(number_vertices*sizeof(int));

  for (int i = 0; i < number_vertices ; i++){
    struct task task = graph->all_tasks[i];
    int task_time = task.time;
    time_array[i] = task_time;
}
  int *completed_task = malloc(number_vertices*sizeof(int));
  for(int i = 0; i<number_vertices; i++){
    completed_task[i]=0;
}

  int sum_of_all_tasks_time = time_sum(graph,time_array);
  int current_time = 0;


  while(sum_of_all_tasks_time != 0){
    int *selected_task = zero_indegree_task_id(graph, indegree_array, completed_task);

    printf("At current time %d, we are working on the following task :",current_time);
    int i = 0;
    while (selected_task[i]!=-1) {
      printf("Task %d ", selected_task[i]);
      i = i+1;
    }
    printf("\n");
    int minimum_time = min_time(selected_task,time_array);
    updated_time(time_array, selected_task, minimum_time);
    update_indegree_of_completed_task(graph,time_array,selected_task,completed_task, indegree_array);
    current_time = current_time + minimum_time;
    sum_of_all_tasks_time = time_sum(graph,time_array);

}

  return current_time;
}

//////////// Random Graph Creation functions ////////


// Verify if they are neighbor
bool are_they_neighbor(struct graph *graph, int src, int dest){

  struct task* source = graph->adjacency_list[src];
    while (source != NULL) {
        int neighbor = source->id;
        if (neighbor == dest) {
          return true;
        }
        source = source ->next;
}
return false;
}


//Generate two random integer for an edge
int *generate_two_random_int(int number_vertices){
  srand(time(NULL));

  int *random_vertices = malloc(2*sizeof(int));
  random_vertices[0] = rand()%number_vertices;
  int second_int = rand()%number_vertices;

  while(second_int == random_vertices[0]){
    second_int = rand()%number_vertices;
  }
  random_vertices[1] = second_int;
  return random_vertices;
}



// function to create a truly random graph
struct graph *create_true_random_graph(int number_vertices, int number_edges, int minimum_time, int maximum_time){

  struct graph *graph = new_graph(number_vertices, minimum_time, maximum_time);
  for (int edge_count = 0; edge_count < number_edges; edge_count++){
    int *random_vertices = generate_two_random_int(number_vertices);
    int src = random_vertices[0];
    int dest = random_vertices[1];

    if(!are_they_neighbor(graph,src, dest)){

      add_edge(graph, src, dest);
      if(hasCycle(graph)){

        struct task *task_just_added = graph->adjacency_list[src];
        graph->adjacency_list[src] = task_just_added->next;
        free(task_just_added);

        edge_count--;
      }
    }
    else {
      edge_count--;
    }

    free(random_vertices);
  }
    return graph;
  }


//Function to create a less random graph using v > u.
struct graph *create_less_random_graph(int number_vertices, int number_edges, int minimum_time, int maximum_time){

    struct graph *graph = new_graph(number_vertices, minimum_time, maximum_time);

  // Initialize Counter
  int edges_added=0;
  while (edges_added!=number_edges){
      int *random_vertices = generate_two_random_int(number_vertices);
      int src = random_vertices[0];
      int dest = random_vertices[1];

       if (src>dest && !are_they_neighbor(graph,dest, src)){
              //printf("Task 2: %d, Task 1: %d\n", rand_task2, rand_task1);
              add_edge(graph, dest, src);
              edges_added++;
          }
          else if (dest>src && !are_they_neighbor(graph,src, dest)){
              //printf("Task 1: %d, Task 2: %d\n", rand_task1, rand_task2);
              add_edge(graph, src, dest);
              edges_added++;
          }
    free(random_vertices);
  }
    return graph;
  }

// Print a separator line with a standard length.
void printSeparator() {
      for (int i = 0; i < 40; i++) {
          printf("--");
      }
      printf("\n");
}



#endif
