#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <omp.h>
#include "defs.h"

using namespace std;

static int threads_cnt = 8;

void loadBalancing(graph_t *G, vector<vertex_id_t> *vertexByThread)
{
	int n = G->n;
	pair<int, int> sizes[n];

	for (int i = 0; i < n; ++i)
	{		
		unsigned int s = G->myRowsIndices[i+1] - G->myRowsIndices[i];
		sizes[i] = make_pair(s, i);
	}

	sort(sizes, sizes + n);

	set< pair<int, int> > t;
	for (int i = 0; i < threads_cnt; ++i)
		t.insert(make_pair(0, i));

	for (int i = n-1; i >=0 ; --i)
	{
		pair<int, int> p = *t.begin();
		t.erase(p);
		vertexByThread[p.second].push_back(sizes[i].second);
		t.insert(make_pair(p.first + sizes[i].first, p.second));
	}
}

extern "C" void init_sssp(graph_t *G)
{
	G->myRowsIndices = (edge_id_t *)malloc((G->n + 3) * sizeof(edge_id_t));
    G->startV = (vertex_id_t *)malloc((G->m + 7) * sizeof(vertex_id_t));
    G->myWeights = (weight_t *)malloc((G->m + 7) * sizeof(weight_t));

	int temp[G->n + 30];
	for (unsigned int i = 0; i < G->n; ++i) 
		temp[i] = 0;

    for (unsigned int i = 0; i < G->n; ++i) {
    	for(unsigned int j = G->rowsIndices[i]; j < G->rowsIndices[i+1]; ++j) {
    		temp[G->endV[j]]++;
    	}
    }

    G->myRowsIndices[0] = 0;
    for (unsigned int i = 1; i <= G->n; ++i) 
		G->myRowsIndices[i] = G->myRowsIndices[i-1] + temp[i -1];

	for (unsigned int i = 0; i < G->n; ++i) 
		temp[i] = 0;
    
    for (unsigned int i = 0; i < G->n; ++i) {
    	for(unsigned int j = G->rowsIndices[i]; j < G->rowsIndices[i+1]; ++j) {
    		int en = G->endV[j];
    		edge_id_t id = G->myRowsIndices[en] + temp[en];
    		G->startV[id] = i;
    		G->myWeights[id] = G->weights[j];
    		temp[en]++;
    	}
    }
}

void init(
	graph_t *G,
	bool *wasChanged)
{
	for (unsigned int i = 0; i < G->n; ++i)
    	wasChanged[i] = false;
}

extern "C" void finalize_sssp(graph_t *G)
{
	free(G->myRowsIndices);
	free(G->myWeights);
	free(G->startV);
}



void debug_print(int n, vector<vertex_id_t> *vertexByThread)
{
	// cout << "=========================================\n";
	// for(int i = 0; i < threads_cnt; i++) {
	// 	for(unsigned int j = 0; j < vertexByThread[i].size(); ++j)
	// 	{
	// 		cout << vertexByThread[i][j] << " ";
	// 	}
	// 	cout << "\n";
	// }
	// cout << "=========================================\n";
	// cout << "=========================================\n";
	// for(int i = 0; i < n; i++) {
	// 	for(int j = 0; j < edgesByEndsVert[i].size(); ++j)
	// 	{
	// 		cout << edgesByEndsVert[i][j].second << "|"  << edgesByEndsVert[i][j].first <<" ";
	// 		cout << "\n";
	// 	}		
	// }
	// cout << "=========================================\n";
}

extern "C" void sssp(vertex_id_t root, graph_t *G, weight_t *dist, uint64_t *traversed_edges)
{
	dist[root] = 0;
    uint64_t nedges = 0;

    bool wasChanged[G->n + 2];
	init(G, wasChanged);
    // loadBalancing(G, vertexByThread);
    // debug_print(G->n, vertexByThread);

    #pragma omp parallel num_threads(threads_cnt) reduction(+:nedges)
	{
    	int myNum = omp_get_thread_num();
	    for (unsigned int k = 0; k < G->n; ++k)
	    {
	    	for (vertex_id_t en = myNum; en < G->n; en+=threads_cnt)
	    	{
	    		for (edge_id_t i = G->myRowsIndices[en]; i < G->myRowsIndices[en + 1]; ++i)
	    		{
	    			if(dist[G->startV[i]] != -1)
		    		{
		    			if(dist[en] == -1 || dist[en] > dist[G->startV[i]] + G->myWeights[i])
		    			{
		    				dist[en] = dist[G->startV[i]] + G->myWeights[i];
		    				wasChanged[k] = true;
		    			}
		    		}
		    		++nedges;
	    		}
			}
	    	#pragma omp barrier
	    	if(!wasChanged[k])
	    		break;
	    }
	}
	*traversed_edges = nedges;
}