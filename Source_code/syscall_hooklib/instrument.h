

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INSTRUMENT_H_FUNCPTR_DEF
#define INSTRUMENT_H_FUNCPTR_DEF 1
	
typedef void (*FP_SYNC_LOOPTAIL)(void);
typedef void (*FP_SYNC_RECURSIVETAIL)(void);
typedef void (*FP_ONPREINDIRECTCALL)(void);
typedef void (*FP_ONPOSTINDIRECTCALL)(void);
typedef void (*FP_ONPRERETURN)(void);
typedef void (*FP_DEBUGCALLBACK)(void);


typedef void (*FP_GETYX)(void* win, int *y, int *x);
typedef int (*FP_WATTR_GET)(void* win, unsigned long* attrs, short *pair, void* opts);
//    (void) wattr_get(win, &attrs, &pair, NULL);


#endif // INSTRUMENT_H_FUNCPTR_DEF

#if INSTRUMENT_H_DEFINE_VARIABLES

int* 							__INSTR__counter_master = 0;
int* 							__INSTR__counter_slave  = 0;

int* 							__INSTR__counter_ret_master = 0;
int* 							__INSTR__counter_ret_slave  = 0;


#ifndef INSTRUMENT_HOOKLIB
int  							__INSTR__exec_index = 0;
int* 							__INSTR__counter_ptr = 0;
int* 							__INSTR__counter_ret_ptr = 0;

// for app
FP_SYNC_LOOPTAIL 				__INSTR__sync_looptail = 0;
FP_SYNC_LOOPTAIL 				__INSTR__sync_looptail_ret = 0;
FP_SYNC_RECURSIVETAIL 			__INSTR__sync_recursivetail = 0;

FP_ONPREINDIRECTCALL			__INSTR__onpre_indirectcall = 0;
FP_ONPOSTINDIRECTCALL			__INSTR__onpost_indirectcall = 0;

FP_ONPRERETURN					__INSTR__onpre_return = 0;

FP_DEBUGCALLBACK				__INSTR__debugcallback = 0;

FP_GETYX						__HOOK__getyx = 0;
FP_WATTR_GET					__HOOK__wattr_get = 0;


#else

#if ONLY_HOOK_FUNCTIONS
int  							__INSTR__exec_index = 0;
int* 							__INSTR__counter_ptr = 0;
int* 							__INSTR__counter_ret_ptr = 0;

FP_SYNC_LOOPTAIL 				__INSTR__sync_looptail = 0;
FP_SYNC_LOOPTAIL 				__INSTR__sync_looptail_ret = 0;
FP_SYNC_RECURSIVETAIL 			__INSTR__sync_recursivetail = 0;

FP_ONPREINDIRECTCALL			__INSTR__onpre_indirectcall = 0;
FP_ONPOSTINDIRECTCALL			__INSTR__onpost_indirectcall = 0;

FP_ONPRERETURN					__INSTR__onpre_return = 0;

FP_DEBUGCALLBACK				__INSTR__debugcallback = 0;

FP_GETYX						__HOOK__getyx = 0;
FP_WATTR_GET					__HOOK__wattr_get = 0;


#else


#if INSTRUMENT_DONOT_IMPORT_FUNCVARS
int  					__INSTR__exec_index;
int* 					__INSTR__counter_ptr;
int* 					__INSTR__counter_ret_ptr;

FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail;
FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail_ret;
FP_SYNC_RECURSIVETAIL 	__INSTR__sync_recursivetail;

FP_ONPREINDIRECTCALL	__INSTR__onpre_indirectcall;
FP_ONPOSTINDIRECTCALL	__INSTR__onpost_indirectcall;

FP_ONPRERETURN			__INSTR__onpre_return;

FP_DEBUGCALLBACK		__INSTR__debugcallback;

FP_GETYX				__HOOK__getyx;
FP_WATTR_GET			__HOOK__wattr_get;
#else
extern 	int  					__INSTR__exec_index;
extern  int* 					__INSTR__counter_ptr;
extern  int* 					__INSTR__counter_ret_ptr;

extern 	FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail;
extern 	FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail_ret;
extern 	FP_SYNC_RECURSIVETAIL 	__INSTR__sync_recursivetail;

extern 	FP_ONPREINDIRECTCALL	__INSTR__onpre_indirectcall;
extern 	FP_ONPOSTINDIRECTCALL	__INSTR__onpost_indirectcall;

extern FP_ONPRERETURN			__INSTR__onpre_return;

extern 	FP_DEBUGCALLBACK		__INSTR__debugcallback;

extern  FP_GETYX				__HOOK__getyx;
extern  FP_WATTR_GET			__HOOK__wattr_get;
#endif

#endif // ONLY_HOOK_FUNCTIONS

#endif  // INSTRUMENT_HOOKLIB

#else

#if INSTRUMENT_DONOT_IMPORT_FUNCVARS
int  					__INSTR__exec_index;
int* 					__INSTR__counter_ptr;
int* 					__INSTR__counter_ret_ptr;

FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail;
FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail_ret;
FP_SYNC_RECURSIVETAIL 	__INSTR__sync_recursivetail;

FP_ONPREINDIRECTCALL	__INSTR__onpre_indirectcall;
FP_ONPOSTINDIRECTCALL	__INSTR__onpost_indirectcall;

FP_ONPRERETURN			__INSTR__onpre_return;

FP_DEBUGCALLBACK		__INSTR__debugcallback;

FP_GETYX				__HOOK__getyx;
FP_WATTR_GET			__HOOK__wattr_get;
#else
extern 	int  					__INSTR__exec_index;
extern  int* 					__INSTR__counter_ptr;
extern  int* 					__INSTR__counter_ret_ptr;

extern 	FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail;
extern 	FP_SYNC_LOOPTAIL 		__INSTR__sync_looptail_ret;
extern 	FP_SYNC_RECURSIVETAIL 	__INSTR__sync_recursivetail;

extern 	FP_ONPREINDIRECTCALL	__INSTR__onpre_indirectcall;
extern 	FP_ONPOSTINDIRECTCALL	__INSTR__onpost_indirectcall;

extern 	FP_ONPRERETURN			__INSTR__onpre_return;

extern 	FP_DEBUGCALLBACK		__INSTR__debugcallback;

extern  FP_GETYX				__HOOK__getyx;
extern  FP_WATTR_GET			__HOOK__wattr_get;
#endif

#endif // DEFINE_VARIABLES

#ifdef __cplusplus
}
#endif




