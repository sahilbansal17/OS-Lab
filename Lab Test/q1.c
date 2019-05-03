#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#define KEY 123456

int msgid;

struct msgbuf {
	long mtype;
	char mtext[80];
};

void sender(char* file_name, int packet_size) {
	// printf("Sender started running...\n");
	// open the input file
	
	FILE *fp1 = fopen(file_name, "r");
	if (fp1) {
		// printf("Sender File opened successfully.\n");
	}
	else {
		printf("File not opened successfully...Exiting\n");
		return;
	}


	struct msgbuf msg;
	// read the message from the file
	long type = 1;
	int done = 0;
	while (!feof(fp1)) {
		fread((void *)&msg.mtext, packet_size, 1, fp1);	
		printf("-------------------------------\nmessage sent:\n %s\n------------------------\n", msg.mtext);	
		msg.mtype = type;
		// type = (type + 1) % 3;
		// send the message to the message queue
		int msgstatus = msgsnd(msgid, (void *) &msg, packet_size, IPC_NOWAIT);
		if (msgstatus == -1) {
			done = 1;
		}
		else {
			printf("Message sent successfully.\n\n");
		}
		// break;		
	}
	// send termination message	
	strcpy(msg.mtext, "Done");
	msg.mtype = 2;
	int msgstatus = msgsnd(msgid, (void *) &msg, packet_size, IPC_NOWAIT);
	if (msgstatus == -1) {
		printf("Termination message not sent successfully.\n");
	}
	// close the file
	fclose(fp1);
}

void receiver(char* file_name) {
	// printf("Receiver started running...\n");
	FILE *fp = fopen(file_name, "w");
	if (fp) {
		// printf("\t\t\tReceiver File opened successfully.\n");
	}
	else {
		printf("\t\t\tFile not opened successfully...Exiting\n");
		return ;
	}

	struct msgbuf msg;
	long type = 1, term = 2;
	int done = 0;
	while (!done) {
		// read the message from the message queue along with the message code
		int msgstatus = msgrcv(msgid, (void *) &msg, sizeof(msg.mtext), type, MSG_NOERROR);
		if (msgstatus == -1) {
			done = 1;
		}
		int msgstatus2 = msgrcv(msgid, (void *) &msg, sizeof(msg.mtext), term, MSG_NOERROR | IPC_NOWAIT);
		if (strcmp(msg.mtext, "Done") == 0) {
			break;
		}
		// write the message to the output file
		printf("\n--------------------------------------------------\n\t\t\tReceiver says, The message from the sender is:\n%s\n\n-----------------------------------------------------\n", msg.mtext);
		fwrite(msg.mtext, sizeof(msg.mtext), 1, fp);
		// type = (type + 1) % 3;
		// break;
	}

	// close the file
	fclose(fp);
	
}

int main (int argc, char** argv) {
	
	if (argc != 4) {
		perror("Usage of the program: ./program1 <input_file> <output_file> <packet_size>");
		exit(-1);
	}

	printf("------------------------------------------\nInput file: %s, Output file: %s, Packet Size: %d\n-------------------------------------------\n", argv[1], argv[2], atoi(argv[3]));
	// create a new message queue
	msgid = msgget(KEY, 0666 | IPC_CREAT);
	if (msgid == -1) {
		perror("Error in creating a message queue...Exiting\n");
		exit(-1);
	}

	int pid = fork();
	if (pid == 0) {
		// child process - receiver
		receiver(argv[2]);
		// printf("\nBack from receiver\n");
	}

	int pid1 = fork();
	if (pid1 == 0) {
		// child process - sender
		sender(argv[1], atoi(argv[3]));
		// printf("\nBack from sender\n");
		exit(0);
	}
	
	wait(NULL);
	wait(NULL);
	return 0;

}
