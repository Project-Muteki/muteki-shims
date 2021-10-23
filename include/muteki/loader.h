#ifndef __MUTEKI_LOADER_H__
#define __MUTEKI_LOADER_H__

#include <muteki/common.h>

enum {
    APP_SUBROUTINE_MAIN = 0,
    APP_SUBROUTINE_RESET_STATES = 5,
};

extern void *LoadProgramA(const char *pathname);
extern void *LoadProgramW(const char16_t *pathname);
extern int ExecuteProgram(void *prog, int invoke_subroutine, const void *app_arg1, const void *app_arg2);
extern int FreeProgram(void *prog);
extern char *GetCurrentPathA();
extern char16_t *GetCurrentPathW();

#endif // __MUTEKI_LOADER_H__
