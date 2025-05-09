/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/* 
 * MASTERs Class 25028 LNX2 - Lab 4
 * This application creates 
 */
#include <memory>

#include <egt/ui>
#include <egt/asio.hpp>
#include <egt/uiloader.h>
#include <iostream>

using egt::asio::ip::tcp;

#define SERVER_ADDRESS      "192.168.2.200"
#define SERVER_MSG_PORT     "8000"
#define SERVER_VIDEO_PORT   "5000"          // this is just a note as it is encoded in the gst string

int main(int argc, char** argv)
{
    egt::Application app(argc, argv);

    egt::VideoWindow player(egt::Size(320, 240), egt::PixelFormat::yuv420, egt::WindowHint::heo_overlay);

    egt::experimental::UiLoader loader;
    auto window = std::static_pointer_cast<egt::Window>(loader.load("file:lab4_ui.xml"));
    if (!window)
    {
        std::cerr << "Error: Unable to load UI from lab4_ui.xml\n";
        return 1;
    }

    auto spinProgress = window->find_child<egt::SpinProgress>("SpinProgress1");
    auto button = window->find_child<egt::Button>("Button1");
    auto lbl = window->find_child<egt::Label>("lblMessage");
    // this is just a placeholder widget, it will be replaced with the actual player later on
    auto videoWindow = window->find_child<egt::Window>("videoWindow");

    // if (!spinProgress || !button || !lbl || !videoWindow)
    // {
    //     std::cerr << "Error: UI elements not found\n";
    //     return 1;
    // }

    // auto& io_context = app.event().io();
    // auto socket = std::make_shared<tcp::socket>(io_context);
    // auto resolver = std::make_shared<tcp::resolver>(io_context);

    // // a lambda function to read from the socket
    // std::function<void()> start_reading;
    // start_reading = [socket, lbl, &start_reading]()
    // {
    //     auto buffer = std::make_shared<std::vector<char>>(1024);

    //     socket->async_read_some(egt::asio::buffer(*buffer),
    //         [socket, lbl, buffer, &start_reading](const egt::asio::error_code& ec, std::size_t length)
    //     {
    //         if (!ec)
    //         {
    //             std::string message(buffer->data(), length);
    //             std::cout << "Recevied from server: " << message << std::endl;

    //             // Post update to UI thread
    //             egt::asio::post(egt::Application::instance().event().io(), [lbl, message]() {
    //                 lbl->text(message);
    //             });

    //             // Recursive call to keep reading
    //             start_reading();
    //         }
    //         else
    //         {
    //             std::cerr << "Read error" << ec.message() << std::endl;
    //             std::exit(1);
    //         }
    //     });
    // };

    // // connect to the server at startup
    // resolver->async_resolve(SERVER_ADDRESS, SERVER_MSG_PORT,
    //     [socket, start_reading](const egt::asio::error_code& ec, tcp::resolver::results_type endpoints)
    // {
    //     if (!ec)
    //     {
    //         egt::asio::async_connect(*socket, endpoints,
    //             [socket, start_reading] (const egt::asio::error_code& ec, const tcp::endpoint&)
    //         {
    //             if (!ec)
    //             {
    //                 std::cout << "Connected to server" << std::endl;
    //                 start_reading();
    //             }
    //             else
    //             {
    //                 std::cerr << "Connect error: " << ec.message() << std::endl;
    //             }
    //         });
    //     }
    //     else
    //     {
    //         std::cerr << "Resolve error: " << ec.message() << std::endl;
    //     }
    // });

    // egt::v1::PeriodicTimer animateTimer(std::chrono::milliseconds(100));
    // animateTimer.on_timeout([&spinProgress, &animateTimer, socket] ()
    // {
    //     if (spinProgress->value() > 0)
    //     {
    //         spinProgress->value(spinProgress->value() - 5);
    //     }
    //     else
    //     {
    //         animateTimer.cancel();

    //         // Send "Lock" message when animation reaches 0
    //         std::string data = "Lock";
    //         egt::asio::async_write(*socket, egt::asio::buffer(data),
    //             [socket](const egt::asio::error_code& ec, std::size_t bytes_transferred)
    //         {
    //             if (!ec)
    //             {
    //                 std::cout << "Lock message sent: " << bytes_transferred << " bytes" << std::endl;
    //             }
    //             else
    //             {
    //                 std::cerr << "Write error: " << ec.message() << std::endl;
    //             }
    //         });
    //     }
    // });

    // button->on_click([&, socket] (egt::Event&) 
    // {
    //     animateTimer.cancel();
    //     spinProgress->value(100);
    //     animateTimer.start();

    //     std::string data = "Unlock";
    //     egt::asio::async_write(*socket, egt::asio::buffer(data), 
    //         [socket](const egt::asio::error_code& ec, std::size_t bytes_transferred)
    //     {
    //         if (!ec)
    //         {
    //             std::cout << "Unlock message sent: " << bytes_transferred << " bytes" << std::endl;
    //         }
    //         else
    //         {
    //             std::cerr << "Write error: " << ec.message() << std::endl;
    //         }
    //     });
    // });

    // egt::Window rc(egt::Rect(0, 0, 320, 240));
    // egt::Rect videoFrameSize(rc.);

    // egt::VideoWindow player(egt::Size(320, 240), egt::PixelFormat::yuv420, egt::WindowHint::overlay);
    player.move_to_center(videoWindow->center());
    // topWindow.add(player);

    // player.move_to_center(videoWindow->center());
    // auto window1 = window->find_child<egt::Window>("Window1");
    // window1->add(player);  
//    player.move_to_center(topWindow->center());
 //   topWindow->add(player);

    // create a button an position it in the middle of the videoWindow widget
    egt::Button streamButton("Start Streaming");
    streamButton.align(egt::AlignFlag::center_vertical | egt::AlignFlag::center_horizontal);
    videoWindow->add(streamButton);
   
    streamButton.on_click([&](egt::Event&)
    {
        std::cout << "Streaming starting" << std::endl;

        player.gst_custom_pipeline("udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
            ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale \
            ! videoconvert ! appsink name=appsink");

//        streamButton.hide();
        videoWindow->hide();
        window->add(player);
        player.zorder_top();
        player.show();
        player.play();
//        window->hide();
//        topWindow.layout();
//        videoWindow->hide();
    });

    window->show();
//    videoWindow->show();
    window->layout();
    app.run();

    // close the connection
    // egt::asio::error_code ec;
    // socket->shutdown(tcp::socket::shutdown_both, ec);
    // socket->close();


    // /////////////////////////////////////////////////////////////////////////////////

    // egt::TopWindow window;

    // std::string ipaddr = "192.168.2.248";
    // std::string portno = "5000";
    // uint32_t swidth = 320;
    // uint32_t sheight = 240;
    // uint32_t frate = 20;

    // auto label1 = std::make_shared<egt::Label>("", egt::AlignFlag::center);
    // window.add(label1);

    // egt::VideoWindow player(egt::Size(swidth,sheight), egt::PixelFormat::yuv420, egt::WindowHint::overlay);
    // player.move_to_center(window.center());
    // window.add(player);

    // auto hpos = std::make_shared<egt::HorizontalBoxSizer>();
    // hpos->align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
    // hpos->margin(10);
    // window.add(hpos);

    // egt::Button button("Start Streaming");
    // button.align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
    // hpos->add(button);

    // const auto ssize = egt::Application::instance().screen()->size();

    // button.on_click([&](egt::Event&)
    // {
    //     player.gst_custom_pipeline("udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
    //         ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale \
    //         ! videoconvert ! appsink name=appsink");
    //     player.show();

    //     // std::ostringstream buffer;
    //     // buffer << "udp://" << ipaddr  << ":" << portno;
    //     // player.media(buffer.str());
    //     player.play();

    //     // auto wscale = static_cast<float>(ssize.width()) / player.width();
    //     // auto hscale = static_cast<float>(ssize.height()) / player.height();
    //     // player.move(egt::Point(0, 0));
    //     // player.scale(wscale, hscale);

    //     button.hide();
    // });

    // window.on_show([&]()
    // {
    // 	std::ostringstream buffer;
    // 	buffer << "\n\n\n Set Device IP address to 10.42.0.10 \n\n\n Run below command on Ubuntu Machine in Terminal and Click Start Streaming: \n\n\n" <<
    // 			"gst-launch-1.0 -v ximagesrc  ! videoconvert  ! videoscale ! video/x-raw,format=I420,width=" << swidth << "\n"\
    // 			",height=" << sheight << ",framerate=" << frate << "/1" << " ! jpegenc quality=80 ! rtpjpegpay " << " ! udpsink host=" << \
	// 			ipaddr  << " port=" << portno << "\n";

    // 	label1->text(buffer.str());
    // });

    // window.show();
    // window.layout();

    // return app.run();
}

