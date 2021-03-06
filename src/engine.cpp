#include <thread>
#include "engine.h"

const int robot::engine::STEP_FORWARD_ROTATION = 100;
const int robot::engine::TURN_NINETY_DEGREES = 220;
const int robot::engine::ENGINE_MAX_SPEED = 1050;
const int robot::engine::BASE_SPEED = 100;
const int robot::engine::WAIT_TIME_MILLISECONDS = 10;

robot::engine::engine() : gyro_sensor{ev3dev::INPUT_2},
                          left_motor{ev3dev::OUTPUT_D},
                          right_motor{ev3dev::OUTPUT_A} {}

void robot::engine::turn_left() {
    left_motor.set_position_sp(-TURN_NINETY_DEGREES)
            .set_speed_sp(BASE_SPEED)
            .run_to_rel_pos();
    right_motor.set_position_sp(TURN_NINETY_DEGREES)
            .set_speed_sp(BASE_SPEED)
            .run_to_rel_pos();

    while (left_motor.state().count("running") ||
           right_motor.state().count("running")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME_MILLISECONDS));
    }
}

void robot::engine::turn(int degrees) {
    stop();

    if (degrees == 0) return;

    /* Reset sensor */
    gyro_sensor.set_mode(ev3dev::gyro_sensor::mode_gyro_rate);
    gyro_sensor.set_mode(ev3dev::gyro_sensor::mode_gyro_ang);

    auto speed = (degrees > 0) ? -BASE_SPEED : BASE_SPEED;

    left_motor.set_speed_sp(-speed).run_forever();
    right_motor.set_speed_sp(speed).run_forever();

    while (abs(gyro_sensor.value()) < abs(degrees)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME_MILLISECONDS));
    }
    stop();
}

void robot::engine::stop() {
    left_motor.stop();
    right_motor.stop();
}

void robot::engine::set_speed(double left_correction,
                              double right_correction) {
    int left_speed = left_correction + BASE_SPEED;
    int right_speed = right_correction + BASE_SPEED;
    /* Throws error if engine exceed ENGINE_MAX_SPEED */
    left_speed = std::max(left_speed, -ENGINE_MAX_SPEED);
    left_speed = std::min(left_speed, ENGINE_MAX_SPEED);
    right_speed = std::max(right_speed, -ENGINE_MAX_SPEED);
    right_speed = std::min(right_speed, ENGINE_MAX_SPEED);

    left_motor.set_speed_sp(left_speed).run_forever();
    right_motor.set_speed_sp(right_speed).run_forever();
//    std::cout << "left_correction speed " << left_speed << std::endl;
//    std::cout << "right_correction speed " << right_speed << std::endl;
}

void robot::engine::move() {
    stop();
    left_motor.set_position_sp(STEP_FORWARD_ROTATION)
            .set_speed_sp(BASE_SPEED)
            .run_to_rel_pos();
    right_motor.set_position_sp(STEP_FORWARD_ROTATION)
            .set_speed_sp(BASE_SPEED)
            .run_to_rel_pos();
    while (left_motor.state().count("running") ||
           right_motor.state().count("running")) {
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME_MILLISECONDS));
    }
    stop();
}


