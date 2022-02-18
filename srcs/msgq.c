#include <sys/select.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "lemipc.h"

int	join_msgq(t_lem *lem)
{
    int key;
  
    key = ftok(MSGQ_NAME, 65);
    if ((lem->msgqid = msgget(key, 0666 | IPC_CREAT)) < 0)
		return (1);
	return (0);
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
	msgq.mes.nb = lem->player_id;
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
	return (msgsnd(lem->msgqid, &msgq, sizeof(msgq.mes), 0));
}

int		receive_message(t_lem *lem, t_msgq *msgq)
{
	fd_set	rset;
	int		ret;

 	FD_ZERO(&rset);
	FD_SET(0, &rset);
	FD_SET(lem->msgqid, &rset);
	ret = select(lem->msgqid + 1, &rset, NULL, NULL, NULL);
	if (ret == -1)
	{
		ft_dprintf(2, "lemipc: select fail (id=%d)\n", lem->player_id);
		return (1);
	}
	if (FD_ISSET(lem->msgqid, &rset))
	{
		msgrcv(lem->msgqid, msgq, sizeof(msgq->mes), 0, 0);
		return (2);
	}	
	return (0);
}
