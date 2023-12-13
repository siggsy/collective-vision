#include "ffmpeg.hpp"
#include <iostream>

extern "C" {
	#include <errno.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <sys/wait.h>
}

using namespace std;


#define WRITE_TIMEOUT_S		0
#define WRITE_TIMEOUT_MS	250


// ----------------------------------- [ Functions ] ---------------------------------------- //


/**
 * @brief Safely write img data to child process.
 *        If the child process crashes, the function returns soon after with its exit status.
 *        The child process is awaited and the file descriptor is closed upon return.
 * 
 * @param childPid PID of child process
 * @param writeFD File descriptor for writing data
 * @param img Img to write to the FD
 * @return Exit status of child process
 */
static int ffEncode_parent(int childPid, int writeFD, const Image& img){
	// Counters for safe write
	const uint8_t* data = img.raw();
	const size_t size = img.rawSize();
	size_t count = 0;
	
	// FD monitors
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(writeFD, &fds);
	struct timeval timeout = {WRITE_TIMEOUT_S, WRITE_TIMEOUT_MS*1000};
	
	// Child status
	int status = 1;
	bool exited = false;
	
	while (count < size){
		int fd_count = select(writeFD+1, NULL, &fds, NULL, &timeout);
		
		// error
		if (fd_count < 0){
			break;
		}
		
		// Write buffer ready
		else if (FD_ISSET(writeFD, &fds)){
			ssize_t n = write(writeFD, data + count, size - count);
			if (n > 0)
				count += n;
		}
		
		// Wait for write buffer, check child process
		else if (waitpid(childPid, &status, WNOHANG) != 0){
			exited = true;
			break;
		}
		
	}
	
	close(writeFD);
	if (!exited){
		waitpid(childPid, &status, 0);
	}
	
	return status;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


static char* str(const string& s){
	char* cs = new char[s.size() + 1];
	copy(s.begin(), s.end(), cs);
	cs[s.size()] = 0;
	return cs;
}


// ----------------------------------- [ Functions ] ---------------------------------------- //


int ffEncode(const Image& img, const string& outFile, bool silent){
	// Create pipe for writing img data to the ffmpeg process
	int p[2];
	if (pipe2(p, O_NONBLOCK) != 0){
		return 1;
	}
	
	// Spawn subprocess (for ffmpeg)
	const pid_t pid = fork();
	
	// Parent
	if (pid > 0){
		close(p[0]);
		return ffEncode_parent(pid, p[1], img);
	}
	
	// Child
	else if (pid == 0){
		// Pipe to stdin
		if (dup2(p[0], 0) != 0){
			exit(1);
		}
		
		close(p[0]);
		close(p[1]);
		
		if (silent){
			close(1);
			close(2);
		}
		
		const char* args[] = {
			"ffmpeg",
			"-vcodec", "rawvideo",
			"-f", "rawvideo",
			"-pix_fmt", "rgba",
			"-s", str(to_string(img.w) + "x" + to_string(img.h)),
			"-i", "-",
			"-vframes", "1",
			str(outFile), "-y",
			NULL
		};
		
		for (int i = 0 ; i < 16 ; i++){
			cout << args[i] << endl;
		}
		
		execvp("ffmpeg", (char**)args);
		exit(1);
	}
	
	return 1;
}


// ------------------------------------------------------------------------------------------ //