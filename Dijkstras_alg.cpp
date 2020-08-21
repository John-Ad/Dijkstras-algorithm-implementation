#include <iostream>
#include <vector>

#define INF 999

using namespace std;

//adjacency list graph implementation for a grid based graph	//stores only the adjacent vertices of each vertex
class graph
{
private:
	struct vertex {
		char v;
		bool visited;
		int distFromSrc;
		vertex* prev;
	};
	struct adjVert {
		char v;
		int weight;
	};

	vector<vertex> vertices;
	vector<vector<adjVert>> adjList;

	vector<adjVert> path;

	int vertSize;
public:
	graph(char verts[], int nVerts) {
		vertSize = nVerts;

		vertex v;
		v.visited = false;
		v.distFromSrc = INF;
		v.prev = NULL;

		for (int i = 0; i < nVerts; i++)
		{
			v.v = verts[i];
			vertices.push_back(v);
		}
	}

	void createEdges(int x) {						// refer to a drawn grid consisting of 15 vertices to help understand the algorithm
		int xHolder{ x - 1 };

		adjVert v;

		int weight = 1;
		int upW = 8;				//controls weights for upwards edges
		int leftW = 2;				//controls weights for leftwards edges

		for (int i = 0; i < vertices.size(); i++)
		{
			adjList.push_back(vector<adjVert>());
		}

		for (int i = 0; i < adjList.size(); i++)			// refer to a drawn grid when working through this algorithm
		{
			for (int e = 0; e < adjList.size(); e++)
			{
				if (e == i + 1 && i != xHolder)		// links a vertex to the one to the right of it
				{
					v.v = vertices[e].v;
					v.weight = weight;

					adjList[i].push_back(v);
					weight += 1;
				}
				else if (e == i - 1 && e != xHolder)	// links a vertex to the one to the left of it
				{
					if (i > vertSize - x)
						leftW = 1;

					v.v = vertices[e].v;
					v.weight = (weight - leftW);

					adjList[i].push_back(v);
				}
				else if (e == i + x)				//links a vertex to the one under it
				{
					v.v = vertices[e].v;
					v.weight = weight;

					adjList[i].push_back(v);
					weight += 1;
				}
				else if (e == i - x)				//links a vertext to the one above it
				{
					v.v = vertices[e].v;
					if (e == xHolder)			//refer to a drawn 5x3 grid
						v.weight = (weight - upW) - 1;
					else
						v.weight = weight - upW;

					adjList[i].push_back(v);
				}

				if (e == xHolder + 1)					//xholder signals the end of a row. Doubling it moves the algorithm to the next row
				{
					xHolder += 5;
				}
			}

			if (i >= vertSize - x)		//changing upW in the e loop does not correctly decrease the variable
				upW -= 1;		
			leftW = 2;
			xHolder = x - 1;
		}
	}
	int adjIndices(int v, vector<int>& adjInd) {
		if (adjInd.size() > 0)
			adjInd.clear();

		for (int i = 0; i < adjList[v].size(); i++)
		{
			//cout << adjList[v][i].v << ", " << adjList[v][i].weight << endl;
			for (int e = 0; e < vertices.size(); e++)
			{
				if (adjList[v][i].v == vertices[e].v)
					adjInd.push_back(e);
			}
		}

		return adjInd.size();
	}
	int adjEdges(int e, vector<adjVert>& adjEdg) {
		for (int i = 0; i < adjList[e].size(); i++)
		{
			adjEdg.push_back(adjList[e][i]);
		}

		return adjEdg.size();
	}

	void shortestPath(int start, int end) {
		int shortestDist = INF;
		int lowestVert = 0;
		int nextVert = 0;

		int numOfEdges = 0;
		int numOfIndices = 0;

		vector<int>adjInd;
		vector<adjVert>adjEdg;
		vector<vertex>path;
		
		//debugging variables
		//vector<vertex>data;
		//-------------------

		if (end != INF)
		{
			vertices[start].distFromSrc = 0;
		}

		adjIndices(start, adjInd);					//gets the indices of the adjacent vertices	eg adjInd{0,2,6} -> vertices[adjInd[0]] = vertices[0]
		numOfEdges = adjEdges(start, adjEdg);	//gets the adjacent edges of current vert

		for (int i = 0; i < numOfEdges; i++)		//numOfEdges remains the same while size of adjEdg will decrease
		{
			for (int e = 0; e < adjEdg.size(); e++)
			{
				if (adjEdg[e].weight + vertices[start].distFromSrc < shortestDist)		//finds the lowest weight
				{
					shortestDist = adjEdg[e].weight + vertices[start].distFromSrc;
					lowestVert = e;														// adjInd and adjEdg have the same array positions
				}																		// both can make use of lowest vert
			}

			if (shortestDist < vertices[adjInd[lowestVert]].distFromSrc)
			{																	// updates the adjacent vert if its distance is higher than the one just found
				vertices[adjInd[lowestVert]].distFromSrc = shortestDist;
				vertices[adjInd[lowestVert]].prev = &vertices[start];

				//debugging
				//data.push_back(vertices[adjInd[lowestVert]]);
			}

			adjEdg.erase(adjEdg.begin() + lowestVert);		//	both adjEdg and adjInd have to decrease in size to keep the same array positions
			adjInd.erase(adjInd.begin() + lowestVert);		//	if only adjEdg is decreased, the index lowestVert found will not match the index of adjInd and the program will produce incorrect results

			shortestDist = INF;
		}

		/*
		//debugging loop
		cout << endl;
		for (int i = 0; i < data.size(); i++)
		{
			cout << "vertex: " << data[i].v << ", " << data[i].distFromSrc << ", " << data[i].prev->v << endl;
		}
		cout << endl;
		//--------------

		
			to do:
			-possibly change algorithm to use lowest weight instead of shortest distFromSrc
		*/

		vertices[start].visited = true;		//all adjacent vertices have been evaluated. This vertex no longer needs to be visited

		//	from here the function will be called recursively until all paths are traversed:
		numOfIndices = adjIndices(start, adjInd);
		for (int i = 0; i < numOfIndices; i++)
		{
			for (int e = 0; e < adjInd.size(); e++)
			{
				if (vertices[adjInd[e]].visited == false && vertices[adjInd[e]].distFromSrc < shortestDist)		//shortestDist is still INF from the previous loop
				{
					shortestDist = vertices[adjInd[e]].distFromSrc;
					nextVert = e;
				}
			}
			if (shortestDist != INF)
			{
				shortestPath(adjInd[nextVert], INF);		//end is set to INF to indicate that the function is called recursively
				adjInd.erase(adjInd.begin() + nextVert);
			}
			shortestDist = INF;
		}

		//display path
		if (end != INF)				//end will not be INF on the very first call to the function
		{
			cout << "start: " << vertices[start].v << endl << "end: " << vertices[end].v << endl << endl << "Path: " << endl << endl;

			path.push_back(vertices[end]);
			while (path[path.size()-1].prev != NULL)
			{
				path.push_back(*path[path.size() - 1].prev);
			}

			for (int i = path.size() - 1; i >= 0; i--)
			{
				cout << path[i].v << endl;
			}
		}
	}
};

int main()
{
	char verts[] = "ABCDEFGHIJKLMNO";

	graph g(verts, 15);				// 15 is the number of vertices
	g.createEdges(5);				// 5 is the width. This specific graph will therefore be 5x3 in size
	g.shortestPath(5, 7);			// eg 3=C, 11=L

	cin.get();
	return 0;
}
