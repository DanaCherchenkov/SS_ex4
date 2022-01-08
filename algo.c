#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void remove_edge(Graph *G, Node *a, Node *b) {
    int save=0;
    int j=0;
    for (int i = 0; i <a->size_close_nodes; i++) {
        if(a->close_nodes[i]->node_num != b->node_num){
            a->close_nodes[j]=a->close_nodes[i];
            j++;
        }else{
            save=i;

        }
    }
    int d=save+1;
    for (int i = d; i <a->size_close_nodes; i++) {
        a->weight[save]=a->weight[i];
        save++;
    }
    a->close_nodes = (Node **)realloc(a->close_nodes, (a->size_close_nodes - 1) * sizeof(Node *));
    if(a->close_nodes==NULL){
        exit(1);
    }
    a->size_close_nodes -= 1;
    a->weight = (double *)realloc(a->weight, (a->size_close_nodes) * sizeof(double));
    if(a->weight==NULL){
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
    int i;
    Node *n;
    for(i = 0; i < G->size_all_nodes; i++) {
        n = G->node_ID[i];
        if(n->node_num == node->node_num) {
            return i;
        }
    }
    printf("id : %c\n", node->node_num);
    printf("Invalid id detected.\n");
    return -1;
}

//---------------------------------Helper Functions-------------------------------------------


int Dijsktra(Graph *graph, Node *start, Node *end) {
    Node *neighbor,*node,*cur;
    int dist[graph->size_all_nodes];
    int previous[graph->size_all_nodes];
    int visit[graph->size_all_nodes];
    double alternative,shortest ;
    int n_visit, indx, shortest_indx,i;

    for(i = 0; i < graph->size_all_nodes; i++) {
        node = graph->node_ID[i];
        if(node->node_num == start->node_num) {
            dist[i] = 0;}
        else {
            dist[i] = 1000;}
        previous[i] = -1;
        visit[i] = 0;
    }

    n_visit = 0;
    while(n_visit < graph->size_all_nodes) {
        shortest = 1000;
        shortest_indx = 0;
        for(i = 0; i < graph->size_all_nodes; i++) {
            if(dist[i] < shortest && !visit[i]) {
                shortest_indx = i;
                shortest = dist[i];
            }
        }

        cur = graph->node_ID[shortest_indx];
        visit[shortest_indx] = 1;
        n_visit += 1;

        if(cur == end && cur->node_num == end->node_num){
            break;
        }

        for(i = 0; i < cur->size_close_nodes; i++) {
            neighbor = cur->close_nodes[i];
            indx = getNodeID(graph, neighbor);
            alternative = dist[shortest_indx] + cur->weight[i];

            if(dist[indx] > alternative) {
                dist[indx] = alternative;
                previous[indx] = shortest_indx;
            }
        }
    }

    i = getNodeID(graph, end);
    int w = dist[i];
    return w;
   
}




//-------------------------------Functions-----------------------------------

Graph * A(char *ans, int len ){
    Graph *g=build_graph();
    int i=1;
    while (i<len-1){
        if (ans[i]=='n'){
            i++;
            
            Node *src = init_node(g,ans[i]);
            insert_node(g,src);
            if (i>=len-2){
                break;
            }
            i++;
            while(ans[i]!='n'){
                Node *dest = init_node(g,ans[i]);
                insert_node(g,dest);
                i++;
                insert_edge(src,dest, ((ans[i])-'0'));
                i++;
            }

        }
    }

    return g;
}
///////////////////////////////////////////////////////////
void B(char ans [],Graph *graph){
    Node *node;
    node = (Node *) malloc(sizeof(Node));
    if(node==NULL){
        exit(1);
    }
    node->node_num = ans[0];
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
            if(graph->node_ID[i]->weight==NULL){
                exit(1);
            }
            Node *s;
            for(int i=0; i<graph->size_all_nodes; i++){
            if(node->node_num == graph->node_ID[i]->node_num){
                s= graph->node_ID[i];}}
                free(node);
                while(ans[i]!='\0'){
                Node *dest = init_node(graph,ans[i]);
                insert_node(graph,dest);
                i++;
                insert_edge(s,dest, ((ans[i])-'0'));
                i++;
            }
            return;
            
        }
    }
    node->size_close_nodes = 0;
    node->close_nodes = (Node **) malloc(node->size_close_nodes * sizeof(Node *));
    if(node->close_nodes==NULL){
        exit(1);
    }
    node->weight = (double *) malloc(node->size_close_nodes * sizeof(double));
    if(node->weight==NULL){
        exit(1);
    }

    insert_node(graph,node);
    int i=1;
    while(ans[i]!='\0'){
        Node *dest = init_node(graph,ans[i]);
        insert_node(graph,dest);
        i++;
        insert_edge(node,dest, ((ans[i])-'0'));
        i++;
    }
}

/////////////////////////////////////////////////////////
void D(char ans [],Graph *graph){
    Node *save;
    for (int i = 0; i <graph->size_all_nodes ; ++i) {
        if(graph->node_ID[i]->node_num == ans[0]){
            save=graph->node_ID[i];
        }
    }
    remove_node(graph,save);
    free(save);
}


//////////////////////////////////////////////////////////
void S(char ans [],Graph *graph){
    Node *src;
    for (int i = 0; i <graph->size_all_nodes ; ++i) {
        if(graph->node_ID[i]->node_num == ans[0]){
            src=graph->node_ID[i];
        }
    }
    Node *dest;
    for (int i = 0; i <graph->size_all_nodes ; ++i) {
        if(graph->node_ID[i]->node_num == ans[1]){
            dest=graph->node_ID[i];
        }
    }
    printf("Dijsktra shortest path: %d ", Dijsktra(graph, src, dest));
    
}

////////////////////////////////////////////////////
void to_start(char *list, int len, Node *s) {
    char cnt[len];
    strcpy(cnt, list);
    int j=2;
    for (int i = 1; i <len; i++) {
        list[j] = cnt[i];
        j++;
    }
    list[1]=s->node_num;

}
////////////////////////////////////////////////////
int tsp(Graph *graph, char list [], int len){
    if (len==1||len==0){
        return -1;
    }
    if (len==2){
        return -1;
    }
    int w=0;
    for(int d=1; d<len-1; d++) {
        Node *save_id1;
        for (int i = 0; i < graph->size_all_nodes; ++i) {
            if (graph->node_ID[i]->node_num == list[d]) {
                save_id1 = graph->node_ID[i];
            }
        }
        Node *save_id2;
        for (int i = 0; i < graph->size_all_nodes; ++i) {
            if (graph->node_ID[i]->node_num == list[d + 1]) {
                save_id2 = graph->node_ID[i];
            }
        }
        if(save_id1->size_close_nodes>0) {
            w += Dijsktra(graph, save_id1, save_id2);
        }
        else{
            w += Dijsktra(graph, save_id2, save_id1);
        }
    }
    if (w<10){
        return w;
    }else {

        for (int i = 0; i < len; ++i) {
            Node *s;
            for (int i = 0; i < graph->size_all_nodes; ++i) {
                if (graph->node_ID[i]->node_num == list[len-1]) {
                    s = graph->node_ID[i];
                    to_start(list,len,s);

                }

            }
        }

        int z=0;
        for(int d=1; d<len-1; d++) {
            Node *save_id1;
            for (int i = 0; i < graph->size_all_nodes; ++i) {
                if (graph->node_ID[i]->node_num == list[d]) {
                    save_id1 = graph->node_ID[i];
                }
            }
            Node *save_id2;
            for (int i = 0; i < graph->size_all_nodes; ++i) {
                if (graph->node_ID[i]->node_num == list[d + 1]) {
                    save_id2 = graph->node_ID[i];
                }
            }
                z += Dijsktra(graph, save_id1, save_id2);

        }

        if(z>=1000){
            return -1;
        }

        return z;
    }


}


//////////////////////////////////////////////
void T(char ans [],Graph *graph){
    printf( "TSP shortest path: %d \n", tsp(graph,ans, strlen(ans)));
}

char* replace(char arr[],char *str , int len, int index){
    int i = index;
    int j = 0;
    for (int n= 0; n < len; n++) {
        str[j++]=arr[i++];
    }
    str[len]='\0';
    return str;
}


int main(){

    char sen[1024]; //max size
    gets(sen); 

    char sen_co[strlen(sen)+1];

    char* str=(char*)malloc((strlen(sen)+1) + 1); 

     if(str == NULL){ //check if the pointer is null
        exit(1);
    }
    
    strcpy(sen_co,replace(sen, str , strlen(sen)+1, 0 ));
    free(str);

    int r=0;
    for(int i = 0; i < strlen(sen)+1; i++){
        if(sen_co[i]==' '){
            r++;
        }
        i++;
    }
    
    char new[strlen(sen)+1];
    int j=0;
    for(int i=0; i < strlen(sen)+1; i++){

        if(sen_co[i]!=' '){
            new[j]=sen_co[i];
            j++;
        }
        i++;
    }

    char str_n[strlen(new)-1];
    char* p_strn=(char*)malloc(strlen(new)-1 + 1);

     if(p_strn == NULL){
        exit(1);
    }

    strcpy(str_n,replace(new ,p_strn , strlen(new)-1, 1));
    free(p_strn);

    int i=0;
    int k =0;
    while (k < strlen(str_n)) {
        if ( str_n[k] =='B'|| str_n[k] =='D'|| str_n[k] =='S'|| str_n[k] =='T'|| str_n[k] =='\0'||  str_n[k]=='A'){
            break;
        }else{
            i++;
        }
        k++;
    }

    int c=0;
    char curr[i+1];
    for (int j = 0; j <= i-1 ; j++) {
        curr[c]= str_n[j];
        c++;
    }

    curr[i]='\0';
    Graph *g= A(curr, i); //create 
    int len = strlen(str_n)-i;
    char arr[len];

    char* p = (char*)malloc(len + 1);
     if(p==NULL){
        exit(1);
    }

    strcpy(arr,replace(str_n,p , len,i)) ;
    free(p);

    arr[len]='\0';
    while (strlen(arr)!=0) {

        if (arr[0] == 'B') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                } else {
                    l++;
                }
            }
           
            char* p=(char*)malloc(l + 1);
            if(p == NULL){
                exit(1);
            }

            B(replace(arr,p , l, 1), g);
            free(p);
            
            int l_n =(strlen(arr) - l - 1); 
            char* po=(char*)malloc((strlen(arr) - l - 1) + 1);
            if(po == NULL){
            exit(1);
            }
            strcpy(arr, replace(arr,po,(strlen(arr) - l - 1), l + 1));
            free(po);
          
        }


        if (arr[0] == 'D') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                    
                } else {
                    l++;
                }
            }

            
            char* p=(char*)malloc(l + 1);
            if(p == NULL){
                exit(1);
            }

            D(replace(arr,p , l, 1), g);
            free(p);
            int len = strlen(arr) - l - 1;
            char* po=(char*)malloc((strlen(arr) - l - 1) + 1);

             if(po == NULL){
                exit(1);
            }

            strcpy(arr, replace(arr,po , (strlen(arr) - l - 1), l + 1));
            free(po);
        }

        if (arr[0] == 'A') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0'|| arr[j] =='A') {
                    break;
                } else {
                    l++;
                }
            }

            char* p=(char*)malloc(l + 1);
             if(p==NULL){
                exit(1);
            }
            release_graph(g);
            g= A(replace(arr,p , l, 1), l);
            free(p);
            
            
            char* po=(char*)malloc((strlen(arr) - l - 1) + 1);
             if(po==NULL){
                exit(1);
            }
            strcpy(arr, replace(arr,po , (strlen(arr) - l - 1), l + 1));
            free(po);
        }


        if (arr[0] == 'S') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                } else {
                    l++;
                }
            }

            char* p=(char*)malloc(l + 1);
            if(p == NULL){
                exit(1);
            }

            S(replace(arr,p , l, 1), g);
            free(p);

            char* po=(char*)malloc((strlen(arr) - l - 1) + 1);

             if(po == NULL){
                exit(1);
            }
            strcpy(arr, replace(arr,po , (strlen(arr) - l - 1), l + 1));
            free(po);
        }


        if (arr[0] == 'T') {
            int l = 0;
            for (int j = 1; j < strlen(arr); j++) {
                if (arr[j] == 'B' || arr[j] == 'D' || arr[j] == 'S' || arr[j] == 'T' || arr[j] == '\0' || arr[j] == 'A') {
                    break;
                } else {
                    l++;
                }
            }
            char* p=(char*)malloc(l + 1);
             if(p==NULL){
                exit(1);
            }

            T(replace(arr,p, l, 1), g);
            free(p);

            char* po=(char*)malloc((strlen(arr) - l - 1) + 1);
            if(po == NULL){
                exit(1);
            }
            strcpy(arr, replace(arr,po ,(strlen(arr) - l - 1), l + 1));
            free(po);
        }
    }


    release_graph(g);
    
return 0;
}
