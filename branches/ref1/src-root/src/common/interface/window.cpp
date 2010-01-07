/* Copyright (C) 2005 ireon.org developers council
 * $Id: window.cpp 510 2006-02-26 21:09:40Z zak $

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
 * @file window.cpp
 * Interface window
 */

#include "stdafx.h"
#include "interface/window.h"
#include <OgreNoMemoryMacros.h>
#include <CEGUI/elements/CEGUICheckbox.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIFrameWindow.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIProgressBar.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#include <CEGUI/elements/CEGUIStaticImage.h>
#include <CEGUI/elements/CEGUIStaticText.h>
#include <CEGUI/elements/CEGUIMultiColumnList.h>
#include <OgreMemoryMacros.h>

CWindow::CWindow(CEGUI::Window *win):
m_window(win),
m_type(CInterface::BASIC),
m_created(false),
m_active(false)
{
};

CWindow::~CWindow()
{
};

bool CWindow::init()
{
	for( uint i = 0; i < m_children.size(); i++ )
		m_children[i]->init();
	m_active = true;
	return true;
};

void CWindow::disable()
{
	m_window->disable();
};


void CWindow::enable()
{
	m_window->enable();
};

void CWindow::activate()
{
	m_window->activate();
};

bool CWindow::isActive()
{
	return m_window->isActive();
};

void CWindow::deInit()
{
	m_active = false;
};


void CWindow::addChildWindow(const WndPtr& child)
{
	assert(child->m_window);
	m_children.push_back(child);
	if (child->m_window->getParent())
		child->m_window->getParent()->removeChildWindow(child->m_window);
	m_window->addChildWindow(child->m_window);
};

void CWindow::removeChild(size_t s)
{
	assert(m_window);
	if( m_children.size() <= s )
		return;
	m_window->removeChildWindow(m_children[s]->m_window);
	m_children.erase(m_children.begin() + s);
};

void CWindow::setAlwaysOnTop(bool val)
{
	m_window->setAlwaysOnTop(val);
};

void CWindow::setUWidth(const Vector2& xy)
{
	m_window->setWindowWidth(CEGUI::UDim(xy.x,xy.y));
};

void CWindow::setUHeight(const Vector2& xy)
{
	m_window->setWindowHeight(CEGUI::UDim(xy.x,xy.y));
};

Vector2 CWindow::getUWidth()
{
	CEGUI::UDim udim = m_window->getWindowWidth();
	return Vector2(udim.d_scale,udim.d_offset);
};

Vector2 CWindow::getUHeight()
{
	CEGUI::UDim udim = m_window->getWindowHeight();
	return Vector2(udim.d_scale,udim.d_offset);
};


float CWindow::getAlpha()
{
	return m_window->getAlpha();
};
void CWindow::setAlpha(float a)
{
	m_window->setAlpha(a);
};

bool CWindow::isVisible()
{
	return m_window->isVisible();
};

void CWindow::setVisible(bool vis)
{
	m_window->setVisible(vis);
};

String CWindow::getName()
{
	return m_window->getName().c_str();
};

Vector2 CWindow::getPosition()
{
	CEGUI::Point p = m_window->getPosition();
	return Vector2(p.d_x,p.d_y);
};
void CWindow::setPosition(const Vector2& p)
{
	m_window->setPosition(CEGUI::Point(p.x,p.y));
};

Vector2 CWindow::getAbsolutePosition()
{
	CEGUI::Point p = m_window->getAbsolutePosition();
	return Vector2(p.d_x,p.d_y);
};

void CWindow::setAbsolutePosition(const Vector2& p)
{
	m_window->setXPosition(CEGUI::Absolute,p.x);
	m_window->setYPosition(CEGUI::Absolute,p.y);
};

Vector2 CWindow::getSize()
{
	CEGUI::Size p = m_window->getSize();
	return Vector2(p.d_width,p.d_height);
};

void CWindow::setSize(const Vector2& p)
{
	m_window->setSize(CEGUI::Size(p.x,p.y));
};

String CWindow::getText()
{
	return m_window->getText().c_str();
};

void CWindow::setText(const String& s)
{
	m_window->setText((CEGUI::utf8*)s.c_str());
};

void CWindow::subscribeEvent(const CEGUI::String& type, const WindowEvent &evt)
{
	m_window->subscribeEvent(type,CEGUI::Event::Subscriber(evt));
};

void CWindow::subscribeEvent(const CEGUI::String& type, const WindowKeyEvent &evt)
{
	m_window->subscribeEvent(type,CEGUI::Event::Subscriber(evt));
};

void CWindow::setUX(const Vector2 &x)
{
	m_window->setWindowXPosition(CEGUI::UDim(x.x,x.y));
};

Vector2 CWindow::getUX()
{
	CEGUI::UDim ud = m_window->getWindowXPosition();
	return Vector2(ud.d_scale,ud.d_offset);
};

void CWindow::setUY(const Vector2 &y)
{
	m_window->setWindowYPosition(CEGUI::UDim(y.x,y.y));
};

Vector2 CWindow::getUY()
{
	CEGUI::UDim ud = m_window->getWindowYPosition();
	return Vector2(ud.d_scale,ud.d_offset);
};

CMultiListWindow::CMultiListWindow(CEGUI::Window *w):
CWindow(w)
{
	m_type = CInterface::MULTI_LIST;
	m_colCount = win()->getColumnCount();
}

bool CMultiListWindow::init()
{
	return CWindow::init();
};

void CMultiListWindow::reset()
{
	win()->resetList();
};

void CMultiListWindow::deInit()
{
	CWindow::deInit();
};

uint CMultiListWindow::getRowCount()
{
	return win()->getRowCount();
};

String CMultiListWindow::getValue(uint row, uint col)
{
	if( CEGUI::ListboxItem* i =  win()->getItemAtGridReference(CEGUI::MCLGridRef(row,col)) )
		return i->getText().c_str();
	return "";
};

void CMultiListWindow::setCol(byte col)
{
	if( col > m_colCount )
	{
		byte additional = 0;
		while( additional < col )
		{
			win()->addColumn(" ",additional,0.1);
			additional++;
		};
	} else
	{
		byte remove = m_colCount;
		while(remove > col)
		{
			win()->removeColumn(0);
			remove--;
		}
	};
	m_colCount = col;
};

void CMultiListWindow::setWidth(std::vector<byte> &vec)
{
	for( uint i = 0; i < vec.size() && i < m_colCount; i++ )
	{
		float width = (float)vec[i]/100;
		win()->getHeaderSegmentForColumn(i).setSize(CEGUI::Size(width,1.0f));
	}
};

void CMultiListWindow::setHead(uint idx, const String &str)
{
	if( idx >= m_colCount )
		return;
	win()->getHeaderSegmentForColumn(idx).setText((CEGUI::utf8*)str.c_str());
};

uint CMultiListWindow::insertRow(StringVector values, int id)
{
	if( id == -1 )
		id = getRowCount();
	uint idx = win()->insertRow(id);
	CEGUI::ListboxItem* item;
	for( uint i = 0; i < m_colCount && i < values.size(); i++ )
	{
		item = new CEGUI::ListboxTextItem((CEGUI::utf8*)values[i].c_str());
		item->setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
		item->setID(idx);
		win()->setItem(item,i,idx);
	}
	return idx;
};

int CMultiListWindow::getFirstSelected()
{
	CEGUI::ListboxItem* it = win()->getFirstSelectedItem();
	if( !it )
		return -1;
	return (int)it->getID();
};

void CMultiListWindow::setSelected(uint row)
{
	if (row >= getRowCount()) return;
	uint columnCount = win()->getColumnCount();
	for (uint col = 0; col < columnCount; col++)
		win()->setItemSelectState(CEGUI::MCLGridRef(row,col),true);
};

void CFrameWindow::setCloseButtonEnabled(bool value)
{
	((CEGUI::FrameWindow*)m_window)->setCloseButtonEnabled(value);
}

void CFrameWindow::setDragMovingEnabled(bool value)
{
	((CEGUI::FrameWindow*)m_window)->setDragMovingEnabled(value);
}

void CFrameWindow::setFrameEnabled(bool value)
{
	((CEGUI::FrameWindow*)m_window)->setFrameEnabled(value);
}

void CFrameWindow::setSizingEnabled(bool value)
{
	((CEGUI::FrameWindow*)m_window)->setSizingEnabled(value);
}

void CStaticTextWindow::setFormatting(CEGUI::StaticText::HorzFormatting horz, CEGUI::StaticText::VertFormatting vert)
{
	((CEGUI::StaticText*)m_window)->setFormatting(horz, vert);
};

void CStaticTextWindow::setTextColour(CEGUI::colour col)
{
	((CEGUI::StaticText*)m_window)->setTextColours(col);
};

void CStaticTextWindow::setVerticalScrollbarEnabled(bool val)
{
	((CEGUI::StaticText*)m_window)->setVerticalScrollbarEnabled(val);
};

void CStaticTextWindow::setBackgroundEnabled(bool val)
{
	((CEGUI::StaticText*)m_window)->setBackgroundEnabled(val);
};

void CStaticTextWindow::setFrameEnabled(bool val)
{
	((CEGUI::StaticText*)m_window)->setFrameEnabled(val);
};

void CStaticImage::setImage(const String& imageset, const String& image)
{
	((CEGUI::StaticImage*)m_window)->setImage((CEGUI::utf8*)imageset.c_str(),(CEGUI::utf8*)image.c_str());
};

void CStaticImage::setColour(const CEGUI::colour& col)
{
	((CEGUI::StaticImage*)m_window)->setImageColours(col);
};

bool CCheckBox::isChecked()
{
	return ((CEGUI::Checkbox*)m_window)->isSelected();
};

void CCheckBox::setChecked(bool checked)
{
	((CEGUI::Checkbox*)m_window)->setSelected(checked);
};

void CListBox::addRow(const String& str, CEGUI::colour col)
{
	CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem((CEGUI::utf8*)str.c_str());
	item->setTextColours(col);
	item->setSelectionBrushImage((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MultiListSelectionBrush");
	((CEGUI::Listbox*)m_window)->insertItem(item,NULL);
};

void CListBox::removeRow(uint row)
{
	CEGUI::ListboxItem* it = ((CEGUI::Listbox*)m_window)->getListboxItemFromIndex(row);
	if( it )
		((CEGUI::Listbox*)m_window)->removeItem(it);
};

void CListBox::clear()
{

	((CEGUI::Listbox*)m_window)->resetList();
};

void CListBox::clearSelection()
{
	((CEGUI::Listbox*)m_window)->clearAllSelections();
};

void CScrollBar::setScrollPosition(float pos)
{
	((CEGUI::Scrollbar*)m_window)->setScrollPosition(pos);
};

float CScrollBar::getScrollPosition()
{
	return ((CEGUI::Scrollbar*)m_window)->getScrollPosition();
};

float CScrollBar::getPageSize()
{
	return ((CEGUI::Scrollbar*)m_window)->getPageSize();
};

void CScrollBar::toEnd()
{
	((CEGUI::Scrollbar*)m_window)->setScrollPosition(((CEGUI::Scrollbar*)m_window)->getDocumentSize() - ((CEGUI::Scrollbar*)m_window)->getPageSize());
};

bool CScrollBar::atEnd()
{
	return (((CEGUI::Scrollbar*)m_window)->getScrollPosition() >= ((CEGUI::Scrollbar*)m_window)->getDocumentSize() - ((CEGUI::Scrollbar*)m_window)->getPageSize());
};

void CProgressBar::setProgress(float progress)
{
	((CEGUI::ProgressBar*)m_window)->setProgress(progress);
};

float CProgressBar::getProgress()
{
	return ((CEGUI::ProgressBar*)m_window)->getProgress();
};