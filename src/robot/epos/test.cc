#include <iostream>
#include <boost/exception/get_error_info.hpp>
#include <boost/array.hpp>
#include <boost/foreach.hpp>
#include <sys/time.h>

#include "epos_access_usb.h"
#include "epos.h"

using namespace mrrocpp::edp::epos;

int main(int argc, char *argv[])
{
	epos_access_usb gateway;

	boost::array<epos *, 3> axis;

	try {
		gateway.open();

		epos node4(gateway, 4);
		epos node5(gateway, 5);
		epos node6(gateway, 6);

		axis[0] = &node4;
		axis[1] = &node5;
		axis[2] = &node6;

		BOOST_FOREACH(epos * node, axis) {

			node->printEPOSstate();

			// Check if in a FAULT state
			if(node->checkEPOSstate() == 11) {
				UNSIGNED8 errNum = node->readNumberOfErrors();
				std::cout << "readNumberOfErrors() = " << (int) errNum << std::endl;
				for(UNSIGNED8 i = 1; i <= errNum; ++i) {

					UNSIGNED32 errCode = node->readErrorHistory(i);

					std::cout << epos::ErrorCodeMessage(errCode) << std::endl;
				}
				if (errNum > 0) {
					node->clearNumberOfErrors();
				}
				node->changeEPOSstate(epos::FAULT_RESET);
			}

			// Change to the operational mode
			node->reset();
		}

		gateway.close();
	} catch (epos_error & error) {
		std::cerr << "EPOS Error." << std::endl;

		if ( std::string const * r = boost::get_error_info<reason>(error) )
			std::cerr << " Reason: " << *r << std::endl;

		if ( std::string const * call = boost::get_error_info<errno_call>(error) )
			std::cerr << " Errno call: " << *call << std::endl;

		if ( int const * errno_value = boost::get_error_info<errno_code>(error) )
			std::cerr << "Errno value: " << *errno_value << std::endl;
	} catch (...) {
		std::cerr << "Unhandled exception" << std::endl;
	}

	return 0;
}
