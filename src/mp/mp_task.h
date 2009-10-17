#ifndef MP_TASK_H_
#define MP_TASK_H_

#include "mp/mp_generator.h"
#include "mp/mp.h"
#include "ecp_mp/ecp_mp_task.h"

#if !defined(USE_MESSIP_SRR)
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#else
#include "lib/messip/messip.h"
#endif

namespace mrrocpp {
namespace mp {

// forward delcaration
namespace generator {
class generator;
}

namespace task {

// klasa globalna dla calego procesu MP
class task: public ecp_mp::task::task
{
	friend class robot::robot;

	public:
#if !defined(USE_MESSIP_SRR)
		static name_attach_t *mp_pulse_attach;

	    //! A server connection ID identifing UI
	    int ui_scoid;

	    //! flag indicating opened pulse connection from UI
	    bool ui_opened;
#else
		static messip_channel_t *mp_pulse_attach;
		messip_dispatch_t *dpp;
#endif

		/// mapa wszystkich robotow
		static common::robots_t robot_m;

		/// KONSTRUKTORY
		task(lib::configurator &_config);
		virtual ~task(void);

		void initialize_communication (void);
		void stop_and_terminate (void);

		// oczekiwanie na puls z ECP
		typedef enum _MP_RECEIVE_PULSE_ENUM {
			NONBLOCK,
			BLOCK
		} RECEIVE_PULSE_MODE;

		typedef enum _WAIT_FOR_NEW_PULSE_ENUM {
		    NEW_ECP_PULSE,
		    NEW_UI_PULSE,
		    NEW_UI_OR_ECP_PULSE
		} WAIT_FOR_NEW_PULSE_MODE;

		void set_next_playerpos_goal (lib::ROBOT_ENUM robot_l, const lib::playerpos_goal_t &goal);

		void set_next_ecps_state (int l_state, int l_variant, const char* l_string, int number_of_robots, ... );

		void send_end_motion_to_ecps (int number_of_robots, ... );
		void send_end_motion_to_ecps (int number_of_robots, lib::ROBOT_ENUM *properRobotsSet);

		void run_extended_empty_gen (bool activate_trigger, int number_of_robots, ... );
		void run_extended_empty_generator_for_set_of_robots_and_wait_for_task_termination_message_of_another_set_of_robots
		(int number_of_robots_to_move, int number_of_robots_to_wait_for_task_termin, ... );
		void run_extended_empty_generator_for_set_of_robots_and_wait_for_task_termination_message_of_another_set_of_robots
		(int number_of_robots_to_move, int number_of_robots_to_wait_for_task_termin, lib::ROBOT_ENUM *robotsToMove, lib::ROBOT_ENUM *robotsWaitingForTaskTermination);

		void wait_ms (int _ms_delay); // zamiast delay

		// Oczekiwanie na zlecenie START od UI
		void wait_for_start (void);// by Y&W

		// Oczekiwanie na zlecenie STOP od UI
		void wait_for_stop (void);// by Y&W dodany tryb

		// Wystartowanie wszystkich ECP
		void start_all (const common::robots_t & _robot_m);

		// Zatrzymanie wszystkich ECP
		void terminate_all (const common::robots_t & _robot_m );

		// Wyslanie rozkazu do wszystkich ECP
		void execute_all (const common::robots_t & _robot_m);

		// funkcja odbierajaca pulsy z UI lub ECP wykorzystywana w MOVE
		void mp_receive_ui_or_ecp_pulse (common::robots_t & _robot_m, generator::generator& the_generator );

		// obsluga sygnalu
		virtual void catch_signal_in_mp_task(int sig);

		/// Zatrzymanie wszystkich ECP
		static void kill_all_ECP (const common::robots_t & _robot_m);

		/// utworzenie robotow
		virtual void create_robots(void);

		/// methods for MP template to redefine in concrete classes
		virtual void task_initialization(void);
		virtual void main_task_algorithm(void);

	private:
		char ui_pulse_code; // kod pulsu ktory zostal wyslany przez ECP w celu zgloszenia gotowosci do komunikacji (wartosci w impconst.h)
		bool ui_new_pulse; // okresla czy jest nowy puls

		bool check_and_optional_wait_for_new_pulse (WAIT_FOR_NEW_PULSE_MODE process_type, RECEIVE_PULSE_MODE wait_mode);

	public:
#if !defined(USE_MESSIP_SRR)
		int wait_for_name_open(void);
#else
		static int pulse_dispatch(messip_channel_t * ch, void * arg);
#endif
};

task* return_created_mp_task (lib::configurator &_config);

} // namespace task
} // namespace mp
} // namespace mrrocpp

#endif /*MP_TASK_H_*/
