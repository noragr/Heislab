#include "channels.h"
#include "elev.h"
#include "io.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int initializer(){
	int floor = elev_get_floor_sensor_signal();
	printf("%d\n", floor);

	if (floor == -1){
		while (floor == -1){
		elev_set_motor_direction(DIRN_DOWN);
		floor = elev_get_floor_sensor_signal();
		printf("Hei\n");
		}	
	}
	elev_set_motor_direction(DIRN_STOP);
	elev_set_floor_indicator(floor);
	return floor;
}
 //Queue matrix
int queue[N_FLOORS][4] = {
	{0, 0, 0, 0} ,
	{0, 0, 0, 0} ,
	{0, 0, 0, 0} ,
	{0, 0, 0, 0} ,
};

int get_order(int floor, elev_motor_direction_t dir){
	if (dir == DIRN_UP) {// direction up
		for (int i = floor+1; i < N_FLOORS; i++) {
			if (queue[0][i] == 1 || queue[1][i]==1) {
				return i;
			}
		}
	} else if (dir == DIRN_DOWN){ // direction down 
		for (int i = floor-1; i > -1; i--) {
			if (queue[0][i] == 1 || queue[1][i] == 1) {
				return i;
			}
		}
	}
	return -1; // No orders to manage. 
}

elev_motor_direction_t get_direction(int pos, elev_motor_direction_t dir) {
	if (get_order(pos, dir) > pos) {
		return DIRN_UP;
	}else if (get_order(pos, dir) < pos) {
		return DIRN_DOWN;
	}
	return DIRN_STOP;
}


void set_order(int pos, int floor, elev_motor_direction_t dir, int button){ // tar inn en etasje og retning. 
	if (button == BUTTON_CALL_UP) {
		queue[0][floor] = 1;
		elev_set_button_lamp(BUTTON_CALL_UP, floor, 1);
		// mer
	}
	if (button == 0){
		queue[1][floor] = 1;
	} 
	if (button == 1) {
		if (pos < floor) {
			queue[0][floor]=1;
		}else {
			queue[1][floor] = 1;
		}
	}

	// husk å sette lys også!! Må bruke elev_set_button_lamp()
	//void elev_set_button_lamp(elev_button_type_t button, int floor, int value);
}

void delete_order(int floor){
	for (int i = 0; i < 4; i++) {
		queue[i][floor] =0;
	}
}