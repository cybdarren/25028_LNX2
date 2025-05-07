
#include <memory>

#include <egt/ui>
#include <egt/uiloader.h>
#include <iostream>
 
int main(int argc, char** argv)
{
    egt::Application app(argc, argv);
 
    egt::experimental::UiLoader loader;
    auto window = std::static_pointer_cast<egt::Window>(loader.load("file:ui.xml"));
    if (!window)
    {
        std::cerr << "Error: Unable to load UI from ui.xml\n";
        return 1;
    }
 
    auto spinProgress = window->find_child<egt::SpinProgress>("SpinProgress1");
    auto button = window->find_child<egt::Button>("Button1");

    if (!spinProgress || !button)
    {
        std::cerr << "Error: UI elements not found\n";
        return 1;
    }

    egt::v1::PeriodicTimer animateTimer(std::chrono::milliseconds(100));
    animateTimer.on_timeout([&spinProgress, &animateTimer] ()
    {
        if (spinProgress->value() > 0)
        {
            spinProgress->value(spinProgress->value() - 5);
        }
        else
        {
            animateTimer.cancel();
        }
    });
    
    button->on_click([&spinProgress, &animateTimer] (egt::Event&)
    {
        animateTimer.cancel();
        spinProgress->value(100);
        animateTimer.start();
    });

    window->show();
    return app.run();
}
