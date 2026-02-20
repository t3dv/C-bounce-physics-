2D Kinematic Physics Sandbox

This is a real-time physics simulation built from scratch using C++ and SFML 3. Instead of using standard containers like std::vector, I implemented a custom Singly Linked List to manage the lifecycle of physics objects, focusing on manual memory management and pointer logic.

Technical Architecture

    Custom Memory Management: Manages entity lifecycles through a manual node-based linked list, demonstrating proficiency in heap allocation and pointer arithmetic.
    Kinematic Logic: Implements frame-independent movement via Delta Time (dt) to ensure consistent velocity and acceleration regardless of hardware refresh rates.
    Collision Resolution: Handles boundary constraints using a custom solver that accounts for restitution (bounce coefficients) and "resting" states to eliminate jitter at low velocities.

Current State & Roadmap

    v1.0 (Current): Functional sandbox with boundary collisions and dynamic spawning.
    v1.1 (Planned): Elastic ball-on-ball collisions using vector projection and momentum conservation.

How to Run

    Ensure SFML 3 is installed and linked.
    Build using the provided CMakeLists.txt.
    Left Click: Spawn a ball at the mouse coordinates.
