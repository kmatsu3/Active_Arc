#include "active_arc_observable.hpp"
void active_arc_observable::output_observable(const long int & number)
{
  std::string message;
  std::string file = observable 
    + io_method.longint_to_string(number) 
    + ".dat";
  for(long int index;
      index < (long int)values.size();
      index++)
    {
      message = io_method.double_to_string(values[index]);
      io_method.output_message(message,file);
    };
};
//
void active_arc_observable::check_observable()
{
  std::string message = "//// observables= " + observable;
  io_method.standard_output(message);
};
//
void active_arc_observable::initialize_observable(
						   const std::string & input_observable,
						   const long int & number_of_values
						   )
{
  observable=input_observable;
  values.resize(number_of_values);
};
//
void active_arc_observable::push(const double & value)
{
  values.push_back(value);
};
//
control_observables::control_observables()
{
};
//
