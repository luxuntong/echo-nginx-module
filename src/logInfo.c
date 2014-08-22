#include "logInfo.h"


#include <ngx_core.h>
#include <ngx_http.h>


#include <stdarg.h>
#include <stdio.h>

#include <stdarg.h>

#define LOG_PATH_OF_LUXUNTONG "/tmp/echoLog"

static ngx_atomic_t lockLog = 0;

void logInfo(const char* fmt, ...)
{
	va_list ap; /* points to each unnamed arg in turn */
#if 0	
	const char *p, *sval;
	
	int ival;

	double dval;
#endif
	u_char buf[1024];
	int ret;
	u_char *pLen;
	ngx_spinlock(&lockLog, 1, 80);
//	chmod("/tmp/echoLog", 0x3f);
    umask(0);
	int fd = open(LOG_PATH_OF_LUXUNTONG, O_WRONLY | O_CREAT | O_APPEND,  S_IROTH | S_IWOTH | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	
	if(fd == -1)
	{
		ngx_unlock(&lockLog);
		return;
	}
	va_start(ap, fmt);
	pLen = ngx_snprintf(buf, 1024, fmt, ap);
	ret = write(fd, buf, pLen - buf);
	ret = write(fd, "\n", strlen("\n"));
	ret = ret;
	close(fd);
	ngx_unlock(&lockLog);
}