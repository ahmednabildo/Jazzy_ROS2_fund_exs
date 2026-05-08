#!/usr/bin/env python3
# The line above is the "shebang". It tells the operating system 
# to execute this script using the Python 3 interpreter.

"""
Description: 
    This ROS 2 node is a simple publisher that broadcasts a "Hello World" 
    message continuously.
------------------------------------------------------------------------------
Publishing topics:
    Channel containing the "Hello World" messages
    py_example_topic - std_msgs/String
------------------------------------------------------------------------------
Subscription Topics:
    None
------------------------------------------------------------------------------
Author: Ahmed Nabil
Date: 2026-05-04
"""

# ==========================================
# 1. IMPORTS
# ==========================================
import rclpy                        # The primary ROS 2 Python client library.
from rclpy.node import Node         # The base Node class we will inherit from.
from std_msgs.msg import String     # The specific message type we are sending.

# ==========================================
# 2. NODE CLASS DEFINITION
# ==========================================
# We define our publisher by inheriting from the standard ROS 2 'Node' class.
class Minimal_py_publisher(Node):
    
    def __init__(self):
        # Call the parent class (Node) constructor and give our node a name.
        # This is the name that will show up when you run 'ros2 node list'.
        super().__init__('minimal_py_publisher')

        # Create the actual publisher. This takes 3 main arguments:
        # 1. Message Type (String)
        # 2. Topic Name ('py_example_topic') - Notice there is no leading slash.
        #    This makes it a "relative" topic, which is standard best practice.
        # 3. QoS Profile / Queue Size (10) - If we publish faster than the network 
        #    can send, ROS will queue up to 10 messages before dropping the oldest.
        self.publisher_1 = self.create_publisher(
            String, 
            'py_example_topic', 
            10
        )

        # Instead of an infinite 'while' loop (which would freeze ROS 2), 
        # we use a Timer. This tells ROS 2 to run our 'timer_callback' 
        # function exactly once every 0.5 seconds.
        timer_period = 0.5 
        self.timer = self.create_timer(timer_period, self.timer_callback)

        # Initialize a counter variable so our messages are unique.
        self.i = 0

    # ==========================================
    # 3. CALLBACK FUNCTION
    # ==========================================
    def timer_callback(self):
        """
        Callback function executed periodically by the timer.
        """
        # Create a new, empty String message object.
        msg = String()

        # Populate the '.data' field of the message with our text and counter.
        msg.data = 'Hello World: %d' % self.i
        
        # Physically send the message out to the DDS network.
        self.publisher_1.publish(msg)

        # Print to the console (and ROS logs) so we can see it working.
        self.get_logger().info('Publishing: "%s"' % msg.data)
        
        # Increment the counter for the next time the timer fires.
        self.i += 1

# ==========================================
# 4. MAIN EXECUTION FUNCTION
# ==========================================
def main(args=None):
    """
    Main function to start the ROS 2 node.
    """
    # 1. Initialize the ROS 2 communications system. 
    rclpy.init(args=args)

    # 2. Create an instance of our publisher class.
    minimal_py_publisher = Minimal_py_publisher()

    # 3. Spin the node. 'rclpy.spin' is a blocking loop.
    # It keeps the script alive and manages the timing, ensuring our 
    # timer_callback fires exactly every 0.5 seconds.
    rclpy.spin(minimal_py_publisher)

    # 4. Clean up (Runs after Ctrl+C breaks the spin loop).
    minimal_py_publisher.destroy_node()

    # 5. Shut down ROS 2 communications completely for this script.
    rclpy.shutdown()    

# ==========================================
# 5. ENTRY POINT
# ==========================================
if __name__ == '__main__':
    # Execute the main function only if this script is run directly.
    main()