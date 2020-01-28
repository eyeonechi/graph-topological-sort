/* COMP20007 Design of Algorithms                      *
 * Semester 1 2016                                     *
 * Assignment 1                                        *
 * Ivan Ken Weng Chee (ichee@student.unimelb.edu.au)   *
 * This module provides graph sorting and verification */

/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "toposort.h"

/***************************************************************************/

/* Array of boolean structs to keep track of visited vertices */
typedef struct bool_t* Bool;

struct bool_t {
	bool temp; /* Temporarily visited vertex */
	bool perm; /* Permanently visited vertex */
};

/***************************************************************************/

/* Recursively visits vertices and prepends them to output */
void visit_node(void *vertex, List *list, Bool marks) {
	/* Check if vertex has been visited */
	if (marks[((Vertex)vertex)->id].temp || marks[((Vertex)vertex)->id].perm) {
		return;
	}

	List out = NULL;
	marks[((Vertex)vertex)->id].temp = true;

	/* Visits outgoing vertices from a vertex */
	for (out = ((Vertex)vertex)->out; out && out->data; out = out->next) {
		visit_node(out->data, list, marks);
	}

	marks[((Vertex)vertex)->id].perm = true;
	marks[((Vertex)vertex)->id].temp = false;
	prepend(list, vertex);
}

/***************************************************************************/

/* Returns a list of topologically sorted vertices using the DFS method */
List dfs_sort(Graph graph) {
	int i;
	Bool marks = (Bool)calloc(sizeof(*marks), graph->order * sizeof(*marks));
	assert(marks);
	List dfs = NULL;

	/* Visits each unvisited node */
	for (i = 0; i < graph->order; i ++) {
		if (!marks[i].perm) {
			visit_node(&graph->vertices[i], &dfs, marks);
		}
	}

	/* Check for empty output */
	if (!dfs && !dfs->data) {
		fprintf(stderr, "Not possible to sort topologically\n");
		exit(EXIT_FAILURE);
	}

	free(marks);
	marks = NULL;
    return dfs;
}

/***************************************************************************/

/* Returns a list of topologically sorted vertices using the Kahn method */
List kahn_sort(Graph graph) {
	List kahn = NULL, source = NULL;
	Vertex vertex = NULL, deleted = NULL;
	int i;

	/* Prepends vertices without incoming edges to 'start' list */
	for (i = 0; i < graph->order; i ++) {
		if (!graph->vertices[i].in) {
			prepend(&source, &graph->vertices[i]);
		}
	}

	/* Graph contains cycles */
	if (!source) {
		fprintf(stderr, "Graph is cyclic\n");
		exit(EXIT_FAILURE);
	}

	/* While graph still has source vertices */
	while (source && source->data) {
		vertex = pop(&source);
		prepend(&kahn, vertex);

		/* Deletes each outgoing edge from a vertex */
		while (vertex->out && vertex->out->data) {
			deleted = pop(&vertex->out);
			del(ptr_eq, vertex, &deleted->in);
			
			/* Found new source vertex */
			if (!deleted->in) {
				prepend(&source, deleted);
			}
		}
	}

	/* Check for empty output */
	if (!kahn && !kahn->data) {
		fprintf(stderr, "Not possible to sort topologically\n");
		exit(EXIT_FAILURE);
	}

    return reverse(kahn);
}

/***************************************************************************/

/* Uses graph to verify vertices are topologically sorted */
bool verify(Graph graph, List vertices) {
	List visit = NULL, out = NULL;
	Vertex vertex = NULL;

	/* Traverses through list of vertices */
	for (; vertices && vertices->data; vertices = vertices->next) {
		vertex = vertices->data;
		prepend(&visit, vertex);

		/* Checks if any connected vertex has been visited */
		for (out = vertex->out; out && out->data; out = out->next) {

			/* Incorrect topological sort */
			if (find(ptr_eq, out->data, visit)) {
				return false;
			}
		}
	}

	/* Correct topological sort */
    return true;

    /* Silence unused warning - redundant code */
    graph->order = graph->order;
}
