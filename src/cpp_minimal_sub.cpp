// ==========================================
// 1. INCLUDES
// ==========================================
// The core ROS 2 client library for C++.
#include "rclcpp/rclcpp.hpp"

// The specific message type we expect to receive from the publisher. 
#include "std_msgs/msg/string.hpp"

// In C++, when we pass a function (like our callback) to another function 
// (like the subscriber), we have to tell C++ how many arguments that callback 
// expects. '_1' means: "Leave exactly one blank space open for the incoming message."
using std::placeholders::_1;  

// ==========================================
// 2. NODE CLASS DEFINITION
// ==========================================
class Minimal_Subscriber_CPP : public rclcpp::Node
{
    public:
        // ------------------------------------------
        // Constructor
        // ------------------------------------------
        // Initialize the node with the name "minimal_cpp_subscriber"
        Minimal_Subscriber_CPP() : Node("minimal_cpp_subscriber")
        {
            // Create the subscriber. 
            // We use templates < > to define the expected message type.
            subscriber = create_subscription<std_msgs::msg::String>(
                "/cpp_example_topic", // The topic name. MUST match the publisher!
                10,                   // QoS profile / Queue size (buffer up to 10 messages)
                
                // std::bind hooks our 'topic_callback' function to this subscriber.
                // 'this' refers to this specific class instance.
                // '_1' represents the message data that will be passed into the callback.
                std::bind(&Minimal_Subscriber_CPP::topic_callback, this, _1)
            );
        }

        // ------------------------------------------
        // Callback Function
        // ------------------------------------------
        // This function automatically triggers EVERY TIME a message arrives.
        // Notice the argument type: 'const std_msgs::msg::String::SharedPtr msg'
        // Why a SharedPtr? In ROS 2 C++, we pass memory pointers instead of copying 
        // the actual message data. If the message was a massive 4K camera image, 
        // copying it would freeze your computer. Pointers keep it lightning fast!
        // The 'const' ensures we don't accidentally modify the message.
        void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
        {
            // RCLCPP_INFO_STREAM is an alternative to RCLCPP_INFO.
            // It allows us to use the C++ insertion operator (<<) to easily chain 
            // text and variables together, just like 'std::cout'.
            // 
            // 'msg->data' grabs the string payload. 
            // '.c_str()' converts the standard C++ string into a raw C-style string, 
            // which the ROS 2 logger requires to print it to the screen correctly.
            RCLCPP_INFO_STREAM(get_logger() , "I heard: " << msg->data.c_str());
        }

    private:
        // ------------------------------------------
        // Class Variables
        // ------------------------------------------
        // A smart pointer holding our subscriber object in memory. 
        // It automatically cleans itself up when the node is destroyed.
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber;
};

// ==========================================
// 3. MAIN EXECUTION FUNCTION
// ==========================================
int main(int argc , char * argv[])
{
    // 1. Initialize ROS 2 communications.
    rclcpp::init(argc , argv);
    
    // 2. Create the node instance safely using a smart pointer.
    auto minimal_subscriber = std::make_shared<Minimal_Subscriber_CPP>();
    
    // 3. Spin the node. This is a blocking loop that keeps the program alive 
    // so it can continuously listen for incoming network messages.
    rclcpp::spin(minimal_subscriber);
    
    // 4. Shut down and clean up safely after the user presses Ctrl+C.
    rclcpp::shutdown();
    
    // Exit successfully.
    return 0;
}