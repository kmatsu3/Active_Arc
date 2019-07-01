#include "active_arc_interface.hpp"
/*
  Constructor
 */
reference_io::reference_io()
{
};
//
configuration_io::configuration_io()
{
};
//
stress_io::stress_io()
{
};
//
inner_stress_io::inner_stress_io()
{
};
//
growth_io::growth_io()
{
};
//
void reference_io::initialize(
			      long int & number_of_arcs
			      )
{
  //
  length.clear();
  width.clear();
  curvature.clear();
  young_modulus.clear();
  poisson_ratio.clear();
  //
  inner_control_volume.clear();
  inner_height.clear();
  inner_width.clear();
  inner_shear.clear();
  inner_young_modulus.clear();
  inner_poisson_ratio.clear();
  //
  dummy.clear();
  //
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      length.push_back(0.0);
      width.push_back(0.0);
      curvature.push_back(0.0);
      young_modulus.push_back(0.0);
      poisson_ratio.push_back(0.0);
      inner_control_volume.push_back(0.0);
      inner_height.push_back(0.0);
      inner_width.push_back(0.0);
      inner_shear.push_back(0.0);
      inner_young_modulus.push_back(0.0);
      inner_poisson_ratio.push_back(0.0);
      dummy.push_back(0.0);
    };
};
//
void configuration_io::initialize(
				  long int & number_of_joints,
				  long int & number_of_arcs,
				  int & space_dimension
				  )
{
  //
  coordinates.clear();
  angles.clear();
  thickness.clear();
  center_of_coordinates.clear();
  //
  for(
      long int joint_index=0;
      joint_index<number_of_joints;
      joint_index++
      )
    {
      coordinates.push_back(0.0);
      coordinates.push_back(0.0);
      angles.push_back(0.0);
      thickness.push_back(0.0);
    };
  //
  for(
      int direction_index=0;
      direction_index<space_dimension;
      direction_index++
      )
    {
      center_of_coordinates.push_back(0.0);
    };
  //
  ripening.clear();
  normal_modulation.clear();
  inner_volume.clear();
  inner_elongation.clear();
  inner_ripening.clear();
  inner_shear.clear();
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      ripening.push_back(0.0);
      normal_modulation.push_back(0.0);
      inner_volume.push_back(0.0);
      inner_elongation.push_back(0.0);
      inner_ripening.push_back(0.0);
      inner_shear.push_back(0.0);
    };
};
//
void stress_io::initialize(
			   long int & number_of_arcs
			   )
{
      tangent_stress.clear();
      normal_stress.clear();
      shear_stress.clear();
      bending_stress.clear();
      shearbending_stress.clear();
      dummy.clear();
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      tangent_stress.push_back(0.0);
      normal_stress.push_back(0.0);
      shear_stress.push_back(0.0);
      bending_stress.push_back(0.0);
      shearbending_stress.push_back(0.0);
      dummy.push_back(0.0);
    };
};
//
void inner_stress_io::initialize(
				 long int & number_of_arcs
				 )
{
      inner_elongation_stress.clear();
      inner_ripening_stress.clear();
      inner_shear_stress.clear();
      dummy.clear();
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      inner_elongation_stress.push_back(0.0);
      inner_ripening_stress.push_back(0.0);
      inner_shear_stress.push_back(0.0);
      dummy.push_back(0.0);
    };
};
//
void growth_io::initialize(
			   long int & number_of_arcs
			   )
{
  growth_on=false;
  growth_table.clear();
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      growth_table.push_back(-1);
    };
}
