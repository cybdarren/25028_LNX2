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

#include <egt/themes/midnight.h>
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
    // create the EGT application
    egt::Application app(argc, argv);

    // create the TopWindow
    egt::TopWindow win(egt::Size(720, 1280), egt::PixelFormat::argb8888);
    // set the Theme
    egt::global_theme(std::make_unique<egt::MidnightTheme>());

    // Create a 24 point font used by multiple controls
    auto customFont = egt::Font("Sans Serif", 24, egt::Font::Weight::normal, egt::Font::Slant::normal);

    // Create the VideoWindow, this is done early so that it can claim a hardware overlay
    egt::VideoWindow player(egt::Size(320, 240), egt::PixelFormat::yuv420, egt::WindowHint::heo_overlay);
    player.move(egt::Point(200, 900));
    win.add(player);

    // Create the start streaming button and place where the video window will appear
    auto streamButton = std::make_shared<egt::Button>("Start Streaming");
    streamButton->align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
    streamButton->font(customFont);
    win.add(streamButton);

    // Create the SpinControl
    auto spinProgress = std::make_shared<egt::SpinProgress>(egt::Rect(160, 30, 400, 400),0, 100, 0);
    spinProgress->font(egt::Font("Sans Serif", 48, egt::Font::Weight::normal, egt::Font::Slant::normal));
    spinProgress->show_label(true);
    win.add(spinProgress);

    // Create the Open request button
    auto button = std::make_shared<egt::Button>("Open", egt::Rect(260, 500, 200, 120));
    button->border(2);
    button->border_radius(12);
    button->font(customFont);
    win.add(button);

    // Create the label for server messages
    auto lbl = std::make_shared<egt::Label>("Press the button to unlock", egt::Rect(60, 680, 600, 100));
    lbl->border(1);
    lbl->text_align(egt::AlignFlag::center);
    lbl->font(customFont);
    win.add(lbl);

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

    // handle click of the streaming button by triggering a GStreamer process to receive video over UDP
    streamButton->on_click([&](egt::Event&)
    {
        std::cout << "Starting Streaming" << std::endl;

        player.gst_custom_pipeline("udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
            ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale \
            ! videoconvert ! appsink name=appsink");

        streamButton->hide();
        player.show();
        player.play();
    });

    win.show();
    win.layout();
    app.run();

//     // close the connection
//     // egt::asio::error_code ec;
//     // socket->shutdown(tcp::socket::shutdown_both, ec);
//     // socket->close();


//     // /////////////////////////////////////////////////////////////////////////////////

//     egt::TopWindow window;
//     egt::global_theme(std::make_unique<egt::MidnightTheme>());    

//     // Create a 24 point font used by multiple controls
//     auto customFont = egt::Font("Sans Serif", 24, egt::Font::Weight::normal, egt::Font::Slant::normal);

//     auto label1 = std::make_shared<egt::Label>("", egt::AlignFlag::center);
//     window.add(label1);

//     egt::VideoWindow player(egt::Size(320,240), egt::PixelFormat::yuv420, egt::WindowHint::heo_overlay);
// //    player.move_to_center(window.center());
//     player.move(egt::Point(200, 900));
// //    player.border(1);
//     window.add(player);
// //     player.move(egt::Point(200, 900));
// //     player.border(1);
// //     win.add(player);

//     egt::Button button("Start Streaming");
//     button.align(egt::AlignFlag::bottom | egt::AlignFlag::center_horizontal);
//     window.add(button);

//     const auto ssize = egt::Application::instance().screen()->size();

//     button.on_click([&](egt::Event&)
//     {
//         player.gst_custom_pipeline("udpsrc port=5000 caps=application/x-rtp,encoding-name=JPEG,payload=26 \
//             ! rtpjpegdepay ! jpegdec ! capsfilter name=vcaps caps=video/x-raw,width=320,height=240,format=I420 ! videoscale \
//             ! videoconvert ! appsink name=appsink");
//         player.show();
//         player.play();

//         button.hide();
//     });

//     window.on_show([&]()
//     {
//     	std::ostringstream buffer;
//         buffer << "Go for it";
//     	label1->text(buffer.str());
//     });

//     window.show();
//     window.layout();

//     return app.run();
}

