// -------------------------------------------------------------------------
//                            ui_class.h
// Definicje klasy Ui
//
// Ostatnia modyfikacja: 2010
// -------------------------------------------------------------------------

#ifndef __UI_R_POLYCRANK_H
#define __UI_R_POLYCRANK_H

#include "../base/ui.h"
#include "../base/ui_robot.h"
#include "robot/polycrank/const_polycrank.h"

//class wgt_spkm_inc;
class wgt_polycrank_int;

namespace mrrocpp {
namespace ui {
namespace common {
class Interface;
}

namespace tfg_and_conv {
class EcpRobot;
}
namespace polycrank {

//
//
// KLASA UiRobotPolycrank
//
//

class UiRobot : public common::UiRobot
{
private:

public:
	double current_pos[lib::polycrank::NUM_OF_SERVOS]; // pozycja biezaca
	double desired_pos[lib::polycrank::NUM_OF_SERVOS]; // pozycja zadana

	//bool is_wind_polycrank_int_open; // informacja czy okno ruchow w radianach stawow jest otwarte

	tfg_and_conv::EcpRobot *ui_ecp_robot;
	wgt_polycrank_int *wgt_int;
	//wgt_spkm_inc *wgt_int;

	UiRobot(common::Interface& _interface);
	void close_all_windows();

	int manage_interface();
	void delete_ui_ecp_robot();
	int synchronise();
	int synchronise_int();

	void edp_create();
	int edp_create_int();

};

}
} //namespace ui
} //namespace mrrocpp

#endif

