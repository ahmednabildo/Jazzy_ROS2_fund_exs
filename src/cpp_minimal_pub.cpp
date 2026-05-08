// ==========================================
// 1. INCLUDES (The C++ equivalent of Python 'imports')
// ==========================================
// The core ROS 2 client library for C++. It provides the Node class, 
// execution timing, logging, and more.
#include "rclcpp/rclcpp.hpp"

// The specific message type we are using. 
// Notice the lowercase 's' in string.hpp that we fixed earlier!
#include "std_msgs/msg/string.hpp"

// This line allows us to use convenient time suffixes like '500ms' or '2s' 
// later in the code without having to write out complex math.
using namespace std::chrono_literals;

// ==========================================
// 2. NODE CLASS DEFINITION
// ==========================================
// Just like in Python, we create our node by inheriting from the standard 
// ROS 2 'Node' class.
class Minimal_Publisher_CPP : public rclcpp::Node
{
    // Public section: These functions can be called from outside the class 
    // (like in our main() function below).
    public:
        // ------------------------------------------
        // Constructor: This runs exactly once when the node is created.
        // ------------------------------------------
        // We use a C++ "initializer list" (the colon syntax) to pass the 
        // node's name ("minimal_cpp_publisher") to the parent Node class, 
        // and to set our starting 'count' to 0.
        Minimal_Publisher_CPP() : Node("minimal_cpp_publisher") , count(0)
        {
            // Create the publisher. 
            // In C++, we use "templates" (the angle brackets < >) to tell the 
            // compiler exactly what type of message this publisher will send.
            // Arguments: Topic name ("/cpp_example_topic"), Queue size (10).
            publisher = create_publisher<std_msgs::msg::String>("/cpp_example_topic", 10);
            
            // Create the timer.
            // 1. We trigger it every 500 milliseconds.
            // 2. We use 'std::bind' to attach our 'timer_callback' function to the timer.
            //    (std::bind is required in C++ to pass class member functions as callbacks).
            timer = create_wall_timer(500ms, std::bind(&Minimal_Publisher_CPP::timer_callback, this));
            
            // This is the C++ equivalent of self.get_logger().info()
            // RCLCPP_INFO is a "macro" that prints to the console and ROS logs.
            RCLCPP_INFO(get_logger() , "publishing at 2Hz");
        }


                // ------------------------------------------
        // Callback Function
        // ------------------------------------------
        // This runs every 500ms when the timer triggers.
        void timer_callback()
        {
            // Create a new empty message object. The 'auto' keyword tells the 
            // compiler to figure out the type automatically.
            auto message = std_msgs::msg::String();
            
            // Populate the data. We use std::to_string() to convert our integer 
            // 'count' into text, then append it to "Hello World: ".
            // The 'count++' increments the number AFTER it is used.
            message.data = "Hello World: " + std::to_string(count++);
            
            // Physically publish the message to the ROS network.
            // We use the arrow '->' because 'publisher' is a pointer.
            publisher->publish(message);
        }

    // Private section: Variables and functions here are hidden and can only 
    // be used internally by this class.
    private:

    
        // ------------------------------------------
        // Class Variables
        // ------------------------------------------
        // We declare our variables here at the bottom. 
        size_t count;
        
        // In ROS 2 C++, everything is managed using "Shared Pointers" (SharedPtr).
        // This is a smart memory management tool that automatically deletes 
        // the publisher and timer from your computer's RAM when the node shuts down, 
        // preventing memory leaks!
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
        rclcpp::TimerBase::SharedPtr timer;
};

// ==========================================
// 3. MAIN EXECUTION FUNCTION
// ==========================================
// In C++, every program MUST start in the 'main' function.
// argc and argv capture any command-line arguments passed when you run 'ros2 run...'
int main(int argc , char * argv[])
{
    // 1. Initialize ROS 2 communications.
    rclcpp::init(argc , argv);
    
    // 2. Create the node. 
    // We use std::make_shared to create a smart pointer to our node object. 
    // This is the standard, safe way to create nodes in modern C++.
    auto minimal_publisher = std::make_shared<Minimal_Publisher_CPP>();
    
    // 3. Spin the node. This is a blocking loop that keeps the program alive 
    // and processes our 500ms timer callbacks.
    rclcpp::spin(minimal_publisher);
    
    // 4. Shut down and clean up safely after the user presses Ctrl+C.
    rclcpp::shutdown();
    
    // Return 0 tells the Linux operating system the program exited successfully.
    return 0;
}