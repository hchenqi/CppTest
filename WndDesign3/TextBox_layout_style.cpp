#pragma once

#include "../window/wnd_traits.h"
#include "../figure/text_block.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType = Assigned>
class TextBox;


class _TextBox_Base : public WndObject {
public:
	using Style = TextBlockStyle;
public:
	_TextBox_Base(Style style, std::wstring text) : style(style), text(text), text_block(style, text) {}
	~_TextBox_Base() {}
private:
	Style style;
	uint width_ref = 0;
protected:
	std::wstring text;
	TextBlock text_block;
protected:
	Size UpdateSize() { return text_block.UpdateSizeRef(Size(width_ref, length_max)); }
	void TextUpdated() { text_block.SetText(style, text); SizeUpdated(UpdateSize()); Redraw(region_infinite); }
public:
	void SetText(std::wstring str) { text.assign(std::move(str)); TextUpdated(); }
	void InsertText(uint pos, wchar ch) { text.insert(pos, 1, ch); TextUpdated(); }
	void InsertText(uint pos, std::wstring str) { text.insert(pos, str); TextUpdated(); }
	void ReplaceText(uint begin, uint length, wchar ch) { text.replace(begin, length, 1, ch); TextUpdated(); }
	void ReplaceText(uint begin, uint length, std::wstring str) { text.replace(begin, length, str); TextUpdated(); }
	void DeleteText(uint begin, uint length) { text.erase(begin, length); TextUpdated(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { width_ref = size_ref.width; return UpdateSize(); }
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
	}
};


template<>
class TextBox<Assigned> : public _TextBox_Base, public WndType<Assigned, Auto> {
public:
	using _TextBox_Base::_TextBox_Base;
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return Size(size_ref.width, _TextBox_Base::OnSizeRefUpdate(size_ref).height); }
};


template<>
class TextBox<Relative> : public _TextBox_Base, public WndType<Relative, Auto> {
public:
	using _TextBox_Base::_TextBox_Base;
};


template<>
class TextBox<Auto> : public _TextBox_Base, public WndType<Auto, Auto> {
public:
	using _TextBox_Base::_TextBox_Base;
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return _TextBox_Base::OnSizeRefUpdate(size_max); }
};


END_NAMESPACE(WndDesign)