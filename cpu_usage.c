/*
 * cpu_stat.c
 *
 *  Created on: Apr 28, 2017
 *      Author: airkiller1
 */
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "sys/select.h"

#define MAX_BUF_LENGTH 1024

typedef unsigned long long TIC_t;

typedef struct CPU_t {
   TIC_t user, nice, system, idle, iowait, irq, softirq, steal,guest,guest_nice; // as represented in /proc/stat
   TIC_t user_sav,nice_sav, system_sav, idle_sav, iowait_sav, irq_sav, softirq_sav, steal_sav,guest_sav,guest_nice_sav; // in the order of our display
} CPU_t;

static void printHelp(void);

int main(int argc,char* argv[]){
	char buf[MAX_BUF_LENGTH];
	char* str = buf;
	struct timeval duration;
	TIC_t delta_cpu_sum = 0;
	CPU_t cpu_stat;
	FILE* stat_file = fopen("/proc/stat","r");
	if(stat_file == NULL){
		fprintf(stderr,"can't open file /proc/stat!\n");
		return -1;
	}
	duration.tv_sec = 0;
	duration.tv_usec = 0;
	if(argc == 2){
		duration.tv_usec = strtol(argv[1],NULL,0);
	}else if(argc == 3){
		duration.tv_sec = strtol(argv[1],NULL,0);
		duration.tv_usec = strtol(argv[2],NULL,0);
	}else{
		fprintf(stderr,"%s(%d):wrong parameter~!\n",__func__,__LINE__);
		printHelp();
		return -1;
	}
	while(1){
		rewind(stat_file);
		fflush(stat_file);
		str = fgets(buf,MAX_BUF_LENGTH,stat_file);
		if(str == NULL){
			fprintf(stderr,"can't open file /proc/stat!\n");
			break;
		}
		sscanf(buf,"cpu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
				&cpu_stat.user_sav,
				&cpu_stat.nice_sav,
				&cpu_stat.system_sav,
				&cpu_stat.idle_sav,
				&cpu_stat.iowait_sav,
				&cpu_stat.irq_sav,
				&cpu_stat.softirq_sav,
				&cpu_stat.steal_sav,
				&cpu_stat.guest_sav,
				&cpu_stat.guest_nice_sav);

		select(0, NULL, NULL, NULL, &duration);

		rewind(stat_file);
		fflush(stat_file);
		str = fgets(buf, MAX_BUF_LENGTH, stat_file);
		if (str == NULL) {
			fprintf(stderr, "can't open file /proc/stat!\n");
			break;
		}
		sscanf(buf, "cpu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
				&cpu_stat.user,
				&cpu_stat.nice,
				&cpu_stat.system,
				&cpu_stat.idle,
				&cpu_stat.iowait,
				&cpu_stat.irq,
				&cpu_stat.softirq,
				&cpu_stat.steal,
				&cpu_stat.guest,
				&cpu_stat.guest_nice);
		delta_cpu_sum = (cpu_stat.user - cpu_stat.user_sav)+
				(cpu_stat.nice - cpu_stat.nice_sav)+
				(cpu_stat.system - cpu_stat.system_sav)+
				(cpu_stat.idle - cpu_stat.idle_sav)+
				(cpu_stat.iowait - cpu_stat.iowait_sav)+
				(cpu_stat.irq - cpu_stat.irq_sav)+
				(cpu_stat.softirq - cpu_stat.softirq_sav)+
				(cpu_stat.steal - cpu_stat.steal_sav)+
				(cpu_stat.guest - cpu_stat.guest_sav)+
				(cpu_stat.guest_nice - cpu_stat.guest_nice_sav);
		fprintf(stdout,"usr:%d%% sys:%d%% idle:%d%%\n",
				(cpu_stat.user - cpu_stat.user_sav)*100/delta_cpu_sum,
				(cpu_stat.system - cpu_stat.system_sav)*100/delta_cpu_sum,
				(cpu_stat.idle - cpu_stat.idle_sav)*100/delta_cpu_sum);


	}
	if(stat_file != NULL){
		fclose(stat_file);
	}
	return 0;
}

static void printHelp(void){
	fprintf(stdout,"\n----------help info. for cpu_usage----------\n");
	fprintf(stdout,"cpu_usage: a simple tool that display CPU usage periodically\n");
	fprintf(stdout,"cpu_usage <microseconds>\n");
	fprintf(stdout,"cpu_usage <seconds> <microseconds>\n");
	fprintf(stdout,"where:\n");
	fprintf(stdout,"\tmicroseconds\t-\tmicroseconds to delay\n");
	fprintf(stdout,"\tseconds\t-\tseconds to delay\n");
	fprintf(stdout,"--------------------end--------------------\n");
}
