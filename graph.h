

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


Graph *build_graph();
void release_graph(struct graph_ *G);
void insert_edge(Node *src, Node *dest, double weight);
void remove_edge(Graph *G, Node *src, Node *dest);
Node *init_node(Graph *g, char data);
void insert_node(Graph *g, Node *new_node) ;
int getNodeID(Graph *G, Node *node);
void remove_node(Graph *g, Node *node);
int Dijsktra(Graph *g, Node *src, Node *dest);
Graph * build_graph_cmd(char *arr, int len);
void create_new_node(char *arr,Graph *graph);
void del_node(char *arr,Graph *g);
void Shortest_path(char *arr,Graph *g);
void reverse(char *arr, int size, Node *src);
int findpath(Graph *g, char *list_of_node, int size)
int tsp(Graph *g, char *list_of_node, int size);
void TSP_cmd(char *ans,Graph *g);
