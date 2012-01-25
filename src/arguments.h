/** @file arguments.h
 *
 *  Defines the procedures related to handling the command-line arguments.
 */


#ifndef ARGUMENTS_H_DEFINED
#define ARGUMENTS_H_DEFINED


void args_handle (int argc, char* argv[]);
void print_help ();
void print_license ();
void print_usage ();
void print_version ();


#endif
