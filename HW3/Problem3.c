#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct vertex_node{
    char color;
    unsigned int discover;
    unsigned int finish;
    unsigned int group;
};
typedef struct vertex_node VertexNode;

struct node{
	unsigned int vertex;
    struct node *next;
};
typedef struct node Node;

struct graph{
    unsigned int num_vertices;
    VertexNode** vertices;
    Node** adjlist;
    Node** adjlist_rev;
};
typedef struct graph Graph;

VertexNode* create_vertex_node() {
    VertexNode *new;
    new = (VertexNode*)malloc(sizeof(VertexNode));
    new -> color = 'w'; // w: white ; g: gray ; b: black
    new -> discover = 0;
    new -> finish = 0;
    new -> group = 0;
    return new;
}

Node* create_node(unsigned int vertex) {
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> vertex = vertex;
    new -> next = NULL;
    return new;
}

Graph* create_graph(unsigned int num_vertices) {
    Graph *new;
    new = (Graph*)malloc(sizeof(Graph));
    new -> num_vertices = num_vertices;
    new -> vertices = malloc((num_vertices + 1) * sizeof(VertexNode*));
    new -> adjlist = malloc((num_vertices + 1) * sizeof(Node*));
    new -> adjlist_rev = malloc((num_vertices + 1) * sizeof(Node*));

    for (unsigned int i=1; i<=num_vertices; i++) {
        new -> vertices[i] = create_vertex_node();
        new -> adjlist[i] = NULL;
        new -> adjlist_rev[i] = NULL;
    }

    return new;
}

Graph* create_condense_graph(unsigned int num_vertices) {
    Graph *new;
    new = (Graph*)malloc(sizeof(Graph));
    new -> num_vertices = num_vertices;
    new -> vertices = malloc((num_vertices + 1) * sizeof(VertexNode*));
    new -> adjlist = malloc((num_vertices + 1) * sizeof(Node*));

    for (unsigned int i=1; i<=num_vertices; i++) {
        new -> vertices[i] = create_vertex_node();
        new -> adjlist[i] = NULL;
    }

    return new;
}

void print_graph(Graph* graph) {
    Node* tmp;
    unsigned int v;
    bool first_element;
    for (unsigned int i=1; i<=graph->num_vertices; i++) {
        printf("%u(%u):", i, graph->vertices[i]->group);
        tmp = graph->adjlist[i];
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

void dfs_visit(Graph* graph, unsigned int u, Node** finish_time, bool* early_stop) {
    if (early_stop != NULL && *early_stop == true) {
        return;
    }

    static unsigned int time = 0;
    unsigned int v;
    bool exist = false;
    Node* tmp;

    time = time + 1;
    graph->vertices[u]->discover = time;
    graph->vertices[u]->color = 'g';
    tmp = graph->adjlist[u];
    while (tmp != NULL) {
        v = tmp->vertex;
        if (graph->vertices[v]->color == 'w') {
            dfs_visit(graph, v, finish_time, early_stop);
        }
        tmp = tmp->next;
    }
    graph->vertices[u]->color = 'b';
    time = time + 1;
    graph->vertices[u]->finish = time;
    if (*finish_time == NULL) {
        *finish_time = create_node(u);
    } else {
        v = (*finish_time)->vertex;
        tmp = create_node(u);
        tmp->next = *finish_time;
        *finish_time = tmp;

        if (early_stop != NULL) {
            tmp = graph->adjlist[u];
            while (tmp!=NULL) {
                if (tmp->vertex == v) {
                    exist = true;
                    break;
                }
                tmp = tmp->next;
            }
            if (exist == false) {
                *early_stop = true;
            }
        }
    }
}

void dfs(Graph* graph, Node** finish_time, bool* early_stop) {
    unsigned int u;
    for (u=1; u<=graph->num_vertices; u++) {
        graph->vertices[u]->color = 'w';
    }
    for (u=1; u<=graph->num_vertices; u++) {
        if (graph->vertices[u]->color == 'w') {
            dfs_visit(graph, u, finish_time, early_stop);
            if (early_stop != NULL && *early_stop == true) {
                break;
            }
        }
    }
}

void print_finish_time(Node* finish_time) {
    bool first_element = true;
    while (finish_time!= NULL) {
        if (first_element) {
            printf("%u", finish_time->vertex);
            first_element = false;
        } else {
            printf(" -> %u", finish_time->vertex);
        }
        finish_time = finish_time->next;
    }
    printf("\n");
}

void dfs_transpose_visit(Graph* graph, unsigned int u, unsigned int* group, bool new_group) {
    Node* tmp;
    unsigned int v;

    if (new_group) {
        *group = (*group) + 1;
    }
    graph->vertices[u]->group = *group;

    graph->vertices[u]->color = 'g';
    tmp = graph->adjlist_rev[u];
    while (tmp != NULL) {
        v = tmp->vertex;
        if (graph->vertices[v]->color == 'w') {
            dfs_transpose_visit(graph, v, group, false);
        }
        tmp = tmp->next;
    }
    graph->vertices[u]->color = 'b';
}

void dfs_transpose(Graph* graph, Node* finish_time, unsigned int* group) {
    unsigned int u;
    for (u=1; u<=graph->num_vertices; u++) {
        graph->vertices[u]->color = 'w';
    }
    while (finish_time!=NULL) {
        u = finish_time->vertex;
        if (graph->vertices[u]->color == 'w') {
            dfs_transpose_visit(graph, u, group, true);
        }
        finish_time = finish_time->next;
    }
}

int main() {
    unsigned int B, A, mode;
    unsigned int u, v;
    unsigned int x, y;
    unsigned int group = 0;
    bool topological_sort_early_stop = false;
    Node* new = NULL;
    Node* tmp = NULL;
    Node* finish_time = NULL;
    Node* topological_sort_finish_time = NULL;

    scanf("%u%u%u", &B, &A, &mode);

    Graph* graph = create_graph(B);

    for (unsigned int i=1; i<=A; i++) {
        scanf("%u%u", &u, &v);

        new = create_node(v);
        new->next = graph->adjlist[u];
        graph->adjlist[u] = new;

        new = create_node(u);
        new->next = graph->adjlist_rev[v];
        graph->adjlist_rev[v] = new;
    }
    
    dfs(graph, &finish_time, NULL);
    dfs_transpose(graph, finish_time, &group);
    printf("%u\n", group);

    // print_graph(graph);
    // print_finish_time(finish_time);
    // printf("\n");

    if (mode==2) {
        Graph* condense_graph = create_condense_graph(group);
        while (finish_time!=NULL) {
            u = finish_time->vertex;
            x = graph->vertices[u]->group;

            tmp = graph->adjlist[u];
            while (tmp != NULL) {
                v = tmp->vertex;
                y = graph->vertices[v]->group;
                if (x!=y) {
                    new = create_node(y);
                    new->next = condense_graph->adjlist[x];
                    condense_graph->adjlist[x] = new;
                }
                tmp = tmp->next;
            }
            finish_time = finish_time->next;
        }

        dfs(condense_graph, &topological_sort_finish_time, &topological_sort_early_stop);
        if (topological_sort_early_stop) {
            printf("0\n");
        } else {
            printf("1\n");
        }
        // print_graph(condense_graph);
        // print_finish_time(topological_sort_finish_time);
        // printf("\n");
    }
    return 0;
}