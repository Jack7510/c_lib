/*
 * File		: msg.c
 * Desc		: implementation of message communication interfaces
 * Author	: Jack Lee
 * Date		: Feb 10, 2017
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#include "msg.h"


/*
 * Func: create_message_list
 * Desc: create all the message queues in system
 * Para: none
 * Ret : 0 if OK
 */
int create_message_list(void)
{
    int msqid;
    int msgflg = IPC_CREAT | 0666;
    key_t key;
    int i;		/* for loop */

    /* create all msg queues */
    for( i = TID_LAUNCHER_HWD; i < TID_MAX; i++ )
    {
    	key = TASK_TO_MQKEY(i);
	    if ((msqid = msgget(key, msgflg )) < 0) {
	        perror("msgget");
	        return -1;
	    }
	    else 
     		(void) fprintf(stderr,"msgget: msgget succeeded: msqid = %d\n", msqid);
    }

	return 0;
}


/*
 * Func: remove_message_list
 * Desc: remove all the message queues in system
 * Para: none
 * Ret : 0 if OK
 */
int remove_message_list(void)
{
    int msqid;
    int msgflg = 0666;
    key_t key;
    int i;		/* for loop */

    /* create all msg queues */
    for( i = TID_LAUNCHER_HWD; i < TID_MAX; i++ )
    {
    	key = TASK_TO_MQKEY(i);
	    if ((msqid = msgget(key, msgflg )) < 0) {
	        perror("msgget");
	        //return -1;
	    }
	    else 
	    {
     		/* Remove the queue */
        	msgctl(msqid, IPC_RMID, 0);
     	}
    }

	return 0;
}


/*
 * Func: send_message
 * Desc: send message to dst task 
 * Para: dst_task_id - task id of recv task id
 *		 src_task_id - task id of sender task id
 *		 msg         - message to be sent
 *		 msg_len	 - the length of msg
 * Ret : 0 if OK
 */
int send_message( int dst_task_id, int src_task_id, message_buf* msg )
{
	int msqid;
    key_t key;
 
 	if( !CHECK_TASK_ID(dst_task_id) 
 		|| !CHECK_TASK_ID(src_task_id)
 		|| (msg == NULL)) 
 	{
 		perror("send_message param error");
 		return -1;
 	}

    key = TASK_TO_MQKEY(dst_task_id);;
    if ((msqid = msgget(key, 0666)) < 0) 
    {
        fprintf(stderr, "msgget %s %d\n", __func__, __LINE__);
        return -1;
    }	

	/*
     * fill message buffer
     */
    msg->mtype 			= 1;
    msg->dst_task_id 	= dst_task_id;
    msg->src_task_id 	= src_task_id;
 
    /*
     * Send a message.
     */
    if (msgsnd(msqid, msg, MSG_LENGTH(msg), IPC_NOWAIT) < 0) 
    {
    	//printf ("%d, %d, %s, %d\n", msqid, sbuf.mtype, sbuf.mtext, buf_length);
        perror("msgsnd");
    }

    return 0;
}


/*
 * Func: recv_message
 * Desc: receive message  
 * Para: task_id - task id of message to read
 *		 msg         - message to be sent
 *		 msg_len	 - the length of msg
 *		 msgflg		 - MSG_WAIT if wait, otherwise,
 * Ret : < 0 if fail, 0 is OK
 */
int recv_message( int task_id, message_buf* msg, int msg_len, int msgflg )
{
    int msqid;
    key_t key;
    message_buf* qbuf;
    int len;
    int flag = (msgflg == MSG_WAIT) ? 0 : IPC_NOWAIT;

 	if( !CHECK_TASK_ID(task_id) 
 		|| (msg_len > MSG_SIZE) 
 		|| (msg == NULL)) 
 	{
 		perror("send_message param error");
 		return -1;
 	}

    key = TASK_TO_MQKEY(task_id);;
    if ((msqid = msgget(key, 0666)) < 0) 
    {
        fprintf(stderr, "msgget %s %d\n", __func__, __LINE__);
        return -1;
    }	

    /*
     * Receive an answer of message type 1.
     */
    if ( (len = msgrcv(msqid, msg, msg_len, 1, flag )) < 0) 
    {
        perror("msgrcv");
        return -1;
    }

    //fprintf(stderr, "recv_message %lu %d\n", MSG_LENGTH(msg), len);
    if( MSG_LENGTH(msg) == len && (msg->dst_task_id == task_id) )
    	return 0;

	return -1;
}

