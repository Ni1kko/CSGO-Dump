#include "LegitAim.h"
#include "../../GUI/Gui.h"


#define GetWeap(a) ((a < 0) ? 0 : (a >= Weapons.size() ? Weapons.size()-1 : a))
void CLegitAim::Menu()
{
	if (SelectedWeapon == -1)
		SelectedWeapon = 0;

	if (SelectedWeapon >= (int)Weapons.size())
		SelectedWeapon = 0;

	DCheckBox("Enable", u8"Включение", Enable, u8"Включает аим");

	VectorEx<const char*> CustomTypesEng = { lolc("One Weapon"),lolc("Standart Subsections") , lolc("Custom Subsections"), lolc("All Weapons") };
	VectorEx<const char*> CustomTypesRus = { lolc(u8"Одно оружие"), lolc(u8"Стандартные подразделы"), lolc(u8"Кастомные подразделы"),lolc(u8"Все оружие") };

	static int old_type = WeaponCustomTypes;
	X1Gui().SameLine(126);
	X1Gui().PushItemWidth(194);
	DComboBox("##pods", "##pods", WeaponCustomTypes, CustomTypesEng, CustomTypesRus, u8"Настройка на");

	if (old_type != WeaponCustomTypes)
		SetSelectedWeapon(false);

	old_type = WeaponCustomTypes;

	string A_WeaponList_L = XorStr("Weapon List");
	string A_Subsections_L = XorStr("Subsections");
	if (false/*MainCheatConfig.RusLanguage*/)
	{
		A_WeaponList_L = XorStr(u8"Список оружия");
		A_Subsections_L = XorStr(u8"Подразделы");
	}

	if (WeaponCustomTypes == 0)
	{
		if (false/*MainCheatConfig.RusLanguage*/)
			X1Gui().SameLine(331);
		else
			X1Gui().SameLine(351);

		if (X1Gui().Button(ShowWeaponList ? (A_WeaponList_L + XorStr(" <<<")).c_str() : (A_WeaponList_L + XorStr(" >>>")).c_str()))
			ShowWeaponList = !ShowWeaponList;
	}
	else if (WeaponCustomTypes == 1 || WeaponCustomTypes == 2)
	{
		if (false/*MainCheatConfig.RusLanguage*/)
			X1Gui().SameLine(349);
		else
			X1Gui().SameLine(355);

		if (X1Gui().Button(ShowWeaponList ? (A_Subsections_L + XorStr(" <<<")).c_str() : (A_Subsections_L + XorStr(" >>>")).c_str()))
			ShowWeaponList = !ShowWeaponList;
	}
	else if (WeaponCustomTypes == 3)
	{
		ShowWeaponList = false;
	}

	X1Gui().Spacing();

	if(X1Gui().BeginChild(XorStr("MainChild"), Vec2(0, 173), true))
	{
		VectorEx<const char*> AimPEng = { lolc("Page1"), lolc("Page2") };
		VectorEx<const char*> AimPRus = { lolc(u8"Страница1"),lolc(u8"Страница2") };
		static int SubtabAimMiscSelected = 0;

		TabsLabels(SubtabAimMiscSelected, AimPEng, AimPRus, Vec2(X1Gui().GetCurWindowSize().x - (X1Gui().GetStyle().wndPadding.x * 2), 0), false);
		if (SubtabAimMiscSelected == 0)
		{
			X1Gui().Spacing();
			DCheckBox("Deathmatch", u8"На всех", Deathmatch, u8"Включает аим на всех");

			X1Gui().SameLine(246);
			DCheckBox("Auto Fire", u8"Автовыстрел", AutoFire, u8"");
			DCheckBox("Flash Check", u8"Проверка ослепления", FlashCheck, u8"Выключает аим если Вы ослеплены");

			X1Gui().SameLine(246);
			DCheckBox("Jump Check##5677", u8"Проверка прыжка##5677", JumpCheck, u8"Выключает аим если Вы пригнули");
			DCheckBox("Smoke Check##343434", u8"Проверка дыма##343434", SmokeCheck, u8"Отключает проверку противников за дымом");
			X1Gui().SameLine(246);
			DCheckBox("Jump Enemy Check", u8"Проверка прыжка врага", JumpEnemyCheck, u8"Выключает проверку игроков в прыжке");

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Draw Fov", u8"Фов", DrawFov, u8"Включает отображение фова аима");

			X1Gui().SameLine(185.f);
			DColorEdit("Color##Fov1", u8"Цвет##фв", FovColor);
			X1Gui().SameLine(246);

			DCheckBox("Show Target", u8"Показывать цель", ShowSpot, u8"Показывает целевую точку");

			DCheckBox("Draw Silent Fov", u8"Фов сайлента", DrawSilentFov, u8"Включает отображение фова сайлент аима");

			X1Gui().SameLine(185.f);
			DColorEdit("Color##SFov1", u8"Цвет##сфв", SilentFovColor);

			X1Gui().SameLine(246);

			DCheckBox("Wall Check", u8"Проверка стен", WallCheck, u8"Включает проверку стен");


			if (SelectedWeapon <= 9 || WeaponCustomTypes == 3 || WeaponCustomTypes == 2 || WeaponCustomTypes == 1)
			{
				DCheckBox("Auto Pistol", u8"Автопистолъ", Weapons[GetWeap(SelectedWeapon)].AutoPistol, u8"");
				X1Gui().SameLine(246);
			}
			if ((SelectedWeapon > 28 && SelectedWeapon <= 33) || SelectedWeapon == 35 || WeaponCustomTypes == 3 || WeaponCustomTypes == 2 || WeaponCustomTypes)
			{
				DCheckBox("Fast Zoom", u8"Быстрый прицел", Weapons[GetWeap(SelectedWeapon)].FastZoom, u8"");
			}

		}
		else if (SubtabAimMiscSelected == 1)
		{
			X1Gui().PushItemWidth(250.f);
			X1Gui().Spacing();

			DCheckBox("FaceIt", u8"Фейсит", FaceIt, u8"Включает FaceIt режим");

			if (FaceIt)
			{
				X1Gui().SameLine();
				SliderFloats("Rcs Correction", u8"Корекция Rcs", MouseSense, 0.0001, 8, u8"Корекция контроля отдачи");
			}
			X1Gui().PushItemWidth(379);
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Entity Aim", u8"Аим на объекты", EntityAim, u8"Переключает аим на игровые объекты");

			if (EntityAim)
			{
				static char ModelN[64] = { 0 };
				TextEdit("Target Entity Name", u8"Имя целевого объекта", EntityTargetName, ModelN, 64);
				DCheckBox("Show Entity Name", u8"Показывать имена объектов", ShowEntitysName, u8"");
			}

			X1Gui().PushItemWidth(379);
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();
			DCheckBox("Aim Bind", u8"Бинд аима", AimBind.Enable, u8"Включает бинд аима на кнопку");
			if (AimBind.Enable)
			{
				X1Gui().SameLine();
				DCheckBox("Hold", u8"Удержание", AimBind.Hold, u8"Включает работу аима только если кнопка зажата");
				X1Gui().Spacing();
				HotsKey("Button", u8"Кнопка", AimBind.Button, u8"Задает кнопку активации аима");

			}
			X1Gui().Spacing();
			DCheckBox("Debug", u8"Дебаг", Debug, u8"");
		}

	}
	X1Gui().EndChild();

	VectorEx<const char*> AimPSettingsEng = { lolc("Basic"),lolc("Rcs"), lolc("pSilent"), lolc("Delays") };
	VectorEx<const char*> AimPSettingsRus = { lolc(u8"Аим"),lolc(u8"Спрей"),lolc(u8"Сайлент"),lolc(u8"Задержки") };

	TabsLabels(SubTabAimSelected, AimPSettingsEng, AimPSettingsRus, Vec2(X1Gui().GetCurWindowSize().x - (X1Gui().GetStyle().wndPadding.x * 2), 0), false);

	if (X1Gui().BeginChild(XorStr("Main2Child4"), Vec2(0, 216), true))
	{
		X1Gui().PushItemWidth(333.f);

		if (SubTabAimSelected == 0)
		{
			//VectorEx<const char*> SmoothMethods = { "Standart" , "New" };
			//ImGui::Combo(Esp::esp_CheckLanguage ? u8"Метод аима" : "Smooth Method", &Aimbot::weapon_aim_settings[iWeaponID].SmoothMethod, SmoothMethods, IM_ARRAYSIZE(SmoothMethods));

			//Weapons[GetWeap(SelectedWeapon)].SmoothMoveFactor

			VectorEx<const char*> itemsSMFEng = { lolc("Deceleration Of Speed") , lolc("Constant Speed") };
			VectorEx<const char*> itemsSMFRus = { lolc(u8"Замедление скорости") , lolc(u8"Постоянная скорость") };
			DComboBox("Smooth Factor", u8"Фактор плавности", Weapons[GetWeap(SelectedWeapon)].SmoothMoveFactor, itemsSMFEng, itemsSMFRus, "");

			if (Weapons[GetWeap(SelectedWeapon)].SmoothMethod == 0)
			{
				SliderFloats("Smooth", u8"Плавность", Weapons[GetWeap(SelectedWeapon)].Smooth, 0, 100, u8"Меньше->быстрее");
			}
			else if (Weapons[GetWeap(SelectedWeapon)].SmoothMethod == 1)
			{
				//ImGui::SliderFloat("Start Smooth", &Weapons[GetWeap(SelectedWeapon)].StartAcceleration, 1, 100);
				//ImGui::SliderFloat("End Smooth", &Weapons[GetWeap(SelectedWeapon)].EndAcceleration, 1, 100);
			}


			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			SliderFloats("Fov", u8"Фов", Weapons[GetWeap(SelectedWeapon)].Fov, 0, 300, "");


			VectorEx<const char*> itemsFPEng = { lolc("Center Screen") , lolc("Recoil"), lolc("On Player") };
			VectorEx<const char*> itemsFPRus = { lolc(u8"Центр экрана") , lolc(u8"Отдача"), lolc(u8"На игроке") };
			DComboBox("Fov Pos", u8"Позиция фова", Weapons[GetWeap(SelectedWeapon)].FovPos, itemsFPEng, itemsFPRus, "");

			if (!EntityAim)
			{
				X1Gui().Spacing();
				X1Gui().Separator();
				X1Gui().Spacing();

				DCheckBox("Nearest", u8"Неарест", Weapons[GetWeap(SelectedWeapon)].Nearest, u8"Автоматический выбор хитбокса");

				if (!Weapons[GetWeap(SelectedWeapon)].Nearest)
				{
					VectorEx<const char*> itemsHBEng = { lolc("Head") , lolc("Neck"),lolc("Low Neck"), lolc("Body"), lolc("Thorax"), lolc("Chest"), lolc("Upper Chest") };
					VectorEx<const char*> itemsHBRus = { lolc(u8"Голова") ,  lolc(u8"Шея"),lolc(u8"Низ шеи"), lolc(u8"Тело"), lolc(u8"Пояс"), lolc(u8"Грудь"),lolc(u8"Верх груди") };
					DComboBox("HitBox", u8"Хитбокс", Weapons[GetWeap(SelectedWeapon)].HitBox, itemsHBEng, itemsHBRus, "");
				}

				if (!FaceIt)
				{
					X1Gui().Spacing();
					X1Gui().Separator();
					X1Gui().Spacing();

					DCheckBox("Backtrack", u8"Откат тиков", Weapons[GetWeap(SelectedWeapon)].Backtrack, u8"");
					if (Weapons[GetWeap(SelectedWeapon)].Backtrack)
					{
						SliderInts("Ticks", u8"Тики", Weapons[GetWeap(SelectedWeapon)].BacktrackTicks, 0, 16, u8"Меньше->быстрее");

						DCheckBox("Show Backtrack", u8"Показать бектрек", ShowBacktrack, u8"");
					}
				}

				//	X1Gui().Spacing();
				//	X1Gui().Separator();
				//	X1Gui().Spacing();
				//	ImGui::Checkbox(L.A_Backtrack_L.c_str(), &Aimbot::aim_Backtrack);
				//	if (ImGui::IsItemHovered())
				//		ImGui::SetTooltip(Esp::esp_CheckLanguage ? u8"Возвращение тиков" : "Return of ticks");
				//	if (Aimbot::aim_Backtrack)
				//	{
				//		//X1Gui().SameLine();
				//		//ImGui::Checkbox("Aim+Backtrack", &Aimbot::AimPlusBacktrack);
				//		ImGui::Checkbox(L.A_BacktrackShow_L.c_str(), &Aimbot::aim_BacktrackShow);
				//		if (Aimbot::aim_BacktrackShow)
				//		{
				//			X1Gui().SameLine();
				//			ImGui::MyDColorEdit3("Color", Aimbot::BacktrackColor, 1 << 7);
				//			X1Gui().SameLine();
				//			ImGui::MyDColorEdit3("Visble Color", Aimbot::BacktrackColorVisble, 1 << 7);
				//			X1Gui().SameLine();
				//			ImGui::Checkbox("Visble Only", &Aimbot::BacktrackVisbleOnly);
				//			ImGui::SliderInt("Alpha##backtrack", &Aimbot::BackTrackAlpha, 0, 255); // 4 5
				//		}
				//		X1Gui().Spacing();
				//		ImGui::SliderInt(L.A_Ticks_L.c_str(), &Aimbot::aim_Backtracktickrate, 1, 16);
				//		if (ImGui::IsItemHovered())
				//			ImGui::SetTooltip(Esp::esp_CheckLanguage ? u8"Сколько тиков возвращает \"Откат тиков\"" : "How many ticks return \"Rollback of ticks\"");

				//	}
				//}
			}
		}
		else if (SubTabAimSelected == 1)
		{

			SliderFloats("Rcs X", u8"Rcs по X", Weapons[GetWeap(SelectedWeapon)].RcsY, 0, 150, u8"Сила контроля отдачи по оси X");
			SliderFloats("Rcs Y", u8"Rcs по Y", Weapons[GetWeap(SelectedWeapon)].RcsX, 0, 150, u8"Сила контроля отдачи по оси Y");

			X1Gui().Spacing();
			X1Gui().Spacing();
			if (false/*MainCheatConfig.RusLanguage*/)
				X1Gui().PushItemWidth(260.f);

			SliderFloats("Smooth Rcs", u8"Плавность после первой пули", Weapons[GetWeap(SelectedWeapon)].SmoothRcs, 0, 100, u8"");
			SliderFloats("Fov Rcs", u8"Фов после первой пули", Weapons[GetWeap(SelectedWeapon)].FovRcs, 0, 300, u8"");
			//if (ImGui::IsItemHovered())
			//{
			//	GDrawRcsFov = true;
			//}
			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Rcs Standelone", u8"Rcs станделон", Weapons[GetWeap(SelectedWeapon)].RcsStandelone, u8"Включает постоянный контроль отдачи");

			if (!EntityAim)
			{
				X1Gui().Spacing();
				X1Gui().Separator();
				X1Gui().Spacing();

				DCheckBox("HitBox after first bullet", u8"Хитбокс после первой пули", Weapons[GetWeap(SelectedWeapon)].HitBoxAfter1B, u8"");

				if (Weapons[GetWeap(SelectedWeapon)].HitBoxAfter1B)
				{
					DCheckBox("Nearest Rcs", u8"Неарест Rcs", Weapons[GetWeap(SelectedWeapon)].NearestRcs, u8"Авто выбор хитбокса после первой пули");

					if (!Weapons[GetWeap(SelectedWeapon)].NearestRcs)
					{
						VectorEx<const char*> itemsHBEng = { lolc("Head") , lolc("Neck"),lolc("Low Neck"), lolc("Body"), lolc("Thorax"), lolc("Chest"), lolc("Upper Chest") };
						VectorEx<const char*> itemsHBRus = { lolc(u8"Голова") ,  lolc(u8"Шея"),lolc(u8"Низ шеи"), lolc(u8"Тело"), lolc(u8"Пояс"), lolc(u8"Грудь"),lolc(u8"Верх груди") };
						DComboBox("HitBox", u8"Хитбокс", Weapons[GetWeap(SelectedWeapon)].HitBoxRcs, itemsHBEng, itemsHBRus, "");
					}
				}
			}
			X1Gui().PushItemWidth(333.f);
		}
		else if (SubTabAimSelected == 2)
		{
			if (!FaceIt)
			{
				DCheckBox("Silent", u8"Включить сайлент", Weapons[GetWeap(SelectedWeapon)].Silent, u8"Включает скрытую наводку");
				SliderInts("%chance", u8"%Попадания", Weapons[GetWeap(SelectedWeapon)].SilentHitchance, 0, 100, u8"");

				SliderFloats("Fov", u8"Фов", Weapons[GetWeap(SelectedWeapon)].SilentFov, 0, 30, "");

				if (!EntityAim)
				{
					X1Gui().Spacing();
					X1Gui().Separator();
					X1Gui().Spacing();

					DCheckBox("Nearest", u8"Nearest", Weapons[GetWeap(SelectedWeapon)].SilentNearest, u8"Автоматический выбор хитбокса");

					if (!Weapons[GetWeap(SelectedWeapon)].SilentNearest)
					{
						VectorEx<const char*> itemsHBEng = { lolc("Head") , lolc("Neck"),lolc("Low Neck"), lolc("Body"), lolc("Thorax"), lolc("Chest"), lolc("Upper Chest") };
						VectorEx<const char*> itemsHBRus = { lolc(u8"Голова") ,  lolc(u8"Шея"),lolc(u8"Низ шеи"), lolc(u8"Тело"), lolc(u8"Пояс"), lolc(u8"Грудь"),lolc(u8"Верх груди") };
						DComboBox("HitBox", u8"Хитбокс", Weapons[GetWeap(SelectedWeapon)].SilentHitBox, itemsHBEng, itemsHBRus, "");
					}

					X1Gui().Spacing();
					X1Gui().Spacing();
				}
				X1Gui().Spacing();
				X1Gui().Separator();
				X1Gui().Spacing();

				SliderInts("Start Bullet", u8"Начальная пуля", Weapons[GetWeap(SelectedWeapon)].SilentStartBullet, 0, 100, "");
				SliderInts("End Bullet", u8"Конечная пуля", Weapons[GetWeap(SelectedWeapon)].SilentEndBullet, 0, 100, u8"0 Для отключения проверки пуль");
			}
		}
		else if (SubTabAimSelected == 3)
		{
			DCheckBox("Target Switch Delay", u8"Задержка выбора", Weapons[GetWeap(SelectedWeapon)].TargetSwitchDelayEnable, u8"Включает задержку выбора новой цели после убийства");

			if (Weapons[GetWeap(SelectedWeapon)].TargetSwitchDelayEnable)
				SliderFloats("Delay##tsw", u8"Задержка##тсв", Weapons[GetWeap(SelectedWeapon)].TargetSwitchDelay, 0, 1.f, "");

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("First Fire Delay", u8"Задержка перед первым выстрелом", Weapons[GetWeap(SelectedWeapon)].FirstFireDelayEnable, u8"Включает задержку для первого выстрела");

			if (Weapons[GetWeap(SelectedWeapon)].FirstFireDelayEnable)
			{
				DCheckBox("One Click Delay", u8"Задержка по 1 нажатию", Weapons[GetWeap(SelectedWeapon)].FirstFireDelayOneClick, u8"");

				if (!EntityAim)
				{
					X1Gui().SameLine();
					DCheckBox("Auto", u8"Авто", Weapons[GetWeap(SelectedWeapon)].FirstFireDelayAuto, u8"Включает автоматическую задержку");
				}

				if (!FaceIt)
				{
					X1Gui().SameLine();
					DCheckBox("Silent Auto Delay", u8"Авто задержка для сайлента", Weapons[GetWeap(SelectedWeapon)].FirstFireDelaySilentAuto, u8"Выстрел просиходит только если цель попала в фов сайлента");
				}

				SliderFloats("Delay##ffdef", u8"Задержка##ффд", Weapons[GetWeap(SelectedWeapon)].FirstFireDelay, 0, 1.f, "");
			}

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			DCheckBox("Kill Stop", u8"Отключение после убийства", Weapons[GetWeap(SelectedWeapon)].KillStop, u8"Выключает аим после первого убийства");
		}
	}
	X1Gui().EndChild();
	X1Gui().Spacing();


	if (X1Gui().BeginChild(XorStr("TriggerChild"), Vec2(0, 206), true))
	{

		static int SubtabTrigMiscSelected = 0;

		static VectorEx<const char*> AimTrigSettingsEng = { "Basic##trig","Bind##trig" };

		TabsLabels(SubtabTrigMiscSelected, AimTrigSettingsEng, AimTrigSettingsEng, Vec2(X1Gui().GetCurWindowSize().x - (X1Gui().GetStyle().wndPadding.x * 2),0), false);

		X1Gui().Spacing();
		X1Gui().PushItemWidth(240.f);

		if (SubtabTrigMiscSelected == 0)
		{
			DCheckBox("Trigger", u8"Триггер", TriggerEnable, u8"Включает триггер");
			X1Gui().SameLine(163);
			DCheckBox("Deathmatch", u8"Триггер на всех", TriggerDeathmatch, u8"Включает триггер на  всех игроков");

			X1Gui().SameLine(360);
			X1Gui().BeginGroup();
			{
				vector<string> TrigHitboxes =
				{
					
					false/*MainCheatConfig.RusLanguage*/ ? lols(u8"Голова") : lols("Head"), //0
					false/*MainCheatConfig.RusLanguage*/ ? lols(u8"Грудь") : lols("Chest"), //1
					false/*MainCheatConfig.RusLanguage*/ ? lols(u8"Живот") : lols("Stomach"), //2
					false/*MainCheatConfig.RusLanguage*/ ? lols(u8"Левая рука") : lols("Left Arm"), //3
					false/*MainCheatConfig.RusLanguage*/ ? lols(u8"Правая рука") : lols("Right Arm"), //4
					false/*MainCheatConfig.RusLanguage*/ ? lols(u8"Левая нога") : lols("Left Leg"), //5
					false/*MainCheatConfig.RusLanguage*/ ? lols(u8"Правая нога") : lols("Right Leg") //6
				};
				if (X1Gui().ListBoxHeader("##TrigHitboxes", Vec2(90, 142)))
				{
					for (int i = 0; i < 7; i++)
					{
						bool selected = ((i == 0) && Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_HEAD) ||
							((i == 1) && Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_CHEST) ||
							((i == 2) && Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_STOMACH) ||
							((i == 3) && Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_LEFTARM) ||
							((i == 4) && Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_RIGHTARM) ||
							((i == 5) && Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_LEFTLEG) ||
							((i == 6) && Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_RIGHTLEG);

						if (X1Gui().SelectLabel(TrigHitboxes.at(i).c_str(), selected))
						{
							switch (i)
							{
							case 0: Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_HEAD = !Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_HEAD; break;
							case 1: Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_CHEST = !Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_CHEST; break;
							case 2: Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_STOMACH = !Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_STOMACH; break;
							case 3: Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_LEFTARM = !Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_LEFTARM; break;
							case 4: Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_RIGHTARM = !Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_RIGHTARM; break;
							case 5: Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_LEFTLEG = !Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_LEFTLEG; break;
							case 6: Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_RIGHTLEG = !Weapons[GetWeap(SelectedWeapon)].TRIG_HITGROUP_RIGHTLEG; break;
							default: break;
							}
						}
					}
				}
				X1Gui().ListBoxFooter();
				
				DCheckBox("All", u8"Все", Weapons[GetWeap(SelectedWeapon)].TriggerAllHitboxes, u8"Включает проверку всех хитбоксов");
			}
			X1Gui().EndGroup(true);

			DCheckBox("Flash Check", u8"Проверка ослепления", TriggerFlashCheck, u8"Выключает триггер если вы ослепли");
			X1Gui().SameLine(163);
			DCheckBox("Smoke Check", u8"Проверка дыма", TriggerSmokeCheck, u8"Выключает триггер если игрок за дымом");
			DCheckBox("Jump Check", u8"Проверка прыжка", TriggerJumpCheck, u8"Выключает триггер если Вы прыгнули");
			X1Gui().SameLine(163);
			DCheckBox("Jump Check Enemy", u8"Прыжок врага", TriggerJumpEnemyCheck, u8"Выключает триггер если игрок прыгнули");

			X1Gui().Spacing();

			SliderInts("Rcs X##Trigger", u8"Rcs по X##трг", Weapons[GetWeap(SelectedWeapon)].TriggerRcsX, 0, 150, u8"Сила контроля отдачи по оси X");
			SliderInts("Rcs Y##Trigger", u8"Rcs по Y##трг", Weapons[GetWeap(SelectedWeapon)].TriggerRcsY, 0, 150, u8"Сила контроля отдачи по оси Y");

			X1Gui().Spacing();

			SliderIntsProc("Hitchance##Trigger", u8"Хитшанс##трг", Weapons[GetWeap(SelectedWeapon)].TriggerChanse, 0, 100, u8"С");

			SliderFloats("Delay##trig", u8"Задержка##трг", Weapons[GetWeap(SelectedWeapon)].TriggerDelay, 0, 1.f, u8"Задержка перед первым выстрелом тригера");
		}
		else if (SubtabTrigMiscSelected == 1)
		{
			DCheckBox("Bind##triggg", u8"Бинд##тригг", TriggerBind.Enable, u8"");
			if (TriggerBind.Enable)
			{
				X1Gui().SameLine();
				DCheckBox("Hold##triggg", u8"Удержание##тригг", TriggerBind.Hold, u8"");
				HotsKey("Button##triggg", u8"Кнопка##тригг", TriggerBind.Button, u8"Задает кнопку активации тригера");
			}
		}
	}
	X1Gui().EndChild();
}


void CLegitAim::SubsectionsMenu()
{
	if (WeaponCustomTypes == 0)
	{
		if (SelectedWeapon == -1)
			SelectedWeapon = 0;

		static int selectedItem = SelectedWeapon;
		if (X1Gui().ListBoxHeader("##0", Vec2(154, 665 - 7)))
		{
			for (int i = 0; i < (sizeof(CGlobal::WeaponNames) / sizeof(*CGlobal::WeaponNames)); i++)
			{
				bool selected = i == SelectedWeapon;
				if (X1Gui().SelectLabel(CGlobal::WeaponNames[i].c_str(), selected, Vec2(144, 0)))
					SelectedWeapon = i;
			}
		}
		X1Gui().ListBoxFooter();
	}
	else if (WeaponCustomTypes == 1)
	{
		static int tabOrderCs[] = { 0 , 1 , 2 , 3 };


		string sub_name0 = lols("Rifles");
		string sub_name1 = lols("Pistols");
		string sub_name2 = lols("Shotguns");
		string sub_name3 = lols("Snipers");

		VectorEx<const char*>tabNamesCs = {
			sub_name0.c_str(),sub_name1.c_str() ,sub_name2.c_str() ,sub_name3.c_str() };

		Vec2 Pos = Vec2(0, 0);

		TabsLabels(StandartSubSelected, tabNamesCs, tabNamesCs, Vec2(X1Gui().GetCurWindowSize().x - (X1Gui().GetStyle().wndPadding.x * 2),
			X1Gui().GetCurWindowSize().y - (X1Gui().GetStyle().wndPadding.y * 2)), true);


		if (StandartSubSelected == 0)
			SelectedWeapon = 34;
		else if (StandartSubSelected == 1)
			SelectedWeapon = 35;
		else if (StandartSubSelected == 2)
			SelectedWeapon = 36;
		else if (StandartSubSelected == 3)
			SelectedWeapon = 37;
	}
	else if (WeaponCustomTypes == 2)
	{
		static char NewSubName[64] = { 't','e','s','t' };

		X1Gui().PushItemWidth(300);

		X1Gui().InputText(XorStr("Name"), NewSubName, 64);

		if (CustomSubs.size() < 33)
		{
			X1Gui().SameLine(365);
			if (X1Gui().Button(XorStr("Add"), Vec2(89, 0)) && string(NewSubName).length() > 0)
			{
				CustomSub  CustomEntry;
				CustomEntry.Idx = Weapons.size();
				CustomEntry.Name = NewSubName;
				CustomEntry.Name += "##" + to_string(CustomEntry.Idx);

				WeaponSettings WeaponEntry;
				WeaponEntry.WeaponName = CustomEntry.Name;
				WeaponEntry.IsCustomSub = true;

				CustomSubs.push_back(CustomEntry);
				Weapons.push_back(WeaponEntry);

				SelectedSub = CustomSubs.size() - 1;

				strcpy(NewSubName, "");
			}
		}

		if (SelectedSub < (int)CustomSubs.size() && SelectedSub >= 0)
		{
			if (X1Gui().Button(XorStr("Rename")))
			{
				if (string(NewSubName).length() > 0)
				{
					CustomSubs[SelectedSub].Name = NewSubName;
					strcpy(NewSubName, "");
				}
			}

			X1Gui().SameLine();
			if (X1Gui().Button(XorStr("Remove")))
			{
				for (size_t i = 0; i < CustomSubs[SelectedSub].AllocatedWeapon.size(); i++)
					UnallocatedWeapons.push_back(CustomSubs[SelectedSub].AllocatedWeapon[i]);

				for (size_t i = 0; i < Weapons.size(); i++)
					if (i == CustomSubs[SelectedSub].Idx)
					{
						for (size_t j(SelectedSub); j < CustomSubs.size(); j++)
							CustomSubs[j].Idx--;
						Weapons.erase(Weapons.begin() + i);
					}

				CustomSubs.erase(CustomSubs.begin() + SelectedSub);
			}

			X1Gui().Spacing();
			X1Gui().Separator();
			X1Gui().Spacing();

			if ((int)CustomSubs.size() <= SelectedSub)
				SelectedSub = CustomSubs.size() - 1;

			if (CustomSubs.size() > 0 && SelectedSub > -1)
			{
				if (SelectedSubAlloc < (int)CustomSubs[SelectedSub].AllocatedWeapon.size())
				{
					X1Gui().SameLine(155);
					if (X1Gui().Button(XorStr(">>>"), Vec2(148, 0)))
					{
						UnallocatedWeapons.push_back(CustomSubs[SelectedSub].AllocatedWeapon[SelectedSubAlloc]);
						CustomSubs[SelectedSub].AllocatedWeapon.erase(CustomSubs[SelectedSub].AllocatedWeapon.begin() + SelectedSubAlloc);
						if (SelectedSubAlloc > 0)
							SelectedSubAlloc--;
					}
				}

				if ((int)UnallocatedWeapons.size() > 0 && SelectedSubUnAlloc < (int)UnallocatedWeapons.size() && SelectedSubUnAlloc >= 0)
				{
					X1Gui().SameLine(310);
					if (X1Gui().Button(XorStr("<<<"), Vec2(144, 0)))
					{
						CustomSubs[SelectedSub].AllocatedWeapon.push_back(UnallocatedWeapons[SelectedSubUnAlloc]);
						UnallocatedWeapons.erase(UnallocatedWeapons.begin() + SelectedSubUnAlloc);

						if (SelectedSubUnAlloc > 0)
							SelectedSubUnAlloc--;
					}
				}
			}
		}

		if (CustomSubs.size() > 0)
		{
			if (X1Gui().ListBoxHeader(XorStr("##Subsections"), Vec2(149, 583)))
			{
				for (size_t i = 0; i < CustomSubs.size(); i++)
				{
					bool selected = i == SelectedSub;
					if (X1Gui().SelectLabel(CustomSubs[i].Name.c_str(), selected))
						SelectedSub = i;
				}
			}
			X1Gui().ListBoxFooter();

			X1Gui().SameLine();
			if (X1Gui().ListBoxHeader(XorStr("##SubsectionWeapons"), Vec2(149, 583)))
			{
				if (SelectedSub < (int)CustomSubs.size())
				{
					for (size_t i = 0; i < CustomSubs[SelectedSub].AllocatedWeapon.size(); i++)
					{
						bool selected = i == SelectedSubAlloc;
						if (X1Gui().SelectLabel(CustomSubs[SelectedSub].AllocatedWeapon[i].c_str(), selected))
							SelectedSubAlloc = i;
					}
				}
			}
			X1Gui().ListBoxFooter();

			X1Gui().SameLine();

			if (X1Gui().ListBoxHeader(XorStr("UnallocatedWeapons"), Vec2(146, 583)))
			{
				if (SelectedSubUnAlloc < (int)UnallocatedWeapons.size())
				{
					for (size_t i = 0; i < UnallocatedWeapons.size(); i++)
					{
						bool selected = i == SelectedSubUnAlloc;
						if (X1Gui().SelectLabel(UnallocatedWeapons[i].c_str(), selected))
							SelectedSubUnAlloc = i;
					}
				}
			}
			X1Gui().ListBoxFooter();

			if (SelectedSub < (int)CustomSubs.size())
				if (CustomSubs[SelectedSub].Idx < (int)Weapons.size())
					SelectedWeapon = CustomSubs[SelectedSub].Idx;
				else
					SelectedWeapon = 0;
			else
				SelectedWeapon = 0;
		}
	}
}