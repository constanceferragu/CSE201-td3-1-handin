#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* array_2 = new double[new_size];
    for (int i = 0; i<length; i++){
        array_2[i] = array[i];
    }
    delete array;
    return array_2; // YOU CAN CHANGE THIS
}


double* shrink_array(double* array, int length, int new_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    double* array_2 = new double[new_size];
    for (int i = 0; i<new_size; i++){
        array_2[i] = array[i];
    }
    delete array;
    return array_2; // YOU CAN CHANGE THIS
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  // IMPLEMENT YOUR FUNCTION HERE
    if (current_size == max_size){
        max_size = max_size + 5;
        array = extend_array(array, current_size, max_size);

    }
    current_size++;
    array[current_size-1] = element;
    return array; // YOU CAN CHANGE THIS
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
    // IMPLEMENT YOUR FUNCTION HERE
    if (current_size == 0){
        return array;
    }
    current_size--;
    if (max_size - current_size >= 5){
        max_size = max_size-5;
        array = shrink_array(array, current_size, max_size);
    }
    return array; // YOU CAN CHANGE THIS
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;
  telemetry = append_to_array(0,telemetry, telemetry_current_size, telemetry_max_size);
  telemetry = append_to_array(0,telemetry, telemetry_current_size, telemetry_max_size);
  telemetry = append_to_array(0,telemetry, telemetry_current_size, telemetry_max_size);

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
      telemetry = append_to_array(t,telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(x,telemetry, telemetry_current_size, telemetry_max_size);
      telemetry = append_to_array(y,telemetry, telemetry_current_size, telemetry_max_size);
    }
  }
  if(y<0){
    telemetry = remove_from_array(telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = remove_from_array(telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = remove_from_array(telemetry, telemetry_current_size, telemetry_max_size);
  }
  return hit_target;
}

void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &telemetry,
                     int &telemetry_current_size,
                     int &telemetry_max_size) {
    // IMPLEMENT YOUR FUNCTION HERE


    double temp1;
    double temp2;
    double temp3;
    for(int i =  0; i < tot_telemetries; i++){
        for(int j = 0; j < telemetries_sizes[i]; j = j+3){
            telemetry = append_to_array(telemetries[i][j],telemetry, telemetry_current_size, telemetry_max_size);
            telemetry = append_to_array(telemetries[i][j+1],telemetry, telemetry_current_size, telemetry_max_size);
            telemetry = append_to_array(telemetries[i][j+2],telemetry, telemetry_current_size, telemetry_max_size);
        }
    }

    for(int i =  0; i < telemetry_current_size; i=i+3){
        for(int j = i+3; j < telemetry_current_size; j = j+3){
            if(telemetry[i]>telemetry[j]){
                temp1 = telemetry[i];
                temp2 = telemetry[i+1];
                temp3 = telemetry[i+2];
                telemetry[i] = telemetry[j];
                telemetry[i+1] = telemetry[j+1];
                telemetry[i+2] = telemetry[j+2];
                telemetry[j] = temp1;
                telemetry[j+1]=temp2;
                telemetry[j+2]=temp3;
            }
        }
    }
}







