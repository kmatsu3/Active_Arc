// constructor for growth model
#include "active_arc_growth_model.hpp"
growth_model::growth_model()
{
  bit_parameter=0.00000000001;
};
//
void growth_model::incriment_growth(
				    schedule & input_schedule
				    )
{
  std::vector<double>::iterator index=input_schedule.values.begin();
  long int counter=0;
  double value;
  while(index!=input_schedule.values.end())
    {
      value = *index 
	+ input_schedule.incriments[counter]
	* input_schedule.adaptations[counter];
      *index= value;
      index++;
      counter++;
    };
};
//
//
void growth_model::inverse_growth(
				  schedule & input_schedule
				  )
{
  std::vector<double>::iterator index=input_schedule.values.begin();
  long int counter=0;
  double value;
  while(index!=input_schedule.values.end())
    {
      if(std::abs((*index)) > bit_parameter)
	{
	  value = 1.0/(*index) 
	    + input_schedule.incriments[counter]
	    * input_schedule.adaptations[counter];
	  *index= 1.0/value;
	}
      else
	{
	  *index = 1.0/input_schedule.incriments[counter];
	};
      index++;
      counter++;
    };
};
//
void growth_model::calculate_adaptation(
					schedule & input_schedule,
					const std::vector<double> & stress,
					const std::vector<double> & volumes
					
					)
{
  double amplitude_sign=copysign(1.0,input_schedule.adaptation_amplitude);
  if(input_schedule.adaptation_on)
    {
      if(input_schedule.adaptations.size()==stress.size())
      	{
	  //  if(input_schedule.parameter=="curvature")
	  //  {
	  //    for(
	  //	  long int index=0;
	  //	  index < (long int) input_schedule.adaptations.size();
	  //	  index ++
	  //	  )
	  //	{
	  //	  input_schedule.adaptations[index]
	  //	    =input_schedule.adaptation_amplitude
	  //	    *tanh(
	  //		  stress[index]/input_schedule.adaptation_width
	  //		  );
	  //	};
	      //    }
	      //  else
	      //  {
	  for(
	      long int index=0;
	      index < (long int) input_schedule.adaptations.size();
	      index ++
	      )
	    {
	      input_schedule.adaptations[index]
		=1.0
		+0.5*input_schedule.adaptation_amplitude
		*(
		  tanh(
		       (stress[index]/volumes[index]-input_schedule.adaptation_threshold)
		       /input_schedule.adaptation_width
		       )
		  -amplitude_sign
		  );
	      //		= input_schedule.adaptation_threshold
	      //		/ (input_schedule.adaptation_threshold
	      //   +input_schedule.adaptation_amplitude*stress[index]);
	    };
	  //   };
	  output_adaptation(
			    input_schedule,
			    stress,
			    volumes
			    );
	}
      else
	{
	  io_method.error_output(
				 "growth_model",
				 "calculation_adaptation",
				 "due to inconsistency in array size!"
				 );
	};
    }
  else
    {
      for(
	  auto index=input_schedule.adaptations.begin();
	  index != input_schedule.adaptations.end();
	  index ++
	  )
	{
	  (*index) = 1.0;
	};
    };
};
//
void growth_model::output_adaptation(
				     schedule & input_schedule,
				     const std::vector<double> & stress,
				     const std::vector<double> & volumes
				     )
{
  io_method.standard_output("//// Adaptation factor: "
			    + input_schedule.categoly
			    + "/"
			    + input_schedule.parameter
			    + " ////");
  std::string message;
  long int counter=0;
  for(
      auto index=input_schedule.adaptations.begin();
      index != input_schedule.adaptations.end();
      index++
      )
    {
      message = "Adp["+io_method.longint_to_string(counter) + "]=(";
      message+= "Val="+io_method.double_to_string(*index)+", ";
      message+= "Str="+io_method.double_to_string(stress[counter]/volumes[*index]);
      io_method.standard_output(message);
      counter ++;
    };
};
//
void schedule::initialize_values(
				 std::unique_ptr<state_active_arc_class> & state
				 )
{
  double position;
  values.clear();
  incriments.clear();
  adaptations.clear();
  long int counter=0;
  for(long int index=0;index<(long int)state->get_number_of_arcs();index++)
    {
      position=((double)counter)/((double)state->get_number_of_arcs());
      values.push_back(start_value);
      incriments.push_back(
			   incriment
			   *(
			     1.0
			     +amplitude
			     *(
			       cos(
				   wave_number
				   *2.0*PI*position
				   )
			       +1.0
			       )/2.0
			     )/(1.0+amplitude)
			   );
      adaptations.push_back(1.0);
      counter++;
    };
  output_growth();
};
//
//
void schedule::replace_values(
			      const std::vector<double> & input_values
			      )
{
  values.clear();
  for(
      auto index=input_values.begin();
      index != input_values.end();
      index++
      )
    {
      values.push_back(*index);
    };
};
//
void schedule::replace_incriments(
				  const std::vector<double> & input_incriments
				  )
{
  incriments.clear();
  for(
      auto index=input_incriments.begin();
      index != input_incriments.end();
      index++
      )
    {
      incriments.push_back(*index);
    };
};
//
void schedule::replace_adaptations( const long int & number_of_arcs)
{
  adaptations.resize(number_of_arcs);
  for(
      long int index=0;
      index<number_of_arcs;
      index++
      )
    {
      adaptations[index]=1.0;
    };
};
//
void schedule::get_values(
			  std ::vector<double> & output_values
			  )
{
  long int counter=0;
  for(
      auto index=output_values.begin();
      index != output_values.end();
      index++
      )
    {
      *index=values[counter];
      counter++;
    };
};
//
void schedule::get_incriments(
			      std ::vector<double> & output_incriments
			      )
{
  long int counter=0;
  for(
      auto index=output_incriments.begin();
      index != output_incriments.end();
      index++
      )
    {
      *index=incriments[counter];
      counter++;
    };
};
//
void schedule::output_growth()
{
  io_method.standard_output(
			    "////Growth Profile["
			    + categoly + " / "
			    + parameter+ "////"
			    );
  for( long int index = 0;
       index < (long int) values.size();
       index ++)
    {
      io_method.standard_output(
				"arc[" 
				+ io_method.longint_to_string(index)
				+"]:"
				+"Val=" + io_method.double_to_string(values[index])
				+"/Inc=" + io_method.double_to_string(incriments[index])
				+"/Adp=" + io_method.double_to_string(adaptations[index])
				);
    }
};
//
schedule::schedule()
{
  PI=3.1415926535897931;
};
