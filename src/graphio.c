/* COMP20007 Design of Algorithms                                   *
 * Semester 1 2016                                                  *
 * Assignment 1                                                     *
 * Ivan Ken Weng Chee (ichee@student.unimelb.edu.au)                *
 *                                                                  *
 * This module provides all the IO functionality related to graphs. */

/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "graphio.h"

#define MAX_LINE_LEN 256

/***************************************************************************/

/* Loads the graph from input */
Graph load_graph(char *input) {
	FILE *file = fopen(input, "r");
	int i, order, start, end;
	
	/* Gets the graph order and initialises graph */
	fscanf(file, "%d\n", &order);
	Graph graph = new_graph(order);

	/* Labels each vertex in the graph */
	for (i = 0; i < graph->order; i ++) {
		graph->vertices[i].id = i;
		fscanf(file, "%s\n", graph->vertices[i].label);
	}

	/* Parses graph edges from input file */
	while (fscanf(file, "%d %d\n", &start, &end) == 2) {
		add_edge(graph, start, end);
	}

	fclose(file);
    return graph;
}

/***************************************************************************/

/* Printing-related strings */
#define ARROW   " -> {"
#define BRACE   "}"
#define DIGRAPH "digraph {\n"
#define NEWLINE	"\n"
#define SPACE   " "

/***************************************************************************/

/* Prints the graph */
void print_graph(char *output, Graph graph) {
	FILE *file = fopen(output, "w");
	List list = NULL;
	int i;

	/* Prints left hand column of vertices */
	fprintf(file, DIGRAPH);
	for (i = 0; i < graph->order; i ++) {
		fprintf(file, SPACE);
		fprintf(file, SPACE);
		print_vertex_label(file, &graph->vertices[i]);

		/* Returns a copy of outgoing vertices to be printed */
		if ((list = reverse(graph->vertices[i].out))) {
			fprintf(file, ARROW);

			/* Prints outgoing vertices of a vertex */
			for (; list && list->data; list = list->next) {
				fprintf(file, SPACE);
				print_vertex_label(file, list->data);
				fprintf(file, SPACE);
			}

			fprintf(file, BRACE);
		}

		fprintf(file, NEWLINE);
	}

	fprintf(file, NEWLINE);
	fprintf(file, BRACE);
	fclose(file);
}

/***************************************************************************/

/* Prints the destination vertex label surrounded by spaces */
void print_vertex_label(FILE *file, void *vertex) {
	fprintf(file, "%s", ((Vertex)vertex)->label);
}

/***************************************************************************/

/* Prints the id of a vertex then a newline */
void print_vertex_id(FILE *file, void *vertex) {
    if (vertex) {
        fprintf(file, "%d\n", ((Vertex)vertex)->id);
    }
}

/***************************************************************************/

/* Returns a sequence of vertices read from file */
List load_vertex_sequence(FILE *file, Graph graph) {
    const char *err_duplicate = "Error: duplicate vertex %d %s\n";
    const char *err_order = "Error: graph order %d, loaded %d vertices\n";
    List list = NULL;
    int id;

    while(fscanf(file, "%d\n", &id) == 1) {
        assert(id >= 0);
        assert(id < graph->order);

        if (!insert_if(id_eq, graph->vertices + id, &list)) {
            fprintf(stderr, err_duplicate, id, graph->vertices[id].label);
            exit(EXIT_FAILURE);
        }
    }

    if (len(list) != graph->order) {
        fprintf(stderr, err_order, graph->order, len(list));
        exit(EXIT_FAILURE);
    }

    return list;
}
