#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct vertex_node{
    char color;
    unsigned int discover;
    unsigned int finish;
};
typedef struct vertex_node VertexNode;

struct node{
	unsigned int vertex;
    char edge_type; // T: tree edge, B:back, F: forward edge, C: cross edge.
    struct node *next;
};
typedef struct node Node;

struct adjlistnode{
    struct node *head;
    struct node *tail;
};
typedef struct adjlistnode AdjListNode;

struct graph{
    unsigned int num_vertices;
    VertexNode** vertices;
    AdjListNode** adjlist;
};
typedef struct graph Graph;

VertexNode* create_vertex_node() {
    VertexNode *new;
    new = (VertexNode*)malloc(sizeof(VertexNode));
    new -> color = 'w'; // w: white ; g: gray ; b: black
    new -> discover = 0;
    new -> finish = 0;
    return new;
}

AdjListNode* create_adjlist_node() {
    AdjListNode *new;
    new = (AdjListNode*)malloc(sizeof(AdjListNode));
    new -> head = NULL;
    new -> tail = NULL;
    return new;
}

Node* create_node(unsigned int vertex) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> vertex = vertex;
    new -> edge_type = ' ';
    new -> next = NULL;
    return new;
}

Graph* create_graph(unsigned int num_vertices) {
    Graph *new;
    new = (Graph*)malloc(sizeof(Graph));
    new -> num_vertices = num_vertices;
    new -> vertices = malloc((num_vertices + 1) * sizeof(VertexNode*));
    new -> adjlist = malloc((num_vertices + 1) * sizeof(AdjListNode*));

    for (unsigned int i=1; i<=num_vertices; i++) {
        new -> vertices[i] = create_vertex_node();
        new -> adjlist[i] = create_adjlist_node();
    }
    return new;
}

void print_graph(Graph* graph) {
    Node* tmp;
    unsigned int v;
    bool first_element;
    for (unsigned int i=1; i<=graph->num_vertices; i++) {
        printf("%u:", i);
        tmp = graph->adjlist[i]->head;
        first_element = true;
        while (tmp != NULL) {
            v = tmp->vertex;
            if (first_element) {
                printf(" %u(%u/%u)", v, graph->vertices[v]->discover, graph->vertices[v]->finish);
                first_element = false;
            } else {
                printf(" -> %u(%u/%u)", v, graph->vertices[v]->discover, graph->vertices[v]->finish);
            }
            tmp = tmp->next;
        }
        printf("\n");
    }
}

void dfs_visit(Graph* graph, unsigned int u) {
    static unsigned int time = 0;
    unsigned int v;
    Node* tmp;

    time = time + 1;
    graph->vertices[u]->discover = time;

    graph->vertices[u]->color = 'g';
    tmp = graph->adjlist[u]->head;
    while (tmp != NULL) {
        v = tmp->vertex;
        switch(graph->vertices[v]->color ) { 
            case 'w': 
                if (tmp->edge_type == ' ') {
                    tmp->edge_type = 'T'; // T: tree edge, B:back, F: forward edge, C: cross edge.
                }
                dfs_visit(graph, v);
                break; 
            case 'g': 
                if (tmp->edge_type == ' ') {
                    tmp->edge_type = 'B'; // T: tree edge, B:back, F: forward edge, C: cross edge.
                }
                break; 
            case 'b':
                if (tmp->edge_type == ' ') {
                    if (graph->vertices[u]->discover < graph->vertices[v]->discover) {
                        tmp->edge_type = 'F';
                    } else {
                        tmp->edge_type = 'C';
                    }
                }
                break;
            default:
                break;
        }
        tmp = tmp->next;
    }
    graph->vertices[u]->color = 'b';
    time = time + 1;
    graph->vertices[u]->finish = time;
}

void dfs(Graph* graph) {
    unsigned int u;
    for (u=1; u<=graph->num_vertices; u++) {
        graph->vertices[u]->color = 'w';
    }
    for (u=1; u<=graph->num_vertices; u++) {
        if (graph->vertices[u]->color == 'w') {
            dfs_visit(graph, u);
        }
    }
}

void print_edge_type(Graph* graph) {
    Node* tmp;
    unsigned int v;

    for (unsigned int i=1; i<=graph->num_vertices; i++) {
        printf("%u ", i);
        tmp = graph->adjlist[i]->head;

        while (tmp != NULL) {
            v = tmp->vertex;
            printf("%c", tmp->edge_type);
            tmp = tmp->next;
        }
        printf("\n");
    }
}

int main() {
    unsigned int V, E;
    unsigned int u, v;
    unsigned int i, j;
    Node* new = NULL;

    scanf("%u", &V);

    Graph* graph = create_graph(V);

    for (i=1; i<=V; i++) {
        scanf("%u", &E);
        u = i;
        for (j=1; j<=E; j++) {
            scanf("%u", &v);
            new = create_node(v);
            if (graph->adjlist[u]->head == NULL) {
                graph->adjlist[u]->head = new;
                graph->adjlist[u]->tail = new;
            } else {
                graph->adjlist[u]->tail->next = new;
                graph->adjlist[u]->tail = graph->adjlist[u]->tail->next;
            }
        }
    }

    dfs(graph);
    // print_graph(graph);
    print_edge_type(graph);
    return 0;
}