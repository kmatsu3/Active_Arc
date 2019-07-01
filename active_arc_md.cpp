#include "active_arc_state.hpp"
/*
  Main routine
 */
void state_active_arc_class::md_relaxation(
					   model_parameters_active_arc_class & model,
					   const int & sweep_step
					   )
{
  md_initialize(model);
  md_main();
  finalization(sweep_step,"md",model);
};
//
void state_active_arc_class::md_initialize(
					   model_parameters_active_arc_class & model
					   )
{
  time_difference
    =model.get_md_time_difference();
  termination_number_of_iteration
    =model.get_md_termination_number_of_iteration();
};
//
void state_active_arc_class::md_main()
{
  //
  double optimization_function;
  //  long int plot_index;
  std::string initializer_real_1d_array;
  generate_initializer(initializer_real_1d_array, "gradient");
  alglib::real_1d_array function_gradient = initializer_real_1d_array.c_str();
  //
  for(
      long int iteration_index=0;
      iteration_index<termination_number_of_iteration;
      iteration_index++
      )
    {
      /*      if(iteration_index==2||iteration_index==30||nan_check(iteration_index))
	{
	  output_reference(iteration_index,181);
	  output_configuration(iteration_index,181);
	  if(nan_check(iteration_index))io_method.error_output("state","md_main","debug");
	}
      */
      //      if(iteration_check(iteration_index,termination_number_of_iteration,10)) output_configuration(iteration_index,1);
      local_geometry();
      calculate_displacement("");
      //  if(iteration_check(iteration_index,termination_number_of_iteration,10))output_displacements(iteration_index,1);
      calculate_strain();
      // if(iteration_check(iteration_index,termination_number_of_iteration,10))output_strain(iteration_index,1);
      calculate_constraint();
      calculate_stress();
      //if(iteration_check(iteration_index,termination_number_of_iteration,10))output_stress(iteration_index,1);
      calculate_elastic_energy();
      calculate_elastic_gradient();
      //if(iteration_check(iteration_index,termination_number_of_iteration,10))output_forces(iteration_index,1);
      //if(iteration_check(iteration_index,termination_number_of_iteration,10))output_preforces(iteration_index,1);
      //if(iteration_check(iteration_index,termination_number_of_iteration,10))output_moments(iteration_index,1);
      calculate_inner();
      //if(iteration_check(iteration_index,termination_number_of_iteration,10))output_inner_values(iteration_index,1);
      summarize_energy_gradient(
				optimization_function,
				function_gradient
				);
      //if(iteration_check(iteration_index,termination_number_of_iteration,10))plot(iteration_index,1,"gnuplot");
      md_update();
      /*
      if(
	 iteration_check(
			 iteration_index,
			 shape_plot_max,
			 shape_plot_frequency,
			 shape_plot_base,
			 plot_index
			 )
	 ) 
	{ 
	  output_configuration(plot_index,1);
	  output_displacements(plot_index,1);
	  output_arc_angles(plot_index,1);
	  output_strain(plot_index,1);
	  output_stress(plot_index,1);
	  output_forces(plot_index,1);
	  output_preforces(plot_index,1);
	  output_moments(plot_index,1);
	  output_inner_values(plot_index,1);
	  plot(plot_index,1,"gnuplot");
	  shape_plot(plot_index,1,"gnuplot");
	  io_method.standard_output(
				    "**** plotted at Iter ="
				    + io_method.longint_to_string(iteration_index)
				    + "->"
				    + io_method.longint_to_string(plot_index)
				    +"****"
				    );
	  show_gradient(plot_index);
	};
      */
      //   show_gradient(iteration_index);
    };
};
//
void state_active_arc_class::md_update()
{
  for(
      int direction_index = 0;
      direction_index<space_dimension;
      direction_index++
      )
    {
    for(
	long int joint_index = 0;
	joint_index < number_of_joints;
	joint_index++
	)
      {
	coordinates[joint_index*space_dimension+direction_index]
	  = coordinates[joint_index*space_dimension+direction_index]
	  + gradient[joint_index*number_of_joint_components+direction_index]
	  * time_difference;
      };
    };
  //
  for(
      long int joint_index = 0;
      joint_index < number_of_joints;
      joint_index++
      )
    {
      angles[joint_index]
	=angles[joint_index]
	+gradient[joint_index*number_of_joint_components+space_dimension]
	*time_difference;
    };
  //
  /*
  for(
      long int joint_index = 0;
      joint_index < number_of_joints;
      joint_index++
      )
    {
      if( 
	 angles[joint_index]
	 > PI
	  )
	{
	  angles[joint_index]
	    = angles[joint_index]-2.0*PI;
	}
      else if( 
	      angles[joint_index]
	      < -PI
	       )
	{
	  angles[joint_index]
	    = angles[joint_index]+2.0*PI;
	};
    };
  */
  //
  for(
      long int arc_index = 0;
      arc_index < number_of_arcs;
      arc_index++
      )
    {
      ripening[arc_index]
	=ripening[arc_index]
	+ gradient[internal_head_pointer+arc_index*number_of_internal_components]*time_difference;
      normal_modulation[arc_index]
	=normal_modulation[arc_index]
	+ gradient[internal_head_pointer+arc_index*number_of_internal_components+1]*time_difference;
    };
  //
  for(
      int direction_index=0;
      direction_index < space_dimension;
      direction_index++
      )
    {
      center_of_coordinates[direction_index]
	+= gradient[constraint_head_pointer+direction_index];
    };
};
  
