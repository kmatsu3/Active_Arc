#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::set_model_parameters_in_construction(
								  model_parameters_active_arc_class & model
								  )
{
  ///////////////////////////////////////////
  /*
    model dimensions
  */
  ///////////////////////////////////////////
  number_of_arcs=model.get_number_of_arcs();
  space_dimension=model.get_space_dimension();

  ///////////////////////////////////////////
  /*
    arc paramters for initialization
   */
  ///////////////////////////////////////////
  // dimensions of arc
  arc_length=model.get_arc_length();
  arc_width=model.get_arc_width();
  arc_curvature = model.get_arc_curvature();
  // elastic parameters of arc
  young_modulus=model.get_young_modulus();
  poisson_ratio=model.get_poisson_ratio();
  // constraint parameters of arc for nonlinear control
  angle_penalty_energy=model.get_angle_penalty_energy();
  // constraint parameters of arc for avoiding null angle singularity
  shear_constraint_switch=model.check_shear_constraint_on();
  shear_constraint_modulus=model.get_shear_constraint_modulus();
  shear_constraint_power=model.get_shear_constraint_power();

  ////////////////////////////////////////////
  /*
    Boundary condition
  */
  ////////////////////////////////////////////
  boundary_condition=model.get_boundary_condition();

  ////////////////////////////////////////////
  /*
    Inner part parameters
  */
  ////////////////////////////////////////////
  inner_type=model.get_inner_type();
};
//
void state_active_arc_class::set_constants()
{
  ////////////////////////////////////////////
  /* 
     Mathematical constants
  */
  ////////////////////////////////////////////
  PI=atan(1.0)*4.0;
  //PI=3.14159265358979323846;
};
//
void state_active_arc_class::calculate_dimensions()
{
  ////////////////////////////////////////////
  /* 
     Dimensions of degrees of freedom
     using mdoel parameters and constants
     This should be called after
     set_model_parameters() 
     & set_constants()
  */
  ////////////////////////////////////////////
  number_of_joint_components=3;
  number_of_internal_components=2;
  // Number of joints between arcs, 
  // which depends on boundary condition.
  if(boundary_condition=="close")
    {
      number_of_joints=number_of_arcs;
    }
  else
    {
      number_of_joints=number_of_arcs+1;
    }
  //////////////////////////////////////////////
  /*
    Pointers of gradients of optimization function 
    for CG & MD.
  */
  //////////////////////////////////////////////
  // pointer of head address of internal degree
  internal_head_pointer
    =number_of_joints*number_of_joint_components;
  // pointer of head address of constraint degree
  constraint_head_pointer
    =number_of_joints*number_of_joint_components
    +number_of_arcs*number_of_internal_components;
};
