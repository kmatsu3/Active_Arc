#include "active_arc_gnuplot.hpp"
void gnuplot_driver::set_axis(
			      int direction_index,
			      double min,
			      double max,
			      double tics,
			      std::string label
			      )
{
  if(direction_index==0)
    {
      xrange = "set xrange [";
      xrange+= io_method.double_to_string(min);
      xrange+= ":";
      xrange+= io_method.double_to_string(max);
      xrange+= "]";
      //
      xtics = "set xtics ";
      xtics+= io_method.double_to_string(tics);
      //
      xlabel = "set xlabel '";
      xlabel+= label;
      xlabel+= "'";
    };
  if(direction_index==1)
    {
      yrange = "set yrange [";
      yrange+= io_method.double_to_string(min);
      yrange+= ":";
      yrange+= io_method.double_to_string(max);
      yrange+= "]";
      //
      ytics = "set ytics ";
      ytics+= io_method.double_to_string(tics);
      //
      ylabel = "set ylabel '";
      ylabel+= label;
      ylabel+= "'";
    };
  if(direction_index==2)
    {
      zrange = "set zrange [";
      zrange+= io_method.double_to_string(min);
      zrange+= ":";
      zrange+= io_method.double_to_string(max);
      zrange+= "]";
      //
      ztics = "set ztics ";
      ztics+= io_method.double_to_string(tics);
      //
      zlabel = "set zlabel '";
      zlabel+= label;
      zlabel+= "'";
    };
};      
//
void gnuplot_driver::set_form(
			      double ratio_input,
			      double size_input,
			      std::string view_text,
			      std::string pm3d_text
			      )
{
  //
  size = "set size ";
  size+= io_method.double_to_string(size_input);
  //
  if(ratio_input>0)
    {
      ratio = "set size ratio";
      ratio+= io_method.double_to_string(ratio_input);
    }
  else
    {
      ratio = "set size ratio";
      ratio+= io_method.double_to_string(ratio_input);
      ratio+= " 1, 1";
    }
  //
  if(view_text=="map")
    {
      view="set view map";
    }else{
    view ="set ";
    view+=view_text;
    };
  if(pm3d_text=="none")
    {
      view="unset pm3d";
    }else{
    pm3d ="set pm3d ";
    pm3d+=pm3d_text;
    unset_surface="unset surface";
    unset_arrow="unset arrow";
  };
};
//
void gnuplot_driver::make_plot_file(
				    const int & iteration,
				    const std::string & terminal_identifier,
				    const std::string & plot_command,
				    const long int & time_index,
				    const int & sweep_step
				    )
{
  std::string data_file_name;
  std::string plot_file_name;
  std::string figure_file_name;
  plot_file_name = file_header
    +io_method.longint_to_format_string(time_index,"%04d")
    +io_method.longint_to_format_string(sweep_step,"%04d")
    + ".plt";
  //
  io_method.file_initialize(plot_file_name);
  io_method.output_message(xrange,plot_file_name);
  io_method.output_message(yrange,plot_file_name);
  io_method.output_message(zrange,plot_file_name);
  io_method.output_message(xtics,plot_file_name);
  io_method.output_message(ytics,plot_file_name);
  io_method.output_message(ztics,plot_file_name);
  io_method.output_message(xlabel,plot_file_name);
  io_method.output_message(ylabel,plot_file_name);
  io_method.output_message(zlabel,plot_file_name);
  io_method.output_message(size,plot_file_name);
  io_method.output_message(ratio,plot_file_name);
  io_method.output_message(view,plot_file_name);
  io_method.output_message(pm3d,plot_file_name);
  io_method.output_message(unset_surface,plot_file_name);
  io_method.output_message(unset_arrow,plot_file_name);
  io_method.output_message(legend,plot_file_name);
  io_method.output_message(key,plot_file_name);
  //
  terminal = gnuplot_driver::get_terminal(terminal_identifier);
  io_method.output_message(terminal,plot_file_name);
  //
  if(iteration>=0){
    std::string define_data_file;
    std::string define_load_file;
    std::string define_figure_file;
    std::string iteration_start;
    std::string iteration_end;
    //
    define_data_file = "datafile(n) = sprintf('" 
      + file_header 
      +"%03d"
      +"_"
      +io_method.longint_to_format_string(time_index,"%04d")
      +"_"
      +io_method.longint_to_format_string(sweep_step,"%04d")
      +".dat',n)";
    io_method.output_message(define_data_file,plot_file_name);
    if(load!="none")
      {
	define_load_file = "loadfile(n) = sprintf('" 
	  + file_header
	  +"%03d"
	  +"_"
	  +io_method.longint_to_format_string(time_index,"%04d")
	  +"_"
	  +io_method.longint_to_format_string(sweep_step,"%04d")
	  +".lod',n)";
	io_method.output_message(define_load_file,plot_file_name);
      };
    //
    if(terminal_identifier=="eps"||terminal_identifier=="landscape")
      {
	define_figure_file = "figurefile(n) = sprintf('" 
	  + file_header 
	  +"%03d"
	  +"_"
	  +io_method.longint_to_format_string(time_index,"%04d")
	  +"_"
	  +io_method.longint_to_format_string(sweep_step,"%04d")
	  +".eps',n)";
      } else if(terminal_identifier =="png")
      {
	define_figure_file = "figurefile(n) = sprintf('"
	  + file_header 
	  +"%03d"
	  +"_"
	  +io_method.longint_to_format_string(time_index,"%04d")
	  +"_"
	  +io_method.longint_to_format_string(sweep_step,"%04d")
	  +".png',n)";
      };
    io_method.output_message(define_figure_file,plot_file_name);
    // loop start
    iteration_start="do for [n=0:" + io_method.int_to_string(iteration) + "]{";
    iteration_end="}";
    //
    io_method.output_message(iteration_start,plot_file_name);
    output = "set output figurefile(n)";
    io_method.output_message(output,plot_file_name);
    if(load!="none")
      {
	load = "load loadfile(n)";
	io_method.output_message(load,plot_file_name);
      };
    plot = plot_command + " datafile(n) ";
    io_method.output_message(plot,plot_file_name);
    pause = "#pouse -1";
    io_method.output_message(pause,plot_file_name);
    io_method.output_message(iteration_end,plot_file_name);
    // loop end
  } else {
    if(load!="none")
      {
	load = "load '";
	load+= file_header;
	load+= ".lod'";
	io_method.output_message(load,plot_file_name);
      };
    output = "set output '";
    output+= file_header;
    if(terminal_identifier=="eps"||terminal_identifier=="landscape")
      {
	output+= ".eps";
      } else if(terminal_identifier =="png")
      {
	output+= ".png'";
      };
    io_method.output_message(output,plot_file_name);
    plot = plot_command + " '" + file_header + ".dat'";
    io_method.output_message(plot,plot_file_name);
  };
};
//
std::string gnuplot_driver::get_terminal(
					 const std::string & terminal_identifier
					 )
{
  if(terminal_identifier=="eps"||terminal_identifier=="eps enhanced")
    {
      return eps;
    } else if (terminal_identifier=="landscape"||terminal_identifier=="eps enhanced")
    {
      return landscape;
    } else if (terminal_identifier=="png")
    {
      return png;
    } else if (terminal_identifier=="x11"||terminal_identifier=="x"||terminal_identifier=="X"||terminal_identifier=="X11")
    {
      return x_terminal;
    } else 
    {
      io_method.error_output(
			     "gnuplot_driver",
			     "get_terminal",
			     "irrigal input identifier of terminal."
			     );
    };
  return "error";
};
//
void gnuplot_driver::output_vectors_on_loading_file(
						    const std::vector<double> & coordinates,
						    const std::vector<double> & arrows,
						    const long int & number_of_arrows,
						    const std::string & head,
						    const std::string & line_type,
						    const long int & plane_index,
						    const long int & time_index,
						    const int & sweep_step,
						    const int & space_dimension,
						    const double & arrow_size,
						    const std::string & file_initialize_flag
						    )
{
  std::string data_file;
  std::string point_file;
  std::string output_message;
  data_file = file_header 
    + io_method.longint_to_format_string(plane_index,"%03d") 
    + "_"
    + io_method.longint_to_format_string(time_index,"%04d")  
    + "_"
    + io_method.longint_to_format_string(sweep_step,"%04d") 
    + ".lod";
  point_file = file_header 
    + io_method.longint_to_format_string(plane_index,"%03d") 
    + "_"
    + io_method.longint_to_format_string(time_index,"%04d")  
    + "_"
    + io_method.longint_to_format_string(sweep_step,"%04d") 
    + ".dat";
  if(file_initialize_flag=="on")
    {
      io_method.file_initialize(data_file);
      io_method.file_initialize(point_file);
    };
  output_message = "# plane number =";
  output_message+= io_method.longint_to_string(plane_index);
  io_method.output_message(output_message,data_file);
  if(file_initialize_flag=="on")
    {
      output_message = "unset arrow";
      io_method.output_message(output_message,data_file);
    };
  if(head=="on")
    {
      output_message = "set style arrow 1 head front linetype "+ line_type + " linewidth 1";
      io_method.output_message(output_message,data_file);
    } 
  else 
    {
    output_message = "set style arrow 1 nohead linetype " + line_type + " linewidth 1";
    io_method.output_message(output_message,data_file);
    };
  //  std::vector<double>::const_iterator vector_index=arrows.begin();
  //  long int vector_counter=0;
  for(long int vector_index=0;vector_index<number_of_arrows;vector_index++)
  //  while(vector_index!=arrows.end())
    {
      /*
	output coordinates
       */
      output_message = "";
      if(file_initialize_flag=="on")
	{
	  for (
	       int direction_index=0;
	       direction_index<space_dimension;
	       direction_index++
	       )
	    {
	      output_message += io_method.double_to_string(
							   coordinates[vector_index*space_dimension+direction_index]
							   );
	      if(direction_index<space_dimension-1) 
		{
		  output_message += " " ;
		}
	    };
      //
	  output_message += " 0.0 ";
	};
      io_method.output_message(output_message,point_file);
      /*
	output arrows
       */
      output_message = "set arrow from " ;
      for (
	   int direction_index=0;
	   direction_index<space_dimension;
	   direction_index++
	   )
	{
	  //	fprintf(stderr,"%f,%f\n",(*vector_index)[direction_index],coordinates[vector_counter][direction_index]);
	  output_message += io_method.double_to_string(
						       coordinates[vector_index*space_dimension+direction_index]
						       );
	  if(direction_index<space_dimension-1) 
	    {
	      output_message += ", " ;
	    }
	};
      //
      output_message += ", 0.0 to  ";
      //
      for (
	   int direction_index=0;
	   direction_index<space_dimension;
	   direction_index++
	   )
	{
	  output_message 
	    += io_method.double_to_string(
					  arrows[vector_index*space_dimension+direction_index]
					  *arrow_size
					  +coordinates[vector_index*space_dimension+direction_index]);
	  if(direction_index<space_dimension-1) 
	    {
	      output_message += ", " ;
	    }
	};
      output_message += ", 0.0 arrowstyle 1";
      io_method.output_message(output_message,data_file);
      //      vector_index++;
      //      vector_counter++;
    };
};
//
void gnuplot_driver::output_vectors_with_type_on_loading_file(
							      const std::vector<std::vector <double> > & coordinates,
							      const std::vector<std::vector <double> > & arrows,
							      const std::vector<int> & types,
							      const long int & plane_index,
							      const long int & time_index,
							      const int & sweep_step,
							      const int & space_dimension,
							      const double & arrow_size
							      )
{
  std::string data_file;
  std::string output_message;
  int type_max=type_max_init;
  int type_min=type_min_init;
  std::vector<int>::const_iterator types_index=types.begin();
  while(types_index!=types.end())
    {
      if((*types_index)<type_min) type_min=(*types_index);
      if((*types_index)>type_max) type_max=(*types_index);
      types_index++;
    }
  data_file = file_header 
    + io_method.longint_to_format_string(plane_index,"%03d") 
    + "_"
    + io_method.longint_to_format_string(time_index,"%04d")  
    + "_"
    + io_method.longint_to_format_string(sweep_step,"%04d") 
    + ".lod";
  io_method.file_initialize(data_file);
  output_message = "# plane number =";
  output_message+= io_method.longint_to_string(plane_index);
  io_method.output_message(output_message,data_file);
  output_message = "unset arrow";
  io_method.output_message(output_message,data_file);
  int type_index;
  for(type_index=1;type_index<type_max-type_min+2;type_index++)
    {
      output_message = "set style arrow "
	+ io_method.int_to_string(type_index+1) 
	+ " front linetype "
	+ io_method.int_to_string(type_index+2)
	+ " linewidth 1 ";
      io_method.output_message(output_message,data_file);
    };
  std::vector<std::vector<double> >::const_iterator vector_index=arrows.begin();
  int direction_index;
  long int vector_counter=0;
  while(vector_index!=arrows.end())
    {
      output_message = "set arrow from " ;
      for (direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  //	fprintf(stderr,"%f,%f\n",(*vector_index)[direction_index],coordinates[vector_counter][direction_index]);
	  output_message += io_method.double_to_string(
						       coordinates[vector_counter][direction_index]
						       );
	  if(direction_index<space_dimension-1) 
	    {
	      output_message += ", " ;
	    }
	};
      //
      output_message += ", 0.0 to  ";
      //
      for (direction_index=0;direction_index<space_dimension;direction_index++)
	{
	  output_message += io_method.double_to_string((*vector_index)[direction_index]*arrow_size+coordinates[vector_counter][direction_index]);
	  if(direction_index<space_dimension-1) 
	    {
	      output_message += ", " ;
	    }
	};
      output_message += ", 0.0 arrowstyle "
	+ io_method.int_to_string(types[vector_counter]-type_min+1);
      io_method.output_message(output_message,data_file);
      vector_index++;
      vector_counter++;
    };
};
//
void gnuplot_driver::output_pm3d_data(
				      const std::vector<std::vector<long int> > & data,
				      const long int & plane_index,
				      const long int & time_index,
				      const int & sweep_step,
				      const long int & x_dimension,
				      const long int & y_dimension
				      )
 {
  std::string data_file;
  long int x_index;
  long int y_index;
  long int data_max=0;
  std::string output_message;
  //
  for(x_index=0;x_index<x_dimension;x_index++)
    {
      for(y_index=0;y_index<y_dimension;y_index++)
	{
	  if(data_max<data[x_index][y_index])
	    {
	      data_max=data[x_index][y_index];
	    };
	};
    };
  //
  data_file = file_header 
    + io_method.longint_to_format_string(plane_index,"%03d")
    + "_"
    + io_method.longint_to_format_string(time_index,"%04d") 
    +"_"
    +io_method.longint_to_format_string(sweep_step,"%04d")
    + ".dat";
  output_message = "# plane number =";
  output_message+= io_method.longint_to_string(plane_index);
  output_message+= "\n";
  io_method.file_initialize(data_file);
  io_method.output_message(output_message,data_file);
  for(x_index=0;x_index<x_dimension;x_index++)
    {
      for(y_index=0;y_index<y_dimension;y_index++)
	{
	  output_message = io_method.longint_to_string(x_index);
	  output_message+= " ";
	  output_message+= io_method.longint_to_string(y_index);
	  output_message+= " ";
	  if(data[x_index][y_index]==data_max)
	    {
	      output_message+= io_method.longint_to_string(0);
	    }
	  else
	    {
	      output_message+= io_method.longint_to_string(data[x_index][y_index]+1);
	    };
	  io_method.output_message(output_message,data_file);
	  output_message = io_method.longint_to_string(x_index);
	  output_message+= " ";
	  output_message+= io_method.longint_to_string(y_index+1);
	  output_message+= " ";
	  if(data[x_index][y_index]==data_max)
	    {
	      output_message+= io_method.longint_to_string(0);
	    }
	  else
	    {
	      output_message+= io_method.longint_to_string(data[x_index][y_index]+1);
	    };
	  io_method.output_message(output_message,data_file);
	};
      io_method.output_message("",data_file);
      for(y_index=0;y_index<y_dimension;y_index++)
	{
	  output_message = io_method.longint_to_string(x_index+1);
	  output_message+= " ";
	  output_message+= io_method.longint_to_string(y_index);
	  output_message+= " ";
	  if(data[x_index][y_index]==data_max)
	    {
	      output_message+= io_method.longint_to_string(0);
	    }
	  else
	    {
	      output_message+= io_method.longint_to_string(data[x_index][y_index]+1);
	    };
	  io_method.output_message(output_message,data_file);
	  output_message = io_method.longint_to_string(x_index+1);
	  output_message+= " ";
	  output_message+= io_method.longint_to_string(y_index+1);
	  output_message+= " ";
	  if(data[x_index][y_index]==data_max)
	    {
	      output_message+= io_method.longint_to_string(0);
	    }
	  else
	    {
	      output_message+= io_method.longint_to_string(data[x_index][y_index]+1);
	    };
	  io_method.output_message(output_message,data_file);
	};
      io_method.output_message("",data_file);
    };
  io_method.output_message("\n",data_file);
};
/*=====================================
    Constructer
   =====================================*/
gnuplot_driver::gnuplot_driver(std::string input_file_header)
  : type_max_init(0),
    type_min_init(10000)
{
  gnuplot_driver::set_axis(
			   0,
			   0.0,
			   1.0,
			   0.5,
			   "x"
			   );
  gnuplot_driver::set_axis(
			   1,
			   0.0,
			   1.0,
			   0.5,
			   "y"
			   );
  gnuplot_driver::set_axis(
			   2,
			   0.0,
			   1.0,
			   0.5,
			   "z"
			   );
  gnuplot_driver::set_form(
			   -1.0,
			   0.6,
			   "map",
			   "at b"
			   );
  file_header=input_file_header;
  eps="set term post eps enhanced 'Times-New-Roman' color 20";
  landscape="set term post eps landscape 'Times-New-Roman' color 20";
  png="set term png";
  x_terminal="set term X11";
  legend="unset colorbox";
  key="unset key";
};
