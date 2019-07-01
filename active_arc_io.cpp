#include "active_arc_io.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
//
class io_control_class
{
  /*======================
    io control flags
   =======================*/
  public:
  std::string input_echo_mode;
  /*======================
    Methods
   =======================*/
  public:
  std::string get_io_mode(
			  const std::string & file_identifier,
			  const std::string & data_identifier
			  ) ;
  /*======================
    Constructor
   =======================*/
  io_control_class();
};
//
class io_active_arc_class
{
  /*======================
    File name definition
    They are initialized at constructor
   =======================*/
private:  std::string debug_output   ; // Debug output file
private:  std::string io_mode_input  ; // IO control input file
private:  std::string model_input    ; // Model parameter input file
private:  std::string standard_output; // Standard output file
private:  std::string cell_type_input; // Cell type definition input file
private:  std::string site_io        ; // Site data file
private:  std::string region_input; // Regions, which are separatly used for control onsite parameter, input file
private:  std::string site_setting_input; // Site data file
private:  std::string configuration_input; // configuration input data file
private:  std::string configuration_output; // configuration output data file
private:  std::string configuration_output_gnuplot_data;    // configuration output for gnuplot
private:  std::string configuration_output_gnuplot_plotter; // configuration output for gnuplot 
private:  std::string polarity_input; // polarity input data file
private:  std::string polarity_output; // polarity output data file
private:  std::string polarity_output_gnuplot_data;    // polarity output for gnuplot
private:  std::string polarity_output_gnuplot_plotter; // polarity output for gnuplot 
private:  std::string observation_result; // observation output for gnuplot 
private:  std::string observation_average; // observation average output for gnuplot 
  //
private:  std::string observables_output_data;// observed data output
private:  std::string observables_average_output_data;// observed data output
private:  std::string track_output_data;//observed data output
  //
private:  std::string initial_region_input; // configuration initialization  
  /*======================
    Common members for io
   =======================*/
private:std::string file_identifier;
private:std::string file_name      ;
private:std::string data_identifier;
private:std::string value          ;
  /*=======================
    Commmon member function declaration
   =======================*/
  // Method setting value into a member specified by data_identifier
public: void data_set(
		      const std::string & input_value, 
		      const std::string & input_data_identifier
		      ); 
  //
public: std::string data_get(
			     const std::string & input_file_identifier, 
			     const std::string & input_data_identifier
			     ) const;
  /*======================
    Members for debug output
   =======================*/
private:std::string calling_function;
private:std::string error_code;
private:std::string main_text;
public: std::string output_line(); // Method for outputting a line into a file
public: void output_trunc() const; // Method for initialization of output filr
public: void show();
  /*======================
    Members for get input
    =======================*/
public: std::string get_input(); // Method for getting a value from a file
public: void get_input_array(
			     std::vector<std::string> &input_data
			     ); // Method for getting a array from a file
public: void get_input_fixeddim_array(
				      std::vector<std::string> & input_data
				      );
  /*======================
    Constructor and deconstructer declaration
  =======================*/
public: io_active_arc_class();
};
/*
  Definition of Operating function
 */
void io_active_arc::debug_output(
				     const std::string & file_identifier ,
				     const std::string & calling_function, 
				     const std::string & error_code      ,
				     const std::string & main_text     
				     )
  const  { 
  io_active_arc_class io_instance;
  io_instance.data_set(file_identifier,"file");
  io_instance.data_set(calling_function,"function");
  io_instance.data_set(error_code,"code");
  io_instance.data_set(main_text,"text");
  io_instance.output_line();
};
//
void io_active_arc::debug_output_longint(
					     const std::string & file_identifier ,
					     const std::string & calling_function, 
					     const std::string & error_code      ,
					     const long int & data     
					     )
  { 
    std::string text=io_active_arc::longint_to_string(data);
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(calling_function,"function");
    io_instance.data_set(error_code,"code");
    io_instance.data_set(text,"text");
    io_instance.output_line();
    io_instance.show();
  };
//
//
void io_active_arc::standard_output(
				    const std::string & main_text       
				    )
  { 
    io_active_arc_class io_instance;
    io_instance.data_set("standard_output","file");
    io_instance.data_set(main_text,"text");
    io_instance.output_line();
  };
//
void io_active_arc::output_message(
				       const std::string & message,
				       const std::string & file_identifier
				       )
  const { 
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(message,"text");
    io_instance.output_line();
  };
//
void io_active_arc::file_initialize(
					const std::string & file_identifier
					)
{ 
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.output_trunc();
};
//
std::string io_active_arc::get_filename(const std::string & file_identifier) 
  const {
  io_active_arc_class io_instance;
  return io_instance.data_get(file_identifier,"file");
};
//
std::string io_active_arc::get_input_string(
						const std::string & file_identifier ,
						const std::string & data_identifier
						)
 {
    std::string return_value("");
    std::string message;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    return_value = io_instance.get_input();
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
      if(io_control.input_echo_mode=="on")
	{
	  message = data_identifier;
	  message+= " = ";
	  message+= return_value;
	  io_active_arc::standard_output(message);
	};
    return return_value;
 };
//
//
int io_active_arc::get_input_int(
				     const std::string & file_identifier ,
				     const std::string & data_identifier
				     )
 {
    std::string return_value("");
    std::string message;
    int value;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    return_value = io_instance.get_input();
    try {
      value = boost::lexical_cast<int>(return_value);
    } catch (const boost::bad_lexical_cast& error){
      std::string error_message;
      error_message = "boost::lexical_cast<int> for value from io_instance.get_input() is failed/ code:";
      error_message += error.what();
      error_message += "input string:";
      error_message += return_value;
      debug_output(
		   "debug" ,
		   "io_active_arc_class::get_input_int", 
		    "error",
		   error_message    
		   );
      abort();
    };
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
      if(io_control.input_echo_mode=="on")
	{
	  message = data_identifier;
	  message+= " = ";
	  message+= boost::lexical_cast<std::string>(value);
	  io_active_arc::standard_output(message);
	};
    return value;
 };
//
long int io_active_arc::get_input_longint(
			       		      const std::string & file_identifier ,
		       			      const std::string & data_identifier
	       				      )
 {
    std::string return_value;
    std::string message;
    long int value;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    return_value = io_instance.get_input();
    try {
      value = boost::lexical_cast<long int>(return_value);
    } catch (boost::bad_lexical_cast& error){
      std::string error_message;
      error_message = "boost::lexical_cast<long> for value from io_instance.get_input() is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc_class::get_input_longint", 
		   "error",
		   error_message    
		   );
      // abort();
    };
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
      if(io_control.input_echo_mode=="on")
	{
	  message = data_identifier;
	  message+= " = ";
	  message+= boost::lexical_cast<std::string>(value);
	  io_active_arc::standard_output(message);
	};
    return value;
 };
//
long long int io_active_arc::get_input_longlongint(
						       const std::string & file_identifier ,
						       const std::string & data_identifier
						       )
 {
    std::string return_value;
    std::string message;
    long int value;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    return_value = io_instance.get_input();
    try {
      value = boost::lexical_cast<long long int>(return_value);
    } catch (boost::bad_lexical_cast& error){
      std::string error_message;
      error_message  = "boost::lexical_cast<long long> for value from io_instance.get_input() is failed in";
      error_message += file_identifier;
      error_message += "/";
      error_message += data_identifier;
      error_message += "code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc_class::get_input_longlongint", 
		   "error",
		   error_message    
		   );
      abort();
    };
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
      if(io_control.input_echo_mode=="on")
	{
	  message = data_identifier;
	  message+= " = ";
	  message+= boost::lexical_cast<std::string>(value);
	  io_active_arc::standard_output(message);
	};
    return value;
 };
//
double io_active_arc::get_input_double(
					   const std::string & file_identifier ,
					   const std::string & data_identifier
					   )
 {
    std::string return_value;
    std::string message;
    double value;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    return_value = io_instance.get_input();
    try {
      value = boost::lexical_cast<double>(return_value);
    } catch (boost::bad_lexical_cast& error){
      std::string error_message;
      error_message = "boost::lexical_cast<double> for value from io_instance.get_input() is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc_class::get_input_double", 
		   "error",
		   error_message    
		   );
      abort();
    };
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
      if(io_control.input_echo_mode=="on")
	{
	  message = data_identifier;
	  message+= " = ";
	  message+= boost::lexical_cast<std::string>(value);
	  io_active_arc::standard_output(message);
	};
    return value;
 };
// put function
std::string io_active_arc::int_array_to_string(
						   const std::vector<int> & array,
						   const std::string & separator
						   ) 
  const {
  std::vector<int>::const_iterator array_index=array.begin();
  std::string array_string;
  array_string="(";
  while(array_index!=array.end())
    {
      array_string += int_to_string(*array_index);
      if(array_index+1!=array.end()) array_string += separator;
      array_index++;
    };
  array_string+=")";
  return array_string;
};
//
std::string io_active_arc::longint_array_to_string(
						       const std::vector<long int> & array,
						       const std::string & separator
						       ) 
  const {
  std::vector<long int>::const_iterator array_index=array.begin();
  std::string array_string;
  array_string="(";
  while(array_index!=array.end())
    {
      array_string += longint_to_string(*array_index);
      if(array_index+1!=array.end()) array_string += separator;
      array_index++;
    };
  array_string+=")";
  return array_string;
};
//
std::string io_active_arc::longlongint_array_to_string(
							   const std::vector<long long int> & array,
							   const std::string & separator
							   ) 
  const {
  std::vector<long long int>::const_iterator array_index=array.begin();
  std::string array_string;
  array_string="(";
  while(array_index!=array.end())
    {
      array_string += longlongint_to_string(*array_index);
      if(array_index+1!=array.end()) array_string += separator;
      array_index++;
    };
  array_string+=")";
  return array_string;
};
//
std::string io_active_arc::double_array_to_string(
						      const std::vector<double> & array,
						      const std::string & separator
						      ) 
  const {
  std::vector<double>::const_iterator array_index=array.begin();
  std::string array_string;
  array_string="(";
  while(array_index!=array.end())
    {
      array_string += double_to_string(*array_index);
      if(array_index+1!=array.end()) array_string += separator;
      array_index++;
    };
  array_string+=")";
  return array_string;
};
//
void io_active_arc::get_input_string_array(
					       const std::string & file_identifier   ,
					       const std::string & data_identifier   ,
					       std::vector<std::string> &data
					       )
{
  io_active_arc_class io_instance;
  io_instance.data_set(file_identifier,"file");
  io_instance.data_set(data_identifier,"data");
  io_instance.get_input_array(data);
 };
//
void io_active_arc::get_input_int_array(
					   const std::string & file_identifier   ,
					   const std::string & data_identifier   ,
					   std::vector<int> & data
					   )
 {
    std::string return_value;
    std::string message;
    std::vector<std::string> input_data;
    int counter;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    io_instance.get_input_array(input_data);
    //
    std::vector<std::string>::iterator index=input_data.begin();
    counter=0;
    message = data_identifier;
    while(index!=input_data.end())
      {
	try {
	  data.push_back(boost::lexical_cast<int>(*index));
	} catch (boost::bad_lexical_cast& error){
	  std::string error_message;
	  error_message = "boost::lexical_cast<int> for value from io_instance.get_input() is failed/ code:";
	  error_message += error.what();
	  debug_output(
		       "debug" ,
		       "io_active_arc_class::get_input_int_array", 
		       "error",
		       error_message    
		       );
	  // abort();
	};
	  message+="(";
	  message+= boost::lexical_cast<std::string>(counter);
	  message+=")";
	  message+= " = ";
	  message+= boost::lexical_cast<std::string>(*index);
    	  index++;
      };
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
    if(io_control.input_echo_mode=="on")
	{
	  io_active_arc::standard_output(message);
	};
 };
//
void io_active_arc::get_input_longint_array(
						const std::string & file_identifier   ,
						const std::string & data_identifier   ,
						std::vector<long int> &data
						)
 {
    std::string return_value;
    std::string message;
    std::vector<std::string> input_data;
    int counter;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    io_instance.get_input_array(input_data);
    //
    std::vector<std::string>::iterator index=input_data.begin();
    counter=0;
    message = data_identifier;
    while(index!=input_data.end())
      {
	try {
	  data.push_back(boost::lexical_cast<long int>(*index));
	} catch (boost::bad_lexical_cast& error){
	  std::string error_message;
	  error_message = "boost::lexical_cast<long> for value from io_instance.get_input() is failed/ code:";
	  error_message += error.what();
	  debug_output(
		       "debug" ,
		       "io_active_arc_class::get_input_longint_array", 
		       "error",
		       error_message    
		       );
	  // abort();
	};
	  message+="(";
	  message+= boost::lexical_cast<std::string>(counter);
	  message+=")";
	  message+= " = ";
	  message+= boost::lexical_cast<std::string>(*index);
    	  index++;
      };
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
    if(io_control.input_echo_mode=="on")
      {
	io_active_arc::standard_output(message);
      };
 };
//
void io_active_arc::get_input_longlongint_array(
						    const std::string & file_identifier   ,
						    const std::string & data_identifier   ,
						    std::vector<long long int> &data
						    )
{
  std::string return_value;
  std::string message;
  std::vector<std::string> input_data;
  int counter;
  io_active_arc_class io_instance;
  io_instance.data_set(file_identifier,"file");
  io_instance.data_set(data_identifier,"data");
  io_instance.get_input_array(input_data);
  //
  std::vector<std::string>::iterator index=input_data.begin();
  counter=0;
  message = data_identifier;
  while(index!=input_data.end())
    {
      try {
	data.push_back(boost::lexical_cast<long long int>(*index));
      } catch (boost::bad_lexical_cast& error){
	std::string error_message;
	error_message = "boost::lexical_cast<long long> for value from io_instance.get_input() is failed in";
	error_message += file_identifier + "/" + data_identifier + " for error code:";
	error_message += error.what();
	debug_output(
		     "debug" ,
		     "io_active_arc_class::get_input_longlongint_array", 
		     "error",
		     error_message    
		     );
	abort();
      };
      message+="(";
      message+= boost::lexical_cast<std::string>(counter);
      message+=")";
      message+= " = ";
      message+= boost::lexical_cast<std::string>(*index);
      index++;
    };
  io_control_class io_control;
  io_control.get_io_mode(
			 "io_mode_input",
			 "io_mode.input_echo"
			 );
  if(io_control.input_echo_mode=="on")
    {
      io_active_arc::standard_output(message);
    };
};
//
void io_active_arc::get_input_unsignedlongint_array(
							const std::string & file_identifier   ,
							const std::string & data_identifier   ,
							std::vector<unsigned long int> &data
							)
{
  std::string return_value;
  std::string message;
  std::vector<std::string> input_data;
  int counter;
  io_active_arc_class io_instance;
  io_instance.data_set(file_identifier,"file");
  io_instance.data_set(data_identifier,"data");
  io_instance.get_input_array(input_data);
  //
  std::vector<std::string>::iterator index=input_data.begin();
  counter=0;
  message = data_identifier;
  while(index!=input_data.end())
    {
      try {
	data.push_back(boost::lexical_cast<unsigned long int>(*index));
      } catch (boost::bad_lexical_cast& error){
	std::string error_message;
	error_message = "boost::lexical_cast<unsgined long> for value from io_instance.get_input() is failed in";
	error_message += file_identifier;
	error_message += "/";
	error_message += data_identifier;
	error_message += ", code:";
	error_message += error.what();
	debug_output(
		     "debug" ,
		     "io_active_arc_class::get_input_unsignedlongint_array", 
		     "error",
		     error_message    
		     );
	// abort();
      };
      message+="(";
      message+= boost::lexical_cast<std::string>(counter);
      message+=")";
      message+= " = ";
      message+= boost::lexical_cast<std::string>(*index);
      index++;
    };
  io_control_class io_control;
  io_control.get_io_mode(
			 "io_mode_input",
			 "io_mode.input_echo"
			 );
  if(io_control.input_echo_mode=="on")
    {
      io_active_arc::standard_output(message);
    };
};
//
void io_active_arc::get_input_double_array(
					       const std::string & file_identifier   ,
					       const std::string & data_identifier   ,
					       std::vector<double> & data
					       )
 {
    std::string return_value;
    std::string message;
    std::vector<std::string> input_data;
    int counter;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    io_instance.get_input_array(input_data);
    //
    std::vector<std::string>::iterator index=input_data.begin();
    counter=0;
    message = data_identifier;
    while(index!=input_data.end())
      {
	try {
	  data.push_back(boost::lexical_cast<double>(*index));
	} catch (boost::bad_lexical_cast& error){
	  std::string error_message;
	  error_message = "boost::lexical_cast<double> for value from io_instance.get_input() is failed/ code:";
	  error_message += error.what();
	  debug_output(
		       "debug" ,
		       "io_active_arc_class::get_input_double_array", 
		       "error",
		       error_message    
		       );
	  abort();
	};
	  message+="(";
	  message+= boost::lexical_cast<std::string>(counter);
	  message+=")";
	  message+= " = ";
	  message+= boost::lexical_cast<std::string>(*index);
	  index++;
      };
    io_control_class io_control;
    io_control.get_io_mode(
			   "io_mode_input",
			   "io_mode.input_echo"
			   );
    //    fprintf(stderr,"%s\n",io_control.input_echo_mode.c_str());
    if(io_control.input_echo_mode=="on")
      {
	io_active_arc::standard_output(message);
      };
 };
//
/* 
   Constructer
*/
io_active_arc_class::io_active_arc_class()
  {
  debug_output="debug.txt";
  io_mode_input="model_input.txt";
  model_input="model_input.txt";
  standard_output="standard_output.txt";
  cell_type_input="cell_type_input.txt";
  site_io="site_io.txt";
  region_input="region_input.txt";
  site_setting_input="configuration_setting_input.txt";
  configuration_output="configuration.txt";
  configuration_input="configuration.txt";
  configuration_output_gnuplot_data="configuration_gnuplot.txt";
  configuration_output_gnuplot_plotter="configuration_gnuplot.plt";
  polarity_output="polarity.txt";
  polarity_input="polarity.txt";
  polarity_output_gnuplot_data="polarity_gnuplot.txt";
  polarity_output_gnuplot_plotter="polarity_gnuplot.plt";
  observables_output_data="observation_result.txt";
  observables_average_output_data="observation_average.txt";
  track_output_data="cell_track.txt";
  //
  };
/*
  Member functions
*/
void io_active_arc_class::show()
{
  io_active_arc function;
  function.standard_output(file_identifier);
  function.standard_output(calling_function);
  function.standard_output(error_code);
  function.standard_output(main_text);
  function.standard_output(data_identifier);
};
//
void io_active_arc_class::data_set(
				      const std::string & input_value,
				      const std::string & input_data_identifier 
				      )
{
  if(input_data_identifier=="file") 
    {
    file_identifier=input_value;
    if(file_identifier=="io_mode_input") file_name=io_mode_input;
    if(file_identifier=="model_input") file_name=model_input;
    if(file_identifier=="standard_output") file_name=standard_output;
    if(file_identifier=="cell_type_input") file_name=cell_type_input;
    if(file_identifier=="site_io") file_name=site_io;
    if(file_identifier=="site_setting_input") file_name=site_setting_input;
    if(file_identifier=="configuration_input") file_name=configuration_input;
    if(file_identifier=="configuration_output") file_name=configuration_output;
    if(file_identifier=="configuration_output_gnuplot_data") file_name=configuration_output_gnuplot_data;
    if(file_identifier=="configuration_output_gnuplot_plotter") file_name=configuration_output_gnuplot_plotter;
    if(file_identifier=="region_input") file_name=region_input;
    if(file_identifier=="polarity_input") file_name=polarity_input;
    if(file_identifier=="polarity_output") file_name=polarity_output;
    if(file_identifier=="polarity_output_gnuplot_data") file_name=polarity_output_gnuplot_data;
    if(file_identifier=="polarity_output_gnuplot_plotter") file_name=polarity_output_gnuplot_plotter;
    if(file_identifier=="observation_result") file_name=observables_output_data;
    if(file_identifier=="observation_average") file_name=observables_average_output_data;
    if(file_identifier=="cell_track") file_name=track_output_data;
    }
  if(input_data_identifier=="function") calling_function=input_value;
  if(input_data_identifier=="code") error_code=input_value;
  if(input_data_identifier=="text") main_text=input_value;
  if(input_data_identifier=="data") data_identifier=input_value;
};
//
std::string io_active_arc_class::output_line()
  {
    std::string return_message="error";
    if(file_identifier=="debug"||file_identifier=="")
      {
	std::ofstream ofs( debug_output.c_str(), std::ios::out | std::ios::app  );

	ofs << "!!!! A debug output from " << calling_function << std::endl;
	ofs << "debug code = " << error_code << std::endl;
	ofs << main_text << std::endl;
	ofs << "!!!! The debug output is finished." << std::endl;
        ofs.close();
	return_message = "successfly finished";
      } else if(file_identifier=="standard_output")
      {
	std::ofstream ofs( standard_output.c_str(), std::ios::out | std::ios::app  );
	ofs << main_text << std::endl;
        ofs.close();
	return_message = "successfly finished";
      } else 
      {
	std::ofstream ofs( file_identifier.c_str(), std::ios::out | std::ios::app  );
	ofs << main_text << std::endl;
        ofs.close();
	return_message = "successfly finished";
      };
    //
    return return_message;
    //
  }
//
void io_active_arc_class::output_trunc()
  const {
  std::ofstream ofs( file_name.c_str(), std::ios::trunc  );
};
//
std::string io_active_arc_class:: data_get(
					       const std::string & input_file_identifier,
					       const std::string & input_data_identifier 
					       )
  const {
  if(input_data_identifier=="file") 
    {
    if(input_file_identifier=="io_mode_input") return io_mode_input;
    if(input_file_identifier=="model_input") return model_input;
    if(input_file_identifier=="standard_output") return standard_output;
    if(input_file_identifier=="cell_type_input") return cell_type_input;
    if(input_file_identifier=="site_io") return site_io;
    if(input_file_identifier=="site_setting_input") return site_setting_input;
    if(input_file_identifier=="configuration_input") return configuration_input;
    if(input_file_identifier=="configuration_output") return configuration_output;
    if(input_file_identifier=="configuration_output_gnuplot_data") return configuration_output_gnuplot_data;
    if(input_file_identifier=="configuration_output_gnuplot_plotter") return configuration_output_gnuplot_plotter;
    if(input_file_identifier=="region_input") return region_input;
    if(input_file_identifier=="polarity_input") return polarity_input;
    if(input_file_identifier=="polarity_output") return polarity_output;
    if(input_file_identifier=="polarity_output_gnuplot_data") return polarity_output_gnuplot_data;
    if(input_file_identifier=="polarity_output_gnuplot_plotter") return polarity_output_gnuplot_plotter;
    if(input_file_identifier=="observation_result") return observables_output_data;
    if(input_file_identifier=="cell_track") return track_output_data;
    };
  if(input_data_identifier=="filename") return file_name;
  if(input_data_identifier=="function") return calling_function;
  if(input_data_identifier=="code") return error_code;
  if(input_data_identifier=="text") return main_text;
  if(input_data_identifier=="data") return data_identifier;
  return "undefined identifier";
};
//
std::string io_active_arc_class::get_input()
{
  boost::property_tree::ptree instance_property_tree;
  boost::property_tree::read_xml(file_name.c_str(),instance_property_tree);
  //
  //
  if (boost::optional<std::string> instance_data = instance_property_tree.get_optional<std::string>(data_identifier.c_str())) 
    {
      return instance_data.get();
    }
    else {
      std::string error_string;
      error_string ="error input:";
      error_string+=instance_data.get();
      return error_string;
    }
  //
};
//
void io_active_arc_class::get_input_array(std::vector<std::string> &input_data)
{
  boost::property_tree::ptree instance_property_tree;
  boost::property_tree::read_xml(file_name.c_str(),instance_property_tree);
  //
  BOOST_FOREACH(const boost::property_tree::ptree::value_type& child, instance_property_tree.get_child(data_identifier.c_str()))
    {
      input_data.push_back(boost::lexical_cast<std::string>(child.second.data()));
    }
};
//
void io_active_arc_class::get_input_fixeddim_array(std::vector<std::string> & input_data)
{
  boost::property_tree::ptree instance_property_tree;
  boost::property_tree::read_xml(file_name.c_str(),instance_property_tree);
  //
  long long int index=-1;
  long long int limit=(long long int)input_data.size();
  BOOST_FOREACH(
		const boost::property_tree::ptree::value_type & child, 
		instance_property_tree.get_child(data_identifier.c_str())
		)
    {
      index++;
      if(index<limit)
	{
	  input_data[index]=boost::lexical_cast<std::string>(child.second.data());
	}
      else
	{
	  fprintf(stderr,"warning!: longer input array (io_active_arc_class::get_input_fixeddim_array)");
	  abort();
	};
    };
  if(index+1!=limit)
    {
      fprintf(stderr,"warning!: shorter input array (io_active_arc_class::get_input_fixeddim_array)");
      abort();
    };
};
//
std::string io_active_arc::int_to_string(
					     const int & data
					     )
  const {
  std::string text;
    try {
      text=boost::lexical_cast<std::string>(data);
    } catch (boost::bad_lexical_cast& error){
      std::string error_message;
      error_message = "boost::lexical_cast<std::string> for value from debug data is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc::int_to_string", 
		   "error",
		   error_message    
		   );
      // abort();
    };
    return text;
};
//
std::string io_active_arc::int_to_format_string(
						    const int & data,
						    const std::string & format_list
						    )
const {
  std::string text;
    try {
      text=(boost::format(format_list) % data).str();
    } catch (const boost::io::format_error& error){
      std::string error_message;
      error_message = "boost::format(";
      error_message+= format_list;
      error_message+= ") for value from debug data is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc::int_to_zero_string", 
		   "error",
		   error_message    
		   );
      // abort();
    };
    return text;
};
//
std::string io_active_arc::longint_to_format_string(
							const long int & data,
							const std::string & format_list
							)
const {
  std::string text;
    try {
      text=(boost::format(format_list) % data).str();
    } catch (const boost::io::format_error& error){
      std::string error_message;
      error_message = "boost::format(";
      error_message+= format_list;
      error_message+= ") for value from debug data is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc::int_to_zero_string", 
		   "error",
		   error_message    
		   );
      // abort();
    };
    return text;
};
//
std::string io_active_arc::longint_to_string(
						 const long int & data
						 )
const {
  std::string text;
    try {
      text=boost::lexical_cast<std::string>(data);
    } catch (boost::bad_lexical_cast& error){
      std::string error_message;
      error_message = "boost::lexical_cast<std::string> for value from debug data is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc::longint_to_string", 
		   "error",
		   error_message    
		   );
      // abort();
    };
    return text;
};
//
std::string io_active_arc::longlongint_to_string(
						     const long long int & data
						     )
const {
  std::string text;
    try {
      text=boost::lexical_cast<std::string>(data);
    } catch (boost::bad_lexical_cast& error){
      std::string error_message;
      error_message = "boost::lexical_cast<std::string> for value from debug data is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc::longint_to_string", 
		   "error",
		   error_message    
		   );
      // abort();
    };
    return text;
};
//
std::string io_active_arc::double_to_string(
						const double & data
						)
const {
  std::string text;
    try {
      text=boost::lexical_cast<std::string>(data);
    } catch (boost::bad_lexical_cast& error){
      std::string error_message;
      error_message = "boost::lexical_cast<int> for value from debug data is failed/ code:";
      error_message += error.what();
      debug_output(
		   "debug" ,
		   "io_active_arc::double_to_string", 
		   "error",
		   error_message    
		   );
      // abort();
    };
    return text;
};
//
std::string io_active_arc::longintarray_to_string(
						      const std::vector<long int> & data
						      )
  const {
  std::string return_message;
  std::vector<long int>::const_iterator iterator=data.begin();
  return_message ="(";
  while(iterator!=data.end())
    {
      return_message+=longint_to_string(*iterator);
      iterator++;
      if(iterator!=data.end()) return_message+=",";
    }
  return_message+=")";
  return return_message;
};
//
std::string io_active_arc::doublearray_to_string(
						     const std::vector<double> & data
						     )
  const {
  std::string return_message;
  std::vector<double>::const_iterator iterator=data.begin();
  return_message ="(";
  while(iterator!=data.end())
    {
      return_message+=double_to_string(*iterator);
      iterator++;
      if(iterator!=data.end()) return_message+=",";
    }
  return_message+=")";
  return return_message;
};
//
std::string io_active_arc::generate_structure(
					      const std::string & name,
					      const int & index,
					      const std::string & child
					      )
{
  std::string structure;
  structure = io_active_arc::generate_base_structure(
						     name,
						     index
						     );
  structure+=".";
  structure+=child;
  return structure;
};
//
std::string io_active_arc::generate_base_structure(
						   const std::string & name,
						   const int & index
						   )
{
  std::string base_structure;
  std::string number;
  base_structure = name;
  base_structure+= "[";
	try {
	  number=boost::lexical_cast<std::string>(index);
	} catch (boost::bad_lexical_cast& error){
	  std::string error_message;
	  error_message = "boost::lexical_cast<int> for value from a number is failed/ code:";
	  error_message += error.what();
	  debug_output(
		       "debug" ,
		       "io_active_arc_class::generate_base_name", 
		       "error",
		       error_message    
		       );
	  abort();
	};
	base_structure+= number;
	base_structure+= "]";
	return base_structure;
};
//
std::string io_active_arc::generate_structure_longint(
							  const std::string & name,
							  const long int & index,
							  const std::string & child
							  )
{
  std::string structure;
  structure = io_active_arc::generate_base_structure(
							 name,
							 index
							 );
  structure+=".";
  structure+=child;
  return structure;
};
//
std::string io_active_arc::generate_base_structure_longint(
							       const std::string & name,
							       const long int & index
							       )
{
  std::string base_structure;
  std::string number;
  base_structure = name;
  base_structure+= "[";
	try {
	  number=boost::lexical_cast<std::string>(index);
	} catch (boost::bad_lexical_cast& error){
	  std::string error_message;
	  error_message = "boost::lexical_cast<int> for value from a number is failed/ code:";
	  error_message += error.what();
	  debug_output(
		       "debug" ,
		       "io_active_arc_class::generate_base_name", 
		       "error",
		       error_message    
		       );
	  abort();
	};
	base_structure+= number;
	base_structure+= "]";
	return base_structure;
};
//
//
std::string io_active_arc::generate_structure_longlongint(
							      const std::string & name,
							      const long long int & index,
							      const std::string & child
							      )
{
  std::string structure;
  structure = io_active_arc::generate_base_structure(
							 name,
							 index
							 );
  structure+=".";
  structure+=child;
  return structure;
};
//
std::string io_active_arc::generate_base_structure_longlongint(
								   const std::string & name,
								   const long long int & index
								   )
{
  std::string base_structure;
  std::string number;
  base_structure = name;
  base_structure+= "[";
	try {
	  number=boost::lexical_cast<std::string>(index);
	} catch (boost::bad_lexical_cast& error){
	  std::string error_message;
	  error_message = "boost::lexical_cast<int> for value from a number is failed/ code:";
	  error_message += error.what();
	  debug_output(
		       "debug" ,
		       "io_active_arc_class::generate_base_name", 
		       "error",
		       error_message    
		       );
	  abort();
	};
	base_structure+= number;
	base_structure+= "]";
	return base_structure;
};
//
//
std::string io_active_arc::get_numbering_file(
						  const std::string & header,
						  const int & number,
						  const std::string & safix
						  ) 
  const {
  std::string return_value;
  return_value = header;
  return_value+= io_active_arc::int_to_string(number);
  return_value+= ".";
  return_value+= safix;
  return return_value;
};						  
//
void io_active_arc::error_output(
				     const std::string & class_name,
				     const std::string & function_name,
				     const std::string & message
				     )
{
  std::string error_message;
  error_message =message;
  error_message+="(";
  error_message+=class_name;
  error_message+="::";
  error_message+=function_name;
  error_message+=")";
  standard_output(error_message);
  std::abort();
}
//
/*=====================
  IO control class methods
=======================*/
std::string io_control_class::get_io_mode(
					  const std::string & file_identifier,
					  const std::string & data_identifier
					  )
{
    std::string return_value("");
    std::string message;
    io_active_arc_class io_instance;
    io_instance.data_set(file_identifier,"file");
    io_instance.data_set(data_identifier,"data");
    return_value = io_instance.get_input();
    if(return_value!="on"&&return_value!="off")
      {
	message = "io flag,";
	message+= file_identifier;
	message+= ", should be set in on or off.\n";
	fprintf(stderr, "%s", message.c_str());
      };
    input_echo_mode = return_value;
    return return_value;
  };
/*=====================
 IO control class Constructor
=======================*/
io_control_class::io_control_class()
{
  input_echo_mode = "off";
};
