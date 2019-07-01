#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::initialize_constraint_stress()
{
  shear_constraint_stress.clear();
  shear_cos_minus_second_power.clear();
  for(long int arc_index=0;arc_index<number_of_arcs;arc_index++)
    {
      shear_constraint_stress.push_back(0.0);
      shear_cos_minus_second_power.push_back(0.0);
    };  
}
//
void state_active_arc_class::calculate_constraint()
{
  if(shear_constraint_switch)
    {
      calculate_shear_cos_minus_second_power();
      calculate_constraint_energy();
      calculate_constraint_stress();
    };
};
//
void state_active_arc_class::calculate_constraint_energy()
{
  calculate_constraint_shear_energy();
};
//
void state_active_arc_class::calculate_constraint_stress()
{
  calculate_constraint_shear_stress();
};
//
void state_active_arc_class::calculate_shear_cos_minus_second_power()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      if(
	 shear[arc_index] < 0.5*PI-bit_parameter
	 &&
	 shear[arc_index] > -0.5*PI+bit_parameter
	 )
	{
	  shear_cos_minus_second_power[arc_index]
	    =pow(cos(shear[arc_index]),shear_constraint_power);
	}
      else
	{
	  shear_cos_minus_second_power[arc_index]
	    =pow(cos(0.5*PI-bit_parameter),shear_constraint_power);
	};
      };
};
//
void state_active_arc_class::calculate_constraint_shear_energy()
{
  constraint_shear_energy
    =std::accumulate(
		     shear_cos_minus_second_power.begin(),
		     shear_cos_minus_second_power.end(),
		     0.0
		     )*shear_constraint_modulus
    /std::fabs(shear_constraint_power);
}
//
void state_active_arc_class::calculate_constraint_shear_stress()
{
  for(
      long int arc_index=0;
      arc_index<number_of_arcs;
      arc_index++
      )
    {
      shear_constraint_stress[arc_index]
	=tan(shear[arc_index])
	*shear_cos_minus_second_power[arc_index]
	*shear_constraint_modulus;
    };
}

