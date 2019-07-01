#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::initialize_configuration(
						      const std::string & criterion
						      )
{
  if(criterion=="curvature")
    {
      curvature_based_construction();
    }
  else if(criterion=="polar_curvaturebase")
    {
      polar_compressed_construction("curvature",initial_amplitude,initial_frequency);
    }
  else if(criterion=="elongated_tiny")
    {
      elongation_construction("curvature",initial_amplitude);
    }
  else if(criterion=="ripenned_tiny")
    {
      ripening_construction("curvature",initial_amplitude*arc_width);
    }
  else if(criterion=="sheared_tiny")
    {
      shear_construction("curvature",initial_amplitude*PI);
    }
  else if(criterion=="bent_tiny")
    {
      bending_construction("curvature",initial_amplitude*PI);
    }
  else if (criterion=="length")
    {
      length_based_construction();
    }
  else if (criterion=="polar_lengthbase")
    {
      polar_compressed_construction("length",initial_amplitude,initial_frequency);
    }
  else if (criterion=="wavy_lengthbase")
    {
      wavy_construction("length",initial_amplitude,initial_frequency);
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "initialize",
			     "Error:No criterion for initial state!"
			     );
    };
  angle_correction();
};
//
void state_active_arc_class::angle_correction()
{
  for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
    {
      if(angles[joint_index]>PI)
	{
	  angles[joint_index]=angles[joint_index]-2.0*PI;
	}
      else if(angles[joint_index]< -PI)
	{
	  angles[joint_index]=angles[joint_index]+2.0*PI;
	}
    };
};
//
void state_active_arc_class::curvature_based_construction()
{
  if(arc_curvature<=0.0)
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "initialize",
			     "Error:Zero or Negative curvature in initialization with arc curvature with criterion = curvature!"
			     );
    } else {
    /*
      the determination of initial interjoint angle
    */
    double initial_average_angle = -2.0*PI/double(number_of_arcs);
    /*
      the generation of initial condition for joint points
    */
    for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
      {
	angles[joint_index]=double(joint_index)*initial_average_angle;
	coordinates[space_dimension*joint_index]
	  =cos(angles[joint_index])/arc_curvature;
	coordinates[space_dimension*joint_index+1]
	  =sin(angles[joint_index])/arc_curvature;
	thickness[joint_index]=arc_width;
      };
  };
};
//
void state_active_arc_class::length_based_construction()
{
  if(arc_length<=0.0)
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "initialize",
			     "Error:Zero or Negative length in initialization with arc curvature with criterion = curvature!"
			     );
    }
  else 
    {
      /*
	the determination of initial interjoint angle
      */
      double initial_average_angle = -2.0*PI/double(number_of_arcs);
      /*
	the generation of initial condition for joint points
      */
      for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
	{
	  angles[joint_index]=double(joint_index)*initial_average_angle;
	  coordinates[space_dimension*joint_index]
	    =-cos(angles[joint_index])*arc_length/initial_average_angle;
	  coordinates[space_dimension*joint_index+1]
	    =-sin(angles[joint_index])*arc_length/initial_average_angle;
	  thickness[joint_index]=arc_width;
	};
    };
};
//
double state_active_arc_class::construction_base_check(
						       const std::string & base
						       )
{
  if(base=="curvature")
    {
      curvature_based_construction();
      return 1.0/arc_curvature;
    }
  else if(base=="length")
    {
      length_based_construction();
      return arc_length/2.0/PI*double(number_of_arcs);
    }
  else
    {
      io_method.error_output(
			     "state_actice_arc_class",
			     "construction_base_check",
			     "Error: no configuraiton construction base in initialization. base= "
			     + base
			     );
    };
  return -1.0;
};
//
void state_active_arc_class::polar_compressed_construction(
							   const std::string & base,
							   const double & perturbation,
							   const double & frequency
							   )
{
  double radius=construction_base_check(base);
  /*
    the generation of initial condition for joint points
  */
  for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
    {
	coordinates[space_dimension*joint_index  ]
	  =coordinates[space_dimension*joint_index  ]
	  +perturbation*radius*cos(frequency*angles[joint_index]);
	coordinates[space_dimension*joint_index+1]
	  =coordinates[space_dimension*joint_index+1]
	  +perturbation*radius*sin(frequency*angles[joint_index]);
    };
};
//
void state_active_arc_class::elongation_construction(
						     const std::string & base,
						     const double & perturbation
						     )
{
  double radius=construction_base_check(base);
  /*
    the generation of initial condition for joint points
  */
  for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
    {
      coordinates[space_dimension*joint_index]
	+=perturbation*radius*cos(angles[joint_index]);
      coordinates[space_dimension*joint_index+1]
	+=perturbation*radius*sin(angles[joint_index]);
    };
};
//
void state_active_arc_class::ripening_construction(
						   const std::string & base,
						   const double & perturbation
						   )
{
  construction_base_check(base);
  /*
    the generation of initial condition for joint points
  */
  for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
    {
      thickness[joint_index]+=perturbation;
    };
};
//
void state_active_arc_class::shear_construction(
						const std::string & base,
						const double & perturbation
						)
{
  construction_base_check(base);
  /*
    the generation of initial condition for joint points
  */
  for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
    {
      angles[joint_index]+=perturbation;
    };
};
//
void state_active_arc_class::bending_construction(
						  const std::string & base,
						  const double & perturbation
						  )
{
  construction_base_check(base);
  /*
    the generation of initial condition for joint points
  */
  for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
    {
      angles[joint_index]+=
	+perturbation*std::pow(-1.0,double(joint_index));
    };
};
//
void state_active_arc_class::wavy_construction(
					       const std::string & base,
					       const double & perturbation,
					       const double & frequency
					       )
{
  double radius=construction_base_check(base);
  /*
    the generation of initial condition for joint points
  */
  for(long int joint_index=0; joint_index<number_of_joints; joint_index++)
    {
      coordinates[space_dimension*joint_index]
	+=perturbation*radius*cos(frequency*angles[joint_index])*cos(angles[joint_index]);
      coordinates[space_dimension*joint_index+1]
	+=perturbation*radius*cos(frequency*angles[joint_index])*sin(angles[joint_index]);
    };
};
//
void state_active_arc_class::output_configuration(
						  const long int & time_index,
						  const int & sweep_step
						  )
{
  state_debug_io debug_configuration(
				     "conf_output",
				     0,
				     number_of_joints,
				     3
				     );
  debug_configuration.set_debug_data(
				     "coordinates",
				     coordinates,
				     0,
				     2
				     );
  debug_configuration.set_debug_data(
				     "angles",
				     angles,
				     2,
				     1
				     );
  debug_configuration.output_debug_data(
					0,
					time_index,
					sweep_step
					);
					
};


