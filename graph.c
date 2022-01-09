#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "graph.h"


//------------------------Structers--------------------------

typedef struct node_ {
    char node_num;
    double *weight;
    struct node_ **close_nodes; 
    int size_close_nodes;
} Node;

typedef struct graph_ {
    int size_all_nodes;
    struct node_ **node_ID; 
} Graph;





Graph *build_graph() {
    Graph *new_graph = (Graph *)malloc(sizeof(Graph));

    new_graph->size_all_nodes = 0;
    new_graph->node_ID = (Node **)malloc(new_graph->size_all_nodes * sizeof(Node *));
    return new_graph;
}

void release_graph(struct graph_ *G) {
    int i;
    for(i = 0 ; i < G->size_all_nodes; i++) {
        free(G->node_ID[i]->weight);
        free(G->node_ID[i]->close_nodes);
        free(G->node_ID[i]);
    }
    free(G->node_ID);
    free(G);
}


////////////////////////////////////////////////////////////////////////////////
//---------------------------------Edge----------------------------------------------

/* 
This function will add an edge from the source node to the destination node, and determine the weight of this edge.
*/
/* 
This function will add an edge from the source node to the destination node, and determine the weight of this edge.
*/
void insert_edge(Node *src, Node *dest, double weight) {
    
    src->weight = (double *)realloc(src->weight, (src->size_close_nodes + 1) * sizeof(double));
    if(src->weight == NULL){
        exit(1);
    }
   
    src->close_nodes =(Node **)realloc(src->close_nodes, (src->size_close_nodes + 1) * sizeof(Node *));
    if(src->close_nodes == NULL){
        exit(1);
    }

    src->close_nodes[src->size_close_nodes] = dest;
    src->weight[src->size_close_nodes] = weight;
    src->size_close_nodes += 1;
}

/* 
This function will delet the edge you asked for.
Parameters: G - the spesific graph we want to remove the edge.
            src - will be the source node_ID of the edge
            dest - will be the destination node_ID of the edge
*/
void remove_edge(Graph *G, Node *src, Node *dest) {
    int temp=0;
    int j=0;
    for (int i = 0; i <src->size_close_nodes; i++) {
        if(src->close_nodes[i]->node_num != dest->node_num){
            src->close_nodes[j]=src->close_nodes[i];
            j++;
        }else{
            temp=i;

        }
    }
    for (int i = temp+1; i <src->size_close_nodes; i++) {
        src->weight[temp]=src->weight[i];
        temp++;
    }
    src->close_nodes = (Node **)realloc(src->close_nodes, (src->size_close_nodes - 1) * sizeof(Node *));
    if(src->close_nodes==NULL){
        exit(1);
    }
    src->size_close_nodes -= 1;
    src->weight = (double *)realloc(src->weight, (src->size_close_nodes) * sizeof(double));
    if(src->weight==NULL){
        exit(1);
    }
}

//--------------------------------Node-----------------------------------------

/*
This function will creat a new node by the data recive into the graph.
*/
Node *init_node(Graph *g, char data) {
     for(int i=0; i<g->size_all_nodes; i++){
        if(data==g->node_ID[i]->node_num){
            return g->node_ID[i];
        }
    }
    Node *new_node;
    new_node = (Node *) malloc(sizeof(Node));
    if(new_node == NULL){
        exit(1);
    }
    new_node->node_num = data;
    new_node->size_close_nodes = 0;
    new_node->close_nodes = (Node **) malloc(new_node->size_close_nodes * sizeof(Node *));

    if(new_node->close_nodes==NULL){
        exit(1);
    }

    new_node->weight = (double *) malloc(new_node->size_close_nodes * sizeof(double));

    if(new_node->weight==NULL){
        exit(1);
    }
    return new_node;
}

/*
This function will add a new node to the graph.
*/
void insert_node(Graph *g, Node *new_node) {
  for(int i=0; i<g->size_all_nodes; i++){
        if(new_node->node_num == g->node_ID[i]->node_num){
            return;
        }
    }

    g->node_ID = (Node **)realloc(g->node_ID, (g->size_all_nodes + 1) * sizeof(Node *));
    if(g->node_ID==NULL){
        exit(1);
    }

    g->node_ID[g->size_all_nodes] = new_node;
    g->size_all_nodes += 1;
}

/*
This function will delet the node from the grapg, and will remove the all our edges of this spisific node.
*/
void remove_node(Graph *g, Node *node) {
    int j=0;
    int i;
    for (i = 0; i <g->size_all_nodes; i++) {
        if (g->node_ID[i]->node_num != node->node_num) {
                g->node_ID[j] = g->node_ID[i];
                j++;
        }else{
            free(g->node_ID[i]->close_nodes);
            free(g->node_ID[i]->weight);    
        }    
    }
    g->node_ID = (Node **) realloc(g->node_ID, (g->size_all_nodes - 1) * sizeof(Node *));
    if(g->node_ID == NULL){
        exit(1);
    }
    g->size_all_nodes -= 1;
    for (int i = 0; i  < g->size_all_nodes; i++){
         Node *s =g->node_ID[i];
        for (int j = 0; j <g->node_ID[i]->size_close_nodes; j++){
            if(s->close_nodes[j]->node_num == node->node_num){
                remove_edge(g,s,node);
            }
        }
    }

}

/*
This function return the index number of the node in the list.
*/
int getNodeID(Graph *G, Node *node) {
 for(int i = 0; i < G->size_all_nodes; i++) {
        if(G->node_ID[i]->node_num == node->node_num) {
            return i;
        }
    }
    return -1;
}

//---------------------------------Helper Functions-------------------------------------------


int Dijsktra(Graph *g, Node *src, Node *dest) {
    Node *curr;
    int distance[g->size_all_nodes];
    int prev[g->size_all_nodes];
    int visited[g->size_all_nodes];
    double path;
    int v;
    int short_i;
    int i;

    for(i = 0; i < g->size_all_nodes; i++) {
        if(g->node_ID[i]->node_num == src->node_num) {
            distance[i] = 0;
        }
        else {
            distance[i] = 1000;
        }
        prev[i] = -1;
        visited[i] = 0; // visited
    }

    for(v=0;v < g->size_all_nodes; v++) {
        path = 1000;
        short_i = 0;
        for(i = 0; i < g->size_all_nodes; i++) {
            if(!visited[i] && distance[i] < path ) {
                short_i = i;
                path = distance[i];
            }
        }

        curr = g->node_ID[short_i];
        visited[short_i] = 1;

        if(curr == dest && curr->node_num == dest->node_num){
            break;
        }

        for(i = 0; i < curr->size_close_nodes; i++) {
            Node *close = curr->close_nodes[i];
            double temp = distance[short_i] + curr->weight[i];
            int idx = getNodeID(g, close);


            if(distance[idx] > temp) {
                distance[idx] = temp;
                prev[idx] = short_i;
            }
        }
    }

    i = getNodeID(g, dest);
    int shortest = distance[i];
    return shortest;
   
}




//-------------------------------Functions-----------------------------------

Graph * build_graph_cmd(char *arr, int len){
    Graph *graph = build_graph();
    int i = 1;
    while (i < len-1){
        if (arr[i]=='n'){
            i++;
        
            
            Node *start = init_node(graph,arr[i]);
            insert_node(graph,start);
            if (i >= len-2){
                break;
            }
            i++;
            while(arr[i]!='n'){
                Node *end = init_node(graph,arr[i]);
                insert_node(graph,end);
                i++;
                double w = (arr[i])-'0';
                insert_edge(start,end, w);
                i++;
            }

        }
    }

    return graph;
}

void Shortest_path(char *arr,Graph *g){
    Node *s;
    for (int i = 0; i < g->size_all_nodes ; ++i) {
        if(arr[0] == g->node_ID[i]->node_num){
            s = g->node_ID[i];
        }
    }
    Node *d;
    for (int i = 0; i < g->size_all_nodes ; ++i) {
        if(arr[1] == g->node_ID[i]->node_num){
            d = g->node_ID[i];
        }
    }
    printf("Dijsktra shortest path: %d \n", Dijsktra(g, s, d));
    
}

void create_new_node(char *arr,Graph *graph){
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    if(node == NULL){
        exit(1);
    }

    node->node_num = arr[0];
     for(int i=0; i<graph->size_all_nodes; i++){
        if(node->node_num == graph->node_ID[i]->node_num){
            free(graph->node_ID[i]->close_nodes);
            free(graph->node_ID[i]->weight);
            graph->node_ID[i]->size_close_nodes=0;
            graph->node_ID[i]->close_nodes = (Node **) malloc(graph->node_ID[i]->size_close_nodes * sizeof(Node *));
            if(graph->node_ID[i]->close_nodes==NULL){
                exit(1);
            }
            graph->node_ID[i]->weight = (double *) malloc(graph->node_ID[i]->size_close_nodes * sizeof(double));
            if(graph->node_ID[i]->weight == NULL){
                exit(1);
            }
            Node *src;
            for(int i=0; i<graph->size_all_nodes; i++){
                if(node->node_num == graph->node_ID[i]->node_num){
                        src= graph->node_ID[i];
                    }
                }
                    free(node);
                    while(arr[i]!='\0'){
                    Node *dest = init_node(graph,arr[i]);
                    insert_node(graph,dest);
                    i++;
                    double w = (arr[i])-'0';
                    insert_edge(src,dest, w);
                    i++;
                
            }
            return;
            
        }
    }
    node->size_close_nodes = 0;
   
    node->weight = (double *) malloc(node->size_close_nodes * sizeof(double));
    if(node->weight==NULL){
        exit(1);
    }
    node->close_nodes = (Node **) malloc(node->size_close_nodes * sizeof(Node *));
    if(node->close_nodes==NULL){
        exit(1);
    }

    insert_node(graph,node);
    int i=1;
    while(arr[i]!='\0'){
        Node *dest = init_node(graph,arr[i]);
        insert_node(graph,dest);
        i++;
        double w = (arr[i])-'0';
        insert_edge(node,dest, w);
        i++;
    }
}


int findpath(Graph *g, char *list_of_node, int size){
    int w = 0;
    for(int j=1; j<size-1; j++) {
        Node *node1;
        for (int i = 0; i < g->size_all_nodes; ++i) {
            if (g->node_ID[i]->node_num == list_of_node[j]) {
                node1 = g->node_ID[i];
            }
        }
        Node *node2;
        for (int i = 0; i < g->size_all_nodes; ++i) {
            if (g->node_ID[i]->node_num == list_of_node[j + 1]) {
                node2 = g->node_ID[i];
            }
        }
        if(node1->size_close_nodes>0) {
            w += Dijsktra(g, node1, node2);
        }
        else{
            w += Dijsktra(g, node2, node1);
        }
    }
    return w;

}




void reverse(char *arr, int size, Node *src) {
    char temp[size];
    strcpy(temp, arr);
    int j=2;
    int i;
    for (i = 1; i < size; i++) {
        arr[j] = temp[i];
        j++;
    }
    arr[1]=src->node_num;

}
int TSP(Graph *g, char *list_of_node, int size){
    if (size==1){
        return -1;
    }
    if (size==0){
            return -1;
    }
    int w1 = findpath(g,list_of_node,size);

    if (w1 < 10){
        return w1;

    }
    else {

        for (int i = 0; i < size; ++i) {
            for (int i = 0; i < g->size_all_nodes; ++i) {
                if (list_of_node[size-1] == g->node_ID[i]->node_num ) {
                    Node *n = g->node_ID[i];
                    reverse(list_of_node,size,n);

                }

            }
        }

        int w2 = findpath(g,list_of_node,size);
        if(w2 >= 1000){
            return -1;

        }

       return w2;
    }
}

void TSP_cmd(Graph *g,char *arr){
    printf( "TSP shortest path: %d \n", TSP(g,arr, strlen(arr)));
}

void del_node(char *arr,Graph *g){
    Node *temp;
    for (int i = 0; i < g->size_all_nodes ; ++i) {
        if(g->node_ID[i]->node_num == arr[0]){
            temp=g->node_ID[i];
        }
    }
    remove_node(g,temp);
    free(temp);
}

