#include "active_arc_state.hpp"
void state_active_arc_class::initialize_observables(
						    control_observables & observable_data
						    )
{
  observable_data.observables.clear();
  active_arc_observable work_observable;
  work_observable.initialize_observable("max_arc_stress",0);
  observable_data.observables.push_back(work_observable);
  observable_data.observables[0].check_observable();
  work_observable.initialize_observable("inner_arc_stress",0);
  observable_data.observables.push_back(work_observable);
  observable_data.observables[1].check_observable();
  work_observable.initialize_observable("cortical_length",0);
  observable_data.observables.push_back(work_observable);
  observable_data.observables[2].check_observable();
};
//
void state_active_arc_class::calculate_observables(
						   control_observables & observable_data
						   )
{
  calculate_max_stress(observable_data);
  calculate_cortical_length(observable_data);
};
//
void state_active_arc_class:: calculate_max_stress(
						   control_observables & observable_data
						   )
{
  double max_stress=0.0;
  double stress=0.0;
  for(long int arc_index;
      arc_index<number_of_arcs;
      arc_index++)
    {
      stress=tangent_stress[arc_index]
	/arc_length_reference[arc_index]
	/arc_width_reference[arc_index];
      if(
	 max_stress<abs(stress)
	 )
	{
	  max_stress=stress;
	};
    };
  observable_data.observables[0].push(max_stress);
  //
  max_stress=0.0;
  stress=0.0;
  for(long int arc_index;
      arc_index<number_of_arcs;
      arc_index++)
    {
      stress=inner_ripening_stress[arc_index]
	/inner_height[arc_index]
	/inner_width[arc_index];
      if(
	 max_stress<abs(stress)
	 )
	{
	  max_stress=stress;
	};
    };
  observable_data.observables[1].push(max_stress);
};
//
void state_active_arc_class::calculate_cortical_length(
						       control_observables & observable_data
						       )
{
  double cortical_length=0.0;
  for(long int arc_index;
      arc_index<number_of_arcs;
      arc_index++)
    {
      cortical_length=cortical_length
	+arc_length_reference[arc_index];
    };
  observable_data.observables[2].push(cortical_length);
};
//
void state_active_arc_class::output_observables(
						control_observables & observable_data,
						const long int & number
)
{
  for(int observable_index=0;
      observable_index<(int)observable_data.observables.size();
      observable_index++)
    {
      observable_data.observables[observable_index].output_observable(number);
    };
};
