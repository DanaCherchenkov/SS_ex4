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
 for(int i = 0; i < G->size_all_nodes; i++) {
        if(G->node_ID[i]->node_num == node->node_num) {
            return i;
        }
    }
    printf("In index: %c\n", node->node_num);
    return -1;
}

//---------------------------------Helper Functions-------------------------------------------


int Dijsktra(Graph *g, Node *src, Node *dest) {
    // Node *close;
    Node *curr;
    int distance[g->size_all_nodes];
    int prev[g->size_all_nodes];
    int visited[g->size_all_nodes];
    // double temp;
    double path;
    int v;
    // int idx;
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

    // v = 0;
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
        // v += 1;

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

Graph * A(char *arr, int len){
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
       printf("Dijsktra shortest path: %d \n", Dijsktra(graph, src, dest));
    
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

char* replace(char arr[] , int len, int index,char *str){
    int i = index;
    int j = 0;
    for (int n= 0; n < len; n++) {
        str[j++]=arr[i++];
    }
    str[len]='\0';
    return str;
}


int main(){
    char f[1024];
    gets(f);
    int len= strlen(f)+1;
    char nn[len];
    char* str_cnt=(char*)malloc(len + 1);
     if(str_cnt==NULL){
        exit(1);
    }
    strcpy(nn,replace(f , len, 0,str_cnt));
    free(str_cnt);
    int r=0, b=0;
    while(b<len){
        if(nn[b]==' '){
            r++;
        }
        b++;
    }
    
    char n[len-r];
    int k=0,j=0;
    while(k<len){
        if(nn[k]!=' '){
            n[j]=nn[k];
            j++;
        }
        k++;
    }
    char cnt[strlen(n)-1];
    char* strr_cnt=(char*)malloc(strlen(n)-1 + 1);
     if(strr_cnt==NULL){
        exit(1);
    }
    strcpy(cnt,replace(n , strlen(n)-1, 1,strr_cnt));
    free(strr_cnt);
    int i=0;
    for (int j = 0; j < strlen(cnt); j++) {
        char c= cnt[j];
        if (c=='B'||c=='D'||c=='S'||c=='T'||c=='\0'|| c=='A'){
            break;
        }else{
            i++;
        }
    }

    int copy=0;
    char current[i+1];
    for (int l = 0; l <=i-1 ; l++) {
        current[copy]= cnt[l] ;
        copy++;
    }
    current[i]='\0';
    Graph *g=A(current, i);
    char ans[strlen(cnt)-i];
    char* str_cntt=(char*)malloc(strlen(cnt)-i + 1);
     if(str_cntt==NULL){
        exit(1);
    }
    strcpy(ans,replace(cnt , (strlen(cnt)-i),i,str_cntt)) ;
    free(str_cntt);
    ans[strlen(cnt)-i]='\0';
    while (strlen(ans)!=0) {
        char first = ans[0];
        if (first == 'B') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }
           
            char* str_cn=(char*)malloc(t + 1);
             if(str_cn==NULL){
             exit(1);
              }
            B(replace(ans, t, 1,str_cn), g);
            free(str_cn);
            char* str_cnt=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(str_cnt==NULL){
            exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,str_cnt));
            free(str_cnt);
          
        }


        if (first == 'D') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }

            
            char* str_cn=(char*)malloc(t + 1);
            if(str_cn==NULL){
                exit(1);
            }
            D(replace(ans, t, 1,str_cn), g);
            free(str_cn);
            char* str_cnt=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(str_cnt==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,str_cnt));
            free(str_cnt);
        }

        if (first == 'A') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0'|| c=='A') {
                    break;
                } else {
                    t++;
                }
            }
            char* str_cn=(char*)malloc(t + 1);
             if(str_cn==NULL){
                exit(1);
            }
            release_graph(g);
            g=A(replace(ans, t, 1,str_cn), t);
            free(str_cn);
            
            
            char* str_cnt=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(str_cnt==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,str_cnt));
            free(str_cnt);
        }


        if (first == 'S') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }
            char* str_cn=(char*)malloc(t + 1);
            if(str_cn==NULL){
                exit(1);
            }
            S(replace(ans, t, 1,str_cn), g);
            free(str_cn);

            char* str_cnt=(char*)malloc((strlen(ans) - t - 1) + 1);
             if(str_cnt==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,str_cnt));
            free(str_cnt);
        }


        if (first == 'T') {
            int t = 0;
            for (int j = 1; j < strlen(ans); j++) {
                char c = ans[j];
                if (c == 'B' || c == 'D' || c == 'S' || c == 'T' || c == '\0' || c == 'A') {
                    break;
                } else {
                    t++;
                }
            }
            char* str_cn=(char*)malloc(t + 1);
             if(str_cn==NULL){
                exit(1);
            }
            T(replace(ans, t, 1,str_cn), g);
            free(str_cn);
         char* str_cnt=(char*)malloc((strlen(ans) - t - 1) + 1);
          if(str_cnt==NULL){
                exit(1);
            }
            strcpy(ans, replace(ans, (strlen(ans) - t - 1), t + 1,str_cnt));
            free(str_cnt);
        }
    }


release_graph(g);
    

return 0;
}
