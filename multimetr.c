#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <time.h>


int main() {

  int fd = open("/dev/ttyS0", O_RDWR | O_NONBLOCK);
  printf("open res: %d\n", fd);

  unsigned char buf[] = {0x10, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10, 0x03, 0x00};
  unsigned char res[22];
  struct timespec t1, t2;
  t1.tv_sec = 0;
  t1.tv_nsec = 200000000L;
  struct termios t;
  speed_t s;
  int actions, RTS_flag;

  tcflush(fd, TCIFLUSH);

  bzero(&t, sizeof(t));
  printf("%d\n",tcgetattr(fd, &t));

  s = B9600;
  cfsetospeed(&t, (speed_t)s);
  cfsetispeed(&t, (speed_t)s);

  /* enable the receiver and set local mode */
  t.c_cflag |= (CLOCAL | CREAD);

  /* [<0;45;43Mset no parity, stop bits, data bits */
  t.c_cflag &= ~PARENB;
  t.c_cflag &= ~CSTOPB;

  /* set character size as 8 bits */
  t.c_cflag &= ~CSIZE;
  t.c_cflag |= CS8;

  t.c_cflag &= ~CRTSCTS;

  t.c_lflag &= ~(ICANON | ECHO | ISIG);
  t.c_oflag = ~OPOST;

  cfmakeraw(&t);
[<0;70;36M
  actions = TCSANOW;
  t.c_cflag |= ((~TIOCM_DTR) | (~TIOCM_RTS));

  printf("%d\n", tcsetattr(fd, actions, &t));

  tcgetattr(fd, &t);

  actions = TCSANOW;
  t.c_cflag |= (TIOCM_DTR | TIOCM_RTS);

  s = B9600;
  cfsetospeed(&t, (speed_t)s);
  cfsetispeed(&t, (speed_t)s);

  printf("%d\n",tcsetattr(fd, actions, &t));
  sleep(1);

  if(fd > 0 ){

    while(1) {
      for (int i = 0; i < 9; i++)
      {
        int writeRes = write(fd, &buf[i], 1);
        printf("write res: %d\n", writeRes);
        nanosleep(&t1,&t2);
      }

      int readRes = read(fd, res, 22);
      printf("read=%d\n", readRes);
      if (readRes == 22)
        for (int i = 0; i < 22; i++)
        {
          printf("%c", res[i]);
        }


  }
  }
  else
    printf("error");

  return 0;
}

//unsigned char buf[] = {0x10, 0x02, 0x42, 0x00, 0x00, 0x00, 0x10, 0x03}
