#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::calculate_inner()
{
  if(inner_type=="volume"||inner_type=="volume_radius")
    {
      calculate_innervolumic();
    }
  else if (inner_type=="elastic")
    {
      calculate_inner_elasticity();
    }
};
//
void state_active_arc_class::initialize_inner_elasticity(
							 model_parameters_active_arc_class & model
							 )
{
  inner_type=model.get_inner_type();
  if(inner_type=="elastic")
    {
      initialize_inner_geometry(model);
      initialize_inner_deformation();
      initialize_inner_stress();
      initialize_inner_force();
    };
};
//
void state_active_arc_class::initialize_inner_geometry(
							 model_parameters_active_arc_class & model
						       )
{
  inner_height.clear();
  inner_width.clear();
  for(
      long int arc_index = 0;
      arc_index< number_of_arcs;
      arc_index++
      )
    {
      inner_height.push_back(model.get_inner_height());
      inner_width.push_back(model.get_inner_width());
    };
};
//
void state_active_arc_class::initialize_inner_deformation()
{
  inner_elongation.clear();
  inner_ripening.clear();
  inner_shear.clear();
  for(
      long int arc_index = 0;
      arc_index< number_of_arcs;
      arc_index++
      )
    {
      inner_elongation.push_back(0.0);
      inner_ripening.push_back(0.0);
      inner_shear.push_back(0.0);
    };
};
//
void state_active_arc_class::initialize_inner_stress()
{
  inner_elongation_stress.clear();
  inner_ripening_stress.clear();
  inner_shear_stress.clear();
  for(
      long int arc_index = 0;
      arc_index< number_of_arcs;
      arc_index++
      )
    {
      inner_elongation_stress.push_back(0.0);
      inner_ripening_stress.push_back(0.0);
      inner_shear_stress.push_back(0.0);
    };
};
//
void state_active_arc_class::initialize_inner_force()
{
  inner_elastic_force.clear();
  center_elastic_force.clear();
  inner_elongation_force.clear();
  inner_ripening_force.clear();
  inner_normal_torque.clear();
  inner_centertangent_torque.clear();
  inner_force_for_plot.clear();
  for(
      int direction_index = 0;
      direction_index < space_dimension;
      direction_index ++
      )
    {
	  center_elastic_force.push_back(0.0);
	  center_force_for_plot.push_back(0.0);
	};
  for(
      long int arc_index = 0;
      arc_index< number_of_arcs;
      arc_index++
      )
    {
      for(
	  int direction_index = 0;
	  direction_index < space_dimension;
	  direction_index ++
	  )
	{
	  inner_elastic_force.push_back(0.0);
	  inner_force_for_plot.push_back(0.0);
	};
      inner_elongation_force.push_back(0.0);
      inner_ripening_force.push_back(0.0);
      inner_normal_torque.push_back(0.0);
      inner_centertangent_torque.push_back(0.0);
    };
};
//
void state_active_arc_class::calculate_inner_elasticity()
{
  local_center_geometry();
  calculate_inner_deformation();
  calculate_inner_stress();
  calculate_inner_preforce();
  calculate_inner_energy();
  calculate_inner_elastic_force();
}
//
void state_active_arc_class::inner_elasticity_plot(
						   const model_parameters_active_arc_class & model,
						   const long int & time_index,
						   const int & sweep_step,
						   const std::string & format
						   )
{
  output_inner_deformation(
			   time_index,
			   sweep_step
			   );
  output_inner_stress(
		      time_index,
		      sweep_step
		      );
  output_inner_preforce(
			time_index,
			sweep_step
			);
  output_inner_force(
		     time_index,
		     sweep_step
		     );
  local_center_geometry_plot(
			     model,
			     time_index,
			     sweep_step,
			     format
			     );
  output_inner_force_plot(
			  model,
			  time_index,
			  sweep_step,
			  format
			  );
};
//
void state_active_arc_class::calculate_inner_deformation()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      inner_elongation[arc_index]=1.0-inner_width[arc_index]/end_to_end_length[arc_index];
      inner_ripening[arc_index]=1.0-inner_height[arc_index]/center_to_center_length[arc_index];
      inner_shear[arc_index]
	=atan2(
	       std::inner_product(
				  center_normal_vectors.begin()+(arc_index  )*space_dimension,
				  center_normal_vectors.begin()+(arc_index+1)*space_dimension,
				  tangent_vectors.begin()      +(arc_index  )*space_dimension,
				  0.0
				  )
	       ,
	       std::inner_product(
				  center_normal_vectors.begin()+(arc_index  )*space_dimension,
				  center_normal_vectors.begin()+(arc_index+1)*space_dimension,
				  normal_vectors.begin()       +(arc_index  )*space_dimension,
				  0.0
				  )
	       );
    };
};
//
void state_active_arc_class::calculate_inner_stress()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      inner_elongation_stress[arc_index]
	=inner_bulk_modulus[arc_index]
	*(inner_elongation[arc_index]
	  +inner_material_Poisson_ratio[arc_index]
	  *inner_ripening[arc_index]);
      inner_ripening_stress[arc_index]
	=inner_bulk_modulus[arc_index]
	*(inner_ripening[arc_index]
	+inner_material_Poisson_ratio[arc_index]
	  *inner_elongation[arc_index]);
      inner_shear_stress[arc_index]
	=inner_bulk_modulus[arc_index]
	*(1-inner_material_Poisson_ratio[arc_index])
	*inner_shear[arc_index];
    };
};
//
void state_active_arc_class::inner_stress_get()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      inner_stress.inner_elongation_stress[arc_index]
	=inner_elongation_stress[arc_index];
      inner_stress.inner_ripening_stress[arc_index]
	=inner_ripening_stress[arc_index];
      inner_stress.inner_shear_stress[arc_index]
	=inner_shear_stress[arc_index];
    };
};
//
void state_active_arc_class::calculate_inner_preforce()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
     inner_elongation_force[arc_index]
       =inner_width[arc_index]
       *inner_elongation_stress[arc_index]
       /end_to_end_length[arc_index]
       /end_to_end_length[arc_index];
     inner_ripening_force[arc_index]
       =-0.5*inner_height[arc_index]
       *inner_ripening_stress[arc_index]
       /center_to_center_length[arc_index]
       /center_to_center_length[arc_index];
     inner_normal_torque[arc_index]
       =inner_shear_stress[arc_index]
       /end_to_end_length[arc_index];
     inner_centertangent_torque[arc_index]
       =-0.5*inner_shear_stress[arc_index]
       /center_to_center_length[arc_index];
    };
};
//
void state_active_arc_class::calculate_inner_energy()
{
  inner_elastic_energy=0.0;
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      inner_elastic_energy
	+=
	0.5*inner_bulk_modulus[arc_index]
	*(
	  inner_elongation[arc_index]
	  *inner_elongation[arc_index]
	  +
	  2.0*inner_material_Poisson_ratio[arc_index]
	  *inner_elongation[arc_index]
	  *inner_ripening[arc_index]
	  +
	  inner_ripening[arc_index]
	  *inner_ripening[arc_index]
	  )
	+
	0.5*(1.0-inner_material_Poisson_ratio[arc_index])
	*inner_shear[arc_index]
	*inner_shear[arc_index];
    };
};
//
void state_active_arc_class::calculate_inner_elastic_force()
{
  if(boundary_condition=="close")
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  center_elastic_force[direction_index]=0.0;
	  for(
	      long int arc_index=0;
	      arc_index<number_of_arcs;
	      arc_index++
	      )
	    {
	      center_elastic_force[direction_index]
		+=
		-
		2.0*inner_ripening_force[arc_index]
		*center_normal_vectors[arc_index*space_dimension+direction_index]
		-
		2.0*inner_centertangent_torque[arc_index]
		*center_tangent_vectors[(arc_index)*space_dimension+direction_index];
	    };
	};
      //
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  for(
	      long int joint_index=1;
	      joint_index<number_of_joints;
	      joint_index++
	      )
	    {
	      inner_elastic_force[joint_index*space_dimension+direction_index]
		=
		+
		inner_elongation_force[joint_index]
		*tangent_vectors[joint_index*space_dimension+direction_index]
		-
		inner_elongation_force[joint_index-1]
		*tangent_vectors[(joint_index-1)*space_dimension+direction_index]
		
		+
		inner_ripening_force[joint_index]
		*center_normal_vectors[joint_index*space_dimension+direction_index]
		+
		inner_ripening_force[joint_index-1]
		*center_normal_vectors[(joint_index-1)*space_dimension+direction_index]
		+
		inner_normal_torque[joint_index]
		*normal_vectors[joint_index*space_dimension+direction_index]
		-
		inner_normal_torque[joint_index-1]
		*normal_vectors[(joint_index-1)*space_dimension+direction_index]
		+
		inner_centertangent_torque[joint_index]
		*center_tangent_vectors[joint_index*space_dimension+direction_index]
		+
		inner_centertangent_torque[joint_index-1]
		*center_tangent_vectors[(joint_index-1)*space_dimension+direction_index];
	    };
	  inner_elastic_force[direction_index]
	    =
	    +
	    inner_elongation_force[0]
	    *tangent_vectors[direction_index]
	    -
	    inner_elongation_force[number_of_joints-1]
	    *tangent_vectors[(number_of_joints-1)*space_dimension+direction_index]
	    +
	    inner_ripening_force[0]
	    *center_normal_vectors[direction_index]
	    +
	    inner_ripening_force[number_of_joints-1]
	    *center_normal_vectors[(number_of_joints-1)*space_dimension+direction_index]
	    +
	    inner_normal_torque[0]
	    *normal_vectors[direction_index]
	    -
	    inner_normal_torque[number_of_joints-1]
	    *normal_vectors[(number_of_joints-1)*space_dimension+direction_index]
	    +
	    inner_centertangent_torque[0]
	    *center_tangent_vectors[direction_index]
	    +
	    inner_centertangent_torque[(number_of_joints)-1]
	    *center_tangent_vectors[(number_of_joints-1)*space_dimension+direction_index]
	    ;
	};
    }
  else
    {
      io_method.error_output(
			     "active_arc_innerelasticity",
			     "calculate_inner_force",
			     "Undefined boundary condition!"
			     );
    }
};
//
void state_active_arc_class::output_inner_force_plot(
						     const model_parameters_active_arc_class & model,
						     const long int & time_index,
						     const int & sweep_step,
						     const std::string & format
						     )
{
  gnuplot_driver gplot("inner_force_plot");
  std::string direction_label;
  if(format=="gnuplot")
    {
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
	  if(space_dimension==2&&direction_index>0)
	    {
	      direction_label="z";
	      gplot.set_axis(
			     direction_index,
			     min*plot_margin,
			     max*plot_margin,
			     (double)(std::max((int)((double)(max-min)/4.0),1)),
			     direction_label
			     );
	    };
	};
      gplot.make_plot_file(
			   0,
			   "png",
			   "splot",
			   time_index,
			   sweep_step
			   );
      double absolute
	=*std::max_element(
			   inner_elastic_force.begin(), 
			   inner_elastic_force.end()  
			   );
      std::vector<double>::iterator index=inner_elastic_force.begin();
      std::vector<double>::iterator index2=inner_force_for_plot.begin();
      while(index!=inner_elastic_force.end())
	{
	  *index2=*index/absolute*((double)(max-min)/15.0);
	  index2++;
	  index++;
	};
      gplot.output_vectors_on_loading_file(
					   coordinates,
					   inner_force_for_plot,
					   number_of_joints,
					   "on",
					   "3",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "on"
					   );
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  center_force_for_plot[direction_index]
	    =center_elastic_force[direction_index]
	    /absolute*((double)(max-min)/15.0);
	}
      gplot.output_vectors_on_loading_file(
					   center_of_coordinates,
					   center_force_for_plot,
					   1,
					   "on",
					   "4",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "off"
					   );
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "inner_force_plot",
			     "Error:undefined format!"
			     );
    };
};
//
void state_active_arc_class::output_inner_deformation(
						      const long int & time_index,
						      const int & sweep_step
						      )
{
  state_debug_io debug_inner_deformation(
			     "inner_deformation_output",
			     1,
			     number_of_arcs,
			     3
			     );
  debug_inner_deformation.set_debug_comment(
			     "center force = ("
			     +
			     io_method.double_to_string(center_elastic_force[0])
			     +
			     ","
			     +
			     io_method.double_to_string(center_elastic_force[1])
			     +
			     ")",
			     0
			     );
  debug_inner_deformation.set_debug_data(
			     "elongation",
			     inner_elongation,
			     0,
			     1
			     );
  debug_inner_deformation.set_debug_data(
			     "elongation",
			     inner_elongation,
			     0,
			     1
			     );
  debug_inner_deformation.set_debug_data(
			     "ripening",
			     inner_ripening,
			     1,
			     1
			     );
  debug_inner_deformation.set_debug_data(
			     "shear",
			     inner_shear,
			     2,
			     1
			     );
  debug_inner_deformation.output_debug_data(
				0,
				time_index,
				sweep_step
				);
};					
//
void state_active_arc_class::output_inner_stress(
						 const long int & time_index,
						 const int & sweep_step
						 )	
{
  state_debug_io debug_inner_stress(
			     "inner_stress_output",
			     0,
			     number_of_arcs,
			     3
			     );
  debug_inner_stress.set_debug_data(
			     "elongation",
			     inner_elongation_stress,
			     0,
			     1
			     );
  debug_inner_stress.set_debug_data(
			     "ripening",
			     inner_ripening_stress,
			     1,
			     1
			     );
  debug_inner_stress.set_debug_data(
			     "shear",
			     inner_shear_stress,
			     2,
			     1
			     );
  debug_inner_stress.output_debug_data(
				0,
				time_index,
				sweep_step
				);
  
};			
//	
void state_active_arc_class::output_inner_preforce(
						   const long int & time_index,
						   const int & sweep_step
						   )	
{
  state_debug_io debug_inner_preforce(
				      "inner_preforce_output",
				      0,
				      number_of_arcs,
				      6
				      );
  debug_inner_preforce.set_debug_data(
				      "elongation",
				      inner_elongation_force,
				      0,
				      1
				      );
  debug_inner_preforce.set_debug_data(
				      "ripening",
				      inner_ripening_force,
				      1,
				      1
				      );
  debug_inner_preforce.set_debug_data(
				      "normal_torque",
				      inner_normal_torque,
				      2,
				      1
				      );
  debug_inner_preforce.set_debug_data(
				      "inner_center_tangent_torque",
				      inner_centertangent_torque,
				      3,
				      1
				      );	
  debug_inner_preforce.set_debug_data(
				      "end_to_end_length",
				      end_to_end_length,
				      4,
				      1
				      );
  debug_inner_preforce.set_debug_data(
				      "center_to_center_length",
				      center_to_center_length,
				      5,
				      1
				      );
  debug_inner_preforce.output_debug_data(
					 0,
					 time_index,
					 sweep_step
					 );
  
};
//	
void state_active_arc_class::output_inner_force(
						const long int & time_index,
						const int & sweep_step
						)	
{
  state_debug_io debug_inner_force(
				   "inner_force_output",
				   1,
				   number_of_arcs,
				   2
				   );
  debug_inner_force.set_debug_comment(
				      "inner elastic energy =" + io_method.double_to_string(inner_elastic_energy),
				      0
				      );
  debug_inner_force.set_debug_data(
				   "inner_elastic_force",
				   inner_elastic_force,
				   0,
				   2
				   );
  debug_inner_force.output_debug_data(
				      0,
				      time_index,
				      sweep_step
				      );
};
