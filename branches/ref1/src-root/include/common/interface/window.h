/* Copyright (C) 2005 ireon.org developers council
 * $Id: window.h 510 2006-02-26 21:09:40Z zak $

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
 * @file window.h
 * Interface window class
 */

#ifndef _WINDOW_H
#define _WINDOW_H

#include <OgreNoMemoryMacros.h>
#include <CEGUI/elements/CEGUIStaticText.h>
#include <CEGUI/CEGUIForwardRefs.h>
#include <OgreMemoryMacros.h>

struct WindowEvent;

class CWindow
{
	friend class CInterface;
protected:
	CWindow(CEGUI::Window* win);

public:
	virtual ~CWindow();

	virtual bool init();
	virtual void deInit();
//	CEGUI::Window* win() {return m_window;}

	uint getChildCount() const {return (uint)m_children.size();}
	WndPtr getChildAtIdx(size_t s) const {return m_children[s];}

	void addChildWindow(const WndPtr& win);
	void removeChild(size_t s);

	void disable();
	void enable();

	bool isVisible();
	void setVisible(bool);

	bool isActive();
	void activate();

	float getAlpha();
	void setAlpha(float);

	String getName();

	Vector2 getPosition();
	void setPosition(const Vector2& p);

	Vector2 getAbsolutePosition();
	void setAbsolutePosition(const Vector2& p);

	Vector2 getSize();
	void setSize(const Vector2&);

	/** Set unified width
	 *  see CEGUI documentation for details about unified
	 *  coords
	*/
	void setUWidth(const Vector2&);

	/** Set unified height
	 *  see CEGUI documentation for details about unified
	 *  coords
	*/
	void setUHeight(const Vector2&);

	/** Get unified width
	*/
	Vector2 getUWidth();
	
	/** Get unified height
	*/
	Vector2 getUHeight();

	/** Set unified X coordinate
	*/
	void setUX(const Vector2& x);

	/** Get unified X coordinate
	*/
	Vector2 getUX();

	/** Set unified Y coordinate
	*/
	void setUY(const Vector2& y);

	/** Get unified Y coordinate
	*/
	Vector2 getUY();

	String getText();
	void setText(const String&);

	void setAlwaysOnTop(bool);

	void subscribeEvent(const CEGUI::String& type, const WindowEvent& evt);
	void subscribeEvent(const CEGUI::String& type, const WindowKeyEvent& evt);

	CInterface::WinType type() {return m_type;}

	bool active() {return m_active;}
protected:
	CEGUI::Window* m_window;

	std::vector<WndPtr> m_children;
	std::vector<WndPtr>::iterator it;

	CInterface::WinType m_type;

	///Window was created, so can be destroyed
	bool m_created;

	///Is window in tree?
	bool m_active;
};

class CFrameWindow : public CWindow
{
	friend class CInterface;
protected:
	CFrameWindow(CEGUI::Window* win): CWindow(win) {}

public:
	
	void setCloseButtonEnabled(bool);

	void setDragMovingEnabled(bool);

	void setFrameEnabled(bool);

	void setSizingEnabled(bool);
};

class CMultiListWindow : public CWindow
{
	friend class CInterface;
protected:
	CMultiListWindow(CEGUI::Window* win);
public:
	bool init();
	void deInit();

	CEGUI::MultiColumnList* win() {return (CEGUI::MultiColumnList*)m_window;}

	///Clear list
	void reset();

	///Set number of cols
	void setCol(byte col);
	///Set column widths
	void setWidth(std::vector<byte> &vec);

	///Insert row with values
	uint insertRow(StringVector values, int id = -1);

	///Get value at cell
	String getValue(uint row, uint col);

	///Get row count
	uint getRowCount();

	///Get col count
	uint getColCount() {return m_colCount;}

	void setHead(uint idx, const String& str);

	///Get first selected item's id
	int getFirstSelected();
	/// Set selected row
	void setSelected(uint row);
protected:

	byte m_colCount;
};

class CStaticTextWindow : public CWindow
{
	friend class CInterface;
protected:
	CStaticTextWindow(CEGUI::Window* win):CWindow(win){};
public:

	void setFormatting(CEGUI::StaticText::HorzFormatting, CEGUI::StaticText::VertFormatting);

	void setBackgroundEnabled(bool);

	void setVerticalScrollbarEnabled(bool);

	void setFrameEnabled(bool);

	void setTextColour(CEGUI::colour col);
};

class CStaticImage : public CWindow
{
	friend class CInterface;
protected:
	CStaticImage(CEGUI::Window* win):CWindow(win){};
public:

	void setImage(const String& imageset, const String& image);
	void setColour(const CEGUI::colour& col);
};

class CCheckBox : public CWindow
{
	friend class CInterface;
protected:
	CCheckBox(CEGUI::Window* win):CWindow(win){};
public:
	bool isChecked();
	void setChecked(bool);
};

class CListBox: public CWindow
{
	friend class CInterface;
protected:
	CListBox(CEGUI::Window* win):CWindow(win){};
public:
	void addRow(const String& str, CEGUI::colour col);
	void removeRow(uint num);
	void clear();
	void clearSelection();
};

class CScrollBar : public CWindow
{
	friend class CInterface;
protected:
	CScrollBar(CEGUI::Window* win):CWindow(win){};
public:
	/// Is scrollbar at the end of document
	bool atEnd();
	/// Set scrollbar position to the end of document
	void toEnd();
	/// Set scrollbar position
	void setScrollPosition(float pos);
	float getScrollPosition();
	float getPageSize();
};

class CProgressBar : public CWindow
{
	friend class CInterface;
protected:
	CProgressBar(CEGUI::Window* win):CWindow(win){};
public:

	void setProgress(float progress);
	float getProgress();
};

#endif