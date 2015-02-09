#ifndef __KD_TREE__
#define __KD_TREE__

#include <vector>
#include "BBox.h"
#include "GeometricObject.h"

// a ObjectData stores pointer to a geometric object, the reason why we need a seperate bbox is 
// to dynamically clip the object's bbox if the object itself overlapping a split plane
typedef struct  {
	int id;
	GeometricObject* obj_ptr;
	BBox bbox;
} ObjectData;

typedef struct SplitCandidate {
	int split_axis,obj_id, type;
	float split_plane;
} SplitCandidate;

inline bool operator<(const SplitCandidate& a, const SplitCandidate& b) {
	return a.split_axis < b.split_axis || (a.split_axis == b.split_axis && a.split_plane < b.split_plane) || 
		   (a.split_axis == b.split_axis && a.split_plane == b.split_plane && a.type < b.type);
}

inline bool operator>(const SplitCandidate& a, const SplitCandidate& b) {
	return a.split_axis > b.split_axis || (a.split_axis == b.split_axis && a.split_plane > b.split_plane) || 
		   (a.split_axis == b.split_axis && a.split_plane == b.split_plane && a.type > b.type);
}

inline bool operator<=(const SplitCandidate& a, const SplitCandidate& b) {
	return a.split_axis < b.split_axis || (a.split_axis == b.split_axis && a.split_plane < b.split_plane) || 
		   (a.split_axis == b.split_axis && a.split_plane == b.split_plane && a.type <= b.type);
}

inline bool operator>=(const SplitCandidate& a, const SplitCandidate& b) {
	return a.split_axis > b.split_axis || (a.split_axis == b.split_axis && a.split_plane > b.split_plane) || 
		   (a.split_axis == b.split_axis && a.split_plane == b.split_plane && a.type >= b.type);
}

inline bool operator==(const SplitCandidate& a, const SplitCandidate& b) {
	return a.split_axis == b.split_axis && a.split_plane == b.split_plane;
}



class KdTreeNode {
public:
	KdTreeNode ();
	~KdTreeNode ();
	KdTreeNode* get_left () const { return left_ptr;}
	KdTreeNode* get_right () const { return right_ptr; }
	void set_right (KdTreeNode* aNode) { right_ptr = aNode; }
	void set_left (KdTreeNode* aNode) { left_ptr = aNode; }
	BBox get_bounding_box () const;
	bool hit(const Ray& ray, float tmin, float tmax, float& thit, ShadeRec& sr) const;
	void clip_plane (float& t_mid);

public:
	BBox bbox;
	float splitPlane;
	int splitAxis;
	bool isLeaf;
	vector<ObjectData*> objList;
	vector<SplitCandidate*> splitList;
	KdTreeNode* left_ptr;
	KdTreeNode* right_ptr;
	int depth;
};

class KdTree
{
public:
	KdTree(void);
	~KdTree(void);

	KdTreeNode* get_root () const { return root_ptr; }
	void build_tree (KdTreeNode* aNode, int depth);
	void build (const World& w);
	bool hit(const Ray& ray, float& tmin, ShadeRec& sr) const;	
	
private:
	KdTreeNode* root_ptr;
	vector<SplitCandidate*> split_cands; // pre-sorted split candidates
};

#endif

