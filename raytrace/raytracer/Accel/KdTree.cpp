#include <cstdlib>
#include <algorithm>
#include <stack>

#include "KdTree.h"
#include "Constants.h"
#include "World.h"

using namespace std;

// Building a K-d Tree using Surface Area Heuristic in NLogN
// Reference: On building fast Kd tree for Ray tracing, and on doing so in O(NlogN) - Ingo Wald, Vlastimil Havran


KdTreeNode::KdTreeNode ():
	isLeaf (false)
{}

BBox KdTreeNode::get_bounding_box () const {
	return bbox;
}

void KdTreeNode::clip_plane (float& t_mid) {
	switch (splitAxis) {
		case X_AXIS: {
										
		}
		case Y_AXIS: {

		}
		case Z_AXIS: {
		
		}
	}
}

bool KdTreeNode::hit(const Ray& ray, float tmin, float tmax, float& t_hit, ShadeRec& sr) const {
	Normal		normal;
	Point3D		local_hit_point;
	
	float t = kEpsilon;
	t_hit = tmax;
	bool hit = false;

	int num_objs = objList.size ();
	for (int i = 0;i<num_objs;++i) 
		if (objList[i]->obj_ptr->hit (ray,t,sr) && t >= tmin - kEpsilon && t < t_hit+kEpsilon) {
			hit				= true;
			t_hit			= t;
			sr.material_ptr	= objList[i]->obj_ptr->get_material();	// lhs is GeometricObject::material_ptr
			normal			= sr.normal;
			sr.hit_point	= ray.o + ray.d*t;
			sr.hit_an_object= true;
			local_hit_point	= sr.local_hit_point;  
		}

	if (hit) {
		sr.t				= t_hit;
		sr.normal 			= normal;   
		sr.local_hit_point 	= local_hit_point;  
	}
	return hit;
}

KdTree::KdTree(void):
	root_ptr (NULL)
{
}


KdTree::~KdTree(void)
{
}

float C (float Pl, float Pr, int Nl, int Nr, bool alpha) {
	// here we assume that the cost for a traversal step 
	// and intersection of one triangle are 1.0 and 1.5 respectively
	float Kt = 1.0, Ki = 1.5; 
	// if 1 of 2 child is non-zeo empty voxel->reduce cost by 20%
	return (alpha&&(Nl==0||Nr==0)?0.8f:1.0f)*(Kt + Ki*(Pl*Nl + Pr*Nr));
}

float calculate_SAH (KdTreeNode* aNode, SplitCandidate* split_ptr, int Nl, int Nr, int Np, int& side) {
	BBox bb = aNode->get_bounding_box ();
	float h,w,l;
	h = bb.x1 - bb.x0; w = bb.y1 - bb.y0, l = bb.z1 - bb.z0;
	float SAV = 2*(h*w + w*l + l*h); // surface area of the node
	BBox bl,br;
	bl = bb;
	br = bb;
	if (split_ptr->split_axis == X_AXIS) {
		bl.x1 = split_ptr->split_plane;
		br.x0 = bl.x1;
	}
	else if (split_ptr->split_axis == Y_AXIS) {
		bl.y1 = split_ptr->split_plane;
		br.y0 = bl.y1;
	}
	else {
		bl.z1 = split_ptr->split_plane;
		br.z0 = bl.z1;
	}
	bool alpha = true;
	h = bl.x1-bl.x0; w = bl.y1-bl.y0; l = bl.z1-bl.z0;
	float SAVL = 2*(h*w + w*l + l*h); // surface area of the left child
	if (h==0 || w == 0 || l == 0) alpha = false;
	h = br.x1-br.x0; w = br.y1-br.y0; l = br.z1-br.z0;
	if (h==0 || w == 0 || l == 0) alpha = false;
	float SAVR = 2*(h*w + w*l + l*h); // surface area of the left child
	
	float Pl = SAVL / SAV;
	float Pr = SAVR / SAV;
	float CL = C(Pl, Pr, Nl + Np, Nr,alpha);
	float CR = C(Pl, Pr, Nl, Nr + Np,alpha);
	if (CL < CR) {
		side = LEFT;
		return CL;
	}
	else {
		side = RIGHT;
		return CR;
	}
}

bool compare_func (SplitCandidate* a, SplitCandidate* b) {
	return ((*a)<(*b));
}

void merge_candidates (vector<SplitCandidate*> &ret, const vector<SplitCandidate*> &a, const vector<SplitCandidate*> &b) {
	int i,j,m,n;
	i = j = 0;
	m = a.size ();
	n = b.size ();
	while (i<m || j<n) {
		if (i<m && (j>=n || (compare_func (a[i],b[j]))) ) {
			ret.push_back(a[i]);
			i++;
		}
		else if (j<=n) {
			ret.push_back (b[j]);
			j++;
		}
	}
}



void KdTree::build_tree (KdTreeNode* aNode, int depth) {
	aNode->isLeaf = true;
	if (aNode->objList.size () <= KD_TREE_MAX_NUM_OBJ_LEAF || depth == MAX_KD_TREE_DEPTH) 
		return;

	// else create 2 child node
	aNode->left_ptr = new KdTreeNode;
	aNode->right_ptr = new KdTreeNode;
	BBox bb= aNode->get_bounding_box ();
	float x_min = bb.x0, y_min = bb.y0, z_min = bb.z0, x_max = bb.x1, y_max = bb.y1, z_max = bb.z1;
	float xx = fabs (x_max - x_min);
	float yy = fabs (y_max - y_min);
	float zz = fabs (z_max - z_min);
	
	// find the splitting plane
	// Nl, Nr, Np are respectively the number of triangles in the left/right/exactly inside of the plane 
	// Note that Nl + Np + Nr may greater than total number of objects in the node

	// a criterion for termination is when we cant find any plane that has cost < cost of testing all triangles
	// first min_cost is intialized to cost of intersecting all triangles, here assume Ti = 1.5 unit time
	float min_cost = 1.5f*aNode->objList.size ();
	float split_plane = kHugeValue;
	int Nl[3],Nr[3],Np[3],split_plane_id,split_plane_axis,split_side;
	split_plane_id = -1;
	for (int k = 0;k<3;++k) {
		Nl[k] = Np[k] = 0;
		Nr[k] = aNode->objList.size ();
	}
	int p_start, p_end, p_planar; // number of triangles starting/ending/lying in the current plane
	
	int num_candidates = aNode->splitList.size ();
	// now iterate over all plane candidates
	for (int i = 0;i<num_candidates;) {
		SplitCandidate* split_ptr = aNode->splitList[i];
		int k = split_ptr->split_axis;
		float split_plane_i = split_ptr->split_plane;
		p_start = p_end = p_planar = 0;
		
		while (i<num_candidates && aNode->splitList[i]->split_axis == k &&
			aNode->splitList[i]->split_plane == split_plane_i && aNode->splitList[i]->type == START) { 
			p_start++; i++;
		}
		
		while (i<num_candidates && aNode->splitList[i]->split_axis == k &&
			aNode->splitList[i]->split_plane ==split_plane_i && aNode->splitList[i]->type == PLANAR) { 
			p_planar++; i++;
		}
		
		while (i<num_candidates && aNode->splitList[i]->split_axis == k &&
			aNode->splitList[i]->split_plane == split_plane_i && aNode->splitList[i]->type == END) { 
			p_end++; i++;
		}
		Np[k] = p_planar;
		Nr[k] -= p_planar+p_end; // triangles ending/lying in plane[i] are no longer in VR
		float cost = calculate_SAH (aNode,split_ptr, Nl[k], Nr[k], Np[k],split_side);
		
		if (cost < min_cost) {
			aNode->isLeaf = false;
			min_cost = cost;
			split_plane_id = i;
			split_plane = split_ptr->split_plane;
			split_plane_axis = split_ptr->split_axis;
		}	
		Nl[k] +=p_planar + p_start;  // triangles starting/lying in plane[i] now overlap VL
		Np[k] = 0; // reset the number of triangles lying in for the next plane
	}
	
	// if termination criterion is met
	if (aNode->isLeaf) return;
	
	/*
	char** obj_side;
	obj_side = new char*[num_objects];
	for (int i = 0;i < num_objects;++i) {
		for (int axis = 0;axis<3;++axis) {
			obj_side[i] = new char[3];
			obj_side[i][0] = obj_side[i][1] = obj_side[i][2] = 2;
		}
	}
	*/

	// Now we classify the list of triangles into 2 halves
	const int num_objects = aNode->objList.size ();
	vector<char> obj_side;
	vector<SplitCandidate*> left_only, right_only;
	vector<ObjectData*> obj_left, obj_right;
	for (int i = 0;i<num_objects;++i) obj_side.push_back (2); // 2 = BOTH
	for (unsigned int i = 0;i<aNode->splitList.size ();++i) {
		SplitCandidate* split_ptr = aNode->splitList[i];
		if (split_ptr->type == START && split_ptr->split_axis == split_plane_axis && split_ptr->split_plane >= split_plane) {
			obj_side[split_ptr->obj_id] = RIGHT;
		}
		if (split_ptr->type == END && split_ptr->split_axis == split_plane_axis && split_ptr->split_plane <= split_plane) {
			obj_side[split_ptr->obj_id] = LEFT;
		}
		else if (split_ptr->type == PLANAR && split_ptr->split_axis == split_plane_axis) {
			if (split_ptr->split_plane < split_plane || (split_ptr->split_plane == split_plane && split_side == LEFT)) {
				obj_side[split_ptr->obj_id] = LEFT;
			}
			else if (split_ptr->split_plane > split_plane || (split_ptr->split_plane == split_plane && split_side == RIGHT)) {
				obj_side[split_ptr->obj_id] = RIGHT;
			}
		}
	}

	// now divide those triangles into 2 halves
	vector<int> new_id;  // new_id is the new index of the triangles in the divided list (obj_left/obj_right)
	for (int i = 0;i<num_objects;++i) new_id.push_back (i);
	for (int i = 0;i<num_objects;++i) 
		if (obj_side[i] == LEFT) {
			int id = aNode->left_ptr->objList.size ();
			aNode->left_ptr->objList.push_back (aNode->objList[i]);
			new_id[i] = id;
		}
		else if (obj_side[i] == RIGHT) {
			int id = aNode->right_ptr->objList.size ();
			aNode->right_ptr->objList.push_back (aNode->objList[i]);
			new_id[i] = id;
		}
	// next classify and divide the split candidates into 2 halves
	for (unsigned int i = 0;i<aNode->splitList.size ();++i) {
		SplitCandidate* split_ptr = aNode->splitList[i];
		if (obj_side[split_ptr->obj_id] == LEFT) {
			SplitCandidate* new_cand = new SplitCandidate;
			new_cand->obj_id = new_id[split_ptr->obj_id];
			new_cand->split_axis = split_ptr->split_axis;
			new_cand->split_plane = split_ptr->split_plane;
			new_cand->type = split_ptr->type;
			left_only.push_back (new_cand);
		}
		else if (obj_side[split_ptr->obj_id] == RIGHT) {
			SplitCandidate* new_cand = new SplitCandidate;
			new_cand->obj_id = new_id[split_ptr->obj_id];
			new_cand->split_axis = split_ptr->split_axis;
			new_cand->split_plane = split_ptr->split_plane;
			new_cand->type = split_ptr->type;
			right_only.push_back (new_cand);
		}
		
	}

	// now we consider those triangles that are overlapping the split plane
	// left_clipped, right_clipped store the split candidates from the clipped aabbs at each side
	vector<SplitCandidate*> left_clipped, right_clipped;
	for (int i = 0;i<num_objects;++i)
		if (obj_side[i] == 2) {
			BBox left_bb, right_bb;
			ObjectData* objdata_ptr = aNode->objList[i];
			BBox bb = objdata_ptr->bbox;
			// triangles overlapping the split plane is cut into 2 halves, each aabb of each half will therefore be clipped
			objdata_ptr->obj_ptr->clipping_bbox (bb,split_plane,split_plane_axis,left_bb,right_bb);
			ObjectData* left_obj_ptr = new ObjectData;
			ObjectData* right_obj_ptr = new ObjectData;
			left_obj_ptr->obj_ptr = objdata_ptr->obj_ptr;
			left_obj_ptr->bbox = left_bb;
			left_obj_ptr->id = objdata_ptr->id;
			int id = aNode->left_ptr->objList.size (); // new id
			aNode->left_ptr->objList.push_back (left_obj_ptr); // push the clipped triangles into left child's triangle list
			// generate 6 new split candidates from the left clipped aabb
			SplitCandidate* temp = new SplitCandidate;
			temp->obj_id = id;
			temp->split_axis = X_AXIS;
			temp->split_plane = left_bb.x0;
			temp->type = START;
			SplitCandidate* temp1 = new SplitCandidate; ;
			temp1->obj_id = id;
			temp1->split_axis = X_AXIS;
			temp1->split_plane = left_bb.x1;
			temp1->type = END;

			SplitCandidate* temp2 = new SplitCandidate;;
			temp2->obj_id = id;
			temp2->split_axis = Y_AXIS;
			temp2->split_plane = left_bb.y0;
			temp2->type = START;
			SplitCandidate* temp3 = new SplitCandidate;;
			temp3->obj_id = id;
			temp3->split_axis = Y_AXIS;
			temp3->split_plane = left_bb.y1;
			temp3->type = END;

			SplitCandidate* temp4 = new SplitCandidate;;
			temp4->obj_id = id;
			temp4->split_axis = Z_AXIS;
			temp4->split_plane = left_bb.z0;
			temp4->type = START;
			SplitCandidate* temp5 = new SplitCandidate;;
			temp5->obj_id = id;
			temp5->split_axis = Z_AXIS;
			temp5->split_plane = left_bb.z1;
			temp5->type = END;
			// push all into array
			left_clipped.push_back (temp);
			left_clipped.push_back (temp1);
			left_clipped.push_back (temp2);
			left_clipped.push_back (temp3);
			left_clipped.push_back (temp4);
			left_clipped.push_back (temp5);
			
			right_obj_ptr->obj_ptr = objdata_ptr->obj_ptr;
			right_obj_ptr->bbox = right_bb;
			right_obj_ptr->id = objdata_ptr->id;
			id = aNode->right_ptr->objList.size (); // new id
			aNode->right_ptr->objList.push_back (right_obj_ptr); // push the clipped triangles into right child's triangle list
			// generate 6 new split candidates from the right clipped aabb
			SplitCandidate* temp6  = new SplitCandidate;;
			temp6->obj_id = id;
			temp6->split_axis = X_AXIS;
			temp6->split_plane = right_bb.x0;
			temp6->type = START;
			SplitCandidate* temp7 = new SplitCandidate;;
			temp7->obj_id = id;
			temp7->split_axis = X_AXIS;
			temp7->split_plane = right_bb.x1;
			temp7->type = END;

			SplitCandidate* temp8 = new SplitCandidate;;
			temp8->obj_id = id;
			temp8->split_axis = Y_AXIS;
			temp8->split_plane = right_bb.y0;
			temp8->type = START;
			SplitCandidate* temp9 = new SplitCandidate;;
			temp9->obj_id = id;
			temp9->split_axis = Y_AXIS;
			temp9->split_plane = right_bb.y1;
			temp9->type = END;

			SplitCandidate* temp10 = new SplitCandidate;;
			temp10->obj_id = id;
			temp10->split_axis = Z_AXIS;
			temp10->split_plane = right_bb.z0;
			temp10->type = START;
			SplitCandidate* temp11 = new SplitCandidate;;
			temp11->obj_id = id;
			temp11->split_axis = Z_AXIS;
			temp11->split_plane = right_bb.z1;
			temp11->type = END;

			// push all into array 
			right_clipped.push_back (temp6);
			right_clipped.push_back (temp7);
			right_clipped.push_back (temp8);
			right_clipped.push_back (temp9);
			right_clipped.push_back (temp10);
			right_clipped.push_back (temp11);
		}

	// sort the 2 lists of clipped candidates
	sort (left_clipped.begin (), left_clipped.end (), compare_func);
	sort (right_clipped.begin (), right_clipped.end (), compare_func);
	// now merge the clipped candidates with the left_only and right_only candidates
	merge_candidates (aNode->left_ptr->splitList,left_only, left_clipped); // merge 2 lists into left child's split candidate list
	merge_candidates (aNode->right_ptr->splitList,right_only, right_clipped); // merge 2 lists into right child's split candidate list

	// finally, set the split plane of the current node, and continue building its children
	aNode->splitAxis = split_plane_axis;
	aNode->splitPlane = split_plane;
	// determine the children's aabb
	aNode->left_ptr->bbox = bb;
	aNode->right_ptr->bbox = bb;
	if (split_plane_axis == X_AXIS) {
		aNode->left_ptr->bbox.x1 = split_plane;
		aNode->right_ptr->bbox.x0 = split_plane;
	}
	else if (split_plane_axis == Y_AXIS) {
		aNode->left_ptr->bbox.y1 = split_plane;
		aNode->right_ptr->bbox.y0 = split_plane;
	}
	else {
		aNode->left_ptr->bbox.z1 = split_plane;
		aNode->right_ptr->bbox.z0 = split_plane;
	}
	build_tree (aNode->left_ptr, depth+1);
	build_tree (aNode->right_ptr, depth+1);
}

 

void KdTree::build (const World& w) {
	root_ptr = new KdTreeNode;
	int num_objs = w.objects.size ();
	float x_min,y_min,z_min,x_max,y_max,z_max;
	x_min = y_min = z_min = kHugeValue;
	x_max = y_max = z_max = -kHugeValue;
	
	// iterate over all objects (triangles) generate the split candidates
	for (int i = 0;i<num_objs;++i) {
		BBox bb = w.objects[i]->get_bounding_box ();
		x_min = min (x_min,bb.x0);
		y_min = min (y_min,bb.y0);
		z_min = min (z_min,bb.z0);
		x_max = max (x_max,bb.x1);
		y_max = max (y_max,bb.y1);
		z_max = max (z_max,bb.z1);
		ObjectData* objdata_ptr = new ObjectData;
		objdata_ptr->obj_ptr = w.objects[i];
		BBox temp = w.objects[i]->get_bounding_box ();
		objdata_ptr->bbox = temp;
		objdata_ptr->id = i;
		root_ptr->objList.push_back (objdata_ptr);
		
		// generate split candidates
		if (objdata_ptr->bbox.x0 == objdata_ptr->bbox.x1) { // object is planar
			SplitCandidate* split_ptr1 = new SplitCandidate;
			split_ptr1->obj_id = i;
			split_ptr1->split_axis = X_AXIS;
			split_ptr1->split_plane = objdata_ptr->bbox.x0;
			split_ptr1->type = PLANAR;
			root_ptr->splitList.push_back (split_ptr1);
		} 
		else { // otherwise, the plane is the 'start' or end of a object
			SplitCandidate* split_ptr1 = new SplitCandidate;
			split_ptr1->obj_id = i;
			split_ptr1->split_axis = X_AXIS;
			split_ptr1->split_plane = objdata_ptr->bbox.x0;
			split_ptr1->type = START;
			root_ptr->splitList.push_back (split_ptr1);
			SplitCandidate* split_ptr2 = new SplitCandidate;
			split_ptr2->obj_id = i;
			split_ptr2->split_axis = X_AXIS;
			split_ptr2->split_plane = objdata_ptr->bbox.x1;
			split_ptr2->type = END;
			root_ptr->splitList.push_back (split_ptr2);
		}

		if (objdata_ptr->bbox.y0 == objdata_ptr->bbox.y1) {
			SplitCandidate* split_ptr1 = new SplitCandidate;
			split_ptr1->obj_id = i;
			split_ptr1->split_axis = Y_AXIS;
			split_ptr1->split_plane = objdata_ptr->bbox.y0;
			split_ptr1->type = PLANAR;
			root_ptr->splitList.push_back (split_ptr1);
		}
		else {
			SplitCandidate* split_ptr1 = new SplitCandidate;
			split_ptr1->obj_id = i;
			split_ptr1->split_axis = Y_AXIS;
			split_ptr1->split_plane = objdata_ptr->bbox.y0;
			split_ptr1->type = START;
			root_ptr->splitList.push_back (split_ptr1);
			SplitCandidate* split_ptr2 = new SplitCandidate;
			split_ptr2->obj_id = i;
			split_ptr2->split_axis = Y_AXIS;
			split_ptr2->split_plane = objdata_ptr->bbox.y1;
			split_ptr2->type = END;
			root_ptr->splitList.push_back (split_ptr2);
		}

		if (objdata_ptr->bbox.z0 == objdata_ptr->bbox.z1) {
			SplitCandidate* split_ptr1 = new SplitCandidate;
			split_ptr1->obj_id = i;
			split_ptr1->split_axis = Z_AXIS;
			split_ptr1->split_plane = objdata_ptr->bbox.z0;
			split_ptr1->type = PLANAR;
			root_ptr->splitList.push_back (split_ptr1);
		}
		else {
			SplitCandidate* split_ptr1 = new SplitCandidate;
			split_ptr1->obj_id = i;
			split_ptr1->split_axis = Z_AXIS;
			split_ptr1->split_plane = objdata_ptr->bbox.z0;
			split_ptr1->type = START;
			root_ptr->splitList.push_back (split_ptr1);
			SplitCandidate* split_ptr2 = new SplitCandidate;
			split_ptr2->obj_id = i;
			split_ptr2->split_axis = Z_AXIS;
			split_ptr2->split_plane = objdata_ptr->bbox.z1;
			split_ptr2->type = END;
			root_ptr->splitList.push_back (split_ptr2);
		}
	}
	sort (root_ptr->splitList.begin (),root_ptr->splitList.end (),compare_func);
	root_ptr->bbox = BBox (x_min,x_max,y_min,y_max,z_min,z_max);
	build_tree (root_ptr, 0);
}
/*
float compute_t_split_plane (const Ray& ray, float split_plane, int split_axis, float tmin, float tmax) {
	switch (split_axis) {
		case X_AXIS: {
			if (fabs (ray.d.x) < kEpsilon) {
				if (ray.o.x < split_plane) return -kHugeValue;  // ray is completely inside left child
				else return kHugeValue; // ray is completely inside right child
			}
			else {
				float temp = (split_axis - ray.o.x) / ray.d.x;
				if (tmin <= temp && temp <= tmax) return temp;
				else {
					float x_min = ray.o.x + ray.d.x*tmin;
					float x_max = ray.o.x + ray.d.x*tmax;
					return (x_min<=split_plane && x_max<=split_plane?-kEpsilon:kEpsilon);
				}
			}
		}
		case Y_AXIS: {
			if (ray.d.y == 0) {
				if (ray.o.y < split_plane) return -kHugeValue;  // ray is completely inside left child
				else return kHugeValue; // ray is completely inside right child
			}
			else {
				float temp = (split_axis - ray.o.y) / ray.d.y;
				if (tmin <= temp && temp <= tmax) return temp;
				else {
					float y_min = ray.o.y + ray.d.y*tmin;
					float y_max = ray.o.y + ray.d.y*tmax;
					return (y_min<=split_plane && y_max<=split_plane?-kEpsilon:kEpsilon);
				}
			}
		}
		case Z_AXIS: {
			if (ray.d.z == 0) {
				if (ray.o.z< split_plane) return -kHugeValue;  // ray is completely inside left child
				else return kHugeValue; // ray is completely inside right child
			}
			else {
				float temp = (split_axis - ray.o.z) / ray.d.z;
				if (tmin <= temp && temp <= tmax) return temp;
				else {
					float z_min = ray.o.z + ray.d.z*tmin;
					float z_max = ray.o.z+ ray.d.z*tmax;
					return (z_min<=split_plane && z_max<=split_plane?-kEpsilon:kEpsilon);
				}
			}
		}
	}
}
*/

int compute_t_split_plane (const Ray& ray, float split_plane, int split_axis, float tmin, float tmax) {
	switch (split_axis) {
		case X_AXIS: {
			float x_min = ray.o.x + ray.d.x*tmin;
			float x_max = ray.o.x + ray.d.x*tmax;
			if (x_min<=split_plane && x_max<=split_plane)
				return -1;
			else if (x_min<split_plane && x_max>split_plane)
				return 0;
			else return 1;
		}
		case Y_AXIS: {
			float y_min = ray.o.y + ray.d.y*tmin;
			float y_max = ray.o.y + ray.d.y*tmax;
			if (y_min<=split_plane && y_max<=split_plane)
				return -1;
			else if (y_min<split_plane && y_max>split_plane)
				return 0;
			else return 1;
		}
		case Z_AXIS: {
			float z_min = ray.o.z + ray.d.z*tmin;
			float z_max = ray.o.z + ray.d.z*tmax;
			if (z_min<=split_plane && z_max<=split_plane)
				return -1;
			else if (z_min<split_plane && z_max>split_plane)
				return 0;
			else return 1;
		}
	}
}

int find_max_depth (KdTreeNode* aNode) {
	if (aNode->isLeaf) return 0;
	else return max (find_max_depth (aNode->left_ptr), find_max_depth (aNode->right_ptr))+1;
}

bool KdTree::hit(const Ray& ray, float& t_hit, ShadeRec& sr) const {
	int temp = find_max_depth (root_ptr);
	// determine the tmin & tmax of the voxel
	BBox bb = root_ptr->get_bounding_box ();
	float x0 = bb.x0-kEpsilon, y0 = bb.y0-kEpsilon, z0 = bb.z0-kEpsilon;
	float x1 = bb.x1+kEpsilon, y1 = bb.y1+kEpsilon, z1 = bb.z1+kEpsilon;
	float tmin, tmax;
	float ox = ray.o.x; float oy = ray.o.y; float oz = ray.o.z;
	float dx = ray.d.x; float dy = ray.d.y; float dz = ray.d.z;
	
	float tx_min, ty_min, tz_min;
	float tx_max, ty_max, tz_max;
	tx_min = ty_min = tz_min = kEpsilon;
	tx_max = ty_max = tz_max = kHugeValue;
	
	if (fabs (dx) > kEpsilon) {
		float a = 1.0f / dx;
		if (a >= 0) {
			tx_min = (x0 - ox) * a;
			tx_max = (x1 - ox) * a;
		}
		else {
			tx_min = (x1 - ox) * a;
			tx_max = (x0 - ox) * a;
		}
	}
	
	if (fabs (dy) > kEpsilon) {
		float b = 1.0f / dy;
		if (b >= 0) {
			ty_min = (y0 - oy) * b;
			ty_max = (y1 - oy) * b;
		}
		else {
			ty_min = (y1 - oy) * b;
			ty_max = (y0 - oy) * b;
		}
	}
	if (fabs (dz) > kEpsilon) {
		float c = 1.0f / dz;
		if (c >= 0) {
			tz_min = (z0 - oz) * c;
			tz_max = (z1 - oz) * c;
		}
		else {
			tz_min = (z1 - oz) * c;
			tz_max = (z0 - oz) * c;
		}
	}
	
	
	if (tx_min > ty_min) 
		tmin = tx_min;
	else 
		tmin = ty_min;
	
	if (tz_min > tmin) 
		tmin = tz_min;	
		
		
	// find smallest exiting t value
	if (tx_max < ty_max) 
		tmax = tx_max;
	else 
		tmax = ty_max;
		
		
	if (tz_max < tmax) 
		tmax = tz_max;

	if (tmin > tmax) return false;  // miss
	
	stack<KdTreeNode*> sta;  // stack to traverse the tree non-recursively
	sta.push (root_ptr);
	stack<float> sta_t; // stack to store the tmin& tmax value of each node
	sta_t.push (tmin); 
	sta_t.push (tmax);

	while (!sta.empty ()) {
		KdTreeNode* aNode = sta.top (); sta.pop ();
		float cur_tmax = sta_t.top (); sta_t.pop ();
		float cur_tmin = sta_t.top (); sta_t.pop ();
		if (aNode->isLeaf) {
			bool temp = aNode->hit (ray, cur_tmin, cur_tmax, t_hit, sr);
			if (temp) return true;
		}
		else {
			int t_split_plane = compute_t_split_plane (ray, aNode->splitPlane, aNode->splitAxis,cur_tmin, cur_tmax); 
			/*
			if (t_split_plane == -kHugeValue) {
				sta.push (aNode->get_left ());
				sta_t.push (cur_tmin);
				sta_t.push (cur_tmax);
			}
			else if (t_split_plane == kHugeValue) {
				sta.push (aNode->get_right ());
				sta_t.push (cur_tmin);
				sta_t.push (cur_tmax);
			}
			else if (cur_tmin<=t_split_plane && t_split_plane<=cur_tmax) {
				sta.push (aNode->get_left ());
				sta.push (aNode->get_right ());
				sta_t.push (cur_tmin);
				sta_t.push (t_split_plane);
				sta_t.push (t_split_plane);
				sta_t.push (cur_tmax);
			}
			else {
				if (fabs (t_split_plane + kEpsilon) < kEpsilon) {
					sta.push (aNode->get_left ());
					sta_t.push (cur_tmin);
					sta_t.push (cur_tmax);
				}
				else  {
					sta.push (aNode->get_right ());
					sta_t.push (cur_tmin);
					sta_t.push (cur_tmax);
				}
			}
			*/
			if (t_split_plane==-1) {
				sta.push (aNode->get_left ());
				sta_t.push (cur_tmin);
				sta_t.push (cur_tmax);
			}
			else if (t_split_plane==0) {
				float temp;
				if (aNode->splitAxis==X_AXIS) 
					temp = (aNode->splitPlane - ray.o.x) / ray.d.x;
				else if (aNode->splitAxis==Y_AXIS)
					temp = (aNode->splitPlane - ray.o.y) / ray.d.y;
				else 
					temp = (aNode->splitPlane - ray.o.z) / ray.d.z;

				sta.push (aNode->get_left ());
				sta_t.push (cur_tmin);
				sta_t.push (temp);
				sta.push (aNode->get_right ());
				sta_t.push (temp);
				sta_t.push (cur_tmax);
			}
			else {
				sta.push (aNode->get_right ());
				sta_t.push (cur_tmin);
				sta_t.push (cur_tmax);
			}
			
		}
	}
	
	return false;
}
