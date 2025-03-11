/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/// @[Example]
#include <egt/ui>

#include <iostream>

int main(int argc, char** argv)
{
    egt::Application app(argc, argv);

    egt::TopWindow window;

    std::string ipaddr = "192.168.2.247";
    std::string portno = "5000";
    uint32_t swidth = 400;
    uint32_t sheight = 400;
    uint32_t frate = 20;


//    auto label = std::make_shared<egt::Label>("Run below command on Ubuntu Machine in Terminal and Click Start Streaming:", egt::AlignFlag::top);
//    window.add(label);

    auto label1 = std::make_shared<egt::Label>("", egt::AlignFlag::center);
    window.add(label1);

    egt::VideoWindow player(egt::Size(swidth,sheight), egt::PixelFormat::xrgb8888, egt::WindowHint::heo_overlay);
    window.add(player);

    auto hpos = std::make_shared<egt::HorizontalBoxSizer>();
    hpos->align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
    hpos->margin(10);
    window.add(hpos);

    egt::Button button("Start Streaming");
    button.align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
    hpos->add(button);

    egt::Label cpulabel("CPU: 0%");
    cpulabel.color(egt::Palette::ColorId::label_text, egt::Palette::red);
    cpulabel.font(egt::Font("Free Sans", 30, egt::Font::Weight::bold));
//    cpulabel.margin(5);
//    cpulabel.align(egt::AlignFlag::bottom | egt::AlignFlag::right);
    cpulabel.hide();
    hpos->add(cpulabel);

    egt::experimental::CPUMonitorUsage tools;
    egt::PeriodicTimer cputimer(std::chrono::seconds(5));
    cputimer.on_timeout([&cpulabel, &tools]()
    {
        tools.update();
        std::ostringstream ss;
        ss << "CPU: " << static_cast<int>(tools.usage()) << "%";
        cpulabel.text(ss.str());
    });

    const auto ssize = egt::Application::instance().screen()->size();

    button.on_click([&](egt::Event&)
    {
        player.show();

        std::ostringstream buffer;
        buffer << "udp://" << ipaddr  << ":" << portno;
        player.media(buffer.str());
        player.play();

        auto wscale = static_cast<float>(ssize.width()) / player.width();
        auto hscale = static_cast<float>(ssize.height()) / player.height();
        player.move(egt::Point(0, 0));
        player.scale(wscale, hscale);

        button.hide();
        cpulabel.show();
        cputimer.start();
    });

    window.on_show([&]()
    {
    	std::ostringstream buffer;
    	buffer << "\n\n\n Set Device IP address to 10.42.0.10 \n\n\n Run below command on Ubuntu Machine in Terminal and Click Start Streaming: \n\n\n" <<
    			"gst-launch-1.0 -v ximagesrc  ! videoconvert  ! videoscale ! video/x-raw,format=I420,width=" << swidth << "\n"\
    			",height=" << sheight << ",framerate=" << frate << "/1" << " ! jpegenc quality=80 ! rtpjpegpay " << " ! udpsink host=" << \
				ipaddr  << " port=" << portno << "\n";

    	label1->text(buffer.str());
    });

    window.show();
    window.layout();
    //ctrlwindow.show();

    return app.run();
}
/// @[Example]
