#include "active_arc_state.hpp"
/*
  Main routine
 */
void state_active_arc_class::calculate_elastic_gradient()
{
  calculate_preforce();
  calculate_elastic_force();
};
//
void state_active_arc_class::initialize_elastic_force()
{
  // Force and Torque
  Normal_force.clear();
  Tangent_force.clear();
  Symmetric_Torque.clear();
  Antisymmetric_Torque.clear();
  Normal_Pullingforce.clear();
  Normal_Nonlinearforce.clear();
  //
  for(long int arc_index=0; arc_index<number_of_arcs;arc_index++)
    {
      Normal_force.push_back(0.0);
      Tangent_force.push_back(0.0);
      Symmetric_Torque.push_back(0.0);
      Antisymmetric_Torque.push_back(0.0);
      Normal_Pullingforce.push_back(0.0);
      Normal_Nonlinearforce.push_back(0.0);
    };
  //
  elastic_force.clear();
  gradient.clear();
  for (
       long int joint_index=0;
       joint_index<internal_head_pointer+
	 number_of_arcs*number_of_internal_components;
       joint_index++
       )
    {
      elastic_force.push_back(0.0);
      gradient.push_back(0.0);
    };
  if(
     inner_type=="elastic"
     )
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  gradient.push_back(0.0);
	};
    };
};
/*
  Operation
 */
//
inline double state_active_arc_class::dsinc(const double & data)
{
  if(data>bit_parameter)
    {
      return -(cos(data)*data-sin(data))/data/data;
    }
  else
    {
      return data/3.0-std::pow(data,3.0)/30.0;
    };
};
//
void state_active_arc_class::calculate_preforce()
{
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    {
      Tangent_force[arc_index]
	=-boost::math::sinc_pi(0.5*difference_angles[arc_index])
	*arc_length_reference[arc_index]
	/end_to_end_length[arc_index]
	/end_to_end_length[arc_index]
	*effective_elongation_stress[arc_index];
      Normal_force[arc_index]
	=effective_shear_stress[arc_index]
	/end_to_end_length[arc_index];
      Symmetric_Torque[arc_index]
	=-0.5*effective_shear_stress[arc_index];
      Antisymmetric_Torque[arc_index]
	=dsinc(0.5*difference_angles[arc_index])
	*0.5*arc_length_reference[arc_index]
	/end_to_end_length[arc_index]
	*effective_elongation_stress[arc_index]
	-bending_stress[arc_index];
      Normal_Pullingforce[arc_index]
	=-effective_ripening_stress[arc_index];
      Normal_Nonlinearforce[arc_index]
	=-shearbending_stress[arc_index];
    };
};
//
void state_active_arc_class::calculate_elastic_force()
{
  if(boundary_condition=="close")
    {
      // force
	for(
	    int direction_index=0;
	    direction_index<space_dimension;
	    direction_index++
	    )
	  {
	    for(
		long int joint_index = 1; 
		joint_index < number_of_joints; 
		joint_index++
		)
	      {
		elastic_force[joint_index*number_of_joint_components+direction_index]
		  =
		  tangent_vectors[(joint_index-1)*space_dimension+direction_index]
		  *Tangent_force[joint_index-1]
		  +
		  normal_vectors[(joint_index-1)*space_dimension+direction_index]
		  *Normal_force[joint_index-1]
		  -
		  tangent_vectors[joint_index*space_dimension+direction_index]
		  *Tangent_force[joint_index]
		  -
		  normal_vectors[joint_index*space_dimension+direction_index]
		  *Normal_force[joint_index];
	      };
	  };
    // for 0
	for(int direction_index=0;direction_index<space_dimension;direction_index++)
	  {
	    elastic_force[direction_index]
	      =
	      tangent_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	      *Tangent_force[number_of_arcs-1]
	      +
	      normal_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	      *Normal_force[number_of_arcs-1]
	      -
	      tangent_vectors[direction_index]
	      *Tangent_force[0]
	      -
	      normal_vectors[direction_index]
	      *Normal_force[0];
	  };
	// torque
	for(long int joint_index = 1; joint_index < number_of_joints; joint_index++)
	  {
	    elastic_force[joint_index*number_of_joint_components+space_dimension]
	      =
	      Symmetric_Torque[joint_index-1]
	      +
	      Antisymmetric_Torque[joint_index-1]
	      +
	      Symmetric_Torque[joint_index]
	      -
	      Antisymmetric_Torque[joint_index];
	  };
    // for 0
	elastic_force[space_dimension]
	  =
	  Symmetric_Torque[number_of_arcs-1]
	  +
	  Antisymmetric_Torque[number_of_arcs-1]
	  +
	  Symmetric_Torque[0]
	  -
	  Antisymmetric_Torque[0];
    }
  else
    {
      // force
	for(int direction_index=0;direction_index<space_dimension;direction_index++)
	  {
	    for(long int joint_index = 1; joint_index < number_of_joints-1; joint_index++)
	      {
		elastic_force[joint_index*number_of_joint_components+direction_index]
		  =
		  tangent_vectors[(joint_index-1)*space_dimension+direction_index]
		  *Tangent_force[joint_index-1]
		  +
		  normal_vectors[(joint_index-1)*space_dimension+direction_index]
		  *Normal_force[joint_index-1]
		  -
		  tangent_vectors[joint_index*space_dimension+direction_index]
		  *Tangent_force[joint_index]
		  -
		  normal_vectors[joint_index*space_dimension+direction_index]
		  *Normal_force[joint_index];
	      };
	  };
    // for 0
	for(int direction_index=0;direction_index<space_dimension;direction_index++)
	  {
	    elastic_force[number_of_joints-1+direction_index]
	      =
	      tangent_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	      *Tangent_force[number_of_arcs-1]
	      +
	      normal_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	      *Normal_force[number_of_arcs-1];
	    elastic_force[                   direction_index]
	      =
	      tangent_vectors[direction_index]
	      *Tangent_force[0]
	      -
	      normal_vectors[direction_index]
	      *Normal_force[0];
	  };
	// torque
	for(long int joint_index = 1; joint_index < number_of_joints; joint_index++)
	  {
	    elastic_force[joint_index*number_of_joint_components+space_dimension]
	      =
	      Symmetric_Torque[joint_index-1]
	      +
	      Antisymmetric_Torque[joint_index-1]
	      +
	      Symmetric_Torque[joint_index]
	      -
	      Antisymmetric_Torque[joint_index];
	  };
    // for 0
	elastic_force[(number_of_joints-1)*number_of_joint_components+space_dimension]
	  =
	  Symmetric_Torque[number_of_arcs-1]
	  +
	  Antisymmetric_Torque[number_of_arcs-1];
	elastic_force[space_dimension]
	  =
	  Symmetric_Torque[0]
	  -
	  Antisymmetric_Torque[0];
    };
	// for complession
  for(long int arc_index = 0; arc_index < number_of_arcs; arc_index++)
    {
      // normal extension
      elastic_force[internal_head_pointer+arc_index*number_of_internal_components  ]
	= Normal_Pullingforce[arc_index];
      // normal modulation
      elastic_force[internal_head_pointer+arc_index*number_of_internal_components+1]
	= Normal_Nonlinearforce[arc_index];
    };
};
//
void state_active_arc_class::local_force_plot(
					      const model_parameters_active_arc_class & model,
					      const long int & time_index,
					      const int & sweep_step,
					      const std::string & format
					      )
{
  gnuplot_driver gplot("local_force_plot");
  std::string direction_label;
  if(format=="gnuplot")
    {
      calculate_force_for_arcs();
      double max=*std::max_element(coordinates.begin(), coordinates.end());
      double min=*std::min_element(coordinates.begin(), coordinates.end());
      //
      if(space_dimension==2)
	{
	  int direction_index=2;
	  direction_label="z";
	  gplot.set_axis(
			 direction_index,
			 min*plot_margin,
			 max*plot_margin,
			 (double)(std::max((int)((double)(max-min)/4.0),1)),
			 direction_label
			 );
	};
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
      double absolute
	=std::max(
		  *std::max_element(
				    force_for_arc_minus.begin(), 
				    force_for_arc_minus.end()  
				    ),
		  *std::max_element(
				    force_for_arc_plus.begin(), 
				    force_for_arc_plus.end()
				    )
		  );
      for(
	  long int component_index=0;
	  component_index<(long int)force_for_arc_plot.size();
	  component_index++
	  )
	  {
	    force_for_arc_plot[component_index]=force_for_arc_minus[component_index]/absolute;
	  };
      gplot.output_vectors_on_loading_file(
					   coordinates,
					   force_for_arc_plot,
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
	  long int component_index=0;
	  component_index<(long int)force_for_arc_plot.size();
	  component_index++
	  )
	  {
	    force_for_arc_plot[component_index]=force_for_arc_plus[component_index]/absolute;
	  };
      gplot.output_vectors_on_loading_file(
					   coordinates,
					   force_for_arc_plot,
					   number_of_joints,
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
			     "local_force_plot",
			     "Error:undefined format!"
			     );
    };
};
//
void state_active_arc_class::local_moment_plot(
					      const model_parameters_active_arc_class & model,
					      const long int & time_index,
					      const int & sweep_step,
					      const std::string & format
					      )
{
  gnuplot_driver gplot("local_moment_plot");
  std::string direction_label;
  if(format=="gnuplot")
    {
      calculate_moment_for_arcs();
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
	=std::max(
		  *std::max_element(
				    moment_for_arc_minus.begin(), 
				    moment_for_arc_minus.end()  
				    ),
		  *std::max_element(
				    moment_for_arc_plus.begin(), 
				    moment_for_arc_plus.end()
				    )
		  );
      for(
	  long int component_index=0;
	  component_index<(long int)moment_for_arc_plot.size();
	  component_index++
	  )
	  {
	    moment_for_arc_plot[component_index]=moment_for_arc_minus[component_index]/absolute;
	  };
      gplot.output_vectors_on_loading_file(
					   edge_head_point_minus,
					   moment_for_arc_plot,
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
	  long int component_index=0;
	  component_index<(long int)moment_for_arc_plot.size();
	  component_index++
	  )
	  {
	    moment_for_arc_plot[component_index]=-moment_for_arc_minus[component_index]/absolute;
	  };
      gplot.output_vectors_on_loading_file(
					   edge_base_point_minus,
					   moment_for_arc_plot,
					   number_of_joints,
					   "on",
					   "3",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "off"
					   );
      for(
	  long int component_index=0;
	  component_index<(long int)moment_for_arc_plot.size();
	  component_index++
	  )
	  {
	    moment_for_arc_plot[component_index]
	      =moment_for_arc_plus[component_index]/absolute;
	  };
      gplot.output_vectors_on_loading_file(
					   edge_head_point_plus,
					   moment_for_arc_plot,
					   number_of_joints,
					   "on",
					   "4",
					   0,
					   time_index,
					   sweep_step,
					   space_dimension,
					   arc_width,
					   "off"
					   );
      for(
	  long int component_index=0;
	  component_index<(long int)force_for_arc_plot.size();
	  component_index++
	  )
	  {
	    moment_for_arc_plot[component_index]
	      =-moment_for_arc_plus[component_index]/absolute;
	  };
      gplot.output_vectors_on_loading_file(
					   edge_base_point_plus,
					   moment_for_arc_plot,
					   number_of_joints,
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
			     "local_force_plot",
			     "Error:undefined format!"
			     );
    };
};
//
void state_active_arc_class::local_elastic_force_plot_initialization()
{
  force_for_arc_minus.clear();
  force_for_arc_plus.clear();
  force_for_arc_plot.clear();
  moment_for_arc_minus.clear();
  moment_for_arc_plus.clear();
  moment_for_arc_plot.clear();
  edge_base_point_plus.clear();
  edge_base_point_minus.clear();
  edge_head_point_plus.clear();
  edge_head_point_minus.clear();
  for(int joint_index = 0; joint_index < number_of_joints; joint_index++)
    {
      force_for_arc_minus.push_back(0.0);
      force_for_arc_minus.push_back(0.0);
      force_for_arc_plus.push_back(0.0);
      force_for_arc_plus.push_back(0.0);
      force_for_arc_plot.push_back(0.0);
      force_for_arc_plot.push_back(0.0);
      moment_for_arc_minus.push_back(0.0);
      moment_for_arc_minus.push_back(0.0);
      moment_for_arc_plus.push_back(0.0);
      moment_for_arc_plus.push_back(0.0);
      moment_for_arc_plot.push_back(0.0);
      moment_for_arc_plot.push_back(0.0);
      edge_base_point_plus.push_back(0.0);
      edge_base_point_plus.push_back(0.0);
      edge_base_point_minus.push_back(0.0);
      edge_base_point_minus.push_back(0.0);
      edge_head_point_plus.push_back(0.0);
      edge_head_point_plus.push_back(0.0);
      edge_head_point_minus.push_back(0.0);
      edge_head_point_minus.push_back(0.0);
    };
};
//
void state_active_arc_class::calculate_force_for_arcs()
{
  for(
      int direction_index=0;
      direction_index<space_dimension;
      direction_index++
      )
    {
      for(
	  int arc_index=0;
	  arc_index<number_of_arcs-1;
	  arc_index++
	  )
	{
	  force_for_arc_minus[arc_index*space_dimension+direction_index]
	    = 
	    -tangent_vectors[arc_index*space_dimension+direction_index]
	    *Tangent_force[arc_index]
	    -
	    normal_vectors[arc_index*space_dimension+direction_index]
	    *Normal_force[arc_index];
	  force_for_arc_plus[(arc_index+1)*space_dimension+direction_index]
	    = 
	    tangent_vectors[arc_index*space_dimension+direction_index]
	    *Tangent_force[arc_index]
	    +
	    normal_vectors[arc_index*space_dimension+direction_index]
	    *Normal_force[arc_index];
	};
      // for arc_index,  number_of_arcs-1
      force_for_arc_minus[(number_of_arcs-1)*space_dimension+direction_index]
	= 
	-tangent_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	*Tangent_force[number_of_arcs-1]
	-
	  normal_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	*Normal_force[number_of_arcs-1];
      force_for_arc_plus[direction_index]
	= 
	tangent_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	*Tangent_force[number_of_arcs-1]
	+
	normal_vectors[(number_of_arcs-1)*space_dimension+direction_index]
	*Normal_force[number_of_arcs-1];
    };
};
//
void state_active_arc_class::calculate_moment_for_arcs()
{
  int direction_index;
  for(int arc_index=0;arc_index<number_of_arcs-1;arc_index++)
    {
      direction_index=0;
      edge_base_point_plus[(arc_index+1)*space_dimension + direction_index]
	=
	coordinates[(arc_index+1)*space_dimension+direction_index]
	-arc_width_reference[arc_index]*cos(angles[arc_index+1])*0.5;
      direction_index=1;
      edge_base_point_plus[(arc_index+1)*space_dimension + direction_index]
	=
	coordinates[(arc_index+1)*space_dimension+direction_index]
	-arc_width_reference[arc_index]*sin(angles[arc_index+1])*0.5;
      direction_index=0;
      edge_head_point_plus[(arc_index+1)*space_dimension + direction_index]
	=
	coordinates[(arc_index+1)*space_dimension+direction_index]
	+arc_width_reference[arc_index]*cos(angles[arc_index+1])*0.5;
      direction_index=1;
      edge_head_point_plus[(arc_index+1)*space_dimension + direction_index]
	=
	coordinates[(arc_index+1)*space_dimension+direction_index]
	+arc_width_reference[arc_index]*sin(angles[arc_index+1])*0.5;
      direction_index=0;
      edge_base_point_minus[arc_index*space_dimension + direction_index]
	=
	coordinates[arc_index*space_dimension+direction_index]
	-arc_width_reference[arc_index]*cos(angles[arc_index])*0.5;
      direction_index=1;
      edge_base_point_minus[arc_index*space_dimension + direction_index]
	=
	coordinates[arc_index*space_dimension+direction_index]
	-arc_width_reference[arc_index]*sin(angles[arc_index])*0.5;
      direction_index=0;
      edge_head_point_minus[arc_index*space_dimension + direction_index]
	=
	coordinates[arc_index*space_dimension+direction_index]
	+arc_width_reference[arc_index]*cos(angles[arc_index])*0.5;
      direction_index=1;
      edge_head_point_minus[arc_index*space_dimension + direction_index]
	=
	coordinates[arc_index*space_dimension+direction_index]
	+arc_width_reference[arc_index]*sin(angles[arc_index])*0.5;
    };
  direction_index=0;
  edge_base_point_plus[direction_index]
    =
    coordinates[direction_index]
    -arc_width_reference[number_of_arcs-1]*cos(angles[0])*0.5;
  direction_index=1;
  edge_base_point_plus[direction_index]
    =
    coordinates[direction_index]
    -arc_width_reference[number_of_arcs-1]*sin(angles[0])*0.5;
  direction_index=0;
  edge_head_point_plus[direction_index]
    =
    coordinates[direction_index]
    +arc_width_reference[number_of_arcs-1]*cos(angles[0])*0.5;
  direction_index=1;
  edge_head_point_plus[direction_index]
    =
    coordinates[direction_index]
    +arc_width_reference[number_of_arcs-1]*sin(angles[0])*0.5;
  direction_index=0;
  edge_base_point_minus[(number_of_arcs-1)*space_dimension + direction_index]
    =
    coordinates[(number_of_arcs-1)*space_dimension+direction_index]
    -arc_width_reference[number_of_arcs-1]*cos(angles[number_of_arcs-1])*0.5;
  direction_index=1;
  edge_base_point_minus[(number_of_arcs-1)*space_dimension + direction_index]
    =
    coordinates[(number_of_arcs-1)*space_dimension+direction_index]
    -arc_width_reference[number_of_arcs-1]*sin(angles[number_of_arcs-1])*0.5;
  direction_index=0;
  edge_head_point_minus[(number_of_arcs-1)*space_dimension + direction_index]
    =
	coordinates[(number_of_arcs-1)*space_dimension+direction_index]
    +arc_width_reference[number_of_arcs-1]*cos(angles[number_of_arcs-1])*0.5;
  direction_index=1;
  edge_head_point_minus[(number_of_arcs-1)*space_dimension + direction_index]
    =
    coordinates[(number_of_arcs-1)*space_dimension+direction_index]
    +arc_width_reference[number_of_arcs-1]*sin(angles[number_of_arcs-1])*0.5;
  for(int arc_index=0;arc_index<number_of_arcs-1;arc_index++)
    {
      direction_index=0;
      moment_for_arc_minus[arc_index*space_dimension+direction_index]
	=
	-sin(angles[arc_index])
	*(  
	  Symmetric_Torque[arc_index]
	  + Antisymmetric_Torque[arc_index]
	    );
      direction_index=1;
      moment_for_arc_minus[arc_index*space_dimension+direction_index]
	=
	cos(angles[arc_index])
	*(  
	  Symmetric_Torque[arc_index]
	  + Antisymmetric_Torque[arc_index]
	    );
      direction_index=0;
      moment_for_arc_plus[(arc_index+1)*space_dimension+direction_index]
	=
	-sin(angles[arc_index+1])
	*(Symmetric_Torque[arc_index]
	  - Antisymmetric_Torque[arc_index]
	  );
      direction_index=1;
      moment_for_arc_plus[(arc_index+1)*space_dimension+direction_index]
	=
	cos(angles[arc_index+1])
	*(Symmetric_Torque[arc_index]
	  - Antisymmetric_Torque[arc_index]
	  );
    };
  // for arc_index,  number_of_arcs-1
      direction_index=0;
      moment_for_arc_minus[(number_of_arcs-1)*space_dimension+direction_index]
	=
	-sin(angles[number_of_arcs-1])
	*(  
	  Symmetric_Torque[number_of_arcs-1]
	  + Antisymmetric_Torque[number_of_arcs-1]
	    );
      direction_index=1;
      moment_for_arc_minus[(number_of_arcs-1)*space_dimension+direction_index]
	=
	cos(angles[number_of_arcs-1])
	*(  
	  Symmetric_Torque[number_of_arcs-1]
	  + Antisymmetric_Torque[number_of_arcs-1]
	    );
      direction_index=0;
      moment_for_arc_plus[direction_index]
	=
	-sin(angles[0])
	*(Symmetric_Torque[number_of_arcs-1]
	  - Antisymmetric_Torque[number_of_arcs-1]
	  );
      direction_index=1;
      moment_for_arc_plus[direction_index]
	=
	cos(angles[0])
	*(Symmetric_Torque[number_of_arcs-1]
	  - Antisymmetric_Torque[number_of_arcs-1]
	  );
};
//
void state_active_arc_class::output_forces(
					   const long int & time_index,
					   const int & sweep_step
					   )
{
  calculate_force_for_arcs();
  state_debug_io debug_force(
			     "force_output",
			     0,
			     number_of_arcs,
			     4
			     );
  debug_force.set_debug_data(
			     "plus",
			     force_for_arc_plus,
			     0,
			     2
			     );
  debug_force.set_debug_data(
			     "minus",
			     force_for_arc_minus,
			     2,
			     2
			     );
  debug_force.output_debug_data(
				0,
				time_index,
				sweep_step
				);
};
//
void state_active_arc_class::output_moments(
					    const long int & time_index,
					    const int & sweep_step
					    )
{
  calculate_moment_for_arcs();
  state_debug_io debug_force(
			     "moment_output",
			     0,
			     number_of_arcs,
			     4
			     );
  debug_force.set_debug_data(
			     "plus",
			     moment_for_arc_plus,
			     0,
			     2
			     );
  debug_force.set_debug_data(
			     "minus",
			     moment_for_arc_minus,
			     2,
			     2
			     );
  debug_force.output_debug_data(
				0,
				time_index,
				sweep_step
				);
};
//
void state_active_arc_class::output_preforces(
					      const long int & time_index,
					      const int & sweep_step
					      )
{
  calculate_force_for_arcs();
  state_debug_io debug_force(
			     "preforce_output",
			     0,
			     number_of_arcs,
			     6
			     );
  debug_force.set_debug_data(
			     "Tangent force",
			     Tangent_force,
			     0,
			     1
			     );
  debug_force.set_debug_data(
			     "Normal force",
			     Normal_force,
			     1,
			     1
			     );
  debug_force.set_debug_data(
			     "Symmetric torque",
			     Symmetric_Torque,
			     2,
			     1
			     );
  debug_force.set_debug_data(
			     "Antisymmetric torque",
			     Antisymmetric_Torque,
			     3,
			     1
			     );
  debug_force.set_debug_data(
			     "Symmetric torque",
			     Symmetric_Torque,
			     4,
			     1
			     );
  debug_force.set_debug_data(
			     "Normal Pulling",
			     Normal_Pullingforce,
			     4,
			     1
			     );
  debug_force.set_debug_data(
			     "Normal Modulation",
			     Normal_Nonlinearforce,
			     4,
			     1
			     );
  debug_force.output_debug_data(
				0,
				time_index,
				sweep_step
				);
};
//
void state_active_arc_class::show_gradient(
					   const long int & time_index
					   )
{
  io_method.standard_output(
			    ":::::: energy & elastic_force: iter=" 
			    + io_method.longint_to_string(time_index)
			    + "::::::"
			    );
  io_method.standard_output(
			    "energy = " 
			    + io_method.double_to_string(total_energy)
			    + "= ("
			    + io_method.double_to_string(total_elastic_energy)
			    + "/"
			    + io_method.double_to_string(inner_volume_energy)
			    + "/"
			    + io_method.double_to_string(inner_elastic_energy)
			    + ")"
			    );
  for(
      long int joint_index = 0;
      joint_index < number_of_joints;
      joint_index ++
      )
    { 
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index ++
	  )
	{
	  io_method.standard_output(
				    "grad["
				    +io_method.int_to_string(
							     joint_index*number_of_joint_components
							     +direction_index
							     )
				    +"]="
				    +io_method.double_to_string(
								-gradient[
									  joint_index*number_of_joint_components
									  +direction_index
									  
									  ]
								)
				    + " = ("
				    + io_method.double_to_string(
								 elastic_force[
									       joint_index*number_of_joint_components
									       +direction_index
									       ]
								 )
				    + "+"
				    + io_method.double_to_string(
								 inner_volume_force[
										    joint_index*number_of_joint_components
										    +direction_index
									       ]
								 )
				    + "+"
				    + io_method.double_to_string(
								 inner_elastic_force[
										     joint_index*number_of_joint_components
										     +direction_index
										     ]
								 )
				    + ")"
				);
	};
      //
      io_method.standard_output(
				"grad["
				+io_method.int_to_string(
							 joint_index*number_of_joint_components
							 +space_dimension
							     )
				+"]="
				+io_method.double_to_string(
							    -gradient[
								      joint_index*number_of_joint_components
								      +space_dimension
								      
								      ]
							    )
				+ " = ("
				+ io_method.double_to_string(
							     elastic_force[
									   joint_index*number_of_joint_components
									   +space_dimension
									   ]
							     )
				+ ")"
				);
      
    };
  //
  for( 
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
       )
    {
      io_method.standard_output(
				"grad["
				+io_method.int_to_string(
							 internal_head_pointer
							 +arc_index*number_of_internal_components
							 )
				+"]="
				+io_method.double_to_string(
							    -gradient[
								      internal_head_pointer
								      +arc_index*number_of_internal_components     
								      ]
							    )
				+ " = ("
				+ io_method.double_to_string(
							     elastic_force[
									   internal_head_pointer
									   +arc_index*number_of_internal_components     
									   ]
							     )
				+ ")"
				);
      io_method.standard_output(
				"grad["
				+io_method.int_to_string(
							 internal_head_pointer
							 +arc_index*number_of_internal_components
							 +1
							 )
				+"]="
				+io_method.double_to_string(
							    -gradient[
								      internal_head_pointer
								      +arc_index*number_of_internal_components     
								      +1
								      ]
							    )
				+ " = ("
				+ io_method.double_to_string(
							     elastic_force[
									   internal_head_pointer
									   +arc_index*number_of_internal_components     
									   +1
									   ]
							     )
				+ ")"
				);
    };
};
