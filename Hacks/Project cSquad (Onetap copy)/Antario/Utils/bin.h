//ColorPicker::ColorPicker(const std::string& strLabelName, Color* colorOutput, ObjectPtr pParent)
//{
//	this->strLabel = strLabelName;
//	this->szSizeObject = { 100, int(float(pFont->iHeight) * 1.5) };
//	this->iPickerSize = 170;
//	this->iHueBarSize = 10;
//	this->colOutput = colorOutput;
//	colOutput->ToHSV(colHSV[0], colHSV[1], colHSV[2]);
//}
//
//void ColorPicker::Render()
//{
//	g_Render.String(this->rcBox.left + 5, this->rcBox.Mid().y - 10,
//		CD3DFONT_DROPSHADOW | CD3DFONT_CENTERED_Y, style.colText, pFont.get(), this->strLabel.c_str());
//
//	/* Render picker rect */
//	g_Render.RectFilled(this->rcSelectable, *this->colOutput);
//
//	if (mouseCursor->IsInBounds(rcBox))
//		g_Render.RectFilled(this->rcSelectable, style.colHover);
//
//
//	if (this->bIsActive)
//	{
//		/* Render sub-window for picker */
//		g_Render.RectFilled(rcWindowBounds, Color(40, 40, 40, 255));
//		g_Render.Rect(rcWindowBounds, Color(2, 2, 2, 255));
//
//		static Color hueColors[7] =
//		{
//			{ 255, 0, 0   },
//			{ 255, 255, 0 },
//			{ 0, 255, 0   },
//			{ 0, 255, 255 },
//			{ 0, 0, 255   },
//			{ 255, 0, 255 },
//			{ 255, 0, 0   }
//		};
//
//		/* Render hue bar */
//		for (auto i = 0; i < 6; ++i)
//		{
//			const SRect tmpHueRect =
//			{
//				rcHueBar.left,
//				int(rcHueBar.top + i * (float(rcPicker.Height()) / 6.f)),
//				rcHueBar.right,
//				int(rcHueBar.top + (i + 1) * (float(rcPicker.Height()) / 6.f))
//			};
//
//			g_Render.RectFilledGradient(tmpHueRect, hueColors[i], hueColors[i + 1], GradientType::GRADIENT_VERTICAL);
//			g_Render.Rect(this->rcHueBar, Color::Black());
//		}
//
//		Color Alpha;
//		Alpha = *this->colOutput;
//
//		/* Render alpha bar */
//		g_Render.RectFilledGradient(this->rcAlphaBar, Color(0, 0, 0, 0), Alpha(255), GradientType::GRADIENT_HORIZONTAL);
//		g_Render.Rect(this->rcAlphaBar, Color::Black());
//
//		/* Render picker itself */
//		Color picker;
//		picker.FromHSV(colHSV[0], 1, 1);
//		g_Render.RectFilledGradientMultiColor(this->rcPicker, Color::White(), picker, Color::White(), picker);
//		g_Render.RectFilledGradientMultiColor(this->rcPicker, Color::Black(0), Color::Black(0), Color::Black(), Color::Black());
//		g_Render.Rect(this->rcPicker, Color::Black());
//
//		/* Render hue bar position indicator */
//		g_Render.RectFilled(ptHueBarMousePos.x - 2 - MulInt(rcHueBar.Width(), 0.5f), ptHueBarMousePos.y - 2,
//			ptHueBarMousePos.x + 2 + MulInt(rcHueBar.Width(), 0.5f), ptHueBarMousePos.y + 2,
//			Color::White());
//		g_Render.Rect(ptHueBarMousePos.x - 2 - MulInt(rcHueBar.Width(), 0.5f), ptHueBarMousePos.y - 2,
//			ptHueBarMousePos.x + 2 + MulInt(rcHueBar.Width(), 0.5f), ptHueBarMousePos.y + 2,
//			Color::Black());
//
//		/* Render alpha bar position indicator */
//		g_Render.RectFilled(ptAlphaMousePos.x - 2, ptAlphaMousePos.y - 2 - MulInt(rcAlphaBar.Height(), 0.5f),
//			ptAlphaMousePos.x + 2, ptAlphaMousePos.y + 2 + MulInt(rcAlphaBar.Height(), 0.5f),
//			Color::White());
//		g_Render.Rect(ptAlphaMousePos.x - 2, ptAlphaMousePos.y - 2 - MulInt(rcAlphaBar.Height(), 0.5f),
//			ptAlphaMousePos.x + 2, ptAlphaMousePos.y + 2 + MulInt(rcAlphaBar.Height(), 0.5f),
//			Color::Black());
//
//		/* Render picker color position */
//		g_Render.RectFilled(ptPickerMousePos.x - 2, ptPickerMousePos.y - 2, ptPickerMousePos.x + 2, ptPickerMousePos.y + 2, Color::White());
//		g_Render.Rect(ptPickerMousePos.x - 2, ptPickerMousePos.y - 2, ptPickerMousePos.x + 2, ptPickerMousePos.y + 2, Color::Black());
//	}
//
//	g_Render.RectFilledGradient(this->rcSelectable, Color(0, 0, 0, 0), Color(3, 3, 3, 100), GradientType::GRADIENT_VERTICAL);
//
//	/* Outline */
//	//g_Render.Rect(this->rcSelectable, style.colSectionOutl);
//}
//
//
///* Called in WndProc hook. Returns true/false depending if we did or didnt capture input. */
//bool ColorPicker::HandleMouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (!this->bIsVisible)
//		return false;
//
//	switch (uMsg)
//	{
//	case WM_MOUSEMOVE:
//	case WM_LBUTTONDOWN:
//	{
//		/* Hovered state */
//		if (mouseCursor->IsInBounds(rcSelectable) || mouseCursor->IsInBounds(rcBox))
//		{
//			if (mouseCursor->bLMBPressed)
//			{
//				this->bIsActive = !this->bIsActive;
//				this->RequestFocus();
//				return true;
//			}
//		}
//		else if (!mouseCursor->IsInBounds(rcAlphaBar) && !mouseCursor->IsInBounds(rcHueBar) && !mouseCursor->IsInBounds(rcPicker) && !mouseCursor->IsInBounds(rcWindowBounds))
//		{
//			if (mouseCursor->bLMBPressed)
//				this->bIsActive = false;
//		}
//
//		/* Static as you still wouldnt have 2 pickers open at the same time & makes it easier */
//		static int  iPickerID = -1;
//
//		if (this->bIsActive)
//		{
//			this->rcBoundingBox.right = this->rcWindowBounds.right;
//			this->rcBoundingBox.bottom = this->rcWindowBounds.bottom;
//			if (mouseCursor->IsInBounds(this->rcWindowBounds))
//			{
//				/* Take over window controls and make color picker draw on top (look at the hint)*/
//
//				/* React only if we start dragging in bounds (first wm_lbuttondown tick) */
//				if (mouseCursor->bLMBPressed)
//				{
//					if (mouseCursor->IsInBounds(rcPicker))
//						iPickerID = 0;
//					else if (mouseCursor->IsInBounds(rcHueBar))
//						iPickerID = 1;
//					else if (mouseCursor->IsInBounds(rcAlphaBar))
//						iPickerID = 2;
//					else
//						iPickerID = -1;
//				}
//			}
//			/* wm_lbuttondown is not pressed / held */
//			if (!mouseCursor->bLMBHeld)
//				iPickerID = -1;
//
//
//			if (iPickerID != -1)
//			{
//				/* Make sure we maintain focus */
//				/* Save actual mouse position depending on the picker used */
//				switch (iPickerID)
//				{
//				case 0:
//					ptPickerMousePos = { std::clamp(GET_X_LPARAM(lParam), rcPicker.left, rcPicker.right),
//										 std::clamp(GET_Y_LPARAM(lParam), rcPicker.top, rcPicker.bottom) };
//					break;
//				case 1:
//					ptHueBarMousePos = { rcHueBar.Mid().x,
//										 std::clamp(GET_Y_LPARAM(lParam), rcPicker.top, rcPicker.bottom) };
//					break;
//				case 2:
//					ptAlphaMousePos = { std::clamp(GET_X_LPARAM(lParam), rcPicker.left, rcPicker.right),
//										rcAlphaBar.Mid().y };
//					break;
//				}
//
//				/* Calculate hsv values (easier than rgb) */
//				colHSV[0] = std::clamp(float(ptHueBarMousePos.y - rcPicker.top) / float(rcPicker.Height() - 1), 0.0f, 1.0f);
//				colHSV[1] = std::clamp(float(ptPickerMousePos.x - rcPicker.left) / float(rcPicker.Width() - 1), 0.0f, 1.0f);
//				colHSV[2] = 1.0f - std::clamp(float(ptPickerMousePos.y - rcPicker.top) / float(rcPicker.Height() - 1), 0.0f, 1.0f);
//
//				/* Save picked value to the color */
//				this->colOutput->FromHSV(colHSV[0], colHSV[1], colHSV[2]);
//				this->colOutput->alpha = int(float(ptAlphaMousePos.x - rcAlphaBar.left) / rcAlphaBar.Width() * 255.f);
//			}
//
//			return iPickerID != -1;
//		}
//	}
//	}
//
//	return false;
//}
//
//
///* Called once for initialization and on gui window drag */
//void ColorPicker::SetupPositions()
//{
//	this->rcBoundingBox.right = this->rcBoundingBox.left + this->szSizeObject.x;
//	this->rcBoundingBox.bottom = this->rcBoundingBox.top + this->szSizeObject.y;
//
//	this->rcBox =
//	{
//		this->rcBoundingBox.left,
//		this->rcBoundingBox.top + MulInt(this->rcBoundingBox.Height(), 0.f),
//		this->rcBoundingBox.left + 185,
//		this->rcBoundingBox.top + MulInt(this->rcBoundingBox.Height(), 2.f)
//	};
//	this->rcSelectable =
//	{
//		this->rcBoundingBox.left + 182,
//		this->rcBoundingBox.top + this->rcBoundingBox.Height() - 6,
//		this->rcBoundingBox.left + 159,
//		this->rcBoundingBox.top + 6,//+ MulInt(this->rcBoundingBox.Height() + 50, .1f)
//	};
//	this->rcWindowBounds =
//	{
//		this->rcBoundingBox.right,
//		this->rcBoundingBox.bottom,
//		this->rcBoundingBox.right + this->iPickerSize + this->iHueBarSize + style.iPaddingX * 3,
//		this->rcBoundingBox.bottom + this->iPickerSize + this->iHueBarSize + style.iPaddingX * 3,
//	};
//	this->rcPicker =
//	{
//		this->rcWindowBounds.left + style.iPaddingX,
//		this->rcWindowBounds.top + style.iPaddingY,
//		this->rcWindowBounds.left + style.iPaddingX + this->iPickerSize,
//		this->rcWindowBounds.top + style.iPaddingY + this->iPickerSize
//	};
//	this->rcHueBar =
//	{
//		this->rcPicker.right + style.iPaddingX,
//		this->rcPicker.top,
//		this->rcPicker.right + style.iPaddingX + this->iHueBarSize,
//		this->rcPicker.bottom
//	};
//	this->rcAlphaBar =
//	{
//		this->rcPicker.left,
//		this->rcPicker.bottom + style.iPaddingX,
//		this->rcPicker.right,
//		this->rcPicker.bottom + style.iPaddingX + this->iHueBarSize
//	};
//
//	ptHueBarMousePos = { rcHueBar.Mid().x, std::clamp(rcHueBar.top + int(std::roundf(colHSV[0] * rcHueBar.Height())), rcHueBar.top, rcHueBar.bottom) };
//
//	ptAlphaMousePos = { rcAlphaBar.left + int(std::roundf(colOutput->alpha / 255.f * rcAlphaBar.Width())), rcAlphaBar.Mid().y };
//
//	ptPickerMousePos = { std::clamp(rcPicker.left + int(std::roundf(colHSV[1] * rcPicker.Width())), rcPicker.left, rcPicker.right),
//						 std::clamp(rcPicker.top + int(std::roundf((1.f - colHSV[2]) * rcPicker.Height())), rcPicker.top, rcPicker.bottom) };
//
//}
//
//inline int  MulInt(const int tomul, const float mult) { return static_cast<int>(std::roundf(static_cast<float>(tomul) * mult)); }
//
//void Section::AddColor(const std::string& strLabelName, Color* colorOutput)
//{
//	this->AddChild(std::make_shared<ColorPicker>(strLabelName, colorOutput, GetThis()));
//}
//
//class ColorPicker : public Control
//{
//public:
//	ColorPicker(const std::string& strLabelName, Color* colorOutput, ObjectPtr pParent);
//	void Render()												   override;
//	bool HandleMouseInput(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
//	void SetupPositions()										   override;
//	bool CanHaveFocus() const override { return true; }
//	void OnFocusOut()         override { this->bIsActive = false; };
//	Color* colOutput = nullptr;
//private:
//	bool   bIsActive = false;
//	int    iPickerSize;
//	int    iHueBarSize;
//	float  colHSV[3];
//
//	SPoint ptAlphaMousePos{}; /* Saved alpha mouse position    */
//	SPoint ptHueBarMousePos{};/* Saved hue mouse position      */
//	SPoint ptPickerMousePos{};/* Saved color mouse position    */
//
//	SRect  rcPicker{};        /* Main picker bbox              */
//	SRect  rcHueBar{};        /* Hue bar bbox                  */
//	SRect  rcAlphaBar{};      /* Alpha bar bbox                */
//	SRect  rcWindowBounds{};  /* Additional picker window bbox */
//	SRect  rcSelectable{};    /* Main menu selectable bbox     */
//	SRect  rcBox;
//};