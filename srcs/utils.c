#include "lemipc.h"

void	init_lem(t_lem *lem)
{
	lem->shm = NULL;
	lem->main_player = 0;
	lem->nb_player = 0;
	lem->team = 1;
}

void	free_shm(t_shm *shm)
{
	if (shm)
		munmap(shm, sizeof(t_lem));
}

void	*create_shared_memory(size_t size)
{
	int protection;
	int visibility;

	protection = PROT_READ | PROT_WRITE; 
 	visibility = MAP_SHARED | MAP_ANONYMOUS;
	return (mmap(NULL, size, protection, visibility, -1, 0));
}
