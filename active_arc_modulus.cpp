#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::initialize_modulus()
{
  bulk_modulus.clear();
  shear_modulus.clear();
  normal_bending_modulus.clear();
  tangent_bending_modulus.clear();
  tangent_bending_modulus_first_power.clear();
  tangent_bending_modulus_second_power.clear();
  inner_bulk_modulus.clear();
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      bulk_modulus.push_back(0.0);
      shear_modulus.push_back(0.0);
      normal_bending_modulus.push_back(0.0);
      tangent_bending_modulus.push_back(0.0);
      tangent_bending_modulus_first_power.push_back(0.0);
      tangent_bending_modulus_second_power.push_back(0.0);
      inner_bulk_modulus.push_back(0.0);
    };
  //
};
//
void  state_active_arc_class::calculate_modulus()
{
  calculate_bulk_modulus();
  calculate_shear_modulus();
  calculate_normal_bending_modulus();
  calculate_tangent_bending_modulus();
  calculate_tangent_bending_modulus_first_power();
  calculate_tangent_bending_modulus_second_power();
  calculate_inner_bulk_modulus();
  output_modulus();
};
//
void state_active_arc_class::calculate_bulk_modulus()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      bulk_modulus[arc_index]
	= arc_young_modulus_reference[arc_index]
	* arc_length_reference[arc_index]
	* arc_width_reference[arc_index]
	/ (1.0 - arc_poisson_ratio_reference[arc_index]
	   *arc_poisson_ratio_reference[arc_index]);
    };
};
//
void state_active_arc_class::calculate_shear_modulus()
{
  for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
    {
      shear_modulus[arc_index]
	= 2.0 * arc_young_modulus_reference[arc_index]
	* arc_length_reference[arc_index]
	* arc_width_reference[arc_index]
	/ (1.0 + arc_poisson_ratio_reference[arc_index]);
    };
};
//
void state_active_arc_class::calculate_normal_bending_modulus()
{
  if(bending_modulus_switch)
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  normal_bending_modulus[arc_index]
	    = arc_young_modulus_reference[arc_index]
	    * pow(arc_length_reference[arc_index],3.0)
	    * arc_width_reference[arc_index]
	    * pow(arc_curvature_reference[arc_index],2.0)
	    / (1.0 - arc_poisson_ratio_reference[arc_index]
	       *arc_poisson_ratio_reference[arc_index]);
	};
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  normal_bending_modulus[arc_index]
	    = 0.0;
	};
    };
};
//
void state_active_arc_class::calculate_tangent_bending_modulus()
{
  if(bending_modulus_switch)
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  tangent_bending_modulus[arc_index]
	    = arc_young_modulus_reference[arc_index]
	    * arc_length_reference[arc_index]
	    * pow(arc_width_reference[arc_index],3.0)
	    * pow(arc_curvature_reference[arc_index],2.0)
	    / (1.0 - arc_poisson_ratio_reference[arc_index]
	       *arc_poisson_ratio_reference[arc_index]);
	};
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  tangent_bending_modulus[arc_index]
	    = 0.0;
	};
    };
};
//
void state_active_arc_class::calculate_tangent_bending_modulus_first_power()
{
  if(coupling_modulus_switch)
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  tangent_bending_modulus_first_power[arc_index]
	    = arc_young_modulus_reference[arc_index]
	    * pow(arc_width_reference[arc_index],3.0)
	    * arc_curvature_reference[arc_index]
	    / (1.0 - arc_poisson_ratio_reference[arc_index]
	       *arc_poisson_ratio_reference[arc_index]);
	};
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  tangent_bending_modulus_first_power[arc_index]
	    = 0.0;
	};
    };
};
//
void state_active_arc_class::calculate_tangent_bending_modulus_second_power()
{
  if(bending_modulus_switch)
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  tangent_bending_modulus_second_power[arc_index]
	    = arc_young_modulus_reference[arc_index]
	    / arc_length_reference[arc_index]
	    * pow(arc_width_reference[arc_index],3.0)
	    / (1.0 - arc_poisson_ratio_reference[arc_index]
	       *arc_poisson_ratio_reference[arc_index]);
	};
    }
  else
    {
      for(long int arc_index=0; arc_index<number_of_arcs; arc_index++)
	{
	  tangent_bending_modulus_second_power[arc_index]
	    = 0.0;
	};
    };
};
//
void state_active_arc_class::calculate_inner_bulk_modulus()
{
  if(inner_type=="volume"||inner_type=="volume_radius")
    {
      for(
	  long int arc_index = 0;
	  arc_index < number_of_arcs;
	  arc_index++
	  )
	{
	  inner_bulk_modulus[arc_index]
	    =inner_material_Young_modulus[arc_index]
	    /(1.0 - inner_material_Poisson_ratio[arc_index]*
	      inner_material_Poisson_ratio[arc_index])
	    /inner_control_volume[arc_index];
	};
    }
  else if(inner_type=="elastic")
    {
      calculate_inner_control_volume();
      for(
	  long int arc_index = 0;
	  arc_index < number_of_arcs;
	  arc_index++
	  )
	{
	  inner_bulk_modulus[arc_index]
	    =inner_material_Young_modulus[arc_index]
	    /(1.0 - inner_material_Poisson_ratio[arc_index]*
	      inner_material_Poisson_ratio[arc_index])
	    *inner_control_volume[arc_index];      
	};
    };
};
//
void state_active_arc_class::output_modulus()
{
  io_method.standard_output("=== values of modulus ===");
  std::string message;
  for(
      long int arc_index = 0;
      arc_index < number_of_arcs;
      arc_index++
      )
    {
      message ="Arc [" + io_method.longint_to_string(arc_index) + "] = (";
      message+="BM = " + io_method.double_to_string(bulk_modulus[arc_index]) + ",";
      message+="SM = " + io_method.double_to_string(shear_modulus[arc_index]) + ",";
      message+="/n NBM = " + io_method.double_to_string(normal_bending_modulus[arc_index]) + ",";
      message+="TBM = " + io_method.double_to_string(tangent_bending_modulus[arc_index]) + ",";
      message+="/n NBM1 = " + io_method.double_to_string(tangent_bending_modulus_first_power[arc_index]) + ",";
      message+=" NBM2 = " + io_method.double_to_string(tangent_bending_modulus_second_power[arc_index]) + ",";
      message+=" IBM = " + io_method.double_to_string(inner_bulk_modulus[arc_index]) + ")";
      io_method.standard_output(message);
    };
};
