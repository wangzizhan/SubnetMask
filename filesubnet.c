#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

#define ARGNUM 3
#define MAXBLINE 80
#define MASKNUM 4

int
main (int argc,char *argv[]) {

		uint32_t ip_start, ip_end;
		struct in_addr addr;
		FILE *fpstart,*fpend,*fpout;
		char bufferstart[MAXBLINE];
		char bufferend[MAXBLINE];
		char bufferout[MAXBLINE];
		uint32_t mask;
		int i;
		char masknum[MASKNUM];

		if(argc != ARGNUM) {
				printf("Usage: %s 2 filename\n",argv[0]);
				exit(1);
		}

		if((fpstart = fopen(argv[1],"r")) == NULL) {
				printf("Cant'open %s\n",argv[1]);
				exit(2);
		}

		if((fpend = fopen(argv[2],"r")) == NULL) {
				printf("Can't open %s\n",argv[2]);
				exit(3);
		}

		if((fpout = fopen("mask","a+")) == NULL) {
				printf("Can't open mask\n");
				exit(4);
		}
		while((fgets(bufferstart,MAXBLINE,fpstart) != NULL)&&(fgets(bufferend,MAXBLINE,fpend) != NULL)) {
				ip_start = ntohl(inet_addr(bufferstart));
				ip_end = ntohl(inet_addr(bufferend));
				uint32_t net = ip_start;
				while ( net <= ip_end ) {
						for(i = 32; i >=0; i--) {
								mask = (i == 0) ? 0 : 0xffffffff << (32 - i);

								if ( (net & mask) != net || (net | ~mask) > ip_end ){
										addr.s_addr = htonl(net);
										strcpy(bufferout,inet_ntoa(addr));
										strcat(bufferout,"\\");
										sprintf(masknum,"%d",i+1);
										strcat(bufferout,masknum);
										strcat(bufferout," ");
										fputs(bufferout,fpout);				
										mask = 0xffffffff << (32 - i - 1);
										net = (net | ~mask) + 1;
										break;
								}
						}

						if( i == -1 ) {
								printf("0.0.0.0/0\n");
								break;
						}
						if( net == 0 ) {
								break;
						}
				}

		}
		//close file with trouble,needs to fix
		//if(fclose(fpstart) != 0 || fclose(fpend) != 0 || fclose(fpout) != 0) 
		//	fprintf(stderr,"Error in closing file.\n");
		
		return 0;
}
