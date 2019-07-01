#include "active_arc_reconstruction.hpp"
// operator
/*
  void model_reconstructor::reconstruction()
  {
  new_reference.initialize(
  new_number_of_arcs
  );
  new_configuration.initialize(
  new_number_of_arcs,
  space_dimension
  );
  reference_reconstruction();
  };
*/
//
long int model_reconstructor::get_number_of_old_arcs()
{
  return (long int) old_division_on.size();
};
//
void model_reconstructor::set_division_on(
					  const std::vector<bool> & input_division_on
					  )
{
  old_division_on.resize(input_division_on.size());
  for(
      long int index=0;
      index<old_division_on.size();
      index++
      )
    {
      old_division_on[index]=input_division_on[index];
    };
};
//
void model_reconstructor::get_division_on(
					  std::vector<bool> & output_division_on
					  )
{
  if(
     (long int)output_division_on.size()
     !=
     (long int)old_division_on.size()
     )
    {
      io_method.error_output(
			     "model_constructor",
			     "get_division_on",
			     "due to inconsistency in array size!"
			     );
    };
  for(
      long int index=0;
      index<old_division_on.size();
      index++
      )
    {
      output_division_on[index]=old_division_on[index];
    };
};
//
void model_reconstructor::reference_reconstruction(
						   reference_io & old_reference,
						   std::vector<bool> & division_on
						   )
{
  long int new_arc_index=0;
  //debug
  std::string message;
  //
  for(long int arc_index=0;arc_index<old_number_of_arcs;arc_index++)
    {
      //debug
      //message = "debug ref:[" + io_method.longint_to_string(new_arc_index)+"]";
      //
      if(division_on[arc_index])
	{
	  // division part 1
	  new_reference.length[new_arc_index]
	    =old_reference.length[arc_index]/2.0;
	  //message+= io_method.double_to_string(new_reference.length[new_arc_index]) + ",";
	  new_reference.width[new_arc_index]
	    =old_reference.width[arc_index];
	  // message+= io_method.double_to_string(new_reference.width[new_arc_index]) + ",";
	  new_reference.curvature[new_arc_index]
	    =old_reference.curvature[arc_index];
	  //message+= io_method.double_to_string(new_reference.curvature[new_arc_index]) + ",";
	  new_reference.young_modulus[new_arc_index]
	    =old_reference.young_modulus[arc_index];
	  //message+= io_method.double_to_string(new_reference.young_modulus[new_arc_index]) + ",";
	  new_reference.poisson_ratio[new_arc_index]
	    =old_reference.poisson_ratio[arc_index];
	  //message+= io_method.double_to_string(new_reference.poisson_ratio[new_arc_index]) + ",";
	  new_reference.inner_control_volume[new_arc_index]
	    =old_reference.inner_control_volume[arc_index]/2.0;
	  //message+= io_method.double_to_string(new_reference.inner_control_volume[new_arc_index]) + ",";
	  new_reference.inner_young_modulus[new_arc_index]
	    =old_reference.inner_young_modulus[arc_index];
	  //message+= io_method.double_to_string(new_reference.inner_young_modulus[new_arc_index]) + ",";
	  if(inner_type=="elastic")
	    {
	      new_reference.inner_width[new_arc_index]
		=old_reference.inner_width[arc_index]/2.0;
	      //message+= io_method.double_to_string(new_reference.inner_width[new_arc_index]) + ",";
	      new_reference.inner_height[new_arc_index]
		=old_reference.inner_height[arc_index];
	      //message+= io_method.double_to_string(new_reference.inner_height[new_arc_index]) + ",";
	    }
	  // debug
	  //io_method.standard_output(message);
	  //message = "debug ref:[" + io_method.longint_to_string(new_arc_index)+"]";
	  //
	  // incriment of index
	  new_arc_index=new_arc_index+1;
	  // division part 2
	  new_reference.length[new_arc_index]
	    =old_reference.length[arc_index]/2.0;
	  //message+= io_method.double_to_string(new_reference.length[new_arc_index]) + ",";
	  new_reference.width[new_arc_index]
	    =old_reference.width[arc_index];
	  //message+= io_method.double_to_string(new_reference.width[new_arc_index]) + ",";
	  new_reference.curvature[new_arc_index]
	    =old_reference.curvature[arc_index];
	  //message+= io_method.double_to_string(new_reference.curvature[new_arc_index]) + ",";
	  new_reference.young_modulus[new_arc_index]
	    =old_reference.young_modulus[arc_index];
	  //message+= io_method.double_to_string(new_reference.young_modulus[new_arc_index]) + ",";
	  new_reference.poisson_ratio[new_arc_index]
	    =old_reference.poisson_ratio[arc_index];
	  //message+= io_method.double_to_string(new_reference.poisson_ratio[new_arc_index]) + ",";
	  new_reference.inner_control_volume[new_arc_index]
	    =old_reference.inner_control_volume[arc_index]/2.0;
	  //message+= io_method.double_to_string(new_reference.inner_control_volume[new_arc_index]) + ",";
	  new_reference.inner_young_modulus[new_arc_index]
	    =old_reference.inner_young_modulus[arc_index];
	  //message+= io_method.double_to_string(new_reference.inner_young_modulus[new_arc_index]) + ",";
	  if(inner_type=="elastic")
	    {
	      new_reference.inner_width[new_arc_index]
		=old_reference.inner_width[arc_index]/2.0;
	      //message+= io_method.double_to_string(new_reference.inner_width[new_arc_index]) + ",";
	      new_reference.inner_height[new_arc_index]
		=old_reference.inner_height[arc_index];
	      //message+= io_method.double_to_string(new_reference.inner_height[new_arc_index]) + ",";
	    }
	}
      else
	{
	  new_reference.length[new_arc_index]
	    =old_reference.length[arc_index];
	  //message+= io_method.double_to_string(new_reference.length[new_arc_index]) + ",";
	  new_reference.width[new_arc_index]
	    =old_reference.width[arc_index];
	  //message+= io_method.double_to_string(new_reference.width[new_arc_index]) + ",";
	  new_reference.curvature[new_arc_index]
	    =old_reference.curvature[arc_index];
	  //message+= io_method.double_to_string(new_reference.curvature[new_arc_index]) + ",";
	  new_reference.young_modulus[new_arc_index]
	    =old_reference.young_modulus[arc_index];
	  //message+= io_method.double_to_string(new_reference.young_modulus[new_arc_index]) + ",";
	  new_reference.poisson_ratio[new_arc_index]
	    =old_reference.poisson_ratio[arc_index];
	  //message+= io_method.double_to_string(new_reference.poisson_ratio[new_arc_index]) + ",";
	  new_reference.inner_control_volume[new_arc_index]
	    =old_reference.inner_control_volume[arc_index];
	  //message+= io_method.double_to_string(new_reference.inner_control_volume[new_arc_index]) + ",";
	  new_reference.inner_young_modulus[new_arc_index]
	    =old_reference.inner_young_modulus[arc_index];
	  //message+= io_method.double_to_string(new_reference.young_modulus[new_arc_index]) + ",";
	  if(inner_type=="elastic")
	    {
	      new_reference.inner_width[new_arc_index]
		=old_reference.inner_width[arc_index];
	      //message+= io_method.double_to_string(new_reference.inner_width[new_arc_index]) + ",";
	      new_reference.inner_height[new_arc_index]
		=old_reference.inner_height[arc_index];
	      //message+= io_method.double_to_string(new_reference.inner_height[new_arc_index]) + ".";
	    }
	};
      new_arc_index=new_arc_index+1;
      // debug
      //io_method.standard_output(message);
      //
    };
}
//
void model_reconstructor::configuration_reconstruction(
						       configuration_io & old_configuration,
						       std::vector<bool> & division_on
						       )
{
  long int old_joint_index=0;
  long int new_joint_index=0;
  long int new_arc_index=0;
  for(
      long int arc_index=0;
      arc_index<old_number_of_arcs;
      arc_index++
      )
    {
      old_joint_index=arc_index;
      //first part
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  new_configuration.coordinates[
					new_joint_index*space_dimension
					+direction_index
					]
	    =old_configuration.coordinates[
					   old_joint_index*space_dimension
					   +direction_index
					   ];
	    };
      new_configuration.angles[new_joint_index]
	=old_configuration.angles[old_joint_index];
      new_configuration.thickness[new_joint_index]
	=old_configuration.thickness[old_joint_index];
      // for arc variables
      new_configuration.ripening[new_arc_index]
	=old_configuration.ripening[arc_index];
      new_configuration.normal_modulation[new_arc_index]
	=old_configuration.normal_modulation[arc_index];
      new_configuration.inner_volume[new_arc_index]
	=old_configuration.inner_volume[arc_index];
      new_configuration.inner_elongation[new_arc_index]
	=old_configuration.inner_elongation[arc_index];
      new_configuration.inner_ripening[new_arc_index]
	=old_configuration.inner_ripening[arc_index];
      new_configuration.inner_shear[new_arc_index]
	=old_configuration.inner_shear[arc_index];
      //incriment of joint index
      new_joint_index++;
      //incriment of arc_index
      new_arc_index++;
      //
      if(division_on[arc_index])
	{
	  // second part
	  if(
	     (long int)old_configuration.angles.size()
	     >
	     old_joint_index+1
	     )
	    {
	      for(
		  int direction_index=0;
		  direction_index<space_dimension;
		  direction_index++
		  )
		{
		  new_configuration.coordinates[
						new_joint_index*space_dimension
						+direction_index
						]
		    =(
		      old_configuration.coordinates[
						    old_joint_index*space_dimension
						    +direction_index
						    ]
		      +
		      old_configuration.coordinates[
						    (old_joint_index+1)*space_dimension
						    +direction_index
						    ]
		      )*0.5;
		};
	      if(
		 (old_configuration.angles[old_joint_index]*old_configuration.angles[old_joint_index+1]>0.0)
		 ||
		 (
		  std::abs(old_configuration.angles[old_joint_index])
		  +
		  std::abs(old_configuration.angles[old_joint_index+1])
		  ) < PI
		 )
		{
		  new_configuration.angles[new_joint_index]
		    =(
		      old_configuration.angles[old_joint_index]
		      +old_configuration.angles[old_joint_index+1]
		      )*0.5;
		}
	      else 
		{
		  new_configuration.angles[new_joint_index]
		    =(
		      old_configuration.angles[old_joint_index]
		      +old_configuration.angles[old_joint_index+1]
		      +2.0*PI
		      )*0.5;
		  if(new_configuration.angles[new_joint_index]>PI)
		    {
		      new_configuration.angles[new_joint_index]
			=new_configuration.angles[new_joint_index]-2.0*PI;
		    };
		};
	      new_configuration.thickness[new_joint_index]
		=(
		  old_configuration.thickness[old_joint_index]
		  +old_configuration.thickness[old_joint_index+1]
		  )*0.5;
	    }
	  else
	    {
	      for(
		  int direction_index=0;
		  direction_index<space_dimension;
		  direction_index++
		  )
		{
	      new_configuration.coordinates[
					    new_joint_index*space_dimension
					    +direction_index
					    ]
		=(
		  old_configuration.coordinates[
						old_joint_index*space_dimension
						+direction_index
						]
		      +
		  old_configuration.coordinates[
						direction_index
						]
		  )*0.5;
		}
	      if(
		 (old_configuration.angles[old_joint_index]*old_configuration.angles[0]>0.0)
		 ||
		 (
		  std::abs(old_configuration.angles[old_joint_index])
		  +
		  std::abs(old_configuration.angles[0])
		  ) < PI
		 )
		{
		  new_configuration.angles[new_joint_index]
		    =(
		      old_configuration.angles[old_joint_index]
		      +old_configuration.angles[0]
		      )*0.5;
		}
	      else
		{
		  new_configuration.angles[new_joint_index]
		    =(
		      old_configuration.angles[old_joint_index]
		      +old_configuration.angles[0]
		      +2.0*PI
		      )*0.5;
		  if(new_configuration.angles[new_joint_index]>PI)
		    {
		      new_configuration.angles[new_joint_index]
			=new_configuration.angles[new_joint_index]-2.0*PI;
		    };
		};
	      new_configuration.thickness[new_joint_index]
		=(
		  old_configuration.thickness[old_joint_index]
		  +old_configuration.thickness[0]
		  )*0.5;
	    };
	  // incriment of new joint index
	  new_joint_index++;
	  // for arc index
	  new_configuration.ripening[new_arc_index]
	    =old_configuration.ripening[arc_index];
	  new_configuration.normal_modulation[new_arc_index]
	    =old_configuration.normal_modulation[arc_index];
	  new_configuration.inner_volume[new_arc_index]
	    =old_configuration.inner_volume[arc_index];
	  new_configuration.inner_elongation[new_arc_index]
	    =old_configuration.inner_elongation[arc_index];
	  new_configuration.inner_ripening[new_arc_index]
	    =old_configuration.inner_ripening[arc_index];
	  new_configuration.inner_shear[new_arc_index]
	    =old_configuration.inner_shear[arc_index];
	  // incriment of new arc index
	  new_arc_index++;
	}
    };
};
//
void model_reconstructor::get_construction(
					   reference_io & reference,
					   configuration_io & configuration
					   )
{
  if(reference.length.size()!=new_reference.length.size())
    {
      io_method.error_output(
			     "model_reconstructor_class",
			     "get_construction",
			     "Error:reference size mismuch!"
			     );
    }
  else if(
	  configuration.coordinates.size()
	  !=
	  new_configuration.coordinates.size()
	  )
    {
      io_method.error_output(
			     "model_reconstructor_class",
			     "get_construction",
			     "Error:configuration size mismuch!"
			     );
    };
  // debug
  std::string message;
  //
  for(
      long int arc_index=0;
      arc_index<new_number_of_arcs;
      arc_index++
      )
    {
      //for reference
      reference.length[arc_index]
	=new_reference.length[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: length";
      //message+= io_method.double_to_string(new_reference.length[arc_index]);
      //io_method.standard_output(message);
      //
      reference.width[arc_index]
	=new_reference.width[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: width";
      //message+= io_method.double_to_string(new_reference.width[arc_index]);
      //io_method.standard_output(message);
      //
      reference.curvature[arc_index]
	=new_reference.curvature[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: curvature";
      //message+= io_method.double_to_string(new_reference.curvature[arc_index]);
      //io_method.standard_output(message);
      //
      reference.young_modulus[arc_index]
	=new_reference.young_modulus[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: young modulus";
      //message+= io_method.double_to_string(new_reference.young_modulus[arc_index]);
      //io_method.standard_output(message);
      //
      reference.poisson_ratio[arc_index]
	=new_reference.poisson_ratio[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: poisson ratio";
      //message+= io_method.double_to_string(new_reference.poisson_ratio[arc_index]);
      //io_method.standard_output(message);
      //
      reference.inner_control_volume[arc_index]
	=new_reference.inner_control_volume[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: inner control volume";
      //message+= io_method.double_to_string(new_reference.inner_control_volume[arc_index]);
      //io_method.standard_output(message);
      //
      reference.inner_young_modulus[arc_index]
	=new_reference.inner_young_modulus[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: inner young modulus";
      //message+= io_method.double_to_string(new_reference.inner_young_modulus[arc_index]);
      //io_method.standard_output(message);
      //
      reference.inner_width[arc_index]
	=new_reference.inner_width[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: inner width";
      //message+= io_method.double_to_string(new_reference.inner_width[arc_index]);
      //io_method.standard_output(message);
      //
      reference.inner_height[arc_index]
	=new_reference.inner_height[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: inner height";
      //message+= io_method.double_to_string(new_reference.inner_width[arc_index]);
      //io_method.standard_output(message);
      //
      //for configuration
      configuration.ripening[arc_index]
	=new_configuration.ripening[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: ripening";
      //message+= io_method.double_to_string(configuration.ripening[arc_index]);
      //io_method.standard_output(message);
      //
      configuration.normal_modulation[arc_index]
	=new_configuration.normal_modulation[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: modulation";
      //message+= io_method.double_to_string(configuration.normal_modulation[arc_index]);
      //io_method.standard_output(message);
      //
      configuration.inner_volume[arc_index]
	=new_configuration.inner_volume[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: innor volume";
      //message+= io_method.double_to_string(configuration.inner_volume[arc_index]);
      //io_method.standard_output(message);
      //
      configuration.inner_elongation[arc_index]
	=new_configuration.inner_elongation[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: inner elongation";
      //message+= io_method.double_to_string(configuration.inner_elongation[arc_index]);
      //io_method.standard_output(message);
      //
      configuration.inner_ripening[arc_index]
	=new_configuration.inner_ripening[arc_index];
      // debug
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: inner_ripening";
      //message+= io_method.double_to_string(configuration.inner_ripening[arc_index]);
      //io_method.standard_output(message);
      //
      configuration.inner_shear[arc_index]
	=new_configuration.inner_shear[arc_index];
      //message = "debug ref:[" + io_method.longint_to_string(arc_index)+"]: inner_shear";
      //message+= io_method.double_to_string(configuration.inner_shear[arc_index]);
      //io_method.standard_output(message);
    };
  //
  for(
      long int joint_index=0;
      joint_index<new_number_of_joints;
      joint_index++
      )
    {
      for(
	  int direction_index=0;
	  direction_index<space_dimension;
	  direction_index++
	  )
	{
	  configuration.coordinates[
				    joint_index*space_dimension
				    +direction_index
				    ]
	    =new_configuration.coordinates[
					   joint_index*space_dimension
					   +direction_index
					   ];
	    };
      configuration.angles[joint_index]
	=new_configuration.angles[joint_index];
      configuration.thickness[joint_index]
	=new_configuration.thickness[joint_index];
      //message = "debug ref:[" + io_method.longint_to_string(joint_index)+"]: conf(";
      //message+= io_method.double_to_string(configuration.coordinates[joint_index*2])+ ",";
      //message+= io_method.double_to_string(configuration.coordinates[joint_index*2+1])+ "), ";
      //message+= io_method.double_to_string(configuration.angles[joint_index])+ ",";
      //message+= io_method.double_to_string(configuration.thickness[joint_index]);
      //io_method.standard_output(message);
    };
  //
};
//
/*
void model_reconstructor::old_construction(
					   long int & input_number_of_arcs,
					   long int & input_number_of_joints,
					   int & space_dimension,
					   std::vector<bool> & input_division_on
					   )
{
  old_number_of_arcs=input_number_of_arcs;
  old_number_of_joints=input_number_of_joints;
  old_reference.initialize(
			   old_number_of_arcs
			   );
  old_configuration.initialize(
			       old_number_of_joints,
			       old_number_of_arcs,
			       space_dimension
			       );
  std::vector<bool>::const_iterator index=input_division_on.begin();
  division_on.clear();
  while(index!=input_division_on.end())
    {
      division_on.push_back(*index);
      index++;
    }
}
*/
void model_reconstructor::new_construction(
					   long int & input_number_of_arcs,
					   long int & input_number_of_joints,
					   int & input_space_dimension,
					   std::vector<bool> & input_division_on,
					   reference_io & old_reference,
					   configuration_io & old_configuration
					   )
{
  //
  space_dimension=input_space_dimension;
  old_number_of_arcs=input_number_of_arcs;
  old_number_of_joints=input_number_of_joints;
  new_number_of_arcs=old_number_of_arcs;
  new_number_of_joints=old_number_of_joints;
  //
  std::vector<bool>::const_iterator index=input_division_on.begin();
  while(index!=input_division_on.end())
    {
      if(*index)
	{
	  new_number_of_arcs++;
	  new_number_of_joints++;
	};
      index++;
    }
  //
  set_division_on(input_division_on);
  //
  new_reference.initialize(
			   new_number_of_arcs
			   );
  reference_reconstruction(old_reference, input_division_on);
  new_configuration.initialize(
			       new_number_of_joints,
			       new_number_of_arcs,
			       space_dimension
			       );
  configuration_reconstruction(old_configuration, input_division_on);
  //
}
// constructer
model_reconstructor::model_reconstructor(
					 model_parameters_active_arc_class & model
					 )
{
  space_dimension=model.get_space_dimension();
  old_number_of_arcs=model.get_number_of_arcs();
  inner_type=model.get_inner_type();
  PI=3.1415926535897931;
}
