#include <list>
#include <vector>
#include <algorithm>
#include <math.h>
#include <limits>
#include <iostream>

#include "../Vector3.h"

#include "BVHTriangle.h"
#include "BVHBoundingSphere.h"

class BVHBoundingGeo
{
public:
	BVHBoundingGeo(){};
	virtual ~BVHBoundingGeo(){};
	
    virtual void init(BVHTriangle tri);
	virtual void init(BVHBoundingGeo g1, BVHBoundingGeo g2);
	virtual void glDraw(float c);
};

/*
class BVHBoundingBox
{
public:
void init(BVHTriangle tri);
void init(BVHBoundingGeo g1, BVHBoundingGeo g2);

}*/




template<typename BoundingGeoT>
class BVHNode
{
protected:
	BoundingGeoT boundingSphere;
public:
	virtual BVHTriangleHit intersect(BVHRay r)=0;
	virtual void glDraw(float c)=0;

	BVHNode() {};
	virtual ~BVHNode() {};


	BoundingGeoT getBoundingSphere()
	{
		return boundingSphere;
	}

	void setBoundingSphere(BoundingGeoT boundingSphere)
	{
		this->boundingSphere = boundingSphere;
	}

	virtual int countLeafs()=0;
	virtual int countMaxDepth()=0;


};

template<typename BoundingGeoT>
class BVHBranch : public BVHNode<BoundingGeoT>
{
public:
	BVHNode<BoundingGeoT> *right;
	BVHNode<BoundingGeoT> *left;
	BVHBranch(BVHNode<BoundingGeoT> *right, BVHNode<BoundingGeoT> *left, BoundingGeoT boundingSphere)
	{
		this->right = right;
		this->left = left;
		this->setBoundingSphere(boundingSphere);
	}

	BVHTriangleHit intersect(BVHRay r)
	{
		BoundingGeoT b = this->getBoundingSphere();
		if(b.intersect(r))
		{
			BVHTriangleHit lHit = left->intersect(r);
			BVHTriangleHit rHit = right->intersect(r);
			if (fabs(lHit.distance)<fabs(rHit.distance)) return lHit; else return rHit;
		}
		return BVHTriangleHit();
	}

	void glDraw(float c)
	{
		this->getBoundingSphere().glDraw(c);
		c=c+0.05f;
		right->glDraw(c);
		left->glDraw(c);

	}

	int countLeafs()
	{
		return right->countLeafs() + left->countLeafs();	
	}

	int countMaxDepth()
	{
		int r = right->countMaxDepth()+1;
		int l = left->countMaxDepth()+1;
		return r>l ? r : l;
	}
};

template<typename BoundingGeoT>
class BVHLeaf : public BVHNode<BoundingGeoT>
{
private:
	BVHTriangle triangle;
public:
	BVHLeaf(BVHTriangle triangle)
	{
		this->triangle = triangle;
		BoundingGeoT b;
		b.init(triangle);
		this->setBoundingSphere(b);

	};
	BVHTriangleHit intersect(BVHRay r)
	{
		BoundingGeoT b= this->getBoundingSphere();

		if(b.intersect(r))
		{
			float d;
			bool intersection;
			intersection = this->triangle.intersect(r,&d);

			if(intersection)
			{
				BVHTriangleHit ret = BVHTriangleHit(d,triangle,r.getOrigin()+r.getDirection()*d);
				return ret;
			}
		}



		return BVHTriangleHit();
	}

	void glDraw(float c)
	{
		this->getBoundingSphere().glDraw(c);
	}

	int countLeafs()
	{
		return 1;	
	}

	int countMaxDepth()
	{
		return 1;
	}
};


bool OrderLessX(BVHTriangle left, BVHTriangle right);
bool OrderLessY(BVHTriangle left, BVHTriangle right);
bool OrderLessZ(BVHTriangle left, BVHTriangle right);

template<typename BoundingGeoT>
class BVH
{
private:

	std::vector<BVHTriangle> triangles;
	BVHNode<BoundingGeoT> *root;
public:

	BVH(){
		root = NULL;
	};
	void insert(BVHTriangle t){
		triangles.push_back(t);
	};
	BVHNode<BoundingGeoT> *buildHierarchy(int begin,int end, int i){
		if (!(begin!=end && end>=0 && begin>=0))
		{
			std::cerr << begin << " " << end << std::endl;
			exit(1);
		}

		if (begin==end-1)
		{
			return new BVHLeaf<BoundingGeoT>(triangles[begin]); // boundingsphere is set by BVHLeaf
		}
		else
		{

			switch(i%3)
			{
			case 0:
				std::sort(triangles.begin()+begin,triangles.begin()+end,OrderLessX);
				break;
			case 1:
				std::sort(triangles.begin()+begin,triangles.begin()+end,OrderLessY);
				break;
			case 2:
				std::sort(triangles.begin()+begin,triangles.begin()+end,OrderLessZ);
				break;
			}
			i++;


			BVHNode<BoundingGeoT> *left = buildHierarchy(begin,begin + (end-begin)/2,i); //end not included in sort
			BVHNode<BoundingGeoT> *right = buildHierarchy(begin + (end-begin)/2,end,i);

			BoundingGeoT b;
			b.init(left->getBoundingSphere(),right->getBoundingSphere());
			return new BVHBranch<BoundingGeoT>(left,right,b);

		}

	};
	void buildHierarchy() 
	{
		root = this->buildHierarchy(0,(int)triangles.size(),0); 
		// report stats
		/*std::cout << "BVH: triangles:      " << triangles.size() << std::endl;

		std::cout << "BVH: Count Leafs     " << root->countLeafs()<< std::endl;
		std::cout << "BVH: Count Max Depth " << root->countMaxDepth() << std::endl;*/

	}

	BVHTriangleHit rayIntersection(BVHRay R){
		return root->intersect(R);
	};

	BVHTriangleHit rayIntersectionBRUTEFORCE(BVHRay r)
	{
		float minDist=-64000;
		BVHTriangle minTriangle;
		Vector3 coord;

		std::vector<BVHTriangle>::iterator i;
		for(i=triangles.begin();i!=triangles.end();i++)
		{
			float testDist;
			BVHTriangle tri = *i;
			if (tri.intersect(r,&testDist) && fabs(testDist)<fabs(minDist))
			{
				minTriangle = tri;
				minDist = testDist;
				coord = r.getOrigin() + r.getDirection()*minDist;
			}

			/*if (tri.intersect(BVHRay(r.getOrigin(),r.getDirection()*-1),&testDist) && fabs(testDist)<fabs(minDist))
			{
			minTriangle = tri;
			minDist = -testDist;
			coord = r.getOrigin() + r.getDirection()*minDist;
			}*/
		}


		return BVHTriangleHit(minDist,minTriangle,coord);
	}
	void glDraw() { root->glDraw(0); }

};
