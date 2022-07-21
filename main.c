#include <unistd.h>
#include <errno.h>

/* Buffer size should be multiple of 4 for memory alignment */
#define BUFSIZE 16384

size_t safe_write(int fd, void *buf, size_t len) {
	int n;
	size_t sum = 0;
	char *off = (char *) buf;

	while(sum < len) {
		if ((n = write(fd, (void *) off, len-sum)) < 0) {
			if (errno == EINTR) continue;
			return n;
		}

		sum += n;
		off += n;
	}

	return sum;
}

int main() {
	char buf[BUFSIZE];

	/* Fill buffer with "y\n" */
	for(int i = 0; i < BUFSIZE; i+=2){
		buf[i] = 'y';
		if (i < BUFSIZE-1) buf[i+1] = '\n';
	}
	
	int n;

	for(;;) {
		if ((n = safe_write(STDOUT_FILENO, buf, BUFSIZE)) != BUFSIZE) return 1;;
	}
	return 0;
}
