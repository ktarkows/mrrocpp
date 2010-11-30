#if !defined(_ECP_T_SWARM_DEMO_H)
#define _ECP_T_SWARM_DEMO_H

#include <cstdio>

#include "base/lib/typedefs.h"
#include "base/lib/impconst.h"
#include "base/lib/com_buf.h"

#include "base/lib/sr/srlib.h"

#include "robot/irp6ot_m/ecp_r_irp6ot_m.h"
#include "robot/irp6p_m/ecp_r_irp6p_m.h"
#include "generator/ecp/force/ecp_g_tff_gripper_approach.h"

#include "ecp_mp_t_swarm_demo.h"

#include "base/ecp/ecp_task.h"
#include "subtask/ecp_mp_st_smooth_file_from_mp.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

class swarm_demo : public common::task::task
{
protected:
	common::generator::tff_gripper_approach* gag;

public:
	/**
	 * Constructor.
	 */
	swarm_demo(lib::configurator &_config);

	// methods for ECP template to redefine in concrete classes
	void mp_2_ecp_next_state_string_handler(void);

};

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp

#endif