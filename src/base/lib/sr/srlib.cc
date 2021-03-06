/*!
 * @file srlib.cc
 * @brief System reporting.
 *
 * @author Piotr Trojanek <piotr.trojanek@gmail.com>
 *
 * @ingroup LIB
 */

#include <cstdio>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <stdint.h>
#include <sys/utsname.h>
#include <sys/time.h>

#include <boost/thread/mutex.hpp>

#include "base/lib/typedefs.h"
#include "base/lib/impconst.h"
#include "base/lib/com_buf.h"

#include "base/lib/sr/srlib.h"
#include "base/lib/sr/Sender.h"

namespace mrrocpp {
namespace lib {

/*
 sr_package::sr_package()
 : process_type(UNKNOWN_PROCESS_TYPE),
 message_type(-1)
 {
 memset(process_name, 0, sizeof(process_name));
 memset(host_name, 0, sizeof(host_name));
 memset(description, 0, sizeof(description));
 ts.tv_sec = ts.tv_nsec = 0;
 }
 */
sr::sr(process_type_t process_type, const std::string & process_name, const std::string & sr_name)
	: sender(sr_name)
{
	struct utsname sysinfo;
	if (uname(&sysinfo) == -1) {
		// TODO: throw
		perror("uname");
	}

	strncpy(sr_message.host_name, sysinfo.nodename, sizeof(sr_message.host_name));
	sr_message.host_name[sizeof(sr_message.host_name)] = 0;

	sr_message.process_type = process_type;
	sr_message.message_type = NEW_MESSAGE;
	strcpy(sr_message.process_name, process_name.c_str());
	for (size_t i = 0; i < ERROR_TAB_SIZE; i++) {
		error_tab[i] = 0;
	}
}

void sr::send_package(void)
{
	struct timeval tv;
	if(gettimeofday(&tv, NULL) == -1) {
		perror("gettimeofday()");
	}

	sr_message.time = ((uint64_t) tv.tv_usec) * 1000 + ((uint64_t) tv.tv_sec) * 1000000000;
	sender.send_package(sr_message);
}

sr::~sr()
{
}

void sr::message(const std::string & text)
{
	boost::mutex::scoped_lock lock(srMutex);

	sr_message.message_type = NEW_MESSAGE;
	if (text.length()) {
		strncpy(sr_message.description, text.c_str(), TEXT_LENGTH);
		sr_message.description[TEXT_LENGTH - 1] = '\0';
	} else {
		sr_message.description[0] = '\0';
	}
	return send_package();
}

void sr::message(error_class_t message_type, const std::string & text)
{
	boost::mutex::scoped_lock lock(srMutex);

	sr_message.message_type = message_type;
	if (text.length()) {
		strncpy(sr_message.description, text.c_str(), TEXT_LENGTH);
		sr_message.description[TEXT_LENGTH - 1] = '\0';
	} else {
		sr_message.description[0] = '\0';
	}
	return send_package();
}

void sr::message(error_class_t message_type, uint64_t error_code, const std::string & text)
{
	boost::mutex::scoped_lock lock(srMutex);

	sr_message.message_type = message_type;
	error_tab[0] = error_code;
	interpret();
	strcat(sr_message.description, text.c_str());
	return send_package();
}

void sr::message(error_class_t message_type, uint64_t error_code)
{
	boost::mutex::scoped_lock lock(srMutex);

	sr_message.message_type = message_type;
	error_tab[0] = error_code;
	interpret();
	send_package();
}

void sr::message(error_class_t message_type, uint64_t error_code0, uint64_t error_code1)
{
	boost::mutex::scoped_lock lock(srMutex);

	sr_message.message_type = message_type;
	error_tab[0] = error_code0;
	error_tab[1] = error_code1;
	interpret();
	send_package();
}

} // namespace lib
} // namespace mrrocpp
