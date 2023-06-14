
/***************************************************************************
 *  roundtrip_client.cpp - Actionlib roundtrip test C++ server
 *
 *  Created: Wed Aug 11 17:49:09 2010
 *  Copyright  2010  Tim Niemueller [www.niemueller.de]
 *
 ****************************************************************************/

/*  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "ros_actionlib_benchmark/RoundtripAction.h"

#include <cmath>

typedef struct {
  ros::Time start;
  ros::Time rcvd;
  ros::Time now;
} roundtrip_t;

int main (int argc, char **argv)
{
  ros::init(argc, argv, "roundtrip_client"); 

  actionlib::SimpleActionClient<ros_actionlib_benchmark::RoundtripAction> ac("roundtrip", true); 
  ac.waitForServer();
 
  unsigned int num_runs = 1;
  if (argc == 2) {
    num_runs = atoi(argv[1]);
    if (num_runs == 0) num_runs = 1;
  }
  printf("\nC++ Action Client test (%u runs)\n\n", num_runs);

  std::list<roundtrip_t> results;

  for (unsigned int i = 0; i < num_runs && ros::ok(); ++i) {
    ros::Rate rate(20);

    // send a goal to the action 
    ros_actionlib_benchmark::RoundtripGoal goal;
    goal.start = ros::Time::now();
    ac.sendGoal(goal);
  
    //wait for the action to return
    bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    if (finished_before_timeout) {
      ros_actionlib_benchmark::RoundtripResultConstPtr res;
      res = ac.getResult();

      if (num_runs == 1) {
	ros::Time now  = ros::Time::now();
	ros::Time rcvd = res->received;
	float difftime_send  = (rcvd - goal.start).toSec();
	float difftime_recv  = (now - rcvd).toSec();
	float difftime_total = (now - goal.start).toSec();
	printf("Sending time:    %f\n", difftime_send);
	printf("Receiving time:  %f\n", difftime_recv);
	printf("Round trip time: %f\n", difftime_total);
      } else {
	roundtrip_t entry;
	entry.start = goal.start;
	entry.rcvd  = res->received;
	entry.now   = ros::Time::now();
	results.push_back(entry);
      }
    } else {
      ROS_INFO("Action did not finish before the time out.");
    }

    rate.sleep();
  }

  if (! results.empty()) {
    float average_send = 0., average_recv = 0., average_total = 0.;
    size_t n = results.size();
    for (std::list<roundtrip_t>::iterator i = results.begin(); i != results.end(); ++i) {
      average_send  += (i->rcvd - i->start).toSec();
      average_recv  += (i->now  - i->rcvd).toSec();
      average_total += (i->now  - i->start).toSec();
    }
    average_send  /= n;
    average_recv  /= n;
    average_total /= n;

    float deviation_send = 0., deviation_recv = 0., deviation_total = 0.;
    for (std::list<roundtrip_t>::iterator i = results.begin(); i != results.end(); ++i) {
      deviation_send  += fabs((i->rcvd - i->start).toSec() - average_send);
      deviation_recv  += fabs((i->now  - i->rcvd).toSec() - average_recv);
      deviation_total += fabs((i->now  - i->start).toSec() - average_total);
    }
    deviation_send  /= n;
    deviation_recv  /= n;
    deviation_total /= n;

    printf("SEND   Average: %f    Deviation: %f\n", average_send, deviation_send);
    printf("RECV   Average: %f    Deviation: %f\n", average_recv, deviation_recv);
    printf("TOTAL  Average: %f    Deviation: %f\n", average_total, deviation_total);
  }

  return 0;
}
