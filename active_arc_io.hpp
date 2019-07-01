#ifndef _IO_ACTIVE_ARC_
#define _IO_ACTIVE_ARC_
#include <string>
#include <vector>

class io_active_arc
{
  //
  /*================================================
     work memory for io_active_arc_class
   =================================================*/
  //
  /*================================================
     operating function for io_active_arc_class
   =================================================*/
public: void debug_output(
			  const std::string & file_identifier ,
			  const std::string & calling_function, 
			  const std::string & error_code      ,
			  const std::string & main_text       
			  ) const;
  //
public: void debug_output_longint(
				  const std::string & file_identifier ,
				  const std::string & calling_function, 
				  const std::string & error_code      ,
				  const long int & data       
				  );
  //
public: void standard_output(
			     const std::string & main_text       
			     );
  //
public: void file_initialize(
			     const std::string & file_identifier 
			     ) ;
  // get functions
public: std::string get_filename(
				 const std::string & file_identifier
				 ) const; 
  //
public: std::string get_input_string(
				     const std::string & file_identifier ,
				     const std::string & data_identifier
				     );
  //
public: int get_input_int(
			  const std::string & file_identifier ,
			  const std::string & data_identifier
			  );
  //
public: long int get_input_longint(
				   const std::string & file_identifier ,
				   const std::string & data_identifier
				   );
  //
public: long long int get_input_longlongint(
					    const std::string & file_identifier ,
					    const std::string & data_identifier
					    );
  //
public: double get_input_double(
				const std::string & file_identifier ,
				const std::string & data_identifier
				);
  // put functions
public: void output_message(
			    const std::string & message,
			    const std::string & file_identifier
			    ) const;
public: std::string int_array_to_string(
					const std::vector<int> & array,
					const std::string & separater
					) const;
public: std::string longint_array_to_string(
					    const std::vector<long int> & array,
					    const std::string & separater
					    ) const;
public: std::string longlongint_array_to_string(
						const std::vector<long long int> & array,
						const std::string & separater
						) const;
public: std::string double_array_to_string(
					   const std::vector<double> & array,
					   const std::string & separater
					   ) const;
  // Array get functions
public: void get_input_string_array(
				    const std::string & file_identifier   ,// input
				    const std::string & data_identifier   ,// input
				    std::vector<std::string> & input_data // output
				    );
public: void get_input_int_array(
				 const std::string & file_identifier   ,// input
				 const std::string & data_identifier   ,// input
				 std::vector<int> & input_data         // output
				 );
  //
public: void get_input_longint_array(
				     const std::string & file_identifier   ,// input
				     const std::string & data_identifier   ,// input
				     std::vector<long int> &input_data    // output
				     );
  //
  //
public: void get_input_longlongint_array(
					 const std::string & file_identifier   ,// input
					 const std::string & data_identifier   ,// input
					 std::vector<long long int> &input_data    // output
					 );
public: void get_input_unsignedlongint_array(
					     const std::string & file_identifier   ,// input
					     const std::string & data_identifier   ,// input
					     std::vector<unsigned long int> & input_data    // output
					     );
public: void get_input_double_array(
				    const std::string & file_identifier   ,// input
				    const std::string & data_identifier   ,// input
				    std::vector<double> & input_data    // output
				    );
  // Type translate for output
public: std::string int_to_string(
				  const int & data
				  )const ;
public: std::string int_to_format_string(
					 const int & data,
					 const std::string & format_list
					 )const ;
public: std::string longint_to_format_string(
					     const long int & data,
					     const std::string & format_list
					     )const ;
public: std::string longint_to_string(
				      const long int & data
				      )const ;
public: std::string longlongint_to_string(
					  const long long int & data
					  )const ;
public: std::string double_to_string(
				     const double & data
				     ) const;
public: std::string longintarray_to_string(
					   const std::vector<long int> & data
					   ) const;
public: std::string doublearray_to_string(
					  const std::vector<double> & data
					  ) const;
  // Make structure for getting structure from a file
public:  std::string generate_base_structure(
					     const std::string & name,
					     const int & index
					     );
  //
public:  std::string generate_structure(
					const std::string & name,
					const int & index,
					const std::string & child
					);
public:  std::string generate_base_structure_longlongint(
							 const std::string & name,
							 const long long int & index
							 );
  //
public:  std::string generate_structure_longlongint(
						    const std::string & name,
						    const long long int & index,
						    const std::string & child
						    );
  //
public:  std::string generate_base_structure_longint(
						     const std::string & name,
						     const long int & index
						     );
  //
public:  std::string generate_structure_longint(
						const std::string & name,
						const long int & index,
						const std::string & child
						);
  //
public: std::string get_numbering_file(
				       const std::string & header,
				       const int & number,
				       const std::string & safix
				       ) const;
  //
public: void error_output(
			  const std::string & class_name,
			  const std::string & function_name,
			  const std::string & message
			  );
};

#endif // #ifndef _IO_ACTIVE_ARC_
