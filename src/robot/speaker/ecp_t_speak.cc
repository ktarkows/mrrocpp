// ------------------------------------------------------------------------
//   task/ecp_t_tran.cc - przezroczyste wersja dla dowolnego z robotow
//
//                     EFFECTOR CONTROL PROCESS (lib::ECP) - main()
//
// Ostatnia modyfikacja: 2006
// ------------------------------------------------------------------------


#include <cstdio>
#include <unistd.h>
#include <map>

#include "base/lib/typedefs.h"
#include "base/lib/impconst.h"
#include "base/lib/com_buf.h"

#include "base/lib/sr/srlib.h"
#include "base/ecp/ecp_task.h"
#include "robot/speaker/ecp_r_speaker.h"
#include "robot/speaker/ecp_t_speak.h"

namespace mrrocpp {
namespace ecp {
namespace speaker {
namespace task {

// KONSTRUKTORY
speaking::speaking(lib::configurator &_config) :
	common::task::_task<ecp::speaker::robot>(_config)
{
	ecp_m_robot = (boost::shared_ptr<robot_t>) new robot(*this);

	speak = new generator::speaking(*this);
	speak->sensor_m = sensor_m;

	sr_ecp_msg->message("ecp loaded");
}

void speaking::main_task_algorithm(void)
{
	for (;;) {
		sr_ecp_msg->message("NOWA SERIA");
		sr_ecp_msg->message("Ruch");
		sr_ecp_msg->message("Zakocz - nacisnij PULSE ECP trigger");
		speak->Move();
	}
}

}
} // namespace speaker

namespace common {
namespace task {

task_base* return_created_ecp_task(lib::configurator &_config)
{
	return new speaker::task::speaking(_config);
}

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp

