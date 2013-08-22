/*
 Copyright (c) 2013 IRIT
 Author: Thomas Zilio <zilio@irit.fr>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.



 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <libec/sensors.h>
#include <libec/estimators.h>

#define RUNNING_DIR     "/tmp"
#define LOCK_FILE       "ecdaq-daemon.lock"
#define LOG_FILE        "ecdaq-daemon.log"

#define MAX_PROCESS         20
#define ERROR_MAX_NUMBER    5
#define ERROR_THRESHOLD     5000

void log_message(char const *filename, char const *message)
{
  FILE *logfile;
  logfile=fopen(filename,"a");
  if ( !logfile )
    return;
  fprintf(logfile,"%s\n",message);
  fclose(logfile);
}

void signal_handler(int sig)
{
  switch (sig) {
  case SIGHUP:
    log_message(LOG_FILE,"hangup signal catched");
    break;
  case SIGTERM:
    log_message(LOG_FILE,"terminate signal catched");
    exit(0);
    break;
  }
}

void daemonize()
{
  int i,lfp;
  char str[10];

  /* already a daemon */
  if ( getppid() == 1 )
    return;

  i=fork();

  /* fork error */
  if ( i<0 )
    exit(1);

  /* parent exits */
  if ( i>0 )
    exit(0);

  /* child (daemon) continues */
  /* obtain a new process group */
  setsid();

  /* close all descriptors */
  for ( i = getdtablesize(); i >= 0; --i )
    close(i);

  /* handle standart I/O */
  i=open("/dev/null",O_RDWR);
  dup(i);
  dup(i);

  /* set newly created file permissions */
  umask(027);

  /* change running directory */
  chdir(RUNNING_DIR);

  lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);

  /* can not open */
  if ( lfp < 0 )
    exit(1);

  /* can not lock */
  if ( lockf(lfp,F_TLOCK,0) < 0 )
    exit(0);

  /* first instance continues */
  sprintf(str,"%d\n",getpid());

  /* record pid to lockfile */
  write(lfp,str,strlen(str));

  /* ignore child */
  signal(SIGCHLD,SIG_IGN);

  /* ignore tty signals */
  signal(SIGTSTP,SIG_IGN);
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTTIN,SIG_IGN);

  /* catch hangup signal */
  signal(SIGHUP,signal_handler);

  /* catch kill signal */
  signal(SIGTERM,signal_handler);
}

struct arg_struct {
  cea::PowerEstimator * pe;
  cea::DiskIO * dio;
  cea::MemRss * mrss;
  cea::MemUsage * musg;
  cea::CpuTimeUsage * cusg;
  cea::CpuFreq * cufreq;
  int socket;
};


void* SocketHandler(void* args)
{
  struct arg_struct *thread_args = (struct arg_struct *)args;

  int client_socket = thread_args->socket;
  cea::PowerEstimator *pwe = thread_args->pe;
  cea::DiskIO *diskio = thread_args->dio;
  cea::MemRss *memrss = thread_args->mrss;
  cea::MemUsage *memusg = thread_args->musg;
  cea::CpuTimeUsage *cpuusg = thread_args->cusg;
  cea::CpuFreq *cpufreq = thread_args->cufreq;

  int *pids_list = (int*)malloc(sizeof(int) * MAX_PROCESS);
  int pids_number = 0;

  char buffer[256];
  char res[256];
  char *tmp;
  char cmd_string[10];
  char pid_string[20];

  float pwe_float;
  float memusg_float;
  float cpuusg_float;
  unsigned long long diskio_U64;
  unsigned long long memrss_U64;
  unsigned long long cpufreq_U64;

  int cmd, pid, i, j;
  int buffer_len = 255;
  int bytecount, closing = 0, error_count = 0;

  while ( !closing ) {
      error_count = 0;
      pwe_float = ERROR_THRESHOLD + 1;

      bzero(buffer,256);

      // Waiting to receive a message
      if ( (bytecount = read(client_socket, buffer, buffer_len)) == -1 ) {
          fprintf(stderr, "Error receiving data %d\n", errno);
          close(client_socket);
          pthread_exit(NULL);
      }

      // If receiving close message or incorrect message (coming from a gmond kill)
      if ( bytecount == 0 ) {
          closing = 1;
      } else {
          tmp = strtok(buffer,"$");
          strcpy(cmd_string, tmp);

          while (tmp != NULL) {
              tmp = strtok(NULL, "$");
              if (tmp != NULL) {
                  strcpy(pid_string, tmp);
              }
          }

          cmd = atoi(cmd_string);
          pid = atoi(pid_string);

          // Finishing the monitoring
          if (cmd == 0) {
              closing = 1;
              break;
          }

          // Adding a process
          if (cmd == 1) {
              if (pids_number < MAX_PROCESS) {
                  pids_list[pids_number] = pid;
                  pids_number ++;
                  sprintf(res, "O$");
              } else {
                  sprintf(res, "-1$");
              }
          }

          // Removing a process
          if (cmd == 2) {
              i = 0;
              while(pids_list[i] != pid && i != pids_number) {
                  i++;
              }

              if (i == pids_number) {
                  sprintf(res, "-1$");
              } else {
                  for(j=i; j<pids_number; j++) {
                      pids_list[j] = pids_list[j+1];
                  }

                  pids_number--;
                  sprintf(res, "O$");
              }
          }

          // Update everything for every pid
          if (cmd == 3) {
              pwe->update();
              diskio->update();
              memrss->update();
              memusg->update();
              cpuusg->update();
              cpufreq->update();

              for (i=0; i<pids_number; i++) {
                  pwe->updatePid(pids_list[i]);
                  diskio->updatePid(pids_list[i]);
                  memrss->updatePid(pids_list[i]);
                  memusg->updatePid(pids_list[i]);
                  cpuusg->updatePid(pids_list[i]);
              }

              sprintf(res, "O$");
          }


          // Requesting a process informations
          if (cmd == 4) {
              //Avoiding inconsistent data sometimes sent by the estimator
              while (pwe_float > ERROR_THRESHOLD && error_count < ERROR_MAX_NUMBER) {
                  pwe_float = pwe->getValuePid(pid).Float;
                  diskio_U64 = diskio->getValuePid(pid).U64;
                  memrss_U64 = memrss->getValuePid(pid).U64;
                  memusg_float = memusg->getValuePid(pid).Float;
                  cpuusg_float = cpuusg->getValuePid(pid).Float;
                  cpufreq_U64 = cpufreq->getValue().U64;

                  error_count++;
              }

              if (error_count == ERROR_MAX_NUMBER) {
                  strcpy(res, "-1");
              }

              sprintf(res, "%f/%llu/%llu/%f/%f/%llu$", pwe_float, diskio_U64, memrss_U64, memusg_float, cpuusg_float, cpufreq_U64);
          }

          // Sending result
          if ( (bytecount = write(client_socket, res, strlen(res))) == -1 ) {
              fprintf(stderr, "Error sending data %d\n", errno);
              close(client_socket);
              pthread_exit(NULL);
          }
      }
  }
  close(client_socket);
  //printf("Closing socket %d now\n", client_socket);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  //daemonize();

  cea::PidStat *cpu;
  cea::PowerEstimator *power_est;
  cea::DiskIO *disk_io;
  cea::MemRss *mem_rss;
  cea::MemUsage *mem_usage;
  cea::CpuTimeUsage *cpu_tusg;
  cea::CpuFreq *cpu_freq;

  cpu = new cea::PidStat(cea::PidStat::CPU_USAGE);
  power_est = new cea::MinMaxCpu(22, 55);
  disk_io = new cea::DiskIO("sda");
  mem_rss = new cea::MemRss();
  mem_usage = new cea::MemUsage();
  cpu_tusg = new cea::CpuTimeUsage();
  cpu_freq = new cea::CpuFreq(0);


  struct arg_struct *thread_args = (struct arg_struct*)malloc(sizeof(struct arg_struct));

  if ( argc < 3 ) {
      fprintf(stderr,"Usage: %s port drive\n", argv[0]);
      exit(1);
  }

  // Socket preparation
  int host_port = atoi(argv[1]);
  int host_socket;
  int opt_val = 1;
  int client_socket;

  struct sockaddr_in serv_addr, client_addr;
  socklen_t addr_size = sizeof(client_addr);

  pthread_t thread_id = 0;

  host_socket = socket(AF_INET, SOCK_STREAM, 0);

  if ( host_socket == -1 ) {
      printf("Error initializing socket %d\n", errno);
      exit(1);
  }

  if ( (setsockopt(host_socket, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1) ||
      (setsockopt(host_socket, SOL_SOCKET, SO_KEEPALIVE, &opt_val, sizeof(opt_val)) == -1) ) {
      printf("Error setting options %d\n", errno);
      exit(1);
  }

  serv_addr.sin_family = AF_INET ;
  serv_addr.sin_port = htons(host_port);
  memset(&(serv_addr.sin_zero), 0, 8);
  serv_addr.sin_addr.s_addr = INADDR_ANY ;

  if ( bind(host_socket, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1 ) {
      fprintf(stderr,"Error binding to socket, make sure nothing else is listening on this port %d\n",errno);
      exit(1);
  }

  if ( listen(host_socket, 20) == -1 ) {
      fprintf(stderr, "Error listening %d\n",errno);
      exit(1);
  }

  if ( power_est->getStatus() ) {
      while ( 1 ) {
          // Accept connection and create a new thread
          if((client_socket = accept( host_socket, (sockaddr*)&client_addr, &addr_size))!= -1) {
              thread_args->pe = power_est;
              thread_args->dio = disk_io;
              thread_args->mrss = mem_rss;
              thread_args->musg = mem_usage;
              thread_args->cusg = cpu_tusg;
              thread_args->cufreq = cpu_freq;
              thread_args->socket = client_socket;

              //printf("Accepting socket %d from port %d\n", client_socket, client_addr.sin_port);
              pthread_create(&thread_id,0,&SocketHandler, thread_args);
              pthread_detach(thread_id);
          }
          else {
              fprintf(stderr, "Error accepting %d\n", errno);
          }
      }
  } else {
      std::cerr << "error: sensor could not be opened." << std::endl;
  }

  delete power_est;
  delete disk_io;
  delete mem_rss;
  delete mem_usage;
  delete cpu_tusg;
  delete cpu;
  free(thread_args);

  return EXIT_SUCCESS;

}

/* EOF */
