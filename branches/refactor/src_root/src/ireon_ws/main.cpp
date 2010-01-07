/* Copyright (C) 2005 ireon.org developers council
 * $Id: main.cpp 378 2005-12-10 01:28:23Z llyeli $

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * @file ireon_ws.cpp
 * Main project .cpp file
 */
#include "stdafx.h"
#include "world_app.h"

#define APPLICATION_CLASS CWorldApp
#include "os_signal_handlers.h"
#undef APPLICATION_CLASS


int main(int argc, char* argv[])
{
	CWorldApp app("config.ini");
	app.init();
	SET_OS_SIGNAL_HANDLERS;
	app.go();
	return 0;
}

