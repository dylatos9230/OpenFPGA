/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - generate_fabric_verilog : generate Verilog netlists about FPGA fabric 
 * - generate_fabric_verilog_testbench : TODO: generate Verilog testbenches 
 *******************************************************************/
#include "openfpga_verilog.h"
#include "openfpga_verilog_command.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * - Add a command to Shell environment: generate fabric Verilog 
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
void add_openfpga_write_fabric_verilog_command(openfpga::Shell<OpenfpgaContext>& shell,
                                               const ShellCommandClassId& cmd_class_id,
                                               const ShellCommandId& shell_cmd_build_fabric_id) {
  Command shell_cmd("write_fabric_verilog");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "Specify the output directory for Verilog netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* Add an option '--explicit_port_mapping' */
  shell_cmd.add_option("explicit_port_mapping", false, "Use explicit port mapping in Verilog netlists");

  /* Add an option '--include_timing' */
  shell_cmd.add_option("include_timing", false, "Enable timing annotation in Verilog netlists");

  /* Add an option '--include_signal_init' */
  shell_cmd.add_option("include_signal_init", false, "Initialize all the signals in Verilog netlists");

  /* Add an option '--support_icarus_simulator' */
  shell_cmd.add_option("support_icarus_simulator", false, "Fine-tune Verilog netlists to support icarus simulator");

  /* Add an option '--print_user_defined_template' */
  shell_cmd.add_option("print_user_defined_template", false, "Generate a template Verilog files for user-defined circuit models");

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");
  
  /* Add command 'write_fabric_verilog' to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate Verilog netlists modeling full FPGA fabric");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_fabric_verilog);

  /* The 'build_fabric' command should NOT be executed before 'link_openfpga_arch' */
  std::vector<ShellCommandId> cmd_dependency;
  cmd_dependency.push_back(shell_cmd_build_fabric_id);
  shell.set_command_dependency(shell_cmd_id, cmd_dependency);
}

/********************************************************************
 * - Add a command to Shell environment: write Verilog testbench
 * - Add associated options 
 * - Add command dependency
 *******************************************************************/
static 
void add_openfpga_write_verilog_testbench_command(openfpga::Shell<OpenfpgaContext>& shell,
                                                  const ShellCommandClassId& cmd_class_id,
                                                  const ShellCommandId& shell_cmd_build_fabric_id) {
  Command shell_cmd("write_verilog_testbench");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId output_opt = shell_cmd.add_option("file", true, "Specify the output directory for Verilog netlists");
  shell_cmd.set_option_short_name(output_opt, "f");
  shell_cmd.set_option_require_value(output_opt, openfpga::OPT_STRING);

  /* Add an option '--reference_benchmark_file_path'*/
  CommandOptionId ref_bm_opt = shell_cmd.add_option("reference_benchmark_file_path", true, "Specify the file path to the reference Verilog netlist");
  shell_cmd.set_option_require_value(ref_bm_opt, openfpga::OPT_STRING);

  /* Add an option '--print_top_testbench' */
  shell_cmd.add_option("print_top_testbench", false, "Generate a full testbench for top-level fabric module with autocheck capability");

  /* Add an option '--print_formal_verification_top_netlist' */
  shell_cmd.add_option("print_formal_verification_top_netlist", false, "Generate a top-level module which can be used in formal verification");

  /* Add an option '--print_preconfig_top_testbench' */
  shell_cmd.add_option("print_preconfig_top_testbench", false, "Generate a pre-configured testbench for top-level fabric module with autocheck capability");

  /* Add an option '--print_simulation_ini' */
  CommandOptionId sim_ini_opt = shell_cmd.add_option("print_simulation_ini", false, "Generate a .ini file as an exchangeable file to enable HDL simulations");
  shell_cmd.set_option_require_value(sim_ini_opt, openfpga::OPT_STRING);

  /* Add an option '--verbose' */
  shell_cmd.add_option("verbose", false, "Enable verbose output");
  
  /* Add command to the Shell */
  ShellCommandId shell_cmd_id = shell.add_command(shell_cmd, "generate Verilog testbenches for full FPGA fabric");
  shell.set_command_class(shell_cmd_id, cmd_class_id);
  shell.set_command_execute_function(shell_cmd_id, write_verilog_testbench);

  /* The command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency;
  cmd_dependency.push_back(shell_cmd_build_fabric_id);
  shell.set_command_dependency(shell_cmd_id, cmd_dependency);
}

void add_openfpga_verilog_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating the dependency graph */
  const ShellCommandId& shell_cmd_build_fabric_id = shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_verilog_cmd_class = shell.add_command_class("FPGA-Verilog");

  /******************************** 
   * Command 'write_fabric_verilog' 
   */
  add_openfpga_write_fabric_verilog_command(shell,
                                            openfpga_verilog_cmd_class,
                                            shell_cmd_build_fabric_id);

  /******************************** 
   * Command 'write_verilog_testbench' 
   */
  add_openfpga_write_verilog_testbench_command(shell,
                                               openfpga_verilog_cmd_class,
                                               shell_cmd_build_fabric_id);
} 

} /* end namespace openfpga */
