/*
 * File		: msg.h
 * Desc		: define message communication interfaces
 * Author	: Jack Lee
 * Date		: Feb 10, 2017
 */
#ifndef __MSG_H__
#define __MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define MSG_SIZE     				256			/* max size of msg text */
#define MQ_KEY						0x1000		/* the beginning of message queue key */
#define TASK_TO_MQKEY( task_id )	( task_id + MQ_KEY)	/* convert task id to mqueue key id */
#define CHECK_TASK_ID( task_id )	( (task_id >= TID_LAUNCHER_HWD) && (task_id < TID_MAX) )

#define MSG_WAIT 					0			/* wait until msg receive */
#define MSG_NOWAIT					1			/* do not wait msg */

/*
 * Declare the ids of task .
 */
enum task_id
{
	TID_LAUNCHER_HWD = 0,		/* launcher Qt window */
	TID_CALL_HWD,				/* Voice call Qt window */
	TID_CALL_SERVICE,			/* Call service process */
	TID_ALARM_HWD,				/* Alarm Qt window */
	TID_ALARM_SERVCIE,			/* Alarm service process */
	TID_IM_HWD,					/* IM Qt window */
	TID_IM_SERVICE,				/* IM service process */
	TID_MAX
};


/*
 * Declare the message structure.
 */
typedef struct  {
	long 	mtype;       /* message type, must be > 0 */
    int 	dst_task_id;
    int 	src_task_id;
    int 	msg_len;
    char   	mtext[1];
} message_buf;
#define MSG_LENGTH( msg )	( msg->msg_len + sizeof(message_buf) - 1 - sizeof(long)) 

/*
 * Declare the message interfaces.
 */
int create_message_list(void);
int remove_message_list(void);
int send_message( int dst_task_id, int src_task_id, message_buf* msg );
int recv_message( int task_id, message_buf* msg, int msg_len, int msgflg );


#ifdef __cplusplus
}		// extern "C"
#endif

#endif 	// __MSG_H__