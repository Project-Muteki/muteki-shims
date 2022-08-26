/*
 * Copyright 2021-present dogtopus
 * SPDX-License-Identifier: MIT
 */

/**
 * @file loader.h
 * @brief Binary loader API.
 */

#ifndef __MUTEKI_LOADER_H__
#define __MUTEKI_LOADER_H__

#include <muteki/common.h>

/**
 * Defines the default subroutine identifiers to be used with @a ExecuteProgram.
 * @see ExecuteProgram
 */
enum applet_subroutine_e {
    /**
     * Run the main subroutine.
     */
    APPLET_SUBROUTINE_MAIN = 0,
    /**
     * Run a subroutine that clears the persistent states of the applet.
     */
    APPLET_SUBROUTINE_RESET_STATES = 5,
};

/**
 * @brief Applet argument passing V4 struct.
 * Mirrors the parameter set by the parent ExecuteProgram() call.
 * @see ExecuteProgram
 */
typedef struct {
    /** DOS 8.3 path to executable. */
    char *dospath;
    /** Subroutine to invoke. */
    int *subroutine;
    /** User argument 1. */
    void **applet_arg1;
    /** User argument 2. */
    void **applet_arg2;
} applet_args_v4_t;

/**
 * @brief Load an app executable.
 *
 * The @p pathname specified must be a DOS 8.3 name.
 *
 * @param pathname Path to executable.
 * @return A pointer to a structure describing the loaded executable, or NULL if the loading process failed.
 */
extern void *LoadProgramA(const char *pathname);

/**
 * @brief Load an applet executable. (UTF-16 variant)
 *
 * The @p pathname specified must be a DOS 8.3 name.
 *
 * @param pathname UTF-16 encoded path to executable.
 * @see LoadProgramA
 * @return A pointer to a structure describing the loaded executable, or NULL if the loading process failed.
 */
extern void *LoadProgramW(const UTF16 *pathname);

/**
 * @brief Invoke a specific subroutine of the loaded applet with arguments.
 *
 * Once called, the control is fully transferred to the loaded applet and this function will block until the invoked subroutine fully exits.
 *
 * Actual format of arguments seem to be applet-specific.
 *
 * @param prog The loaded executable description returned by LoadProgramA or LoadProgramW.
 * @param subroutine Subroutine to invoke.
 * @param applet_arg1 Argument 1. Can either be a pointer to some data or an integer.
 * @param applet_arg2 Argument 2. Can either be a pointer to some data or an integer.
 * @return The return value of the subroutine
 * @see LoadProgramA
 * @see LoadProgramW
 * 
 */
extern int ExecuteProgram(void *prog, int subroutine, const void *applet_arg1, const void *applet_arg2);

/**
 * @brief Unload a loaded applet.
 *
 * @param prog The loaded executable description returned by LoadProgramA or LoadProgramW.
 * @see LoadProgramA
 * @see LoadProgramW
 */
extern int FreeProgram(void *prog);

/**
 * @brief Get the path to the current running executable (argv[0]).
 *
 * @return The DOS 8.3 path to the current running executable.
 */
extern char *GetCurrentPathA();

/**
 * @brief Get the path to the current running executable (argv[0]). (UTF-16 variant)
 *
 * @return The UTF-16 encoded DOS 8.3 path to the current running executable.
 */
extern UTF16 *GetCurrentPathW();

#endif // __MUTEKI_LOADER_H__
