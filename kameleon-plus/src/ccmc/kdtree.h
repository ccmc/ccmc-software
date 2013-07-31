/*
 * A variant of NanoKdTree.h from Starlab https://code.google.com/p/starlab/ which is released licensed with GPL v3
 */


#include <nanoflann.hpp>

using namespace std;
using namespace nanoflann;


template <typename T>
struct PointCloud
{
	struct Point
	{
		T  x,y,z;
	};

	std::vector<Point>  pts;
	// Must return the number of data points
	inline size_t kdtree_get_point_count() const { return pts.size(); }

	// Returns the distance between the vector "p1[0:size-1]" and the data point with index "idx_p2" stored in the class:
	inline T kdtree_distance(const T *p1, const size_t idx_p2,size_t size) const
	{
		const T d0=p1[0]-pts[idx_p2].x;
		const T d1=p1[1]-pts[idx_p2].y;
		const T d2=p1[2]-pts[idx_p2].z;
		return d0*d0+d1*d1+d2*d2;
	}

	// Returns the dim'th component of the idx'th point in the class:
	// Since this is inlined and the "dim" argument is typically an immediate value, the
	//  "if/else's" are actually solved at compile time.
	inline T kdtree_get_pt(const size_t idx, int dim) const
	{
		if (dim==0) return pts[idx].x;
		else if (dim==1) return pts[idx].y;
		else return pts[idx].z;
	}

	// Optional bounding-box computation: return false to default to a standard bbox computation loop.
	//   Return true if the BBOX was already computed by the class and returned in "bb" so it can be avoided to redo it again.
	//   Look at bb.size() to find out the expected dimensionality (e.g. 2 or 3 for point clouds)
	template <class BBOX>
	bool kdtree_get_bbox(BBOX &bb) const { return false; }
};



template <typename num_t>
class NanoKdTree{
typedef std::pair<size_t, num_t> KDResultPair;
typedef std::vector< KDResultPair  > KDResults;
public:

        PointCloud<num_t> cloud;

        typedef KDTreeSingleIndexAdaptor< L2_Simple_Adaptor<num_t, PointCloud<num_t> >, PointCloud<num_t>, 3 /* dim */ > my_kd_tree;

        my_kd_tree * tree;

        void addPoint(const num_t & p){
                cloud.pts.push_back(p);
        }

        void build()
        {
                // construct a kd-tree index:
        		printf("NanoKdTree::building kd tree index\n");
                tree = new my_kd_tree(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );
                tree->buildIndex();
        }

        size_t k_closest(num_t * p, int k, KDResults & ret_matches)
        {
        k = k < (int)cloud.pts.size() ? k : cloud.pts.size();

                std::vector<size_t> ret_index(k);
                std::vector<num_t> out_dist(k);

                tree->knnSearch(&p[0], k, &ret_index[0], &out_dist[0]);

                for(int i = 0; i < k; i++)
                {
                        ret_matches.push_back( std::make_pair(ret_index[i], out_dist[i]) );
//                  		std::cout <<"ret_index=" << ret_index[i] << " out_dist_sqr=" << out_dist[i] << std::endl;
                }
                return k;
        }
        void nearest(const num_t * query_point, KNNResultSet<num_t> & resultSet)
        {
        	tree->findNeighbors(resultSet, &query_point[0], nanoflann::SearchParams());

        }

        size_t ball_search(num_t * p, num_t search_radius, KDResults & ret_matches)
        {
                nanoflann::SearchParams params;
                //params.sorted = false;

                return tree->radiusSearch(&p[0], search_radius, ret_matches, params);
        }
};

