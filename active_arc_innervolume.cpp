#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::initialize_inner_volume_force(
							   model_parameters_active_arc_class & model
							   )
{
  inner_volume.clear();
  inner_volume_force.clear();
  inner_volume_force_for_plot.clear();
  for(
      long int joint_index=0;
      joint_index<number_of_joints;
      joint_index++
      )
    {
      inner_volume_force.push_back(0.0);
      inner_volume_force.push_back(0.0);
      inner_volume_force_for_plot.push_back(0.0);
      inner_volume_force_for_plot.push_back(0.0);
    };
  //
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      inner_volume.push_back(0.0);
      inner_control_volume.push_back(model.get_control_volume());
      inner_material_Young_modulus.push_back(model.get_inner_material_Young_modulus());
      inner_material_Poisson_ratio.push_back(model.get_inner_material_Poisson_ratio());
    };
}
//
void state_active_arc_class::calculate_innervolumic()
{
  calculate_inner_volume();
  calculate_inner_volume_energy();
  calculate_inner_volume_force();
};
//
void state_active_arc_class::calculate_inner_control_volume()
{
  if(inner_type=="volume"||inner_type=="volume_radius")
    {
      io_method.standard_output("Warning: unexpectedly no effect passing (calculate_inner_control_volume)");
    }
  else if(inner_type=="elastic")
    {
      for(
	  long int arc_index=0;
	  arc_index < number_of_arcs;
	  arc_index++
	  )
	{
	  inner_control_volume[arc_index]
	    =inner_width[arc_index]
	    *inner_height[arc_index]
	    *0.5;
	};
    }
  else
    {
      io_method.error_output(
			     "active_arc_innervolume",
			     "calculate_inner_control_volume",
			     "irregal inner type assign!"
			     );
    };
};
//
void state_active_arc_class::calculate_inner_volume()
{
  if(boundary_condition=="close")
    {
      for(
	  long int joint_index = 0; 
	  joint_index < number_of_joints-1;
	  joint_index++
	  )
	{
	  inner_volume[joint_index]
	    =
	    (
	    -coordinates[(joint_index+0)*space_dimension+0]
	    *coordinates[(joint_index+1)*space_dimension+1]
	    +
	    +coordinates[(joint_index+0)*space_dimension+1]
	    *coordinates[(joint_index+1)*space_dimension+0]
	    )*0.5;
	};
      inner_volume[number_of_arcs-1]
	=
	(
	-coordinates[(number_of_arcs-1)*space_dimension+0]
	*coordinates[                                   1]
	+
	+coordinates[(number_of_arcs-1)*space_dimension+1]
	*coordinates[                                   0]
	 )*0.5;
    }
  else
    {
      for(
	  long int joint_index = 0; 
	  joint_index < number_of_joints-1;
	  joint_index++
	  )
	{
	  inner_volume[joint_index]=0.0;
	};
      /*
      for(
	  long int joint_index = 0; 
	  joint_index < number_of_joints;
	  joint_index++
	  )
	{
	  inner_volume
	    =inner_volume
	    +coordinates[(joint_index+0)*space_dimension+0]
	    *coordinates[(joint_index+1)*space_dimension+1]
	    -
	    +coordinates[(joint_index+0)*space_dimension+1]
	    *coordinates[(joint_index+1)*space_dimension+0];
	    };
      */
    };
};
//
void state_active_arc_class::calculate_inner_volume_energy()
{
  inner_volume_energy=0.0;
  if(boundary_condition=="close")
    {
      for(
	  long int arc_index=0;
	  arc_index<number_of_arcs;
	  arc_index++
	  )
	{
	  inner_volume_energy
	    =inner_volume_energy
	    +inner_bulk_modulus[arc_index]*0.5
	    *(inner_volume[arc_index]-inner_control_volume[arc_index])
	    *(inner_volume[arc_index]-inner_control_volume[arc_index]);
	};
    };
};
//
void state_active_arc_class::calculate_inner_volume_force()
{
  if(boundary_condition=="close")
    {
      for(
	  long int joint_index = 1; 
	  joint_index < number_of_joints-1;
	  joint_index++
	  )
	{
	  inner_volume_force[joint_index*space_dimension+0]
	    =
	    coordinates[(joint_index+1)*space_dimension+1]
	    *(inner_volume[joint_index]-inner_control_volume[joint_index])
	    *inner_bulk_modulus[joint_index]*0.5
	    -
	    coordinates[(joint_index-1)*space_dimension+1]
	    *(inner_volume[joint_index-1]-inner_control_volume[joint_index-1])
	    *inner_bulk_modulus[joint_index-1]*0.5;
	  inner_volume_force[joint_index*space_dimension+1]
	    =
	    -coordinates[(joint_index+1)*space_dimension+0]
	    *(inner_volume[joint_index]-inner_control_volume[joint_index])
	    *inner_bulk_modulus[joint_index]*0.5
	    +coordinates[(joint_index-1)*space_dimension+0]
	    *(inner_volume[joint_index-1]-inner_control_volume[joint_index-1])
	    *inner_bulk_modulus[joint_index-1]*0.5;
	};
      // for 0
      inner_volume_force[0]
	=
	coordinates[                      space_dimension+1]
	*(inner_volume[0]-inner_control_volume[0])
	*inner_bulk_modulus[0]*0.5
	-coordinates[(number_of_joints-1)*space_dimension+1]
	*(inner_volume[number_of_joints-1]-inner_control_volume[number_of_joints-1])
	*inner_bulk_modulus[number_of_joints-1]*0.5;
      inner_volume_force[1]
	=
	-coordinates[                     space_dimension+0]
	*(inner_volume[0]-inner_control_volume[0])
	*inner_bulk_modulus[0]*0.5
	+coordinates[(number_of_joints-1)*space_dimension+0]
	*(inner_volume[number_of_joints-1]-inner_control_volume[number_of_joints-1])
	*inner_bulk_modulus[number_of_joints-1]*0.5;
      // for number_of_joints-1
      inner_volume_force[(number_of_joints-1)*space_dimension+0]
	=
	coordinates[1]
	*(inner_volume[number_of_joints-1]-inner_control_volume[number_of_joints-1])
	*inner_bulk_modulus[number_of_joints-1]*0.5
	-coordinates[(number_of_joints-2)*space_dimension+1]
	*(inner_volume[number_of_joints-2]-inner_control_volume[number_of_joints-2])
	*inner_bulk_modulus[number_of_joints-2]*0.5;
      inner_volume_force[(number_of_joints-1)*space_dimension+1]
	=
	-coordinates[0]
	*(inner_volume[number_of_joints-1]-inner_control_volume[number_of_joints-1])
	*inner_bulk_modulus[number_of_joints-1]*0.5
	+coordinates[(number_of_joints-2)*space_dimension+0]
	*(inner_volume[number_of_joints-2]-inner_control_volume[number_of_joints-2])
	*inner_bulk_modulus[number_of_joints-2]*0.5;      
    }
  else
    {
      for(int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++)
	{
	  for(
	      long int joint_index = 0; 
	      joint_index < number_of_joints;
	      joint_index++
	      )
	    {
	      inner_volume_force[joint_index*space_dimension+direction_index]
		= 0.0;
	    };
	};
      /*
      for(
	  long int joint_index = 1; 
	  joint_index < number_of_joints-1;
	  joint_index++
	  )
	{
	  inner_volume_force[joint_index*space_dimension+0]
	    =
	     coordinates[(joint_index-1)*space_dimension+1]
	    -coordinates[(joint_index+1)*space_dimension+1];
	  inner_volume_force[joint_index*space_dimension+1]
	    =
	    -coordinates[(joint_index-1)*space_dimension+0]
	    -coordinates[(joint_index+1)*space_dimension+0];
	};
      // for 0
      inner_volume_force[0]
	=
	-coordinates[(                 1)*space_dimension+1];
      inner_volume_force[1]
	=
	+coordinates[(                 1)*space_dimension+0];
      // for number_of_joints-1
      inner_volume_force[(number_of_joints-1)*space_dimension+0]
	=
	coordinates[(number_of_joints-2)*space_dimension+1];
      inner_volume_force[(number_of_joints-1)*space_dimension+1]
	=
	-coordinates[(number_of_joints-2)*space_dimension+0]; 
      */
    };
  //
  /*
  for(
      int direction_index = 0;
      direction_index < space_dimension;
      direction_index++
      )
    {
      for(
	  long int joint_index = 0; 
	  joint_index < number_of_joints;
	  joint_index++
	  )
	{
	  inner_volume_force[joint_index*space_dimension+direction_index]
	    =
	  inner_volume_force[joint_index*space_dimension+direction_index]
	    *inner_bulk_modulus[arc_index]*0.5;
	};
    };
  */
};
void state_active_arc_class::output_inner_values(
						 const long int & time_index,
						 const int & sweep_step
						 )
{
  state_debug_io debug_inner_values(
				    "inner_volume_output",
				    1,
				    number_of_joints,
				    2
				    );
  //
  /*
  debug_inner_values.set_debug_comment(
				       "Volume =" + io_method.double_to_string(inner_volume),
				       0
				       );
  */
  //
  debug_inner_values.set_debug_comment(
				       "Volume energy =" + io_method.double_to_string(inner_volume_energy),
				       1
				       );
  //
  debug_inner_values.set_debug_data(
				    "volume force",
				    inner_volume_force,
				    0,
				    2
				    );
  //
  debug_inner_values.output_debug_data(
				       0,
				       time_index,
				       sweep_step
				       );
};
//
/*
void state_active_arc_class::add_inner_volume_force()
{
  total_elastic_energy=total_elastic_energy+inner_volume_energy;
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
};
*/
//
void state_active_arc_class::local_inner_force_plot(
						    const model_parameters_active_arc_class & model,
						    const long int & time_index,
						    const int & sweep_step,
						    const std::string & format
						    )
{
  gnuplot_driver gplot("local_inner_force_plot");
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
			   inner_volume_force.begin(), 
			   inner_volume_force.end()  
			   );
      if(absolute>0.0)
	{
	  for(
	      long int component_index=0;
	      component_index<(long int)inner_volume_force.size();
	      component_index++
	      )
	    {
	      inner_volume_force_for_plot[component_index]
		=inner_volume_force[component_index]/absolute;
	    };
	};
      gplot.output_vectors_on_loading_file(
					   coordinates,
					   inner_volume_force_for_plot,
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
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "local_force_plot",
			     "Error:undefined format!"
			     );
    };
};
