#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "can_plugins/Frame.h"
#include "can_utils.hpp"
#include<math.h>

void  chatterCallback(const sensor_msgs::Joy::ConstPtr& msg);

ros::Publisher* chatter;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "controler4");
  
  ros::NodeHandle n;

  ros::Publisher lchatter = n.advertise<can_plugins::Frame>("can_tx",1000);

  chatter = &lchatter;

  ros::Subscriber sub = n.subscribe("joy", 1000, chatterCallback);

  ros::spin();

  return 0;
}

void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
  ROS_INFO("I heard: [%f]", msg->axes[0]);
  
  if(msg->buttons[1]==1)
  {
    chatter->publish(get_frame(0x100,static_cast<uint8_t>(5)));
    chatter->publish(get_frame(0x110,static_cast<uint8_t>(5)));
    chatter->publish(get_frame(0x120,static_cast<uint8_t>(5)));
    chatter->publish(get_frame(0x130,static_cast<uint8_t>(5)));
  }
  //↑mode_velへ移行

  //can_plugins::Frame a = get_frame(0x101, 1.0f);
  if(msg->buttons[4]==1)
  {
    chatter->publish(get_frame(0x101, 1.0f));
    chatter->publish(get_frame(0x111, 1.0f));
    chatter->publish(get_frame(0x121, 1.0f));
    chatter->publish(get_frame(0x131, 1.0f));
  } 
  else if(msg->buttons[4]==0)
  {
    chatter->publish(get_frame(0x101, 0.0f));
    chatter->publish(get_frame(0x111, 0.0f));
    chatter->publish(get_frame(0x121, 0.0f));
    chatter->publish(get_frame(0x131, 0.0f));
  }
  //↑左回転
  if(msg->buttons[5]==1)
  {
    chatter->publish(get_frame(0x121, -1.0f));
    chatter->publish(get_frame(0x121, -1.0f));
    chatter->publish(get_frame(0x121, -1.0f));
    chatter->publish(get_frame(0x121, -1.0f));
  }
  else if(msg->buttons[5]==0)
  {
    chatter->publish(get_frame(0x101, 0.0f));
    chatter->publish(get_frame(0x111, 0.0f));
    chatter->publish(get_frame(0x121, 0.0f));
    chatter->publish(get_frame(0x131, 0.0f));
  }
  //↑右回転
  float x= -(msg->axes[0]);
  float y=  (msg->axes[1]);
  chatter->publish(get_frame(0x101, y-x));
  chatter->publish(get_frame(0x111, x+y));
  chatter->publish(get_frame(0x121, x-y));
  chatter->publish(get_frame(0x131, -x-y));
  //chatter.publish(get_frame(0x101, x/static_cast<float>(sqrt(2))-y/static_cast<float>(sqrt(2))));
  //100右上、110左上、120左下、130右下
}