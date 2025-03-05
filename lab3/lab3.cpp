
#include <memory>

#include <egt/ui>
#include <egt/uiloader.h>
 
int main(int argc, char** argv)
{
    egt::Application app(argc, argv);
 
    egt::experimental::UiLoader loader;
    auto window = std::static_pointer_cast<egt::Window>(loader.load("file:ui.xml"));
 
    auto spinProgress = window->find_child<egt::SpinProgress>("SpinProgress1");

    egt::v1::PeriodicTimer animateTimer(std::chrono::milliseconds(100));
    animateTimer.on_timeout([&spinProgress, &animateTimer] ()
    {
        spinProgress->value(spinProgress->value() - 5);
        if (spinProgress->value() == 0)
            animateTimer.cancel();
    });
    
    auto button = window->find_child<egt::Button>("Button1");
    button->on_click([&spinProgress, &animateTimer] (egt::Event&)
    {
        spinProgress->value(100);
        animateTimer.start();
    });

    window->show();
    return app.run();
}
