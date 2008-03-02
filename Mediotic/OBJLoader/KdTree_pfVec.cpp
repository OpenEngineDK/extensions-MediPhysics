#include "kdTree_pfVec.h"
//#ifdef _WIN32
//#define alloca malloc
//#endif

#include <list>
#include <iostream>
using namespace std;

KdTree_pfVec::KdTree_pfVec(const int maxPositions)
{
	stored_positions = half_stored_positions = 0;
	prev_scale = 1;
	max_positions = maxPositions;

	positions = (Position*)malloc( sizeof( Position ) * (max_positions + 1 ) );

	if (positions == NULL) 
	{
		printf("Out of memort initializing KdTree_pfVec\n");
		exit(-1);
	}

	bbox_min[0] = bbox_min[1] = bbox_min[2] = 1e8f;
	bbox_max[0] = bbox_max[1] = bbox_max[2] = -1e8f;
}

KdTree_pfVec :: ~KdTree_pfVec()
{
	free(positions);
}


bool KdTree_pfVec::exists(Vector3 *n)
{
	list<Vector3*>* l = find(n,0.0000001,1);
	if (l->begin()==l->end())
	{
		delete(l);
		return false;
	}
	else
	{
		delete(l);
		return true;		    
	}
}

list<Vector3*>* KdTree_pfVec::find(Vector3 *n,float max_dist,int npositions)
{
	NearestPositions np;

	np.dist2 = (float*)alloca( sizeof(float)*(npositions+1) );
	np.index = (const Position**)alloca( sizeof(Position*)*(npositions+1) );

	np.pos[0] = n->e[0];
	np.pos[1] = n->e[1];
	np.pos[2] = n->e[2];

	np.max = npositions;
	np.found = 0;
	np.got_heap = 0;
	np.dist2[0] = max_dist*max_dist;

	locate_positions( &np,1);

	list<Vector3*> * returnList = new list<Vector3*>;

	for(int i = 1; i <= np.found; i++)
	{
		returnList->push_back(np.index[i]->node);
	}

	return returnList;


}

void KdTree_pfVec :: locate_positions(NearestPositions *const np, const int index) const
{
	const Position *p = &positions[index];
	float dist1;

	if( index < half_stored_positions)
	{
		dist1 = np->pos[p->plane] - p->pos[p->plane];

		if (dist1>0.0)
		{
			locate_positions(np,2*index+1);
			if (dist1*dist1 < np->dist2[0] )
				locate_positions(np, 2*index);
		}
		else
		{
			locate_positions(np, 2*index);
			if (dist1*dist1 < np->dist2[0] )
				locate_positions(np, 2*index+1);
		}
	}

	dist1 = p->pos[0] - np->pos[0];
	float dist2 = dist1*dist1;
	dist1 = p->pos[1] - np->pos[1];
	dist2 += dist1*dist1;
	dist1 = p->pos[2] - np->pos[2];
	dist2 += dist1*dist1;

	if (dist2 < np->dist2[0] )
	{
		if ( np->found < np->max )
		{
			np->found++;
			np->dist2[np->found] = dist2;
			np->index[np->found] = p;
		}
		else
		{
			int j,parent;
			if (np->got_heap==0)
			{
				float dst2;
				const Position *phot;
				int half_found = np->found>>1;
				for(int k=half_found; k >= 1; k--)
				{
					parent = k;
					phot = np->index[k];
					dst2 = np->dist2[k];
					while ( parent <= half_found)
					{
						j = parent+parent;
						if (j <np->found && np->dist2[j]<np->dist2[j+1])
							j++;
						if (dst2 >= np->dist2[j])
							break;
						np->dist2[parent] = np->dist2[j];
						np->index[parent] = np->index[j];
						parent = j;
					}
					np->dist2[parent] =dst2;
					np->index[parent] = phot;
				}
				np->got_heap = 1;
			}

			parent = 1;

			j = 2;
			while ( j <= np->found )
			{
				if (j < np->found && np->dist2[j] < np->dist2[j+1] )
					j++;
				if (dist2 > np->dist2[j] )
					break;
				np->dist2[parent] = np->dist2[j];
				np->index[parent] = np->index[j];
				parent = j;
				j += j;
			}

			np->index[parent] = p;
			np->dist2[parent] = dist2;

			np->dist2[0] = np->dist2[1];
		}
	}
}

void KdTree_pfVec :: store(Vector3 *n)
{
	if (stored_positions>max_positions)
	{
		cerr << "ERROR: Not enough room in the KdTree_pfVec" << endl;
		return;
	}


	stored_positions++;
	Position *const node = &positions[stored_positions];

	float pos[3];
	pos[0]=n->e[0];
	pos[1]=n->e[1];
	pos[2]=n->e[2];

	node->node = n;

	for(int i = 0; i<3; i++)
	{
		node->pos[i] = pos[i];

		if (node->pos[i] < bbox_min[i])
			bbox_min[i] = node->pos[i];

		if (node->pos[i] > bbox_max[i])
			bbox_max[i] = node->pos[i];
	}
}

void KdTree_pfVec :: balance()
{
	if (stored_positions>1)
	{
		Position **pa1 = (Position**)malloc(sizeof(Position*)*(stored_positions+1));
		Position **pa2 = (Position**)malloc(sizeof(Position*)*(stored_positions+1)); 

		for (int i = 0; i <= stored_positions; i++)
			pa2[i] = &positions[i];

		balance_segment(pa1,pa2,1,1,stored_positions);
		free(pa2);

		int d, j=1, foo=1;
		Position foo_position = positions[j];

		for(int i =1; i <= stored_positions; i++)
		{
			d = pa1[j]-positions;
			pa1[j] = NULL;
			if (d != foo)
				positions[j] = positions[d];
			else
			{
				positions[j] = foo_position;

				if (i<stored_positions)
				{
					for (; foo<=stored_positions; foo++)
						if (pa1[foo] != NULL)
							break;
					foo_position = positions[foo];
					j = foo;
				}
				continue;
			}
			j = d;
		}
		free(pa1);
	}    
	half_stored_positions = stored_positions/2-1;      
}


#define swap(ph,a,b) { Position *ph2=ph[a]; ph[a]=ph[b]; ph[b] = ph2; }

void KdTree_pfVec :: median_split(Position **p, const int start, const int end, const int median, const int axis)
{
	int left = start;
	int right = end;

	while (right > left)
	{
		const float v = p[right]->pos[axis];
		int i = left-1;
		int j = right;
		for (;;) 
		{
			while (p[++i]->pos[axis] < v) ;
			while (p[--j]->pos[axis] > v && j > left) ;

			if ( i >= j)
				break;
			swap(p,i,j);
		}

		swap(p,i,right);
		if (i >= median)
			right = i-1;
		if ( i <= median)
			left = i+1;
	}
}

void KdTree_pfVec :: balance_segment(Position **pbal, Position **porg, const int index, const int start, const int end)
{
	int median = 1;

	while ((4*median) <= (end-start+1))
		median += median;

	if ((3*median) <= (end-start+1))
	{
		median += median;
		median += start-1;
	}
	else
		median = end-median+1;

	int axis = 2;

	if ((bbox_max[0]-bbox_min[0])>(bbox_max[1]-bbox_min[1]) &&
		(bbox_max[0]-bbox_min[0])>(bbox_max[2]-bbox_min[2]))
		axis = 0;
	else if ((bbox_max[1]-bbox_min[1])>(bbox_max[2]-bbox_min[2]))
		axis = 1;

	median_split( porg, start, end, median, axis);

	pbal[index] = porg[median];
	pbal[index]->plane = axis;

	if (median > start)
	{
		if (start < median-1)
		{
			const float tmp = bbox_max[axis];
			bbox_max[axis] = pbal[index]->pos[axis];
			balance_segment(pbal,porg,2*index,start,median-1);
			bbox_max[axis] = tmp;
		}
		else
		{
			pbal[2*index] = porg[start];
		}
	}

	if (median < end)
	{
		if (median+1 <end)
		{
			const float tmp = bbox_min[axis];
			bbox_min[axis] = pbal[index]->pos[axis];
			balance_segment(pbal,porg,2*index+1,median+1,end);
			bbox_min[axis] = tmp;
		}
		else
		{
			pbal[2*index+1] = porg[end];	  
		}
	}
}
