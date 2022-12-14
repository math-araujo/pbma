#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <atomic>
#include <thread>

#include "semaphore.hpp"

namespace physscope
{

class Application
{
public:
    Application() = default;
    virtual ~Application() = default;

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    // Run application: should only be called once.
    void run();

protected:
    // Return true if the application is running
    bool is_running() const;

    /*
    Return true if the application is currently
    set to animate the objects.
    This member-function is thread-safe.
    */
    bool is_animating() const;

    /*
    Start the animation. This member-function is thread-safe.
    */
    void start_animation();

    /*
    Pause the animation. This member-function is thread-safe.
    */
    void pause_animation();

    /*
    Restarts the simulation. All state is set to it's initial
    configuration as defined at the initialize() member-function.
    If you initialized data outside initialize(), it will not be
    affected by a call to restart().
    */
    void restart();

    /*
    This member-function specifies the default settings for
    rendering using Polyscope. It can be overridden to change the
    functionality on derived classes.
    */
    virtual void set_polyscope_settings();

    /*
    This member-function allows derived classes to initialize
    Polyscope-related objects. Polyscope structures can be
    constructed and accessed inside this function.
    */
    virtual void initialize();

    /*
    Implements physics simulation/animation.
    Each derived class specifies it's own rules and logics
    based on the Application objective.
    E.g. animate a ball (i.e. it's center) falling due to
    gravity force.
    See template.cpp for an example.
    */
    virtual void physics_update(float delta_time) = 0;

    /*
    Implements custom updates to render-related structures
    before they are draw to the screen. Polyscope structures
    can be constructed and accessed inside this function.
    E.g. apply transform on Polyscope structures based on
    the physics_update output.
    See template.cpp for an example.
    */
    virtual void pre_draw();

private:
    void main_loop();
    void update(float delta_time);
    void user_callback();
    void shutdown();

    std::atomic<bool> running_{true};
    std::atomic<bool> animate_{false};
    physscope::Semaphore empty_{1};
    physscope::Semaphore full_{0};
    physscope::Semaphore mutex_{1};
};

} // namespace physscope

#endif // APPLICATION_HPP