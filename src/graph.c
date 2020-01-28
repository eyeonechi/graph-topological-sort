/* COMP20007 Design of Algorithms                    *
 * Semester 1 2016                                   *
 * Assignment 1                                      *
 * Ivan Ken Weng Chee (ichee@student.unimelb.edu.au) *
 * This module provides operations on graphs         */

/***************************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "graph.h"

#define MAX_LINE_LEN 256

/***************************************************************************/

/* Returns a pointer to a new graph with order vertices */
Graph new_graph(int order) {
	int i;
    Graph graph = (Graph)calloc(sizeof(*graph), sizeof(*graph));
    assert(graph);
    graph->vertices = (Vertex)calloc(order, order * sizeof(*graph->vertices));
    assert(graph->vertices);

    /* Allocates memory for the label */
    for (i = 0; i < order; i ++) {
    	graph->vertices[i].label =
    		(char *)malloc(MAX_LINE_LEN * sizeof(*graph->vertices[i].label));
    	assert(graph->vertices[i].label);
    }

    graph->order = order;
	return graph;
}

/***************************************************************************/

/* Returns whether aim and vertex are pointing to the same location */
bool ptr_eq(void *aim, void *vertex) {
	return (aim == vertex)
		? true
		: false;
}

/***************************************************************************/

/* Returns whether aim and vertex have the same id */
bool id_eq(void *aim, void *vertex) {
	return (((Vertex)aim)->id == ((Vertex)vertex)->id)
		? true
		: false;
}

/***************************************************************************/

/* Add the edge from v1 to v2 to graph */
void add_edge(Graph graph, int v1, int v2) {
	insert(&graph->vertices[v2], &graph->vertices[v1].out);
	insert(&graph->vertices[v1], &graph->vertices[v2].in);
	graph->size ++;
}

/***************************************************************************/

/* Free the memory allocated to graph */
void free_graph(Graph graph) {
	int i;

	/* Frees elements in each vertex */
	for (i = 0; i < graph->order; i ++) {
		free_list(graph->vertices[i].in);
		graph->vertices[i].in = NULL;
		free_list(graph->vertices[i].out);
		graph->vertices[i].out = NULL;
		free(graph->vertices[i].label);
		graph->vertices[i].label = NULL;
	}

	free(graph->vertices);
	graph->vertices = NULL;
	free(graph);
	graph = NULL;
}
