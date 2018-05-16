

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/io/io.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <iostream>

using namespace std;
namespace bg = boost::geometry;

template<typename T> using compact_point = boost::tuple<T, T>;
template<typename T> using compact_box  = vector<T>;

static_assert(sizeof(compact_point<float>)  == 2*sizeof(float), "");
static_assert(alignof(compact_point<float>) >= alignof(float), "");

BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)
BOOST_GEOMETRY_REGISTER_RING_TEMPLATED(compact_box)

using point = compact_point<float>;
using box = compact_box<point>;

typedef bg::model::polygon< point > polygon;

vector<point> as_compact_points(vector<float> x) {
  float* x_p = &(x[0]);
  auto x_point_p = reinterpret_cast<point*>(x_p);
  vector<point> x_point_v(x_point_p, x_point_p + x.size() / 2);
  return x_point_v;
}

box as_compact_box(point* p) {
    vector<point> r(p, p + 4);
    return r;
}


extern "C" {
  void cpp_greedy_nms(float *coords, int *output, int length, float thresh);
  void hello();
}


void cpp_greedy_nms(float *coords_p, int *output, int length, float thresh) {


  vector<float> coords(coords_p, coords_p+length);

  vector<point> points = as_compact_points(coords);
  vector<box> boxes;

  for (uint i=0; i<points.size(); i+=4) {
     box b = as_compact_box(&(points[i]));
     bg::correct(b);
     boxes.push_back(b);
  }
  
  uint output_length = length / 8;
  for (uint i=0; i<output_length; i++) output[i] = -1;

  vector<polygon> intersections;
  int group_n = -1;
  for (uint i=0; i<boxes.size(); i+=1) {
    if (output[i] >= 0 && i > 0) continue;
    box top = boxes[i];
    group_n++;
    output[i] = group_n;
    float top_area = bg::area(top);
    for (uint j=i+1; j<boxes.size(); j+=1) {
      if (output[j] < 0) {
        bg::intersection(top, boxes[j], intersections);
        if (intersections.size() > 0) {
          float inter_a = bg::area(intersections[0]);
          intersections.clear();
          float union_a = top_area + bg::area(boxes[j]);
          if (inter_a / union_a >= thresh) {
            output[j] = group_n;
          }
        }
      }
    } 
  }
}

void hello() {
  cout << "Hello from c++" << endl;
}
