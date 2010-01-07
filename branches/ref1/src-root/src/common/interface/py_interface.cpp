/* Copyright (C) 2005 ireon.org developers council
 * $Id: py_interface.cpp 510 2006-02-26 21:09:40Z zak $

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
 * @file py_export.cpp
 * Export interface to python
 */

#include "stdafx.h"
#include "interface/interface.h"
#include "interface/window.h"
#include "interface/event_manager.h"
#include "log/log.h"
#include "file/directory.h"
#include "file/file.h"

#define BOOST_PYTHON_STATIC_MODULE
#include "boost/python.hpp"
using namespace boost::python;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads1, getWindow, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads2, linkWindow, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads3, unlinkWindow, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads4, findWindow, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads5, createWindow, 2, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads6, destroyWindow, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads7, disableWindows, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads8, enableWindows, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads9, getTextExtent, 1, 2)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CInterface_overloads10, getFontHeight, 0, 1)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(CMultiList_overloads1, insertRow, 1, 2)

void (CWindow::*subscribeEvent1)(const CEGUI::String &type, const WindowEvent &evt) = &CWindow::subscribeEvent;
void (CWindow::*subscribeEvent2)(const CEGUI::String &type, const WindowKeyEvent &evt) = &CWindow::subscribeEvent;

BOOST_PYTHON_MODULE(Interface)
{
/*	enum_<CEGUI::TextFormatting>
		("TextFormatting")
		.value("Centred",CEGUI::TextFormatting::Centred)
		.value("Justified",CEGUI::TextFormatting::Justified)
		.value("LeftAligned",CEGUI::TextFormatting::LeftAligned)
		.value("RightAligned",CEGUI::TextFormatting::RightAligned)
		.value("WordWrapCentred",CEGUI::TextFormatting::WordWrapCentred)
		.value("WordWrapJustified",CEGUI::TextFormatting::WordWrapJustified)
		.value("WordWrapLeftAligned",CEGUI::TextFormatting::WordWrapLeftAligned)
		.value("WordWrapRightAligned",CEGUI::TextFormatting::WordWrapRightAligned);
*/	
	enum_<CEGUI::StaticText::HorzFormatting>
		("HorzFormatting")
		.value("HorzCentered",CEGUI::StaticText::HorzCentred)
		.value("HorzJustified",CEGUI::StaticText::HorzJustified)
		.value("RightAligned",CEGUI::StaticText::RightAligned)
		.value("LeftAligned",CEGUI::StaticText::LeftAligned)
		.value("WordWrapCentered",CEGUI::StaticText::WordWrapCentred)
		.value("WordWrapJustified",CEGUI::StaticText::WordWrapJustified)
		.value("WordWrapLeftAligned",CEGUI::StaticText::WordWrapLeftAligned)
		.value("WordWrapRightAligned",CEGUI::StaticText::WordWrapRightAligned);

	enum_<CEGUI::StaticText::VertFormatting>
		("VertFormatting")
		.value("VertCentered",CEGUI::StaticText::VertCentred)
		.value("BottomAligned",CEGUI::StaticText::BottomAligned)
		.value("TopAligned",CEGUI::StaticText::TopAligned);

	class_<CEGUI::colour>
		("Colour", init<float,float,float>());

	class_< CWindow >
		("CWindow",no_init)
		.def("disable",&CWindow::disable)
		.def("enable",&CWindow::enable)
		.def("setSize",&CWindow::setSize)
		.def("setPosition",&CWindow::setPosition)
		.def("subscribeEvent",subscribeEvent1)
		.def("subscribeEvent",subscribeEvent2)
		.def("setAlwaysOnTop",&CWindow::setAlwaysOnTop)
		.def("isActive",&CWindow::isActive)
		.def("activate",&CWindow::activate)
		.add_property("UWidth",&CWindow::getUWidth,&CWindow::setUWidth)
		.add_property("UHeight",&CWindow::getUHeight,&CWindow::setUHeight)
		.add_property("alpha",&CWindow::getAlpha,&CWindow::setAlpha)
		.add_property("name",&CWindow::getName)
		.add_property("pos",&CWindow::getPosition,&CWindow::setPosition)
		.add_property("absPos",&CWindow::getAbsolutePosition, &CWindow::setAbsolutePosition)
		.add_property("size",&CWindow::getSize,&CWindow::setSize)
		.add_property("text",&CWindow::getText,&CWindow::setText)
		.add_property("UX",&CWindow::getUX,&CWindow::setUX)
		.add_property("UY",&CWindow::getUY,&CWindow::setUY)
		.add_property("visible",&CWindow::isVisible, &CWindow::setVisible);
	
	class_< CFrameWindow, bases<CWindow> >
		("FrameWindow", no_init )
		.def("setFrameEnabled",&CFrameWindow::setFrameEnabled)
		.def("setCloseButtonEnabled",&CFrameWindow::setCloseButtonEnabled)
		.def("setSizingEnabled",&CFrameWindow::setSizingEnabled)
		.def("setDragMovingEnabled",&CFrameWindow::setDragMovingEnabled);

	class_< CMultiListWindow, bases<CWindow> >
		("MultiListWindow", no_init )
		.def("reset",&CMultiListWindow::reset)
		.def("setWidth",&CMultiListWindow::setWidth)
		.def("insertRow",&CMultiListWindow::insertRow,CMultiList_overloads1())
		.def("getValue",&CMultiListWindow::getValue)
		.def("setHead",&CMultiListWindow::setHead)
		.def("getFirstSelected",&CMultiListWindow::getFirstSelected)
		.def("setSelected", &CMultiListWindow::setSelected)
		.add_property("cols",&CMultiListWindow::getColCount,&CMultiListWindow::setCol)
		.add_property("rows",&CMultiListWindow::getRowCount);

	class_<CStaticTextWindow, bases<CWindow> >
		("StaticText", no_init )
		.def("setFormatting", &CStaticTextWindow::setFormatting)
		.def("setBackgroundEnabled", &CStaticTextWindow::setBackgroundEnabled)
		.def("setVerticalScrollbarEnabled", &CStaticTextWindow::setVerticalScrollbarEnabled)
		.def("setFrameEnabled", &CStaticTextWindow::setFrameEnabled)
		.def("setTextColour", &CStaticTextWindow::setTextColour );

	class_<CStaticImage, bases<CWindow> >
		("StaticImage", no_init )
		.def("setImage", &CStaticImage::setImage)
		.def("setColour", &CStaticImage::setColour);

	class_<CCheckBox, bases<CWindow> >
		("CheckBox", no_init )
		.add_property("checked",&CCheckBox::isChecked,&CCheckBox::setChecked);

	class_<CListBox, bases<CWindow> >
		("ListBox", no_init )
		.def("addRow",&CListBox::addRow)
		.def("removeRow",&CListBox::removeRow)
		.def("clear",&CListBox::clear)
		.def("clearSelection",&CListBox::clearSelection);

	class_<CScrollBar, bases<CWindow> >
		("ScrollBar", no_init )
		.def("atEnd",&CScrollBar::atEnd)
		.def("toEnd",&CScrollBar::toEnd)
		.def("getPageSize",&CScrollBar::getPageSize)
		.add_property("position",&CScrollBar::getScrollPosition, &CScrollBar::setScrollPosition);

	class_<CProgressBar, bases<CWindow> >
		("ProgressBar", no_init )
		.add_property("progress",&CProgressBar::getProgress, &CProgressBar::setProgress );

	class_<CInterface>
		("CInterface",no_init)
	.def("getWindow",&CInterface::getWindow, CInterface_overloads1())
	.def("linkWindow",&CInterface::linkWindow, CInterface_overloads2())
	.def("unlinkWindow",&CInterface::unlinkWindow, CInterface_overloads3())
	.def("findWindow",&CInterface::findWindow, CInterface_overloads4())
	.def("createWindow",&CInterface::createWindow, CInterface_overloads5())
	.def("destroyWindow",&CInterface::destroyWindow, CInterface_overloads6())
	.def("disableWindows",&CInterface::disableWindows, CInterface_overloads7())
	.def("enableWindows",&CInterface::enableWindows, CInterface_overloads8())
	.def("isKeyPressed",&CInterface::isKeyPressed)
	.add_property("onlyOneKeyPressed", &CInterface::onlyOneKeyPressed)
	.def("switchLayout",&CInterface::switchLayout)
	.def("getFontHeight",&CInterface::getFontHeight, CInterface_overloads10())
	.def("getTextExtent",&CInterface::getTextExtent, CInterface_overloads9())
	.add_property("showCursor",&CInterface::isCursorShown, &CInterface::setShowCursor );

	enum_<CInterface::WinType>
		("WinType")
		.value("BASIC",CInterface::BASIC)
		.value("STATIC_TEXT",CInterface::STATIC_TEXT)
		.value("STATIC_IMAGE",CInterface::STATIC_IMAGE)
		.value("FRAME",CInterface::FRAME)
		.value("MULTI_LIST",CInterface::MULTI_LIST)
		.value("CHECK_BOX",CInterface::CHECK_BOX)
		.value("LIST",CInterface::LIST)
		.value("SCROLL_BAR",CInterface::SCROLL_BAR)
		.value("PROGRESS_BAR", CInterface::PROGRESS_BAR );

	register_ptr_to_python< boost::shared_ptr<CWindow> >();
};

void initInterfaceModule()
{
	initInterface();
};