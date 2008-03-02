#ifndef kdtree_p
#define kdtree_p

#include "../Vector3.h"
#include <list>
using namespace std;

typedef struct Position
{
  Vector3 *node;
  float pos[3];
  short plane;
} Position;



typedef struct NearestsPositions
{
  int max;
  int found;
  int got_heap;
  float pos[3];
  float *dist2;
  const Position **index;
} NearestPositions;

class KdTree_pfVec
{
public:
  KdTree_pfVec(int maxPositions);
  ~KdTree_pfVec();

  void store(Vector3 *node);
  void balance();
  
  bool exists(Vector3 *n);
  list<Vector3*>* find(Vector3 *n,float max_dist,int npositions);

  void locate_positions(NearestPositions *const np,const int index) const;
  int getSize()
    {
      return stored_positions;
    }
    Position *positions;
private:
  void balance_segment(Position **pbal, Position **porg, const int index, const int start, const int end);
  void median_split(Position **p, const int start, const int end, const int median, const int axis);

  int stored_positions;


  int half_stored_positions;
  int max_positions;
  int prev_scale;


  float bbox_min[3];
  float bbox_max[3];
};

#endif
