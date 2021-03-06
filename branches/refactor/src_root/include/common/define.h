/* Copyright (C) 2005 ireon.org developers council
 * $Id: define.h 238 2005-11-23 08:05:06Z zak $

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
 * @file define.h
 * Defines and constants
 */
#ifndef _DEFINE_H
#define _DEFINE_H

#pragma warning(disable:4267) ///Disable warning 'possible loss of data'

#define _(txt) (txt)
#define g_log(txt) CLog::instance()->log(txt);

#endif
