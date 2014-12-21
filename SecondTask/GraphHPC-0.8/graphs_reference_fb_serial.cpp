#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "defs.h"

using namespace std;

extern "C" void init_sssp(graph_t *G)
{
}

extern "C" void finalize_sssp(graph_t *G)
{
}

extern "C" void sssp(vertex_id_t root, graph_t *G, weight_t *dist, uint64_t *traversed_edges)
{
	dist[root] = 0;
    uint64_t nedges = 0;

    for (unsigned int k = 0; k < G->n; ++k)
    {
		bool wasChanged = false;

    	for (vertex_id_t st = 0; st < G->n; ++st)
    	{
    		if(dist[st] != -1)
    		{
    			for (edge_id_t i = G->rowsIndices[st]; i < G->rowsIndices[st + 1]; ++i)
	    		{
	    			vertex_id_t en = G->endV[i];
	    			weight_t w = G->weights[i];
	    			if(dist[en] == -1 || dist[en] > dist[st] + w)
	    			{
	    				dist[en] = dist[st] + w;
	    				wasChanged = true;
	    			}
	    			++nedges;
	    		}
    		}
		}
    	if(!wasChanged)
    		break;
	}

	*traversed_edges = nedges;
}