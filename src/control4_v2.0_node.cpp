#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "can_plugins/Frame.h"
#include "can_utils.hpp"
#include<math.h>

void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg);

ros::Publisher* chatter; 

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controler4_ver2.0");

  ros::NodeHandle n;

  chatter = &(n.advertise<can_plugins::Frame>("can_tx",1000));

  ros::Subscriber sub = n.subscribe("Joy", 1000, chatterCallback);

  ros::spin();

  return 0;
}

void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{ 
  if(msg->buttons[1]==1)
  {
    chatter->publish(get_frame(0x100,static_cast<uint8_t>(3)));
  }
  //↑mode_velへ移行

  //can_plugins::Frame a = get_frame(0x101, 1.0f);
  float x= -(msg->axes[0]);
  float y=  (msg->axes[1]);
  float r= 0;
  if(msg->buttons[4]==1)
  {
    r =1.0f;
  }
  //↑左回転
  if(msg->buttons[5]==1)
  {
    r =-1.0f;
  }
  //右回転
  chatter->publish(get_frame(0x101, y-x+r/2));
  chatter->publish(get_frame(0x111, x+y+r/2));
  chatter->publish(get_frame(0x121, x-y+r/2));
  chatter->publish(get_frame(0x131, -x-y+r/2));
  //chatter.publish(get_frame(0x101, x/static_cast<float>(sqrt(2))-y/static_cast<float>(sqrt(2))));
  //100右上、110左上、120左下、130右下
}