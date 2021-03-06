/*
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ros/ros.h"
#include "pluginlib/class_list_macros.h" 
#include "nodelet/nodelet.h" 
#include <geometry_msgs/Twist.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "dynamic_reconfigure/server.h"
#include "turtlebot_follower/FollowerConfig.h"
#include <turtlebot_follower/Relationship.h>
#include <iostream>

enum { SPIN_CW = 1, SPIN_CCW = 2, FORWARD = 3, BACKWARD = 4, COW = 5}; 

namespace turtlebot_follower
{
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;


//* The turtlebot follower nodelet.
/**
 * The turtlebot follower nodelet. Subscribes to point clouds
 * from the kinect, processes them, and publishes command vel
 * messages.
 */
class TurtlebotFollower : public nodelet::Nodelet 
{
public:
  /*!
   * @brief The constructor for the follower.
   * Constructor for the follower.
   */
  TurtlebotFollower() : min_y_(0.1), max_y_(0.5),
                        min_x_(-0.2), max_x_(0.2),
                        max_z_(0.8), goal_z_(0.6),
                        z_scale_(1.0), x_scale_(5.0) 
  {
    
  }

  ~TurtlebotFollower()
  {
    delete srv_;
  }
  
private:
  double min_y_; /**< The minimum y position of the points in the box. */
  double max_y_; /**< The maximum y position of the points in the box. */
  double min_x_; /**< The minimum x position of the points in the box. */
  double max_x_; /**< The maximum x position of the points in the box. */
  double max_z_; /**< The maximum z position of the points in the box. */
  double goal_z_; /**< The distance away from the robot to hold the centroid */
  double z_scale_; /**< The scaling factor for translational robot speed */
  double x_scale_; /**< The scaling factor for rotational robot speed */


  // Dynamic reconfigure server
  dynamic_reconfigure::Server<turtlebot_follower::FollowerConfig>* srv_;
  
  /*!
   * @brief OnInit method from node handle.
   * OnInit method from node handle. Sets up the parameters
   * and topics.
   */
  virtual void onInit() 
  {
    ros::NodeHandle& nh = getNodeHandle();
    ros::NodeHandle& private_nh = getPrivateNodeHandle();
    
    private_nh.getParam("min_y", min_y_);
    private_nh.getParam("max_y", max_y_);
    private_nh.getParam("min_x", min_x_);
    private_nh.getParam("max_z", max_z_);
    private_nh.getParam("goal_z", goal_z_);
    private_nh.getParam("z_scale", z_scale_);
    private_nh.getParam("x_scale", x_scale_);
    
    cmdpub_ = nh.advertise<geometry_msgs::Twist> ("cmd_vel", 1);
    sub_= nh.subscribe<PointCloud>("/camera/depth/points", 1, &TurtlebotFollower::cloudcb, this);

    relationship_sub_ = nh.subscribe<turtlebot_follower::Relationship>("/stim_react", 1, &TurtlebotFollower::processStimuli, this);
//make another subscriber to subscribe to topic that Android publishes relationships to
//callback gets objects 

    srv_ = new dynamic_reconfigure::Server<turtlebot_follower::FollowerConfig>(private_nh);
    dynamic_reconfigure::Server<turtlebot_follower::FollowerConfig>::CallbackType f = boost::bind(&TurtlebotFollower::reconfigure, this, _1, _2);
    srv_->setCallback(f);

  }

  void processStimuli(const turtlebot_follower::Relationship::ConstPtr& msg)
  {
    //ROS_INFO("%s", msg->prox2);
    /*for(int i=0; i < msg->prox2.size(); i++) 
    {
          ROS_INFO("%s", msg->prox2[i].c_str());
    }
    */
   stim_prox2_ = msg->prox2;
   stim_prox4_ = msg->prox4;
    
  }

  void react(int reaction, double z) 
  {
    ROS_INFO("%i", reaction);
    geometry_msgs::Twist cmd;
    switch(reaction)
    {
      case SPIN_CW:
        cmd.linear.x = 0;
        cmd.angular.z = -6;
        break;
      case SPIN_CCW:
        cmd.linear.x = 0;
        cmd.angular.z = 6;
        break;
      case FORWARD:
        cmd.linear.x = (z - goal_z_) * z_scale_;
        cmd.angular.z = 0;
        break;
      case BACKWARD:
        cmd.linear.x = -5;
        cmd.angular.z = 0;
        break;
      case COW:
        system("/opt/ros/electric/stacks/turtlebot_apps/turtlebot_follower/src/chirp.bash");
        break;
      default:
        cmd.linear.x = 0;
        cmd.angular.z = 0;
        ROS_INFO("%s", "NO SUCH THING.");
        break;
    }
    cmdpub_.publish(cmd);
      
  } 

  void reconfigure(turtlebot_follower::FollowerConfig &config, uint32_t level)
  {
    min_y_ = config.min_y;
    max_y_ = config.max_y;
    min_x_ = config.min_x;
    max_x_ = config.max_x;
    max_z_ = config.max_z;
    goal_z_ = config.goal_z;
    z_scale_ = config.z_scale;
    x_scale_ = config.x_scale;
  }
  
  /*!
   * @brief Callback for point clouds.
   * Callback for point clouds. Uses PCL to find the centroid
   * of the points in a box in the center of the point cloud.
   * Publishes cmd_vel messages with the goal from the cloud.
   * @param cloud The point cloud message.
   */
  void cloudcb(const PointCloud::ConstPtr&  cloud)
  {
    //X,Y,Z of the centroid
    double x_2 = 0.0;
    double y_2 = 0.0;
    double z_2 = 0.0;

    double x_4 = 0.0;
    double y_4 = 0.0;
    double z_4 = 0.0;
    //Number of points observed
    unsigned int n = 0;
    unsigned int m = 0;
    //Iterate through all the points in the region and find the average of the position
    BOOST_FOREACH (const pcl::PointXYZ& pt, cloud->points)
    {
      //First, ensure that the point's position is valid. This must be done in a seperate
      //if because we do not want to perform comparison on a nan value.
        //Test to ensure the point is within the aceptable box.
        //add min_z_
        if (!std::isnan(x_2) && !std::isnan(y_2) && !std::isnan(z_2) && -pt.y > min_y_ && -pt.y < max_y_ && pt.x < max_x_ && pt.x > min_x_ && pt.z < max_z_)
        {
          //Add the point to the totals
          x_2 += pt.x;
          y_2 += pt.y;
          z_2 += pt.z;
          n++;
        }
        if (!std::isnan(x_4) && !std::isnan(y_4) && !std::isnan(z_4) && -pt.y > min_y_ && -pt.y < max_y_ && pt.x < max_x_ && pt.x > min_x_ && pt.z < 4.0 && pt.z > 1.0)
        {
          x_4 += pt.x;
          y_4 += pt.y;
          z_4 += pt.z;
          m++;
        } 
    }
    
    //If there are points, find the centroid and calculate the command goal.
    //If there are no points, simply publish a stop goal.
    if (n>=m && n>0)
    { 
      x_2 /= n; 
      y_2 /= n; 
      z_2 /= n;  
      
      ROS_DEBUG("Centriod at %f %f %f with %d points", x_2, y_2, z_2, n);
      
      BOOST_FOREACH (const int reaction, stim_prox2_)
      {
        ROS_INFO("%i", reaction);
	react(reaction, z_2);
	ros::Duration(0.5).sleep();
	
      }


//      geometry_msgs::Twist cmd;
//      cmd.linear.x = (z - goal_z_) * z_scale_;
//      cmd.angular.z = -x * x_scale_;
      //cmdpub_.publish(cmd);

//check the objects for proximity2, object.proximity2,
//loop through object.prox2 
//make react function containing switch statement

    }
    else if (n<m)
    {
      x_4 /= m;
      y_4 /= m;
      z_4 /= m;

      BOOST_FOREACH (const int reaction, stim_prox4_)
      {
        ROS_INFO("%i", reaction);
	react(reaction, z_4);
	ros::Duration(0.5).sleep();
	
      }
    }    
    else
    {
      ROS_DEBUG("No points detected, stopping the robot");
      cmdpub_.publish(geometry_msgs::Twist());
    }
  }
  
  
  ros::Subscriber sub_;
  ros::Subscriber relationship_sub_;
  ros::Publisher cmdpub_;
  std::vector<int> stim_prox2_; 
  std::vector<int> stim_prox4_;
};

PLUGINLIB_DECLARE_CLASS(turtlebot_follower, TurtlebotFollower, turtlebot_follower::TurtlebotFollower, nodelet::Nodelet);

}
