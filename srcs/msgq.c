#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "lemipc.h"

int	join_msgq(t_lem *lem)
{
    int key;
  
    //key = ftok(MSGQ_NAME, 65);
    if ((key = ftok("/tmp", 65)) == -1)
	{
		perror("ftok");
		exit(1);
	}
	lem->msgqid = msgget(key, 0);
    if (errno == ENOENT)
	{
    	lem->msgqid = msgget(key, 0644 | IPC_CREAT);
	}
	if (lem->msgqid == -1)
	{
		perror("msgget");
		exit(3);
	}
	return (lem->msgqid);
}

int	free_msgq(t_lem *lem)
{
	if (lem->msgqid > 0)
		return (msgctl(lem->msgqid, IPC_RMID, NULL));
	return (0);
}

int	send_die_msg(t_lem *lem)
{
	t_msgq	msgq;

	msgq.mesg_type = 2;
	return (msgsnd(lem->msgqid, &msgq, sizeof(msgq.mes), 0));
}

int	send_turn_msg(t_lem *lem, int id)
{
	t_msgq	msgq;

	msgq.mesg_type = 1;
	if (id > lem->nb_player)
		msgq.mes.nb = 1;
	else
		msgq.mes.nb = id;
	ft_printf("next id = %d\n", msgq.mes.nb);
	return (msgsnd(lem->msgqid, &msgq, sizeof(msgq.mes), 0));
}

int		receive_message(t_lem *lem, t_msgq *msgq)
{
	if (msgrcv(lem->msgqid, msgq, sizeof(msgq->mes), 0, 0) == -1)
		return (1);
	return (0);
}
