#!/bin/bash
# The line above is called a "shebang". It tells the operating system 
# to use the bash shell to execute this script.

# ==========================================
# 1. CLEANUP FUNCTION DEFINITION
# ==========================================
# We define a function called 'cleanup' that contains all the commands 
# we want to run when it is time to shut down the script.
cleanup() {
    echo "Cleaning up..."
    
    # The ROS 2 daemon is a background process that helps nodes discover 
    # each other faster. Stopping and starting it clears out stale network 
    # data. (Note: This isn't always strictly necessary, but it ensures a 
    # completely clean slate for your next run).
    ros2 daemon stop
    sleep 1
    ros2 daemon start
    
    echo "Done."
    
    # 'kill 0' is the magic command here. The '0' refers to the current 
    # process group. This command tells bash to kill the script itself AND 
    # any background jobs it started (like your publisher).
    kill 0
    
    # Exit the script safely.
    exit
}

# ==========================================
# 2. SIGNAL TRAPPING
# ==========================================
# The 'trap' command intercepts system signals. 
# Here, we tell it: "If you receive a SIGINT signal (which is what gets 
# sent when the user presses Ctrl+C), do NOT just crash. Instead, run 
# the 'cleanup' function we defined above."
trap 'cleanup' SIGINT 

# ==========================================
# 3. LAUNCHING THE PUBLISHER (BACKGROUND)
# ==========================================
# We use 'ros2 run' to start the publisher node.
# The ampersand '&' at the very end is CRITICAL. It tells bash to run this 
# command in the "background". This means bash starts the publisher and 
# immediately moves on to the next line of the script without waiting for 
# the publisher to finish.
ros2 run ros2_fun_exs minimal_cpp_publisher &

# ==========================================
# 4. DISCOVERY PAUSE
# ==========================================
# We pause the script for 2 seconds. 
# Why? ROS 2 uses a networking protocol (DDS) that requires a fraction of 
# a second for nodes to "discover" each other. This sleep gives the publisher 
# time to fully boot up and register itself on the network before the 
# subscriber starts looking for it.
sleep 2

# ==========================================
# 5. LAUNCHING THE SUBSCRIBER (FOREGROUND)
# ==========================================
# We start the subscriber node. Notice there is NO ampersand '&' here.
# This runs in the "foreground". The script will pause on this line indefinitely, 
# outputting the subscriber's print statements to your terminal. 
# It will stay here until you press Ctrl+C, which triggers the trap and 
# shuts everything down.
ros2 run ros2_fun_exs minimal_cpp_subscriber 