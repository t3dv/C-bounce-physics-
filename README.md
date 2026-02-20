2D Physics Sandbox
This is a real-time physics simulation built from scratch using C++ and SFML 3. Instead of using industry standards like std::vector, I instead opted for a  custom Singly Linked List to manage the lifecycle of physics objects, focusing on manual memory management and pointer logic.

Technical Architecture
    Custom Memory Management: Manages entity lifecycles through a linked list, demonstrating proficiency in heap allocation and pointer arithmetic.
    Kinematic Logic: Implements frame-independent movement via Delta Time (dt) to ensure consistent speed and acceleration regardless of  refresh rates.
    Collision Resolution: Handles boundary constraints and "resting" states to eliminate bouncing at low velocities.


Current State & Roadmap
    v1.0 (Current): Functional sandbox with boundary collisions and dynamic spawning.
    v1.1 (Current): Elastic ball-on-ball collisions using vector
    v1.2 (Upcomming): Custom mass and more user input

How to Run
    Ensure SFML 3 is installed and linked.
    Build using the provided CMakeLists.txt.
    Left Click: Spawn a ball at the mouse coordinates.
