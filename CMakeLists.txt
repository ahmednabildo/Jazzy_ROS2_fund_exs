# ==========================================
# 1. PROJECT SETUP
# ==========================================
# Specify the minimum version of CMake required to build this project.
cmake_minimum_required(VERSION 3.8)

# Define the name of the package. This MUST match the name in your package.xml.
project(ros2_fun_exs)

# ==========================================
# 2. COMPILER FLAGS (C++)
# ==========================================
# If we are using GCC or Clang to compile C++ code, turn on strict compiler warnings.
# -Wall, -Wextra, and -Wpedantic help catch bugs and bad practices early.
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ==========================================
# 3. DEPENDENCIES
# ==========================================
# 'find_package' tells CMake to look for other ROS 2 packages installed on your system.
# We need 'ament_cmake' because this is an ament-based build system.
# We need 'ament_cmake_python' to help us install Python modules correctly.
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)

# These are the actual ROS 2 libraries our nodes will use.
find_package(rclcpp REQUIRED)    # C++ ROS 2 client library
find_package(rclpy REQUIRED)     # Python ROS 2 client library
find_package(std_msgs REQUIRED)  # Standard message types (like String)

# We group our dependencies into a CMake variable called 'dependencies'.
# This makes it easier to apply them to multiple C++ executables later without typing them all out.
set(dependencies
  rclcpp
  rclpy
  std_msgs
)

# Tell the C++ compiler where to look for header (.hpp) files.
include_directories(
  include
)

# ==========================================
# 4. C++ EXECUTABLES (Currently Commented Out)
# ==========================================
# To build C++ nodes, you use 'add_executable' to compile the source code, 
# and 'ament_target_dependencies' to link the libraries.

add_executable(minimal_cpp_publisher src/cpp_minimal_pub.cpp)
ament_target_dependencies(minimal_cpp_publisher ${dependencies})

# NOTE: I spotted a small typo here! Your subscriber was pointing to the publisher's source file.
# It should likely be: src/cpp_minimal_subscriber.cpp
add_executable(minimal_cpp_subscriber src/cpp_minimal_sub.cpp)
ament_target_dependencies(minimal_cpp_subscriber  ${dependencies})

# ==========================================
# 5. INSTALLATION RULES (Where files go after 'colcon build')
# ==========================================

# 5A. Install Directories
# This copies entire folders into the 'share' directory of your ROS 2 workspace.
# The 'share' directory is where ROS 2 looks for launch files, URDFs, and custom scripts.
install(
  DIRECTORY ros2_fun_exs scripts
  DESTINATION share/${PROJECT_NAME}
)

# Note: This is slightly redundant since you already installed 'ros2_fun_exs' above, 
# but it doesn't hurt anything!
install(
  DIRECTORY ros2_fun_exs
  DESTINATION share/${PROJECT_NAME}
)

# Install C++ header files so other packages can '#include' them.
install(
  DIRECTORY include/
  DESTINATION include
)

# 5B. Install C++ Executables (Currently Commented Out)
# This places your compiled C++ programs into the 'lib' directory so 'ros2 run' can find them.
install(
  TARGETS
  minimal_cpp_publisher
  minimal_cpp_subscriber
  DESTINATION lib/${PROJECT_NAME}
)

# 5C. Install Python Library Modules
# If you have standard Python files (like a custom math library) inside a folder called 'ros2_fun_exs',
# this command installs them so that other Python scripts can do: 'from ros2_fun_exs import my_module'
ament_python_install_package(${PROJECT_NAME}) 

# 5D. Install Python Executable Scripts
# THIS IS THE MOST IMPORTANT PART FOR YOUR PYTHON NODES.
# 'install(PROGRAMS ...)' copies your Python scripts into the 'lib' directory and 
# ensures they are marked as executable (chmod +x). 
# Without this, 'ros2 run ros2_fun_exs py_minimal_pub.py' will fail saying "executable not found".
install(
  PROGRAMS
  ros2_fun_exs/py_minimal_pub.py
  ros2_fun_exs/py_minimal_sub.py
  DESTINATION lib/${PROJECT_NAME}
)

# ==========================================
# 6. TESTING & LINTING
# ==========================================
# This block sets up automated code testing. 
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # We are skipping copyright and cpplint checks here, which is standard 
  # for learning/hobby projects to prevent annoying build warnings.
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# ==========================================
# 7. EXPORTING & FINALIZING
# ==========================================
# Tell ROS 2 that other packages depend on this package, they should 
# include this package's 'include' directory.
ament_export_include_directories(include)

# This must be the absolute LAST command in the file.
# It gathers all the install rules, exports, and dependencies, and registers 
# this package in the 'ament index' so ROS 2 knows it exists!
ament_package()