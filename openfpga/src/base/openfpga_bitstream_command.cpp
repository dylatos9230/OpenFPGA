/********************************************************************
 * Add commands to the OpenFPGA shell interface, 
 * in purpose of generate Verilog netlists modeling the full FPGA fabric
 * This is one of the core engine of openfpga, including:
 * - repack : create physical pbs and redo packing 
 *******************************************************************/
#include "openfpga_repack.h"
#include "openfpga_bitstream.h"
#include "openfpga_bitstream_command.h"

/* begin namespace openfpga */
namespace openfpga {

void add_openfpga_bitstream_commands(openfpga::Shell<OpenfpgaContext>& shell) {
  /* Get the unique id of 'build_fabric' command which is to be used in creating the dependency graph */
  const ShellCommandId& shell_cmd_build_fabric_id = shell.command(std::string("build_fabric"));

  /* Add a new class of commands */
  ShellCommandClassId openfpga_bitstream_cmd_class = shell.add_command_class("FPGA-Bitstream");

  /******************************** 
   * Command 'repack' 
   */
  Command shell_cmd_repack("repack");
  /* Add an option '--verbose' */
  shell_cmd_repack.add_option("verbose", false, "Enable verbose output");
  
  /* Add command 'repack' to the Shell */
  ShellCommandId shell_cmd_repack_id = shell.add_command(shell_cmd_repack, "Pack physical programmable logic blocks");
  shell.set_command_class(shell_cmd_repack_id, openfpga_bitstream_cmd_class);
  shell.set_command_execute_function(shell_cmd_repack_id, repack);

  /* The 'repack' command should NOT be executed before 'build_fabric' */
  std::vector<ShellCommandId> cmd_dependency_repack;
  cmd_dependency_repack.push_back(shell_cmd_build_fabric_id);
  shell.set_command_dependency(shell_cmd_repack_id, cmd_dependency_repack);

  /******************************** 
   * Command 'fpga_bitstream' 
   */
  Command shell_cmd_fpga_bitstream("fpga_bitstream");

  /* Add an option '--file' in short '-f'*/
  CommandOptionId fpga_bitstream_opt_file = shell_cmd_fpga_bitstream.add_option("file", true, "file path to output the bitstream database");
  shell_cmd_fpga_bitstream.set_option_short_name(fpga_bitstream_opt_file, "f");
  shell_cmd_fpga_bitstream.set_option_require_value(fpga_bitstream_opt_file, openfpga::OPT_STRING);
  /* Add an option '--verbose' */
  shell_cmd_fpga_bitstream.add_option("verbose", false, "Enable verbose output");
  
  /* Add command 'fpga_bitstream' to the Shell */
  ShellCommandId shell_cmd_fpga_bitstream_id = shell.add_command(shell_cmd_fpga_bitstream, "Build bitstream database");
  shell.set_command_class(shell_cmd_fpga_bitstream_id, openfpga_bitstream_cmd_class);
  shell.set_command_execute_function(shell_cmd_fpga_bitstream_id, fpga_bitstream);

  /* The 'fpga_bitstream' command should NOT be executed before 'repack' */
  std::vector<ShellCommandId> cmd_dependency_fpga_bitstream;
  cmd_dependency_fpga_bitstream.push_back(shell_cmd_repack_id);
  shell.set_command_dependency(shell_cmd_fpga_bitstream_id, cmd_dependency_fpga_bitstream);

  /******************************** 
   * Command 'build_fabric_bitstream' 
   */
  Command shell_cmd_fabric_bitstream("build_fabric_bitstream");
  /* Add an option '--verbose' */
  shell_cmd_fabric_bitstream.add_option("verbose", false, "Enable verbose output");

  /* Add command 'fabric_bitstream' to the Shell */
  ShellCommandId shell_cmd_fabric_bitstream_id = shell.add_command(shell_cmd_fabric_bitstream, "Reorganize the fabric-independent bitstream for the FPGA fabric created by FPGA-Verilog");
  shell.set_command_class(shell_cmd_fabric_bitstream_id, openfpga_bitstream_cmd_class);
  shell.set_command_execute_function(shell_cmd_fabric_bitstream_id, build_fabric_bitstream);

  /* The 'fabric_bitstream' command should NOT be executed before 'fpga_bitstream' */
  std::vector<ShellCommandId> cmd_dependency_fabric_bitstream;
  cmd_dependency_fabric_bitstream.push_back(shell_cmd_fpga_bitstream_id);
  shell.set_command_dependency(shell_cmd_fabric_bitstream_id, cmd_dependency_fabric_bitstream);
} 

} /* end namespace openfpga */