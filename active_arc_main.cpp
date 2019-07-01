#include "active_arc_definition.hpp"
#include "active_arc_state.hpp"
#include "active_arc_schedule.hpp"
#include "active_arc_reconstruction.hpp"
#include "active_arc_observable.hpp"
#include <stdio.h>
#include <memory>
int main()
{
  fprintf(stderr,"program is started.\n");
  model_parameters_active_arc_class model;
  model.input_model();
  std::unique_ptr<state_active_arc_class> state 
    = std::make_unique<state_active_arc_class>(model);
  state->initialize(
		   model,
		   model.get_initial_state()
		   );
  state->initialize_reference_parameters("uniform_initialize");
  //
  schedule_active_arc schedule;
  schedule.input_schedule(state);
  //
  model_reconstructor reconstructor(model);
  control_observables observable_data;
  state->initialize_observables(observable_data);
  //
  int iteration_index=0;
  while(schedule.check_termination(iteration_index))
    {
      //      schedule.set_schedule(state,iteration_index);
      if(model.check_cg_on())
	{
	  state->cg_relaxation(model,iteration_index);
	  state->plot(
		     998,
		     iteration_index,
		     "gnuplot"
		     );
	  state->shape_plot(
			   998,
			   iteration_index,
			   "gnuplot"
			   );
	  if(state->nan_check(iteration_index)) std::exit(EXIT_FAILURE);  
	};
      if(model.check_md_on())
	{
	  state->md_relaxation(model,iteration_index);
	  state->plot(
		     999,
		     iteration_index,
		     "gnuplot"
		     );
	  state->shape_plot(
			   999,
			   iteration_index,
			   "gnuplot"
			   );
	  if(state->nan_check(iteration_index)) std::exit(EXIT_FAILURE); 
	};
      state->calculate_observables(observable_data);
      iteration_index++;
      schedule.update(state);
      schedule.reference_replace(state);
      {
	if(state->reconstruction_check(reconstructor))
	  {
	    model.change_number_of_arcs(reconstructor.new_number_of_arcs);
	    state.reset(new state_active_arc_class(model));
	    state->reconstruction(reconstructor,iteration_index);
	    schedule.reconstruction(
				    model,
				    reconstructor
				    );
	  };
      };
    };
  //
  state->output_observables(observable_data,999);
  //
  fprintf(stderr,"program is finished.\n");
  //
};
//
