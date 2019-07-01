#ifndef __INTERFACE_ACTIVE_ARC__
#define __INTERFACE_ACTIVE_ARC__
#include "active_arc_io.hpp"
#include "active_arc_gnuplot.hpp"
#include "active_arc_definition.hpp"
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <boost/math/special_functions/sinc.hpp>
#include "stdafx.h"
#include "interpolation.h"
#include "optimization.h"
  /*
    Reference class
   */
class reference_io
{
  // interior part
public:std::vector<double> inner_control_volume;
public:std::vector<double> inner_young_modulus;
public:std::vector<double> inner_poisson_ratio;
public:std::vector<double> inner_height;
public:std::vector<double> inner_width;
public:std::vector<double> inner_shear;
  // arc part
public:std::vector<double> length;
public:std::vector<double> width;
public:std::vector<double> curvature;
public:std::vector<double> young_modulus;
public:std::vector<double> poisson_ratio;
  //
public:std::vector<double> dummy;
  /*
   setter/getter
   */
public:void initialize(
		       long int & number_of_arcs
		       );
  /*
    Constractor
   */
public:
  reference_io();
};
  /*
    Configuration class
   */
class configuration_io
{
public:std::vector<double> coordinates;
public:std::vector<double> center_of_coordinates;
public:std::vector<double> angles;
public:std::vector<double> thickness;
public:std::vector<double> ripening;
public:std::vector<double> normal_modulation;
public:std::vector<double> inner_volume;
public:std::vector<double> inner_elongation;
public:std::vector<double> inner_ripening;
public:std::vector<double> inner_shear;

  /*
    setter/getter
   */
public:void initialize(
		       long int & number_of_joints,
		       long int & number_of_arcs,
		       int & space_dimension
		       );
  /*
    Constractor
   */
public:
  configuration_io();
};
class stress_io
{
public:std::vector<double> tangent_stress;
public:std::vector<double> normal_stress;
public:std::vector<double> shear_stress;
public:std::vector<double> bending_stress;
public:std::vector<double> shearbending_stress;
public:std::vector<double> dummy;
  /*
    setter/getter
   */
public:void initialize(
		       long int & number_of_arcs
		       );
  /*
    Constractor
   */
public:
  stress_io();
};
class inner_stress_io
{
public:std::vector<double> inner_elongation_stress;
public:std::vector<double> inner_ripening_stress;
public:std::vector<double> inner_shear_stress;
public:std::vector<double> dummy;
  /*
    setter/getter
   */
public:void initialize(
		       long int & number_of_arcs
		       );
  /*
    Constractor
   */
public:
  inner_stress_io();
};
//
class growth_io
{
public: bool growth_on;
public: std::vector<long int> growth_table;
  /*
    setter/getter
   */
void initialize(
		long int & number_of_arcs
		);
  /*
    Constractor
   */
public: growth_io();
};
#endif //__INTERFACE_ACTIVE_ARC__
