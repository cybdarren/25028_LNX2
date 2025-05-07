/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* 
 * MASTERs Class 25028 LNX2 - Lab 4
 * This application creates 
 */
#include <egt/ui>
#include <iostream>

int main(int argc, char** argv)
{
    egt::Application app(argc, argv);

    egt::TopWindow window;

    std::string ipaddr = "192.168.2.248";
    std::string portno = "5000";
    uint32_t swidth = 320;
    uint32_t sheight = 240;
    uint32_t frate = 20;

    auto label1 = std::make_shared<egt::Label>("", egt::AlignFlag::center);
    window.add(label1);

//    egt::VideoWindow player(egt::Size(swidth,sheight), egt::PixelFormat::xrgb8888, egt::WindowHint::overlay);
    egt::VideoWindow player(egt::Size(swidth,sheight), egt::PixelFormat::yuv420, egt::WindowHint::overlay);
    player.move_to_center(window.center());
    window.add(player);

    auto hpos = std::make_shared<egt::HorizontalBoxSizer>();
    hpos->align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
    hpos->margin(10);
    window.add(hpos);

    egt::Button button("Start Streaming");
    button.align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
    hpos->add(button);

    const auto ssize = egt::Application::instance().screen()->size();

    button.on_click([&](egt::Event&)
    {
        player.gst_custom_pipeline("udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
            ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale \
            ! videoconvert ! appsink name=appsink");
        player.show();

        // std::ostringstream buffer;
        // buffer << "udp://" << ipaddr  << ":" << portno;
        // player.media(buffer.str());
        player.play();

        // auto wscale = static_cast<float>(ssize.width()) / player.width();
        // auto hscale = static_cast<float>(ssize.height()) / player.height();
        // player.move(egt::Point(0, 0));
        // player.scale(wscale, hscale);

        button.hide();
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

    return app.run();
}

