#!/usr/bin/env python3
# The line above is the "shebang". It tells the operating system 
# to execute this script using the Python 3 interpreter.

"""
Description: 
    A minimal subscriber node in ROS2 using Python.
------------------------------------------------------------------------------
Publishing topics: 
    None
------------------------------------------------------------------------------
Subscription topics:
    channel containing the "Hello World" messages
    /py_example_topic - std_msgs/String
------------------------------------------------------------------------------
Author: 
    Ahmed
------------------------------------------------------------------------------
Date:
    2026-05-05    
"""

# ==========================================
# 1. IMPORTS
# ==========================================
import rclpy                       # The primary ROS 2 Python client library.
from rclpy.node import Node        # The base Node class we will inherit from.
from std_msgs.msg import String    # The specific message type we expect to receive (a standard string).

# ==========================================
# 2. NODE CLASS DEFINITION
# ==========================================
# We define our subscriber by inheriting from the standard ROS 2 'Node' class.
# This Object-Oriented approach is the standard best practice in ROS 2.
class Minimal_PY_Subscriber(Node):
    
    def __init__(self):
        # Call the parent class (Node) constructor and give our node a name.
        # This is the name that will show up when you run 'ros2 node list'.
        super().__init__('minimal_py_subscriber')
        
        # Create the actual subscription. This takes 4 main arguments:
        # 1. Message Type (String)
        # 2. Topic Name ('py_example_topic') - Must match the publisher exactly!
        # 3. Callback Function (self.listener_callback) - What to do when a message arrives.
        # 4. QoS Profile / Queue Size (10) - How many messages to buffer if we process them too slowly.
        self.subscriber_1 = self.create_subscription(
            String,
            'py_example_topic',
            self.listener_callback,
            10
        )
        
    # ==========================================
    # 3. CALLBACK FUNCTION
    # ==========================================
    # This function is automatically triggered EVERY TIME a new message 
    # arrives on the 'py_example_topic'.
    def listener_callback(self, msg):
        # self.get_logger().info() is ROS 2's version of a print statement.
        # It prints to the console but also logs the data to ROS 2's logging system.
        # 'msg' is the actual message object, and '.data' extracts the payload.
        self.get_logger().info(f'I heard: "{msg.data}"')

# ==========================================
# 4. MAIN EXECUTION FUNCTION
# ==========================================
def main(args=None):
    # 1. Initialize the ROS 2 communications system. 
    # This MUST be called before creating any nodes.
    rclpy.init(args=args) 
    
    # 2. Create an instance (object) of our subscriber class.
    minimal_PY_Subscriber = Minimal_PY_Subscriber()
    
    # 3. Spin the node. 'rclpy.spin' is a blocking loop. 
    # It keeps the script alive and constantly checks for incoming network 
    # messages, routing them to your listener_callback. 
    # It will stay stuck on this line until you press Ctrl+C.
    rclpy.spin(minimal_PY_Subscriber) 
    
    # 4. Clean up (Runs after Ctrl+C breaks the spin loop)
    # NOTE: Fixed a bug here! You must destroy the INSTANCE (minimal_PY_Subscriber), 
    # not the CLASS (Minimal_PY_Subscriber).
    minimal_PY_Subscriber.destroy_node() 
    
    # 5. Shut down ROS 2 communications completely for this script.
    rclpy.shutdown()  

# ==========================================
# 5. ENTRY POINT
# ==========================================
# This standard Python check ensures that the main() function is only run 
# if this script is executed directly from the terminal, not if it is 
# imported into another script.
if __name__ == '__main__':
    main()