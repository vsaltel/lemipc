#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "lemipc.h"

int	join_msgq(t_lem *lem)
{
    int key;
  
    if ((key = ftok("/tmp", 65)) == -1)
	{
		perror("ftok");
		exit(1);
	}
	lem->msgqid = msgget(key, 0);
    if (errno == ENOENT)
	{
    	lem->msgqid = msgget(key, 0666 | IPC_CREAT);
		if (lem->msgqid != -1 && lem->v)
			ft_printf("msg queue created\n");
	}
	else if (lem->msgqid != -1 && lem->v)
		ft_printf("msg queue joined\n");
	if (lem->msgqid == -1)
	{
		perror("msgget");
		exit(3);
	}
	return (lem->msgqid);
}

int	free_msgq(t_lem *lem)
{
	return (msgctl(lem->msgqid, IPC_RMID, NULL));
}

int	send_target_msg(t_lem *lem, int y, int x)
{
	t_msgq	msgq;
	int		nb;
	int		ret;

	msgq.mes.x = x;
	msgq.mes.y = y;
	msgq.mes.pid = lem->pid;
	msgq.mesg_type = lem->team;
	nb = check_nb_player_team(lem, lem->team) + 1;
	while (--nb > 0)
		if ((ret = msgsnd(lem->msgqid, &msgq, sizeof(msgq.mes), 0)) == -1)
			break;
	if (ret == -1)
		perror("msgsnd");
	return (ret);
}

int		receive_message(t_lem *lem, t_msgq *msgq)
{
	int	ret;

	ret = msgrcv(lem->msgqid, msgq, sizeof(msgq->mes), 0, IPC_NOWAIT | MSG_NOERROR);
	if (ret == -1 && errno != ENOMSG)
		perror("msgrcv");
	if (ret == -1)
		return (0);
	return (1);
}
