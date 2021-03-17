/*****************************************************************************
 * discovery.cpp: Test vlcpp renderer discoverer binding
 *****************************************************************************
 * Copyright © 2015-2018 libvlcpp authors & VideoLAN
 *
 * Authors: Hugo Beauzée-Luyssen <hugo@beauzee.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "vlcpp/vlc.hpp"

#include <thread>
#include <iostream>

int main()
{
#if LIBVLC_VERSION_INT >= LIBVLC_VERSION(3, 0, 0, 0)
    VLC::Instance inst( 0, nullptr );
    VLC::RendererDiscoverer disc( inst, "microdns" );

    auto& em = disc.eventManager();
    em.onItemAdded( []( const VLC::RendererDiscoverer::Item& item ) {
        std::cout << "New item discovered: " << item.name() << '\n'
                  << "\tType: " << item.type() << '\n';
        if ( item.canRenderVideo() )
            std::cout << "\tCan render video\n";
        if ( item.canRenderAudio() )
            std::cout << "\tCan render audio\n";
        std::cout << std::endl;
    });
    if ( disc.start() == false )
        abort();
    std::this_thread::sleep_for( std::chrono::seconds{ 10 } );
#else
    std::cerr << "Renderer discovery isn't available before VLC 3." << std::endl;
    return 1;
#endif
}
