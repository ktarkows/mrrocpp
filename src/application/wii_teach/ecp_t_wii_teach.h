#ifndef ECP_T_WII_TEACH_H
#define ECP_T_WII_TEACH_H

#include "ecp_mp/task/ecp_mp_task.h"
#include "ecp/common/generator/ecp_g_smooth.h"
#include "application/wii_teach/generator/ecp_g_wii_relative.h"
#include "application/wii_teach/generator/ecp_g_wii_joint.h"

namespace mrrocpp {
namespace ecp {
namespace irp6ot {
namespace task {

/**
 * @author jkurylo
 */
class wii_teach: public common::task::task
{
    protected:
	//Generator ruchu
        common::generator::smooth* sg;
        irp6ot::generator::wii_relative* wg;
        irp6ot::generator::wii_joint* jg;
        lib::sensor_image_t::sensor_union_t::wiimote_t lastButtons;
        lib::sensor_image_t::sensor_union_t::wiimote_t buttonsPressed;
        char path[80];
        char filename[20];

        char gripper_path[80];
        char gripper_filename[20];

        lib::Homog_matrix homog_matrix;

        class n;
        class n
        {
            public:
                n* next;
                n* prev;
                int id;
                double position[6];
                double gripper;

                n() : next(NULL), prev(NULL) {}

        };

        typedef n node;

        struct
        {
            node* head;
            node* tail;
            int count;
            node* current;
            int position;
        } trajectory;

        void updateButtonsPressed();

    public:
	/**
	 * Tworzy obiekt zadania
	 * @param _config konfigurator
	 * @author jedrzej
	 */
	wii_teach(lib::configurator &_config);

	/**
	 * Realizuje zadanie
	 * @author jkurylo
	 */
	void main_task_algorithm(void);

        void print_trajectory(void);

        void move_to_current(void);

        bool get_filenames(void);

        int load_trajectory(void);

        void save_trajectory(void);
};

}
} // namespace irp6ot
} // namespace ecp
} // namespace mrrocpp


#endif //ECP_T_WII_TEACH_H