/*
 Copyright 2019 Alain Dargelas

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * File:   ErrorDefinition.cpp
 * Author: alain
 *
 * Created on March 5, 2017, 11:25 PM
 */
#include "Surelog/ErrorReporting/ErrorDefinition.h"

#include <cstdint>
#include <string>
#include <string_view>

#include "Surelog/Utils/NumUtils.h"
#include "Surelog/Utils/StringUtils.h"

namespace SURELOG {

ErrorDefinition::ErrorMap* ErrorDefinition::mutableGlobalErrorInfoMap() {
  static ErrorMap error_info_map;
  return &error_info_map;
}

void ErrorDefinition::rec(ErrorType type, ErrorSeverity severity,
                          ErrorCategory category, std::string_view text,
                          std::string_view extraText) {
  mutableGlobalErrorInfoMap()->emplace(
      type, ErrorInfo(severity, category, text, extraText));
}

void ErrorDefinition::setSeverity(ErrorDefinition::ErrorType type,
                                  ErrorDefinition::ErrorSeverity severity) {
  ErrorMap::iterator found = mutableGlobalErrorInfoMap()->find(type);
  if (found != mutableGlobalErrorInfoMap()->end()) {
    found->second.m_severity = severity;
  }
}

ErrorDefinition::ErrorType ErrorDefinition::getErrorType(
    std::string_view errorId) {
  // TODO: this needs to take std::string_view
  errorId = StringUtils::rtrim_until(errorId, ']');
  errorId = StringUtils::ltrim_until(errorId, '[');
  errorId.remove_prefix(8);
  uint32_t id = 0;
  return NumUtils::parseUint32(errorId, &id) ? (ErrorDefinition::ErrorType)id
                                             : NO_ERROR_MESSAGE;
}

ErrorDefinition::ErrorSeverity ErrorDefinition::getErrorSeverity(
    const std::string_view errorSeverity) {
  if (errorSeverity == "FATAL")
    return FATAL;
  else if (errorSeverity == "ERROR")
    return ERROR;
  else if (errorSeverity == "WARNI" || errorSeverity == "WARNING")
    return WARNING;
  else if (errorSeverity == "INFO" || errorSeverity == "INFO ")
    return INFO;
  else if (errorSeverity == "NOTE" || errorSeverity == "NOTE ")
    return NOTE;
  else if (errorSeverity == "SYNTX" || errorSeverity == "SYNTAX")
    return NOTE;
  else
    return FATAL;
}

std::string ErrorDefinition::getCategoryName(
    ErrorDefinition::ErrorCategory category) {
  std::string cat;
  switch (category) {
    case ErrorDefinition::CMD:
      cat = "CM";
      break;
    case ErrorDefinition::PP:
      cat = "PP";
      break;
    case ErrorDefinition::PARSE:
      cat = "PA";
      break;
    case ErrorDefinition::PYTH:
      cat = "PY";
      break;
    case ErrorDefinition::LANG:
      cat = "LA";
      break;
    case ErrorDefinition::SEMA:
      cat = "SM";
      break;
    case ErrorDefinition::COMP:
      cat = "CP";
      break;
    case ErrorDefinition::ELAB:
      cat = "EL";
      break;
    case ErrorDefinition::LIB:
      cat = "LIB";
      break;
    case ErrorDefinition::LINT:
      cat = "LN";
      break;
    case ErrorDefinition::USER:
      cat = "US";
      break;
    case ErrorDefinition::UHDM:
      cat = "UH";
      break;
  }
  return cat;
}

ErrorDefinition::ErrorCategory ErrorDefinition::getCategory(
    std::string_view cat) {
  if (cat == "CM")
    return ErrorDefinition::CMD;
  else if (cat == "PP")
    return ErrorDefinition::PP;
  else if (cat == "PA")
    return ErrorDefinition::PARSE;
  else if (cat == "PY")
    return ErrorDefinition::PYTH;
  else if (cat == "LA")
    return ErrorDefinition::LANG;
  else if (cat == "SM")
    return ErrorDefinition::SEMA;
  else if (cat == "CP")
    return ErrorDefinition::COMP;
  else if (cat == "EL")
    return ErrorDefinition::ELAB;
  else if (cat == "LIB")
    return ErrorDefinition::LIB;
  else if (cat == "LN")
    return ErrorDefinition::LINT;
  else if (cat == "US")
    return ErrorDefinition::USER;
  else if (cat == "UH")
    return ErrorDefinition::UHDM;
  return ErrorDefinition::USER;
}

bool ErrorDefinition::init() {
  rec(CMD_FILE_DOES_NOT_EXIST, FATAL, CMD, "File \"%s\" does not exist");
  rec(CMD_CANNOT_OPEN_FILE_FOR_READ, FATAL, CMD,
      "Cannot open file \"%s\" for read operation");
  rec(CMD_CANNOT_OPEN_FILE_FOR_WRITE, FATAL, CMD,
      "Cannot open file \"%s\" for write operation");
  rec(CMD_DASH_F_FILE_DOES_NOT_EXIST, FATAL, CMD,
      "Command file (-f) \"%s\" does not exist");
  rec(CMD_INCLUDE_PATH_DOES_NOT_EXIST, WARNING, CMD,
      "Include path \"%s\" does not exist");
  rec(CMD_LIBRARY_FILE_DOES_NOT_EXIST, FATAL, CMD,
      "Library file \"%s\" does not exist");
  rec(CMD_LIBRARY_PATH_DOES_NOT_EXIST, WARNING, CMD,
      "Library path \"%s\" does not exist");
  rec(CMD_VERILOG_FILE_DOES_NOT_EXIST, FATAL, CMD,
      "Verilog File \"%s\" does not exist");
  rec(CMD_PLUS_ARG_IGNORED, NOTE, CMD, "Command line argument \"%s\" ignored");
  rec(CMD_MINUS_ARG_IGNORED, WARNING, CMD,
      "Command line argument \"%s\" ignored");
  rec(CMD_DEBUG_MISSING_LEVEL, ERROR, CMD,
      "Option -d is missing the debug level <level>");
  rec(CMD_DEBUG_INCORRECT_LEVEL, WARNING, CMD,
      "Option -d received incorrect level: \"%s\", level should be 0-3");
  rec(CMD_LIBRARY_FILE_MISSING_FILE, ERROR, CMD,
      "Library File option \"%s\" is missing the file name");
  rec(CMD_LIBRARY_PATH_MISSING_PATH, ERROR, CMD,
      "Library Path option \"%s\" is missing the path name");
  rec(CMD_LOG_FILE_MISSING_FILE, ERROR, CMD,
      "Log file option \"%s\" is missing the path name");
  rec(CMD_PP_FILE_MISSING_FILE, ERROR, CMD,
      "Output file option \"%s\" is missing the path name");
  rec(CMD_MT_MISSING_LEVEL, ERROR, CMD,
      "Option -mt is missing the number of threads <max_threads>");
  rec(CMD_MT_INCORRECT_LEVEL, ERROR, CMD,
      "Option -mt received incorrect level: \"%s\", level should be 0-64");
  rec(CMD_SEPARATE_COMPILATION_UNIT_ON, INFO, CMD,
      "Separate compilation-unit mode is on");
  rec(CMD_PP_FILE_MISSING_ODIR, FATAL, CMD,
      "Missing output directory argument for -odir option");
  rec(CMD_PP_CANNOT_CREATE_OUTPUT_DIR, FATAL, CMD,
      "Cannot create output directory \"%s\"");
  rec(CMD_CREATING_LOG_FILE, INFO, CMD, "Creating log file \"%s\"");
  rec(CMD_NUMBER_THREADS, INFO, CMD, "Executing with %s threads");
  rec(CMD_PP_CANNOT_CREATE_CACHE_DIR, FATAL, CMD,
      "Cannot create cache directory \"%s\"");
  rec(CMD_TIMESCALE_MISSING_SETTING, FATAL, CMD, "Missing timescale setting");
  rec(CMD_SPLIT_FILE_MISSING_SIZE, FATAL, CMD, "Missing file splitting size");
  rec(CMD_UNDEFINED_CONFIG, ERROR, CMD, "Undefined configuration: \"%s\"");
  rec(CMD_USING_GLOBAL_TIMESCALE, INFO, CMD, "Using global timescale: \"%s\"");
  rec(CMD_CACHE_CAPACITY_EXCEEDED, WARNING, CMD,
      "Cache capacity exceeded, turning off cache");
  rec(CMD_WD_MISSING_DIR, WARNING, CMD,
      "Working directory option \"%s\" is missing directory");
  rec(CMD_CD_MISSING_DIR, WARNING, CMD,
      "Current directory option \"%s\" is missing directory");
  rec(CMD_REMAP_MISSING_DIRS, WARNING, CMD,
      "Remapping option \"%s\" expects two absolute directory entries");
  rec(PP_CANNOT_OPEN_FILE, ERROR, PP, "Cannot open file \"%s\"");
  rec(PP_CANNOT_OPEN_INCLUDE_FILE, ERROR, PP,
      "Cannot open include file \"%s\"");
  rec(PP_UNKOWN_MACRO, ERROR, PP, "Unknown macro \"%s\"");
  rec(PP_UNDEF_UNKOWN_MACRO, WARNING, PP, "Undefining an unknown macro \"%s\"");
  rec(PP_OPEN_FILE_FOR_WRITE, FATAL, PP,
      "Cannot open file \"%s\" for write operation");
  rec(PP_MULTIPLY_DEFINED_MACRO, NOTE, PP, "Multiply defined macro \"%s\"",
      "%exloc previous definition");
  rec(PP_SYNTAX_ERROR, SYNTAX, PP, "Syntax error: %s", "%exobj");
  rec(PP_TOO_MANY_ARGS_MACRO, ERROR, PP,
      "Too many arguments (%exobj) for macro \"%s\"",
      "%exloc macro definition takes %exobj");
  rec(PP_MACRO_SYNTAX_ERROR, SYNTAX, PP, "Syntax error in macro: %s",
      "%exloc macro instantiation");
  rec(PP_MACRO_NO_DEFAULT_VALUE, ERROR, PP,
      "Macro instantiation omits argument %exobj for \"%s\"",
      "%exloc No default value for argument %exobj in macro definition");
  rec(PP_MACRO_PARENTHESIS_NEEDED, ERROR, PP,
      "Macro instantiation omits parenthesis for \"%s\"",
      "%exloc macro definition has arguments");
  rec(PP_MACRO_NAME_RESERVED, ERROR, PP,
      "Illegally redefining compiler directive \"`%s\" as a macro name");
  rec(PP_MACRO_HAS_SPACE_BEFORE_ARGS, INFO, PP,
      "Illegal space in between macro name \"%s\" and open parenthesis");
  rec(PP_MACRO_UNUSED_ARGUMENT, WARNING, PP, "Unused macro argument \"%s\"");
  rec(PP_MACRO_UNDEFINED_ARGUMENT, WARNING, PP,
      "Undefined macro argument \"%s\"");
  rec(PP_RECURSIVE_MACRO_DEFINITION, ERROR, PP,
      "Recursive macro definition for \"%s\"",
      "%exloc macro used in macro \"%exobj\"");
  rec(PP_UNTERMINATED_STRING, ERROR, PP, "Illegal unterminated string: >>%s<<",
      "%exloc macro instance");
  rec(PP_UNESCAPED_CHARACTER_IN_STRING, WARNING, PP,
      "Illegal un-escaped character '%s' in string");
  rec(PP_UNRECOGNIZED_ESCAPED_SEQUENCE, WARNING, PP,
      "Unknown escaped sequence '%s'");
  rec(PP_INVALID_INCLUDE_FILENAME, ERROR, PP, "Invalid include filename");
  rec(PP_ILLEGAL_DIRECTIVE_IN_DESIGN_ELEMENT, ERROR, PP,
      "Illegal directive in design element \"%s\"", "%exloc macro instance");
  rec(PP_CANNOT_CREATE_DIRECTORY, FATAL, PP, "Cannot create directory \"%s\"");
  rec(PP_PROCESSING_SOURCE_FILE, INFO, PP, "Preprocessing source file \"%s\"");
  rec(PP_PROCESSING_INCLUDE_FILE, INFO, PP,
      "Preprocessing include file \"%s\"");
  rec(PP_ILLEGAL_DIRECTIVE_ELSEIF, ERROR, PP,
      "Illegal directive `elseif, correct directive is `elsif");
  rec(PP_CANNOT_READ_FILE_CONTENT, ERROR, PP,
      "Cannot read the file's content \"%s\". Only UTF-8 is supported");
  rec(PP_NON_ASCII_CONTENT, NOTE, PP,
      "Non ASCII character detected, replaced by space");
  rec(PP_RECURSIVE_INCLUDE_DIRECTIVE, FATAL, PP,
      "Recursive include directive for file \"%s\"");
  rec(PP_ILLEGAL_TICK_LINE_VALUE, ERROR, PP,
      "Illegal type value for `line directive: %s, legal values are 0, 1, 2");
  rec(PA_CANNOT_SPLIT_FILE, WARNING, PARSE, "Cannot split large file \"%s\"");
  rec(PA_PROCESSING_SOURCE_FILE, INFO, PARSE, "Parsing source file \"%s\"");
  rec(PA_CANNOT_OPEN_FILE, FATAL, PARSE, "Cannot open file \"%s\"");
  rec(PA_UNKOWN_MACRO, ERROR, PARSE, "Unknown macro \"%s\"");
  rec(PA_MAX_LENGTH_IDENTIFIER, ERROR, PARSE,
      "Indentifier exceeds max length \"%s\"");
  rec(PA_NOTIMESCALE_INFO, WARNING, PARSE, "No timescale set for \"%s\"");
  rec(PA_MISSING_TIMEUNIT, WARNING, PARSE,
      "Missing timeunit/timeprecision for \"%s\"");
  rec(PA_SYNTAX_ERROR, SYNTAX, PARSE, "Syntax error: %s", "%exobj");
  rec(PA_RESERVED_KEYWORD, ERROR, PARSE, "Reserved keyword: %s");
  rec(PA_UNSUPPORTED_KEYWORD_LIST, ERROR, PARSE, "Unsupported keyword set: %s");
  rec(PA_UNCONNECTED_DRIVE_VALUE, ERROR, PARSE,
      "Illegal unconnected_drive value: %s");
  rec(PA_TIMESCALE_INVALID_VALUE, ERROR, PARSE,
      "Invalid timescale value: %s, legal values: 1, 10, 100");
  rec(PA_TIMESCALE_INVALID_SCALE, ERROR, PARSE,
      "Timescale precision less precise than timeunit");
  rec(PA_INTERNAL_ERROR, ERROR, PARSE, "Internal error: %s");
  rec(PA_INTERNAL_WARNING, WARNING, PARSE, "Internal warning: %s");
  rec(COMP_COMPILE, INFO, COMP, "Compilation..");
  rec(COMP_COMPILE_PACKAGE, INFO, COMP, "Compile package \"%s\"");
  rec(COMP_COMPILE_CLASS, INFO, COMP, "Compile class \"%s\"");
  rec(COMP_COMPILE_MODULE, INFO, COMP, "Compile module \"%s\"");
  rec(COMP_COMPILE_UDP, INFO, COMP, "Compile udp \"%s\"");
  rec(COMP_COMPILE_PROGRAM, INFO, COMP, "Compile program \"%s\"");
  rec(COMP_COMPILE_CHECKER, INFO, COMP, "Compile checker \"%s\"");
  rec(COMP_COMPILE_INTERFACE, INFO, COMP, "Compile interface \"%s\"");
  rec(COMP_UNDEFINED_INTERFACE, ERROR, COMP, "Undefined interface \"%s\"");
  rec(COMP_PORT_MISSING_TYPE, NOTE, COMP,
      "Implicit port type (wire) for \"%s\"",
      "there are %exobj more instances of this message");
  rec(COMP_PORT_MISSING_DIRECTION, WARNING, COMP,
      "Port \"%s\" definition missing its direction (input, output, inout)",
      "there are %exobj more instances of this message");
  rec(COMP_MODPORT_UNDEFINED_PORT, ERROR, COMP,
      "Undefined net used in modport: \"%s\"");
  rec(COMP_MODPORT_UNDEFINED_CLOCKING_BLOCK, ERROR, COMP,
      "Undefined clocking block used in modport: \"%s\"");
  rec(COMP_NO_MODPORT_IN_GENERATE, ERROR, COMP,
      "Illegal modport in generate statement");
  rec(COMP_PROGRAM_OBSOLETE_USAGE, WARNING, COMP,
      "Using programs is discouraged \"%s\", programs are obsoleted by UVM");
  rec(COMP_UNDEFINED_CLASS, ERROR, COMP, "Undefined class \"%s\"");
  rec(COMP_UNDEFINED_PACKAGE, ERROR, COMP, "Undefined package \"%s\"");
  rec(COMP_UNDEFINED_TYPE, ERROR, COMP, "Undefined type \"%s\"");
  rec(COMP_MULTIPLY_DEFINED_PROPERTY, WARNING, COMP,
      "Multiply defined property \"%s\"", "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_CLASS, WARNING, COMP,
      "Multiply defined class \"%s\"", "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_FUNCTION, WARNING, COMP,
      "Multiply defined function \"%s\"", "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_TASK, WARNING, COMP, "Multiply defined task \"%s\"",
      "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_CONSTRAINT, WARNING, COMP,
      "Multiply defined constraint \"%s\"", "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_TYPEDEF, WARNING, COMP,
      "Multiply defined typedef \"%s\"", "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_INNER_CLASS, WARNING, COMP,
      "Multiply defined inner class \"%s\"", "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_COVERGROUP, WARNING, COMP,
      "Multiply defined covergroup \"%s\"", "%exloc previous definition");
  rec(COMP_MULTIPLY_DEFINED_PARAMETER, WARNING, COMP,
      "Multiply defined parameter \"%s\"", "%exloc previous definition");
  rec(COMP_UNDEFINED_VARIABLE, ERROR, COMP, "Undefined variable \"%s\"");
  rec(COMP_UNDEFINED_BASE_CLASS, ERROR, COMP,
      R"(Undefined base class "%s" extended by "%exobj")");
  rec(COMP_MULTIPLY_DEFINED_PACKAGE, WARNING, COMP,
      "Multiply defined package: \"%s\"", "%exloc previous definition");
  rec(COMP_INCOMPATIBLE_TYPES, ERROR, COMP,
      R"(Incompatible types: "%s" is assigned "%exobj")");
  rec(COMP_MULTIPLY_DEFINED_VARIABLE, WARNING, COMP,
      "Multiply defined variable \"%s\"", "%exloc previous definition");
  rec(COMP_NO_METHOD_FOR_TYPE, ERROR, COMP,
      "Function \"%exobj\" is not defined for variable %s",
      "%exloc type definition");
  rec(COMP_UNDEFINED_SYSTEM_FUNCTION, ERROR, COMP,
      "Undefined system task/function \"$%s\"");
  rec(COMP_UNDEFINED_USER_FUNCTION, ERROR, COMP,
      "Undefined user task/function \"%s\"");
  rec(COMP_MULTIPLY_DEFINED_DESIGN_UNIT, WARNING, COMP,
      "Colliding compilation unit name: \"%s\"", "%exloc previous usage");
  rec(COMP_COMPILE_GENERATE_BLOCK, INFO, COMP, "Compile generate block \"%s\"");
  rec(COMP_INTERNAL_ERROR_OUT_OF_BOUND, ERROR, COMP,
      "Internal out of bound error");
  rec(COMP_CANNOT_REDEFINE_BUILTIN_METHOD, ERROR, COMP,
      "Cannot override builtin method: %s");
  rec(COMP_ILLEGAL_TIMESCALE, ERROR, COMP, "Illegal timescale: %s");
  rec(COMP_UNSPECIFIED_PORT, ERROR, COMP, "Port \"%s\" of unspecified type");
  rec(COMP_UNMATCHED_LABEL, ERROR, COMP, "Unmached label \"%s\"",
      "%exloc closing label \"%exobj\"");
  rec(COMP_SKIPPING_BLACKBOX_MODULE, INFO, COMP,
      "Skipping blackboxed module \"%s\"");
  rec(COMP_SKIPPING_BLACKBOX_INSTANCE, INFO, COMP,
      "Skipping blackboxed instance \"%s\"");
  rec(COMP_ILLEGAL_DEFAULT_PORT_VALUE, ERROR, COMP,
      "Illegal default value for port \"%s\"");
  rec(COMP_UNPACKED_IN_PACKED, ERROR, COMP,
      "Illegal unpacked member in packed struct \"%s\"");
  rec(PY_PROCESSING_SOURCE_FILE, INFO, PYTH, "Processing source file \"%s\"");
  rec(PY_NO_PYTHON_LISTENER_FOUND, FATAL, PYTH,
      "No Python listener found (slSV3_1aPythonListener.py)");
  rec(ELAB_NO_MODULE_DEFINITION, WARNING, ELAB,
      "Cannot find a module definition for \"%s\"");
  rec(ELAB_NO_UDP_DEFINITION, WARNING, ELAB,
      "Cannot find a Udp definition for \"%s\"");
  rec(ELAB_NO_INTERFACE_DEFINITION, WARNING, ELAB,
      "Cannot find an interface definition for \"%s\"");
  rec(ELAB_TOP_LEVEL_MODULE, NOTE, ELAB, "Top level module \"%s\"");
  rec(ELAB_TOP_LEVEL_DOES_NOT_EXIST, FATAL, ELAB,
      "Cmd line top level module does not exist \"%s\"");
  rec(ELAB_TOP_LEVEL_IS_NOT_A_TOP_LEVEL, WARNING, ELAB,
      "Cmd line top level is not a top level \"%s\"");
  rec(ELAB_MULTIPLE_TOP_LEVEL_MODULES, NOTE, ELAB,
      "Multiple top level modules in design");
  rec(ELAB_MULTIPLY_DEFINED_MODULE, WARNING, ELAB,
      "Multiply defined module \"%s\"", "%exloc previous definition");
  rec(ELAB_NO_TOP_LEVEL_MODULE, WARNING, ELAB, "No top level module in design");
  rec(ELAB_INSTANTIATION_LOOP, ERROR, ELAB, "Instantiation loop for \"%s\"",
      "%exloc previous instantiation");
  rec(ELAB_NB_TOP_LEVEL_MODULES, NOTE, ELAB, "Nb Top level modules: %s");
  rec(ELAB_MAX_INSTANCE_DEPTH, NOTE, ELAB, "Max instance depth: %s");
  rec(ELAB_NB_INSTANCES, NOTE, ELAB, "Nb instances: %s");
  rec(ELAB_NB_LEAF_INSTANCES, NOTE, ELAB, "Nb leaf instances: %s");
  rec(ELAB_NB_UNDEF_MODULES, WARNING, ELAB, "Nb undefined modules: %s");
  rec(ELAB_NB_UNDEF_INSTANCES, WARNING, ELAB, "Nb undefined instances: %s");
  rec(ELAB_UNDEF_VARIABLE, ERROR, ELAB, "Undefined variable: %s");
  rec(ELAB_UNMATCHED_DEFPARAM, WARNING, ELAB,
      "Defparam does not match any design object: \"%s\"");
  rec(ELAB_DEFPARAM_OUTSIDE_SCOPE, ERROR, ELAB,
      "Defparam sets an object outside its generate scope: \"%s\"");
  rec(ELAB_MULTI_DEFPARAM_ON_OBJECT, WARNING, ELAB,
      "Multiple defparam on object: \"%s\"", "%exloc previous setting");
  rec(ELAB_UNDEFINED_CONFIG, ERROR, ELAB, "Undefined configuration: \"%s\"");
  rec(ELAB_CONFIGURATION_USED, NOTE, ELAB, "Using configuration \"%s\"");
  rec(ELAB_CONFIGURATION_IGNORED, NOTE, ELAB, "Ignoring configuration \"%s\"");
  rec(ELAB_USE_CLAUSE_IGNORED, WARNING, ELAB,
      "Use clause has no effect, no matching design object found \"%s\"");
  rec(ELAB_SCOPE_PATH, NOTE, ELAB, "Scope \"%s\"");
  rec(ELAB_INSTANCE_PATH, NOTE, ELAB, "Instance \"%s\"");
  rec(ELAB_INTERFACE_INSTANCE_PATH, NOTE, ELAB, "Interface Instance \"%s\"");
  rec(ELAB_PROGRAM_INSTANCE_PATH, NOTE, ELAB, "Program Instance \"%s\"");
  rec(ELAB_ELABORATING_DESIGN, INFO, ELAB, "Design Elaboration..");
  rec(ELAB_ELABORATING_TESTBENCH, INFO, ELAB, "Testbench Elaboration..");
  rec(ELAB_UNDEFINED_PACKAGE, ERROR, ELAB,
      "Undefined imported package: \"%s\"");
  rec(ELAB_OUT_OF_RANGE_PARAM_INDEX, ERROR, ELAB,
      "Out of range parameter index: \"%s\"");
  rec(ELAB_NEGATIVE_VALUE, NOTE, ELAB, "Negative value in instance %s");
  rec(ELAB_DIVIDE_BY_ZERO, ERROR, ELAB, "Division by zero in instance \"%s\"");
  rec(ELAB_ILLEGAL_IMPLICIT_NET, ERROR, ELAB, "Illegal implicit net \"%s\"");
  rec(ELAB_ILLEGAL_ZERO_VALUE, ERROR, ELAB, "Illegal Zero or negative size %s");
  rec(ELAB_EMPTY_PARAM_OVERRIDE, NOTE, ELAB, "Empty parameter override for %s");
  rec(ELAB_USE_INTERFACE_AS_SIGNAL_TYPE, ERROR, ELAB,
      "Illegal use of an interface as a signal type for \"%s\"");
  rec(ELAB_UNKNOWN_STRUCT_MEMBER, ERROR, ELAB, "Unknown struct member \"%s\"",
      "%exloc struct definition for \"%exobj\"");
  rec(ELAB_UNKNOWN_INTERFACE_MEMBER, ERROR, ELAB,
      "Unknown interface member \"%s\"",
      "%exloc interface definition for \"%exobj\"");
  rec(ELAB_UNKNOWN_PARAMETER_OVERRIDE, ERROR, ELAB,
      "Unknown parameter override \"%s\"");
  rec(ELAB_UNKNOWN_PARAMETER_COMMAND, ERROR, ELAB,
      "Unknown parameter in command line override \"%s\"");
  rec(ELAB_EXPRESSION_LOOP, ERROR, ELAB, "Expression loop in instance \"%s\"");
  rec(ELAB_SKIPPING_BLACKBOX_MODULE, INFO, ELAB,
      "Skipping blackboxed module \"%s\"");
  rec(ELAB_SKIPPING_BLACKBOX_INSTANCE, INFO, ELAB,
      "Skipping blackboxed instance \"%s\"");
  rec(ELAB_INVALID_CASE_STMT_VALUE, ERROR, ELAB,
      "Invalid generate case stmt value");
  rec(ELAB_UNKNOWN_PORT, ERROR, ELAB, "Unknown port \"%s\"");
  rec(ELAB_TOP_PARAMETER_NO_DEFAULT, ERROR, ELAB,
      "Top-level parameter with no default value \"%s\"");
  rec(ELAB_SYSTEM_FATAL, FATAL, ELAB, "Fatal elaboration %s");
  rec(ELAB_SYSTEM_ERROR, ERROR, ELAB, "Elaboration error %s");
  rec(ELAB_SYSTEM_WARNING, WARNING, ELAB, "Elaboration warning %s");
  rec(ELAB_SYSTEM_INFO, INFO, ELAB, "Elaboration info %s");
  rec(LIB_FILE_MAPS_TO_MULTIPLE_LIBS, ERROR, LIB,
      R"(File "%exobj" maps to multiple libraries: "%s")");
  rec(UHDM_UNSUPPORTED_EXPR, ERROR, UHDM, "Unsupported expression \"%s\"");
  rec(UHDM_UNSUPPORTED_STMT, ERROR, UHDM, "Unsupported statement \"%s\"");
  rec(UHDM_UNSUPPORTED_SIGNAL, ERROR, UHDM, "Unsupported signal type \"%s\"");
  rec(UHDM_WRONG_OBJECT_TYPE, ERROR, UHDM, "%s");
  rec(UHDM_WRONG_COVERAGE_LINE, ERROR, UHDM,
      "UHDM coverage pointing to empty source line");
  rec(UHDM_UNSUPPORTED_TYPE, ERROR, UHDM, "Unsupported data type \"%s\"");
  rec(UHDM_CREATING_MODEL, INFO, UHDM, "Creating UHDM Model..");
  rec(UHDM_ELABORATION, INFO, UHDM, "Elaborating UHDM..");
  rec(UHDM_WRITE_DB, INFO, UHDM, "Writing UHDM DB: %s ..");
  rec(UHDM_WRITE_HTML_COVERAGE, INFO, UHDM,
      "Writing UHDM Html Coverage: %s ..");
  rec(UHDM_LOAD_DB, INFO, UHDM, "Loading UHDM DB: %s ..");
  rec(UHDM_VISITOR, INFO, UHDM, "Decompiling UHDM..");
  rec(UHDM_UNDEFINED_PATTERN_KEY, ERROR, UHDM, "Undefined pattern key \"%s\"");
  rec(UHDM_UNMATCHED_FIELD_IN_PATTERN_ASSIGN, ERROR, UHDM,
      "Unmatched member during pattern assignment \"%s\"");
  rec(UHDM_REAL_TYPE_AS_SELECT, ERROR, UHDM,
      "Can't use a real type as bit select \"%s\"");
  rec(UHDM_RETURN_VALUE_VOID_FUNCTION, ERROR, UHDM,
      "Void function \"%s\" returns a value");
  rec(UHDM_ILLEGAL_DEFAULT_VALUE, ERROR, UHDM, "Illegal default value");
  rec(UHDM_MULTIPLE_CONT_ASSIGN, ERROR, UHDM,
      "Multiple continuous assignments to: %s", "%exloc other assignment");
  rec(UHDM_ILLEGAL_WIRE_LHS, ERROR, UHDM, "Illegal lhs of type wire \"%s\"");
  rec(UHDM_ILLEGAL_PACKED_DIMENSION, ERROR, UHDM,
      "Illegal dynamic packed dimension \"%s\"");
  rec(UHDM_NON_SYNTHESIZABLE, WARNING, UHDM,
      "Non-synthesizable construct \"%s\"");
  rec(UHDM_ENUM_CONST_SIZE_MISMATCH, ERROR, UHDM,
      "Enum const size does not match base type size \"%s\"",
      "%exloc base type");
  rec(UHDM_DIVIDE_BY_ZERO, ERROR, UHDM, "Division by zero in instance \"%s\"");
  rec(UHDM_INTERNAL_ERROR_OUT_OF_BOUND, ERROR, UHDM,
      "Internal out of bound error");
  rec(UHDM_UNDEFINED_USER_FUNCTION, ERROR, UHDM,
      "Undefined user task/function \"%s\"");
  rec(UHDM_UNRESOLVED_HIER_PATH, ERROR, UHDM,
      "Unresolved hierarchical reference \"%s\"");
  rec(UHDM_UNDEFINED_VARIABLE, ERROR, UHDM, "Undefined variable \"%s\"");
  rec(UHDM_INVALID_CASE_STMT_VALUE, ERROR, UHDM, "Invalid case stmt value");
  rec(UHDM_UNSUPPORTED_TYPESPEC, ERROR, UHDM, "Unsupported typespec \"%s\"");
  rec(UHDM_UNRESOLVED_PROPERTY, ERROR, UHDM, "Unresolved property \"%s\"");
  rec(UHDM_NON_TEMPORAL_SEQUENCE_USE, ERROR, UHDM,
      "Sequence used in non-temporal context \"%s\"");
  rec(UHDM_NON_POSITIVE_VALUE, ERROR, UHDM,
      "Required positive value (>=1), \"%s\" given");
  rec(UHDM_SIGNED_UNSIGNED_PORT_CONN, WARNING, UHDM,
      "Critical: Signed vs Unsigned port connection \"%s\"");
  rec(UHDM_FORCING_UNSIGNED_TYPE, WARNING, UHDM,
      "Critical: Forcing signal to unsigned type due to unsigned port binding "
      "\"%s\"");
  return true;
}

}  // namespace SURELOG
