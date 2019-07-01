#include "active_arc_state.hpp"
/*
  Operation
 */
void state_active_arc_class::calculate_elastic_energy()
{
  for(long int arc_index=1;arc_index<number_of_arcs;arc_index++)
    {
      arc_elastic_energy[arc_index] 
	= (
	   calculate_strech_energy(arc_index)
	   *bulk_modulus[arc_index]*0.5
	   + calculate_bending_energy(arc_index)
	   *tangent_bending_modulus_second_power[arc_index]*0.5
	   + calculate_coupling_energy(arc_index)
	   *tangent_bending_modulus_first_power[arc_index]
	   );
    };
  total_elastic_energy
    =std::accumulate(
		     arc_elastic_energy.begin(),
		     arc_elastic_energy.end(),
		     0.0
		     )
    +energy_penalty();
};
//
inline double state_active_arc_class::calculate_strech_energy(const long int & arc_index)
{
  return pow(tangent_strain[arc_index],2.0)
    +2.0*arc_poisson_ratio_reference[arc_index]
    *tangent_strain[arc_index]*normal_strain[arc_index]
    +pow(normal_strain[arc_index],2.0)
    +2.0*(1.0-arc_poisson_ratio_reference[arc_index])
    *pow(shear_strain[arc_index],2.0);
};
//
inline double state_active_arc_class::calculate_bending_energy(const long int & arc_index)
{
  return pow(bending_strain[arc_index],2.0)
    +2.0
    *arc_poisson_ratio_reference[arc_index]
    *bending_strain[arc_index]
    *shearbending_strain[arc_index]
    +pow(shearbending_strain[arc_index],2.0);
};
//
inline double state_active_arc_class::calculate_coupling_energy(const long int & arc_index)
{
  return 
    (
      (
       shearbending_strain[arc_index]
       +arc_poisson_ratio_reference[arc_index]
       *bending_strain[arc_index]
       )
      *normal_strain[arc_index]
      +(
	bending_strain[arc_index]
	+arc_poisson_ratio_reference[arc_index]*shearbending_strain[arc_index]
	)
      *tangent_strain[arc_index]
      );
};
//
double state_active_arc_class::energy_penalty()
{
  double penalty_energy=0.0;
  for(
      long int joint_index=0;
      joint_index<number_of_joints;
      joint_index++
      )
    {
      if(angles[joint_index]>PI)
	{
	  penalty_energy=penalty_energy+angle_penalty_energy;
	}
      else if(angles[joint_index]<-PI)
	{
	  penalty_energy=penalty_energy+angle_penalty_energy;
	}
      else
	{
	};
     };
  if(shear_constraint_switch) penalty_energy
				= penalty_energy
				+ shear_constraint_switch;
  return penalty_energy;
};
