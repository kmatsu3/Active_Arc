#include "active_arc_definition.hpp"
#include <sstream>
#include <cstdlib>
#include <cmath>
//
  /*
    Constructer
  */
model_parameters_active_arc_class::model_parameters_active_arc_class()
{
  number_of_arcs=-1;
  space_dimension=2;
  arc_length=1.0;
  arc_width=-1.0;
  arc_curvature=-1.0;
  poisson_ratio=-1.0;
  young_modulus=-1.0;
  angle_penalty_energy=-1.0;
  bending_energy_on=true;
  coupling_energy_on=true;
  uniform_growth_ratio=-1.0;
  normal_gradient_growth=-1.0;
  tangent_gradient_growth=-1.0;
  boundary_condition="close";
  initial_state="unknown";
  inner_control_volume=-1.0;
  inner_material_Young_modulus=-1.0;
  inner_material_Poisson_ratio=-1.0;
  shear_constraint_on=false;
  shear_constraint_modulus=0.0;
  shear_constraint_power=1.0;
  initial_frequency=0.0;
  initial_amplitude=0.0;
  cg_on=true;
  md_on=false;
  shape_plot_max=-2;
  shape_plot_base=-1;
  shape_plot_frequency=-1;
  //
  inner_radius=-1.0;
  inner_control_volume=-1.0;
  inner_material_Young_modulus=-1.0;
  inner_material_Poisson_ratio=-1.0;
  inner_height=-1.0;
  inner_width=-1.0;
  inner_type="unknown";
  PI = 3.141592653589793238;
};
  /*
    Member functions
   */
//
void model_parameters_active_arc_class::input_model()
{
  input_model_parameters();
  input_inner_material_parameters();
  input_cg_parameter();
  input_md_parameter();
  input_initialstate_parameter();
  input_constraint_parameter();
  input_io_parameter();
};
//
int model_parameters_active_arc_class::show_model_parameters()
{
    std::string output_text;
    //    std::stringstream output_stringstream;
    //    output_stringstream << number_of_arcs;
    //    output_text = output_stringstream.str();
    // number_of_arcs
    io_method.standard_output("=== Model parameters started  ===");
    output_text = "Dimention of Space =";
    output_text+= io_method.int_to_string(space_dimension);
    io_method.standard_output(output_text);
    output_text = "The Number of Arcs =";
    output_text+= io_method.longint_to_string(number_of_arcs);
    io_method.standard_output(output_text);
    output_text = "Arc Length =";
    output_text+= io_method.double_to_string(arc_length);
    io_method.standard_output(output_text);
    output_text = "Arc width =";
    output_text+= io_method.double_to_string(arc_width);
    io_method.standard_output(output_text);
    output_text = "Arc curvature =";
    output_text+= io_method.double_to_string(arc_curvature);
    io_method.standard_output(output_text);
    output_text = "Young's modulus =";
    output_text+= io_method.double_to_string(young_modulus);
    io_method.standard_output(output_text);
    output_text = "Poisson's ratio =";
    output_text+= io_method.double_to_string(poisson_ratio);
    io_method.standard_output(output_text);
    output_text = "Angle penalty energy =";
    output_text+= io_method.double_to_string(angle_penalty_energy);
    io_method.standard_output(output_text);
    output_text = "Uniform growth rate =";
    output_text+= io_method.double_to_string(uniform_growth_ratio);
    io_method.standard_output(output_text);
    output_text = "Normal gradient growth =";
    output_text+= io_method.double_to_string(normal_gradient_growth);  
    io_method.standard_output(output_text); 
    output_text = "Tangential gradient growth =";
    output_text+= io_method.double_to_string(tangent_gradient_growth);
    io_method.standard_output(output_text);  
    output_text = "Boundary condition =";
    io_method.standard_output(boundary_condition);  
    output_text = "Initial state =";
    io_method.standard_output(initial_state); 
    output_text = "switch for bending energy = ";
    if(bending_energy_on)
      {
	output_text += "on";
      }
    else
      {
	output_text += "off";
      };
    io_method.standard_output(output_text); ; 
    output_text = "switch for coupling energy = ";
    if(coupling_energy_on)
      {
	output_text += "on";
      }
    else
      {
	output_text += "off";
      };
    io_method.standard_output(output_text); 
    io_method.standard_output("=== Model parameters finished ===");
    return 0;
};
//
int model_parameters_active_arc_class::input_model_parameters()
{
    std::string file_name="standard_output";
    io_method.file_initialize(file_name);
    number_of_arcs=io_method.get_input_longint(
						"model_input" ,
						"model.number_of_arcs"
						);
    //
    /*
    arc_length=io_method.get_input_double(
		      			"model_input" ,
	       				"model.arc_length"
       					);
    */
    //
    //    model_parameters_active_arc_class::check_model_parameter("arc_length");
    //
    arc_width=io_method.get_input_double(
					 "model_input" ,
					 "model.arc_width"
					 );
    //
    arc_length=io_method.get_input_double(
					  "model_input" ,
					  "model.arc_length"
					  );
    //
    arc_curvature=io_method.get_input_double(
					     "model_input" ,
					     "model.arc_curvature"
					     );
    //
    young_modulus=io_method.get_input_double(
					     "model_input" ,
					     "model.young_modulus"
					     );
    //
    poisson_ratio=io_method.get_input_double(
					     "model_input" ,
					     "model.poisson_ratio"
					     );
    //
    angle_penalty_energy=io_method.get_input_double(
						    "model_input" ,
						    "model.angle_penalty"
						    );
    //
    uniform_growth_ratio=io_method.get_input_double(
						    "model_input" ,
						    "model.uniform_growth_ratio"
						    );
    //
    normal_gradient_growth=io_method.get_input_double(
						      "model_input" ,
						      "model.normal_gradient_growth"
						      );
    //
    tangent_gradient_growth=io_method.get_input_double(
						       "model_input" ,
						       "model.tangent_gradient_growth"
						       );
    //
    boundary_condition=io_method.get_input_string(
						  "model_input" ,
						  "model.boundary_condition"
						  );
    //
    initial_state=io_method.get_input_string(
					     "model_input" ,
					     "model.initial_state"
					     );
    //
   if(
      io_method.get_input_string(
				 "model_input",
				 "model.bending.switch"
				 )=="off"
      )
     {
       bending_energy_on=false;
     }
   else
     {
       bending_energy_on=true;
     };
    //
   if(
      "off"==io_method.get_input_string(
				       "model_input",
				       "model.coupling.switch"
				       )
      )
     {
       coupling_energy_on=false;
     }
   else
     {
       coupling_energy_on=true;
     };
   //
    show_model_parameters();
    return 0;
};
//
/*
void model_parameters_active_arc_class::check_model_parameter(
	 		  std::string model_parameter
			  )
{
  io_active_arc io_method;
  if(model_parameter=="space_dimensionW")
    {
      if(space_dimension<=space_dimension_upper_bound &&
	 space_dimension>=space_dimension_lower_bound)
	{
	  io_method.debug_output(
				 "debug" ,
				 "model_parameters_active_arc.check_model_parameter", 
				 "error",
				 "abort due to uncapable space dimension"  
				 );
	};
    };
}
*/
//
long int model_parameters_active_arc_class::get_number_of_arcs()
{
  if(number_of_arcs!=-1){
    return number_of_arcs;
  } 
  else
{
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_number_of_arcs )";
    io_method.standard_output(message);
    abort();
  };
};
//
void model_parameters_active_arc_class::change_number_of_arcs(
							      long int & input_number_of_arcs
							      )
{
  number_of_arcs=input_number_of_arcs;
  std::string message;
  message = "!!!!Change of the number of arcs to " 
    + io_method.longint_to_string(number_of_arcs)
    + ".!!!!";
    io_method.standard_output(message);
};
//
void model_parameters_active_arc_class::input_inner_material_parameters()
{
  inner_material_Young_modulus=io_method.get_input_double(
							  "model_input" ,
							  "inner.young_modulus"
							  );
  inner_material_Poisson_ratio=io_method.get_input_double(
							  "model_input" ,
							  "inner.poisson_ratio"
							  );
  inner_type=io_method.get_input_string(
					"model_input",
					"inner.type"
					);
  if(
     inner_type=="volume_radius"
     )
    {
      inner_radius=io_method.get_input_double(
					      "model_input" ,
					      "inner.radius"
					      );
      //
      inner_control_volume
	=
	((double)number_of_arcs)
	*0.5*inner_radius*inner_radius
	*sin(2.0*PI/(double)number_of_arcs);
    }
  else if (
	   inner_type=="volume"
	   )
    {
      inner_control_volume=io_method.get_input_double(
						      "model_input" ,
						      "inner.control_volume"
						      );
    }
  else if (inner_type=="elastic")
    {
      inner_height=io_method.get_input_double(
					      "model_input" ,
					      "inner.height"
					      );
      inner_width=io_method.get_input_double(
					     "model_input" ,
					     "inner.width"
					     );
      inner_control_volume=inner_width*inner_height*0.5;
    }
  else
    {
      io_method.error_output(
			     "active_arc_definition",
			     "input_inner_material_parameters",
			     "irregal inner type!"
			     );
    };
  //
  show_inner_material_parameters();
};
//
void model_parameters_active_arc_class::show_inner_material_parameters()
{
    std::string output_text;
    io_method.standard_output("=== Inner Material parameters started  ===");
    output_text = "Inner Radius =";
    output_text+= io_method.double_to_string(inner_radius);
    io_method.standard_output(output_text);
    output_text = "Inner Hight =";
    output_text+= io_method.double_to_string(inner_height);
    io_method.standard_output(output_text);
    output_text = "Inner Width=";
    output_text+= io_method.double_to_string(inner_width);
    io_method.standard_output(output_text);
    output_text = "Inner Volume =";
    output_text+= io_method.double_to_string(inner_control_volume);
    io_method.standard_output(output_text);
    output_text = "Inner Young Modulus =";
    output_text+= io_method.double_to_string(inner_material_Young_modulus);
    io_method.standard_output(output_text);
    output_text = "Inner Poisson Ratio =";
    output_text+= io_method.double_to_string(inner_material_Poisson_ratio);
    io_method.standard_output(output_text);
    io_method.standard_output("=== Inner material parameters finished ===");
};
//
void model_parameters_active_arc_class::input_md_parameter()
{
   md_time_difference=io_method.get_input_double(
						 "model_input" ,
						 "md.time_difference"
						 );
   //
   md_termination_number_of_iteration
     =io_method.get_input_double(
				 "model_input" ,
				 "md.termination_of_iteration"
				 );
   //
   if(
      "on"==io_method.get_input_string(
				       "model_input",
				       "md.switch"
				       )
      )
     {
       md_on=true;
     }
   else
     {
       md_on=false;
     };
   //
   show_md_parameters();
};
//
void model_parameters_active_arc_class::show_md_parameters()
{
    std::string output_text;
    io_method.standard_output("=== MD parameters  ==="); 
    output_text = "switch for md = ";
    if(md_on)
      {
	output_text += "on";
      }
    else
      {
	output_text += "off";
      };
    io_method.standard_output(output_text); 
    output_text = "Iteration number for md termination =";
    output_text+= io_method.double_to_string(md_termination_number_of_iteration);
    io_method.standard_output(output_text);
    output_text = "Time difference =";
    output_text+= io_method.double_to_string(md_time_difference);
    io_method.standard_output(output_text);
    io_method.standard_output("=== MD parameters finished ===");
};
//
void model_parameters_active_arc_class::input_cg_parameter()
{
   if(
      "on"==io_method.get_input_string(
				       "model_input",
				       "cg.switch"
				       )
      )
     {
       cg_on=true;
     }
   else
     {
       cg_on=false;
     };
   //
   show_cg_parameters();
};
//
void model_parameters_active_arc_class::show_cg_parameters()
{
    std::string output_text;
    io_method.standard_output("=== CG parameters  ==="); 
    output_text = "switch for cg = ";
    if(cg_on)
      {
	output_text += "on";
      }
    else
      {
	output_text += "off";
      };
    io_method.standard_output(output_text); 
    io_method.standard_output("=== CG parameters finished ===");
};
//
void model_parameters_active_arc_class::input_initialstate_parameter()
{
  initial_frequency
    =io_method.get_input_double(
				"model_input" ,
				"initial.frequency"
				);
   //
  initial_amplitude
     =io_method.get_input_double(
				 "model_input" ,
				 "initial.amplitude"
				 );
   //
   show_initialstate_parameters();
};
//
void model_parameters_active_arc_class::show_initialstate_parameters()
{
    std::string output_text;
    io_method.standard_output("=== Initial parameters  ===");
    io_method.standard_output(output_text); 
    output_text = "Frequency of modulation in initial state =";
    output_text+= io_method.double_to_string(initial_frequency);
    io_method.standard_output(output_text);
    output_text = "Amplitude of perturbation =";
    output_text+= io_method.double_to_string(initial_amplitude);
    io_method.standard_output(output_text);
    io_method.standard_output("=== Initial parameters finished ===");
};
//
//
void model_parameters_active_arc_class::input_constraint_parameter()
{
   if(
      "on"==io_method.get_input_string(
				       "model_input",
				       "constraint.shear.switch"
				       )
      )
     {
       shear_constraint_on=true;
     }
   else
     {
       shear_constraint_on=false;
     };
   //
  
   shear_constraint_modulus
     =io_method.get_input_double(
				 "model_input" ,
				 "constraint.shear.modulus"
				 );
   //
   shear_constraint_power
     =io_method.get_input_double(
				 "model_input" ,
				 "constraint.shear.power"
				 );
   //
   show_constraint_parameters();
};
//
void model_parameters_active_arc_class::show_constraint_parameters()
{
    std::string output_text;
    io_method.standard_output("=== Constraint parameters  ==="); 
    output_text = "switch for shear constraint = ";
    if(shear_constraint_on)
      {
	output_text += "on";
      }
    else
      {
	output_text += "off";
      };
    io_method.standard_output(output_text); 
    output_text = "Modulus of Shear constraint =";
    output_text+= io_method.double_to_string(shear_constraint_modulus);
    io_method.standard_output(output_text);
    output_text = "Power of Shear constraint =";
    output_text+= io_method.double_to_string(shear_constraint_power);
    io_method.standard_output(output_text);
    io_method.standard_output("=== Constraint parameters finished ===");
};
//
void model_parameters_active_arc_class::input_io_parameter()
{
  shape_plot_frequency
    =io_method.get_input_double(
				"model_input" ,
				"io.shape_plot.frequency"
				);
  shape_plot_max
    =io_method.get_input_double(
				"model_input" ,
				"io.shape_plot.max"
				);
  shape_plot_base
    =io_method.get_input_double(
				"model_input" ,
				"io.shape_plot.base"
				);
   //
  show_io_parameters();
};
//
void model_parameters_active_arc_class::show_io_parameters()
{
    std::string output_text;
    io_method.standard_output("=== Io parameters  ===");
    io_method.standard_output(output_text); 
    output_text = "Frequency of Shape Plot =";
    output_text+= io_method.double_to_string(shape_plot_frequency);
    io_method.standard_output(output_text);
    output_text = "Maximum point of Shape Plot =";
    output_text+= io_method.double_to_string(shape_plot_max);
    io_method.standard_output(output_text);
    output_text = "Base point of Shape Plot =";
    output_text+= io_method.double_to_string(shape_plot_base);
    io_method.standard_output(output_text);
    io_method.standard_output("=== Initial parameters finished ===");
};
//
double model_parameters_active_arc_class::get_arc_length()
{
  if(arc_length>0.0){
    return arc_length;
  } else{
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_arc_length):";
    message+= io_method.double_to_string(arc_length);
    io_method.standard_output(message);
    abort();
  };
};
//
int model_parameters_active_arc_class::get_space_dimension()
{
  return space_dimension;
};
//
double model_parameters_active_arc_class::get_arc_width()
{
  if(arc_width>0.0)
    {
    return arc_width;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_arc_width)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_arc_curvature()
{
  return arc_curvature;
};
//
double model_parameters_active_arc_class::get_young_modulus()
{
  if(young_modulus>=0.0)
    {
    return young_modulus;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_young_modulus)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_angle_penalty_energy()
{
  if(angle_penalty_energy>=0.0)
    {
    return angle_penalty_energy;
    } 
  else
    {
    std::string message;
    message = "Irrigal model. (model_parameters_active_arc_class::get_angle_penalty_energy)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_poisson_ratio()
{
  if(poisson_ratio>=0.0)
    {
    return poisson_ratio;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_poisson_ratio)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_uniform_growth_ratio() 
{
  if(uniform_growth_ratio<=0.0)
    {
    return uniform_growth_ratio;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_uniform_growth_ratio)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_normal_gradient_growth()
{
  if(normal_gradient_growth>=0.0)
    {
    return normal_gradient_growth;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_normal_gradient_growth)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_tangent_gradient_growth() 
{
  if(tangent_gradient_growth>=0.0)
    {
    return tangent_gradient_growth;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_tangent_gradient_growth)";
    io_method.standard_output(message);
    abort();
    };
};
std::string model_parameters_active_arc_class::get_boundary_condition() 
{
  if(boundary_condition!="close")
    {
    std::string message;
    message = "Inputted irregal boundary condition. (model_parameters_active_arc_class::get_boundary_condition)";
    io_method.standard_output(message);
    abort();
    }
  return boundary_condition;
};
//
double model_parameters_active_arc_class::get_control_volume()
{
  if(inner_control_volume>=0.0)
    {
    return inner_control_volume;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_control_volume)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_inner_material_Young_modulus()
{
  if(inner_material_Young_modulus>=0.0)
    {
    return inner_material_Young_modulus;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_inner_material_Young_modulus)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_inner_material_Poisson_ratio()
{
  if(inner_material_Poisson_ratio>=0.0)
    {
    return inner_material_Poisson_ratio;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_inner_material_Poisson_ratio)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_inner_height()
{
  if(inner_height>0.0)
    {
    return inner_height;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_inner_height)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_inner_width()
{
  if(inner_width>0.0)
    {
    return inner_width;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_inner_width)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_md_time_difference()
{
  if(md_time_difference>=0.0)
    {
    return md_time_difference;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_md_time_difference)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_md_termination_number_of_iteration()
{
  if(md_termination_number_of_iteration>=0.0)
    {
    return md_termination_number_of_iteration;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::md_termination_number_of_iteration)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_initial_frequency()
{
  return initial_frequency;
};
//
double model_parameters_active_arc_class::get_initial_amplitude()
{
  return initial_amplitude;
};
//
std::string model_parameters_active_arc_class::get_initial_state() 
{
    return initial_state;
};
//
bool model_parameters_active_arc_class::check_md_on()
{
  return md_on;
};
//
bool model_parameters_active_arc_class::check_cg_on()
{
  return cg_on;
};
//
std::string model_parameters_active_arc_class::get_inner_type()
{
  return inner_type;
};
//
bool model_parameters_active_arc_class::check_bending_energy_on()
{
  if(bending_energy_on)
    {
    }
  else
    {
      std::string message;
      message = "!!! Membrane approximation (check_bending_couplig_on)";
      io_method.standard_output(message);
    };
  return bending_energy_on;
};
//
bool model_parameters_active_arc_class::check_coupling_energy_on()
{
  if(coupling_energy_on)
    {
    }
  else
    {
      std::string message;
      message = "!!! Decoupling approximation (check_coupling_couplig_on)";
      io_method.standard_output(message);
    };
  return  coupling_energy_on;
};
//
bool model_parameters_active_arc_class::check_shear_constraint_on()
{
  if(shear_constraint_on)
    {
    }
  else
    {
      std::string message;
      message = "!!! Shear unconstraint (check_shear_constraint_on)";
      io_method.standard_output(message);
    };
  return  shear_constraint_on;
};
//
double model_parameters_active_arc_class::get_shear_constraint_modulus()
{
  if(shear_constraint_modulus>=0.0)
    {
    return shear_constraint_modulus;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_shear_constraint_modulus)";
    io_method.standard_output(message);
    abort();
    };
};
//
double model_parameters_active_arc_class::get_shear_constraint_power()
{
  if(shear_constraint_power<=-1.0)
    {
    return shear_constraint_power;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_shear_constraint_@pwer)";
    io_method.standard_output(message);
    abort();
    };
};
//
long int model_parameters_active_arc_class::get_shape_plot_frequency()
{
  if(shape_plot_frequency>=0)
    {
    return shape_plot_frequency;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_shape_plot_frequency)";
    io_method.standard_output(message);
    abort();
    };
};
//
long int model_parameters_active_arc_class::get_shape_plot_max()
{
  if(shape_plot_max>=0)
    {
    return shape_plot_max;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_shape_plot_max)";
    io_method.standard_output(message);
    abort();
    };
};
//
long int model_parameters_active_arc_class::get_shape_plot_base()
{
  if(shape_plot_base>=0)
    {
    return shape_plot_base;
    } 
  else
    {
    std::string message;
    message = "Uninputted model. (model_parameters_active_arc_class::get_shape_plot_base)";
    io_method.standard_output(message);
    abort();
    };
};
  /*======================
    Inconsistency checker
   =======================*/
