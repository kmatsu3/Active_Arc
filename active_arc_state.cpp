#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::initialize(
					model_parameters_active_arc_class & model,
					const std::string & criterion
					)
{
  initial_frequency=model.get_initial_frequency();
  initial_amplitude=model.get_initial_amplitude();
  initialize_configuration(criterion);
  translate_center_to_zero();
  //
  plot(
       0,
       0,
       "gnuplot"
       );
  //
};
  /*
    Constructer
  */
state_active_arc_class::state_active_arc_class(
					       model_parameters_active_arc_class & model
					       )
{
  ////////////////////////////////////////////
  /*
    Setter of model parameters
   */
  ////////////////////////////////////////////
  set_model_parameters_in_construction(model);
  ////////////////////////////////////////////
  /*
    Calculation of mathematical constants
   */
  ////////////////////////////////////////////
  set_constants();
  ////////////////////////////////////////////
  /*
    Calculation of necessary memory size
   */
  ////////////////////////////////////////////
  calculate_dimensions();
  ////////////////////////////////////////////
  /*
    Initialization of arrays for each modules
   */
  ////////////////////////////////////////////
  // Since configuration interface initialized with using number_of_joints.
  // This should be called after calculation_dimensions.
  configuration.initialize(
			   number_of_joints,
			   number_of_arcs,
			   space_dimension
			   );
  //
  plot_margin=1.5;
  strain_width=1.0;
  // For interior calculation
  //
  /*
   for output
  */
  number_of_displacements=5;
  number_of_displacements_supplements=5;
  number_of_joints_for_arc=2;
  labels_for_output.clear();
  for(
      int observable_index=0;
      observable_index<number_of_displacements
	+number_of_displacements_supplements;
      observable_index++
      )
    {
      labels_for_output.push_back("");
    };
  observables_for_output.clear();
  for(int observable_index=0;
      observable_index
	<number_of_displacements
	+number_of_displacements_supplements;
      observable_index++)
    {
      for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
	{
	  observables_for_output.push_back(0.0);
	};
    };
  //
  /*
    initialization for arc
   */
  initialize_reference(model);
  //
  initialize_local_geometry();
  //
  initialize_displacements();
  //
  initialize_strain();
  //
  initialize_stress();
  /*
    initialization for joints
   */
  coordinates.clear();
  arrows.clear();
  centers.clear();
  work_vectors.clear();
  for(long int joint_index=0;joint_index<number_of_joints;joint_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  coordinates.push_back(0.0);
	  arrows.push_back(0.0);
	  centers.push_back(0.0);
	  work_vectors.push_back(0.0);
	};
     angles.push_back(0.0);
     thickness.push_back(0.0);
    };
  /*
   for reference plot
  */
  number_of_reference_coordinates=10;
  reference_coordinates.clear();
  reference_line_vectors.clear();
  for(int reference_point_index=0;reference_point_index<number_of_reference_coordinates;reference_point_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  reference_coordinates.push_back(0.0);
	  reference_line_vectors.push_back(0.0);
	};
    };
  shape_coordinates.clear();
  for(int reference_point_index=0;reference_point_index<number_of_reference_coordinates;reference_point_index++)
    {
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  shape_coordinates.push_back(0.0);
	};
    };
  // eigenvalues and vectors with arcs
  initialize_diagonlizar();
  //
  /*
    arc energy
   */
  arc_elastic_energy.clear();
  for(int arc_index=0; arc_index<number_of_arcs;arc_index++)
    {
      arc_elastic_energy.push_back(0.0);
    };
  //
  initialize_elastic_force();
  // inner volume force
  initialize_inner_volume_force(model);
  //
  initialize_constraint_stress();
  //
  local_elastic_force_plot_initialization();
  //
  initialize_inner_elasticity(model);
  /*
    Caluculation modulus
   */
  initialize_modulus();
  bending_modulus_switch=model.check_bending_energy_on();
  coupling_modulus_switch=model.check_coupling_energy_on();
  //
  reconstructor_initialize();
  //
  /*
    File IO
  */
  configuration_plot="confplot";
  bit_parameter = 0.000000000000001;
  cg_upper_margin=plot_margin;
  cg_lower_margin=plot_margin;
  //
  shape_plot_max=model.get_shape_plot_max();
  shape_plot_frequency=model.get_shape_plot_frequency();
  shape_plot_base=model.get_shape_plot_base();
  //
  reference.initialize(number_of_arcs);
  stress.initialize(number_of_arcs);
  inner_stress.initialize(number_of_arcs);
};
//
long int state_active_arc_class::get_number_of_arcs()
{
  return (long int)number_of_arcs;
}; 
//
void state_active_arc_class::plot(
				  const long int & time_index,
				  const int & sweep_step,
				  const std::string & format
				  )
{
  gnuplot_driver gplot(configuration_plot);
  std::string direction_label;
  if(format=="gnuplot")
    {
      for(long int joint_index=0;joint_index<number_of_joints-1;joint_index++)
	{
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      arrows[joint_index*space_dimension+direction_index]
		=coordinates[(joint_index+1)*space_dimension+direction_index]
		-coordinates[(joint_index+0)*space_dimension+direction_index];
	    };
	};
      if(boundary_condition=="close")
	{
	  for(int direction_index=0;direction_index<space_dimension;direction_index++)
	    {
	      arrows[(number_of_joints-1)*space_dimension+direction_index]
		=coordinates[                                     direction_index]
		-coordinates[(number_of_joints-1)*space_dimension+direction_index];
	    };
	};
      //
      double max=*std::max_element(coordinates.begin(), coordinates.end());
      double min=*std::min_element(coordinates.begin(), coordinates.end());
      //
      for(int direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  if(direction_index==0)
	    {
	      direction_label="x";
	    } 
	  else if(direction_index==1)
	    {
	      direction_label="y";
	    }
	  else
	    {
	      direction_label="z";
	    }
	  gplot.set_axis(
			 direction_index,
			 min*plot_margin,
			 max*plot_margin,
			 (double)(std::max((int)((double)(max-min)/4.0),1)),
			 direction_label
			 );
	};
      gplot.make_plot_file(
			   0,
			   "png",
			   "splot",
			   time_index,
			   sweep_step
			   );
      gplot.output_vectors_on_loading_file(
					   coordinates,
					   arrows,
					   number_of_joints,
					   "off",
					   "1",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   1.0,
					   "on"
					   );
      for(long int joint_index=1;joint_index<number_of_joints;joint_index++)
	{
	  arrows[joint_index*space_dimension+0]
	    =cos(angles[joint_index])
	    *(arc_width_reference[joint_index-1]
	      +arc_width_reference[joint_index])*0.5;
	  arrows[joint_index*space_dimension+1]
	    =sin(angles[joint_index])
	    *(arc_width_reference[joint_index-1]
	      +arc_width_reference[joint_index])*0.5;
	};
      arrows[0]
	=cos(angles[0])
	*(arc_width_reference[0]
	  +arc_width_reference[number_of_joints-1])*0.5;
      arrows[1]
	=sin(angles[0])
	*(arc_width_reference[0]
	  +arc_width_reference[number_of_joints-1])*0.5;
      gplot.output_vectors_on_loading_file(
					   coordinates,
					   arrows,
					   number_of_joints,
					   "on",
					   "2",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   1.0,
					   "off"
					   );
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "plot",
			     "Error:undefined format!"
			     );
    };
};
//
void state_active_arc_class::update(
				    model_parameters_active_arc_class & model
				    )
{
  local_geometry();
  local_geometry_plot(
		      model,
		      0,
		      0,
		      "gnuplot"
		      );
  calculate_displacement("initialization");
  shape_plot(
	     0,
	     0,
	     "gnuplot"
	     );
  output_displacements(0,0);
  calculate_strain();
  strain_plot(
	      model,
	      0,
	      0,
	      "gnuplot"
	      );
  calculate_stress();
  calculate_elastic_energy();
  calculate_elastic_gradient();
};
void state_active_arc_class::state_debug_io::output_debug_data(
							       const long int & plane_index,
							       const long int & time_index,
							       const int & sweep_step
					       )
{
  std::string message;
  std::string file=
    file_header
    + io_method.longint_to_format_string(plane_index,"%03d") 
    + "_"
    + io_method.longint_to_format_string(time_index,"%04d")  
    + "_"
    + io_method.int_to_format_string(sweep_step,"%04d") 
    +".dat";
  //
  for(
      int comment_index=0;
      comment_index<number_of_comments;
      comment_index++
      )
    {
      message=comments[comment_index];
      io_method.output_message(message,file);
    };
  //
  message="#";
  //
  for(
      int debug_data_index = 0;
      debug_data_index<number_of_debug_data_components;
      debug_data_index++
      )
    {
      message
	+=labels_for_debug_data[debug_data_index] + "/";
    };
  io_method.output_message(message,file);
  for(
      long int debug_data_index = 0;
      debug_data_index<number_of_debug_data;
      debug_data_index++
      )
    {
      message = io_method.longint_to_string(debug_data_index) + " ";
      for(
	  int debug_data_component_index = 0;
	  debug_data_component_index < number_of_debug_data_components;
	  debug_data_component_index ++
	  )
	{
	  message
	    += io_method.double_to_string(
					  debug_data_for_output[
								 debug_data_index
								 *(number_of_debug_data_components)
								 +debug_data_component_index
								 ]
					  ) + " ";
	};
      io_method.output_message(message,file);
    };
};
//
void state_active_arc_class::state_debug_io::set_debug_data(
							    const std::string & label,
							    const std::vector<double> & input_data,
							    const int & label_header,
							    const int & number_of_components
							    )
{
  if(
     label_header>number_of_debug_data_components
     ||
     label_header<0
     )
    {
      io_method.error_output(
			     "state_active_arc_class::state_debug_io",
			     "sed_debug_data",
			     "due to irregual label_header"
			     + io_method.int_to_string(label_header)
			     );
	};
  if(number_of_debug_data*number_of_components!=(long int)input_data.size()
     ||
     (long int)input_data.size()<=0)
    { 
      io_method.error_output(
			     "state_active_arc_class::state_debug_io",
			     "sed_debug_data",
			     "inconsistency or zero in input data size on "
			     +  label + " for number_of_debug_data = "
			     + io_method.longint_to_string(
							   number_of_debug_data
							   )
			     + " and input_data.size() = "
			     + io_method.longint_to_string(
							   (double)input_data.size()
							   )
			     );
    };
  if(
     (long int)debug_data_for_output.size()
     <(number_of_debug_data-1)*number_of_debug_data_components
     +label_header+number_of_components)
    { 
      io_method.error_output(
			     "state_active_arc_class::state_debug_io",
			     "sed_debug_data",
			     "inconsistency in data size on "
			     +  label + " debug_data_for_output.size() = "
			     + io_method.longint_to_string(
							   debug_data_for_output.size()
							   )
			     + " max data number = "
			     + io_method.longint_to_string(
							   (double)(
								    (number_of_debug_data-1)*number_of_debug_data_components+label_header+number_of_components
								    )
							   )
			     );
    };
  //
  for(
      int component_index=0;
      component_index<number_of_components;
      component_index++
      )
    {
      labels_for_debug_data[label_header+component_index]=label;
      for(
	  long int data_index=0;
	  data_index<number_of_debug_data;
	  data_index++
	  )
	{
	  debug_data_for_output[
				data_index*number_of_debug_data_components
				+label_header+component_index
				]
	    =input_data[data_index*number_of_components+component_index];
	};
    };
};
//
state_active_arc_class::state_debug_io::state_debug_io(
						       const std::string & input_file_header,
						       const int & input_number_of_comments,
						       const long int & input_number_of_debug_data,
						       const int & input_number_of_debug_data_components
						       )
{
  file_header=input_file_header;
  number_of_comments=input_number_of_comments;
  number_of_debug_data=input_number_of_debug_data;
  number_of_debug_data_components=input_number_of_debug_data_components;
  labels_for_debug_data.clear();
  debug_data_for_output.clear();
  comments.clear();
  for(
      int comment_index=0;
      comment_index<number_of_comments;
      comment_index++
      )
    {
      comments.push_back("");
    };
  for(
      int component_index=0;
      component_index<number_of_debug_data_components;
      component_index++
      )
    {
      labels_for_debug_data.push_back("");
      for(
	  long int data_index=0;
	  data_index<number_of_debug_data;
	  data_index++
	  )
	{
	  debug_data_for_output.push_back(0.0);
	};
    };
};
//
void state_active_arc_class::state_debug_io::set_debug_comment(
							       const std::string & comment,
							       const int & comment_index
							       )
{
  comments[comment_index]=comment;
};
//
void state_active_arc_class::summarize_energy_gradient(
						       double & optimization_function,
						       alglib::real_1d_array & function_gradient
						       )
{
  // optimization_funtion
  total_energy
    =total_elastic_energy;
  if(inner_type=="volume"||inner_type=="volume_radius")
    {
      total_energy
	+=inner_volume_energy;
    }
  else if (inner_type=="elastic")
    {
      total_energy
	+=inner_elastic_energy;
    }
  optimization_function
    =total_energy;
  // gradient
  for(
      long int component_index = 0;
      component_index < (long int)gradient.size();
      component_index ++
      )
    {
      gradient[component_index] 
	= elastic_force[component_index];
    };  
  //
  if(inner_type=="volume"||inner_type=="volume_radius")
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  for(
	      long int joint_index=0;
	      joint_index<number_of_joints;
	      joint_index++
	      )
	    {
	      gradient[joint_index*number_of_joint_components+direction_index]
		=gradient[joint_index*number_of_joint_components+direction_index]
		+inner_volume_force[joint_index*space_dimension+direction_index];
	    };
	};
    }
  else if (inner_type=="elastic")
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  for(
	      long int joint_index=0;
	      joint_index<number_of_joints;
	      joint_index++
	      )
	    {
	      gradient[joint_index*number_of_joint_components+direction_index]
		=gradient[joint_index*number_of_joint_components+direction_index]
		+inner_elastic_force[joint_index*space_dimension+direction_index];
	    };
	};
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  gradient[constraint_head_pointer+direction_index]
	    =center_elastic_force[direction_index];
	};
    } 
  else
    {
      io_method.error_output(
			     "active arc state",
			     "summarize_energy_gradient",
			     "irrigal inner type");
    };
  // gradient
  for(
      long int component_index = 0;
      component_index < (long int)gradient.size();
      component_index ++
      )
    {
      function_gradient[component_index] 
	= -gradient[component_index];
    };  
  //
};
//
void state_active_arc_class::finalization(
					  const int & sweep_step,
					  const std::string & label,
					  const model_parameters_active_arc_class & model
					  )
{
  io_method.standard_output("///////////Last of " + label +"///////////");
  //
  long int time_index;
  if(label=="md")
    {
      time_index=999;
    }
  else  if(label=="cg")
    {
      time_index=998;
    }
  // for local_geometry
  local_geometry_plot(
		      model,
		      time_index,
		      sweep_step,
		      "gnuplot"
		      );
  // for displacements
  shape_plot(
	     time_index,
	     sweep_step,
	     "gnuplot"
	     );
  output_displacements(time_index,sweep_step);
  //  strain;
  output_strain(time_index,sweep_step);
  //  stress;
  output_stress(time_index,sweep_step);
  local_stress_plot(
		    model,
		    time_index,
		    sweep_step,
		    "gnuplot"
		    );
  //  energy & gradients;
  output_forces(time_index,sweep_step);
  output_preforces(time_index,sweep_step);
  output_moments(time_index,sweep_step);
  local_force_plot(model,time_index,sweep_step,"gnuplot");
  local_moment_plot(model,time_index,sweep_step,"gnuplot");
  //  inner_force;
  if(inner_type=="volume"||inner_type=="volume_radius")
    {
      output_inner_values(time_index,sweep_step);
      local_inner_force_plot(model,time_index,sweep_step,"gnuplot");
    }
  else if (inner_type=="elastic")
    {
      inner_elasticity_plot(model,time_index,sweep_step,"gnuplot");
    }
  else
    {
      io_method.error_output(
			     "active_arc_state",
			     "driver_cg",
			     "irrigal inner type!"
			     );
    }
  //
  io_method.standard_output("Result of energy & gradient");
  io_method.standard_output("energy = " + io_method.double_to_string(total_energy));
  for(
      long int component_index = 0;  
      component_index < (long int)gradient.size(); 
      component_index ++
      )
    {
      io_method.standard_output("grad["+io_method.int_to_string(component_index)+"]="
				+io_method.double_to_string(-gradient[component_index]));
    };
};
//
bool state_active_arc_class::iteration_check(
					     const long int & iteration_index,
					     const long int & iteration_max,
					     const long int & times,
					     const long int & iteration_base,
					     long int & plot_index
					     )
{
  long int period=(long int)(std::max((iteration_max-iteration_base)/times,(long int)2));
  plot_index = (iteration_index-iteration_base)/period; 
  if(
     (iteration_max>=iteration_index)
     &&
     (iteration_index>=iteration_base)
     &&
     ((iteration_index-iteration_base)%period==0)
     ) return true;
  return false;
};
//
void state_active_arc_class::reference_set()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      arc_length_reference[arc_index]=reference.length[arc_index];
      arc_width_reference[arc_index]=reference.width[arc_index];
      arc_curvature_reference[arc_index]=reference.curvature[arc_index];
      arc_young_modulus_reference[arc_index]=reference.young_modulus[arc_index];
      arc_poisson_ratio_reference[arc_index]=reference.poisson_ratio[arc_index];
      inner_control_volume[arc_index]=reference.inner_control_volume[arc_index];
      inner_material_Young_modulus[arc_index]=reference.inner_young_modulus[arc_index];
      inner_material_Poisson_ratio[arc_index]=reference.inner_poisson_ratio[arc_index];
    };
  if(inner_type=="elastic")
    {
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  inner_width[arc_index]=reference.inner_width[arc_index];
	  inner_height[arc_index]=reference.inner_height[arc_index];
	};
      calculate_inner_control_volume();
    };
  calculate_modulus();
  push_reference_box();
};
//
void state_active_arc_class::reference_get()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      reference.length[arc_index]=arc_length_reference[arc_index];
      reference.width[arc_index]=arc_width_reference[arc_index];
      reference.curvature[arc_index]=arc_curvature_reference[arc_index];
      reference.young_modulus[arc_index]=arc_young_modulus_reference[arc_index];
      reference.poisson_ratio[arc_index]=arc_poisson_ratio_reference[arc_index];
      reference.inner_control_volume[arc_index]=inner_control_volume[arc_index];
      reference.inner_young_modulus[arc_index]=inner_material_Young_modulus[arc_index];
      reference.inner_poisson_ratio[arc_index]=inner_material_Poisson_ratio[arc_index];
    };
  if(inner_type=="elastic")
    {
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  reference.inner_width[arc_index]=inner_width[arc_index];
	  reference.inner_height[arc_index]=inner_height[arc_index];
	};
    };
};
//
void state_active_arc_class::configuration_set()
{
  for(
      long int joint_index=0;
      joint_index<number_of_joints;
      joint_index++
      )
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  coordinates[joint_index*space_dimension+direction_index]
	    = configuration.coordinates[joint_index*space_dimension+direction_index];
	};
      angles[joint_index]
	= configuration.angles[joint_index];
      thickness[joint_index]
	= configuration.thickness[joint_index];
    };
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  ripening[arc_index]=configuration.ripening[arc_index];
	  normal_modulation[arc_index]=configuration.normal_modulation[arc_index];
	  inner_volume[arc_index]=configuration.inner_volume[arc_index];
	  inner_elongation[arc_index]=configuration.inner_elongation[arc_index];
	  inner_ripening[arc_index]=configuration.inner_ripening[arc_index];
	  inner_shear[arc_index]=configuration.inner_shear[arc_index];
	};
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  center_of_coordinates[direction_index]
	    =configuration.center_of_coordinates[direction_index];
	};
};
//
void state_active_arc_class::configuration_get()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      configuration.inner_volume[arc_index]=inner_volume[arc_index];
    };
  for(
      long int joint_index=0;
      joint_index<number_of_joints;
      joint_index++
      )
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  configuration.coordinates[joint_index*space_dimension+direction_index]
	    = coordinates[joint_index*space_dimension+direction_index];
	};
      configuration.angles[joint_index]
	= angles[joint_index];
      configuration.thickness[joint_index]
	= thickness[joint_index];
    };
  //
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  configuration.ripening[arc_index]=ripening[arc_index];
	  configuration.normal_modulation[arc_index]=normal_modulation[arc_index];
	  configuration.inner_volume[arc_index]=inner_volume[arc_index];
	  configuration.inner_elongation[arc_index]=inner_elongation[arc_index];
	  configuration.inner_ripening[arc_index]=inner_ripening[arc_index];
	  configuration.inner_shear[arc_index]=inner_shear[arc_index];
	};
      //
  for(
      int direction_index=0;
      direction_index<space_dimension;
      direction_index++
      )
    {
      configuration.center_of_coordinates[direction_index]
	=center_of_coordinates[direction_index];
    };
};
//
void  state_active_arc_class::reconstructor_initialize()
{
  division_on.clear();
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      division_on.push_back(false);
    };
};
//
bool state_active_arc_class::reconstruction_check(
						  model_reconstructor & reconstructor
						  )
{
  // initialization
  reconstruction_on=false;
  long int new_number_of_arcs=number_of_arcs;
  // check of division
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      if(arc_length_reference[arc_index]>2.0*arc_length)
	{
	  division_on[arc_index]=true;
	  reconstruction_on=true;
	  new_number_of_arcs=new_number_of_arcs+1;
	}
      else
	{
	  division_on[arc_index]=false;
	};
    };
  if(reconstruction_on)
    {
      reconstructor.new_number_of_arcs=new_number_of_arcs;
      reference_get();
      configuration_get();
      reconstructor.new_construction(
				     number_of_arcs,
				     number_of_joints,
				     space_dimension,
				     division_on,
				     reference,
				     configuration
				     );
    }
  return reconstruction_on;
};
//
void state_active_arc_class::get_division_on(
					     std::vector<bool> & output_division_on
					     )
{
  if(output_division_on.size()==division_on.size())
    {
      long int counter=0;
      for(
	  auto index=output_division_on.begin();
	  index != output_division_on.end();
	  index++
	  )
	{
	  *index=division_on[counter];
	  counter++;
	}
    }
  else
    {
      io_method.error_output(
			     "state_active_arc_class",
			     "get_division_on",
			     "inconsistency in array size !:"
			     + io_method.longint_to_string(
							   (long int)output_division_on.size()
							   ) 
			     + "!=" 
			     + io_method.longint_to_string(
							   (long int)division_on.size()
							   )
			     );
    };
};
//
bool state_active_arc_class::nan_check( const long long int & error_code)
{
  bool return_value=false;
  long int count=0;
  for(auto index = coordinates.begin();
      index != coordinates.end();
      index++ )
    {
      if(isnan(*index)!= 0)
	{
	  return_value = true;
	  io_method.standard_output(
				    "::::::NaN is found!::::: at "
				    + io_method.longlongint_to_string(error_code)
				    + " for coordinates # "
				    + io_method.longint_to_string(count)
				    +" ");
	};
    };
  count=0;
  for(auto index = angles.begin();
      index != angles.end();
      index++ )
    {
      if(isnan(*index)!= 0)
	{
	  return_value = true;
	  io_method.standard_output(
				    "::::::NaN is found!::::: at "
				    + io_method.longlongint_to_string(error_code)
				    + " for angles # "
				    + io_method.longint_to_string(count)
				    +" ");
	};
    };
  count=0;
  for(auto index = thickness.begin();
      index != thickness.end();
      index++ )
    {
      if(isnan(*index)!= 0)
	{
	  return_value = true;
	  io_method.standard_output(
				    "::::::NaN is found!::::: at "
				    + io_method.longlongint_to_string(error_code)
				    + " for thickness # "
				    + io_method.longint_to_string(count)
				    +" ");
	};
    };
  return return_value;
}
//
void  state_active_arc_class::reconstruction(
					     model_reconstructor & reconstructor,
					     const long int & sweep_step
					     )
{
  reconstructor.get_construction(
				 reference,
				 configuration
				 );
  reference_set();
  configuration_set();
  output_configuration(
		       1000,
		       sweep_step
		       );
  output_reference(
		   1002,
		   sweep_step
		   );
  shape_plot(
	     1001,
	     sweep_step,
	     "gnuplot"
	     );
  reference_plot(
		 1002,
		 sweep_step,
		 "gnuplot"
		 );
};
