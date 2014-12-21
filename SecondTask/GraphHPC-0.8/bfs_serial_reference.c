#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include "defs.h"

static vertex_id_t* q;
static vertex_id_t* q_next;

void init_bfs(graph_t *G)
{
    q = (vertex_id_t *)malloc(sizeof(vertex_id_t) * G->n);
    assert(q != NULL); 
    q_next = (vertex_id_t *)malloc(sizeof(vertex_id_t) * G->n);
    assert(q_next != NULL);
}

void bfs(uint8_t *marked, vertex_id_t *validateNLevels, int *validateNLevelsLength, vertex_id_t root, graph_t *G, edge_id_t *traversed_edges)
{
    edge_id_t traversedEdgesLevel = 0; /* traversed edges at each level */
    edge_id_t nedges = 0;
    vertex_id_t *w; 

    marked[root] = 1;
    q[0] = root;
    vertex_id_t qNextSize = 1, qSize = 1; 
    uint8_t numberOfLevel = 0; /* level number */
    int counterForValidate = 0;

    while (qNextSize > 0) {
        if (counterForValidate == MAX_NLEVELS) {
            fprintf(stderr, "number of levels in bfs > MAX_NLEVELS (size of array validateNLevels)\n");
            exit(1);
        }
        /* saving number of vertices in level for validation */
        validateNLevels[counterForValidate] = qSize;
        ++counterForValidate;

        qNextSize = 0;
        traversedEdgesLevel = 0;
        for (vertex_id_t i = 0; i < qSize; i++) {
            for (edge_id_t j = G->rowsIndices[q[i]]; j < G->rowsIndices[q[i]+1]; j++) {
                if (!marked[G->endV[j]]) {
                    marked[G->endV[j]] = 1;
                    q_next[qNextSize] = G->endV[j];
                    qNextSize++;
                }
                traversedEdgesLevel++;
            }
        }
        w = q;
        q = q_next;
        q_next = w;
        qSize = qNextSize;
        numberOfLevel++;
        nedges = nedges + traversedEdgesLevel;
    }
    *traversed_edges = nedges;
    *validateNLevelsLength = numberOfLevel;
}

void finalize_bfs()
{
    free(q);
    free(q_next);
}

