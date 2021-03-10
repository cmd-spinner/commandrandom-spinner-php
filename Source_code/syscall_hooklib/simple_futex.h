#ifdef __cplusplus
extern "C" {
#endif
struct simplefu_semaphore {
  int avail;
  int waiters;
};

typedef struct simplefu_semaphore *simplefu;

void simplefu_down(simplefu who);
void simplefu_up(simplefu who);

struct simplefu_mutex 
{
  struct simplefu_semaphore sema;
};

typedef struct simplefu_mutex *simplemu;



#define simplefu_mutex_init(__smu) {\
		__smu->sema.avail = 1;\
		__smu->sema.waiters = 0;\
	}
	
#define simplefu_mutex_lock(__smu) simplefu_down(&__smu->sema);
#define simplefu_mutex_unlock(__smu) simplefu_up(&__smu->sema);

#define simplefu_enter 		simplefu_mutex_lock
#define simplefu_leave 		simplefu_mutex_unlock


#ifdef __cplusplus
}
#endif