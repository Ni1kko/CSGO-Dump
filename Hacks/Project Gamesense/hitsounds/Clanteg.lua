local globals_realtime = globals.realtime
local globals_curtime = globals.curtime
local globals_frametime = globals.frametime
local globals_absolute_frametime = globals.absoluteframetime
local globals_maxplayers = globals.maxplayers
local globals_tickcount = globals.tickcount
local globals_tickinterval = globals.tickinterval
local globals_mapname = globals.mapname

local client_set_event_callback = client.set_event_callback
local client_console_log = client.log
local client_color_log = client.color_log
local client_console_cmd = client.exec
local client_userid_to_entindex = client.userid_to_entindex
local client_get_cvar = client.get_cvar
local client_set_cvar = client.set_cvar
local client_draw_debug_text = client.draw_debug_text
local client_draw_hitboxes = client.draw_hitboxes
local client_draw_indicator = client.draw_indicator
local client_random_int = client.random_int
local client_random_float = client.random_float
local client_draw_text = client.draw_text
local client_draw_rectangle = client.draw_rectangle
local client_draw_line = client.draw_line
local client_draw_gradient = client.draw_gradient
local client_draw_cricle = client.draw_circle
local client_draw_circle_outline = client.draW_circle_outline
local client_world_to_screen = client.world_to_screen
local client_screen_size = client.screen_size
local client_visible = client.visible
local client_delay_call = client.delay_call
local client_latency = client.latency
local client_camera_angles = client.camera_angles
local client_trace_line = client.trace_line
local client_eye_position = client.eye_position
local client_set_clan_tag = client.set_clan_tag
local client_system_time = client.system_time

local entity_get_local_player = entity.get_local_player
local entity_get_all = entity.get_all
local entity_get_players = entity.get_players
local entity_get_classname = entity.get_classname
local entity_set_prop = entity.set_prop
local entity_get_prop = entity.get_prop
local entity_is_enemy = entity.is_enemy
local entity_get_player_name = entity.get_player_name
local entity_get_player_weapon = entity.get_player_weapon
local entity_hitbox_position = entity.hitbox_position
local entity_get_steam64 = entity.get_steam64
local entity_get_bounding_box = entity.get_bounding_box
local entity_is_alive = entity.is_alive
local entity_is_dormant = entity.is_dormant

local ui_new_checkbox = ui.new_checkbox
local ui_new_slider = ui.new_slider
local ui_new_combobox = ui.new_combobox
local ui_new_multiselect = ui.new_multiselect
local ui_new_hotkey = ui.new_hotkey
local ui_new_button = ui.new_button
local ui_new_color_picker = ui.new_color_picker
local ui_reference = ui.reference
local ui_set = ui.set
local ui_get = ui.get
local ui_set_callback = ui.set_callback
local ui_set_visible = ui.set_visible
local ui_is_menu_open = ui.is_menu_open

local math_floor = math.floor
local math_random = math.random
local math_sqrt = math.sqrt
local table_insert = table.insert
local table_remove = table.remove
local table_size = table.getn
local table_sort = table.sort
local string_format = string.format
local string_length = string.len
local string_reverse = string.reverse
local string_sub = string.sub

-- Key value table for holding preset clan tags
local clantag = {

    ["Skeet"] = "skeet.cc",
    ["Pasteware"] = "pasteware",
    ["13 godz"] = "13 godz",

}

-- Function for collecting the keys for the clantag combobox
local function getMenuItems()

    local names = {}

    for k, v in pairs(clantag) do

        names[#names + 1] = k

    end

    table_sort(names)
    table_insert(names, 1, "Disabled")
    table_insert(names, "Custom")

    return names

end

-- Menu
local menu = {

    enabled = ui_new_checkbox("MISC", "Miscellaneous", "Clantag changer"),
    clantags = ui_new_combobox("MISC", "Miscellaneous", "Clan tags", getMenuItems()),
    animated = ui_new_checkbox("MISC", "Miscellaneous", "Animated tag"),
    style = ui_new_combobox("MISC", "Miscellaneous", "Animation style", "Default", "Reverse"),
    speed = ui_new_slider("MISC", "Miscellaneous", "Animation speed", 0, 100, 30, true, "%", 1)

}

-- Variables
local sClanTag
local bSendPacket
local bStaticStatus
local iClanTagIndex
local iClanTagReverseIndex
local iClantTagPreviousIndex
local iTagLength

-- Function for handling the menu
local function handleMenu()

    if ui_get(menu.enabled) then

        ui_set_visible(menu.clantags, true)
        ui_set_visible(menu.animated, true)

        if ui_get(menu.animated) then

            ui_set_visible(menu.style, true)
            ui_set_visible(menu.speed, true)

        else

            ui_set_visible(menu.style, false)
            ui_set_visible(menu.speed, false)

        end

    else

        client_set_clan_tag("\0")
        ui_set_visible(menu.clantags, false)
        ui_set_visible(menu.animated, false)
        ui_set_visible(menu.style, false)
        ui_set_visible(menu.speed, false)

    end

end

handleMenu()
ui_set_callback(menu.enabled, handleMenu)
ui_set_callback(menu.animated, handleMenu)

-- Run command event
client_set_event_callback("run_command", function(e)

    if not ui_get(menu.enabled) then

        return

    end

    -- Checks to see if packets are being sent, and setting the value of bSendPacket
    if e.chokedcommands == 0 then

        bSendPacket = false

    else

        bSendPacket = true

    end

end)

-- Function for updated clantag information
local function handleClanTags()

    if not ui_get(menu.enabled) or ui_get(menu.clantags) == "Disabled" then

        -- Disabling the clantag if it's set to "Disabled"
        client_set_clan_tag("\0")
        return

    end

    -- Getting and settings the clantag string
    if ui_get(menu.clantags) == "Custom" then

        if sClanTag ~= client_get_cvar("r_eyegloss") then

            sClanTag = client_get_cvar("r_eyegloss")
            bStaticStatus = false

        end

    else

        if sClanTag ~= clantag[ui_get(menu.clantags)] then

            sClanTag = clantag[ui_get(menu.clantags)]
            bStaticStatus = false

        end

    end

    -- Getting the tag length, and clamping it to 16
    iTagLength = math.min(16, string_length(sClanTag))

end

handleClanTags()
ui_set_callback(menu.clantags, handleClanTags)

-- Animating function
local function animateClanTag(style, index)

    if not ui_get(menu.animated) then

        return

    end

    if style == "Default" then
		local sTime = math.fmod(globals_curtime(), 48)

		if sTime == 0  then client_set_clan_tag("gideonproject") end
		if sTime == 10 then client_set_clan_tag("deonproject  ") end
		if sTime == 11 then client_set_clan_tag("onproject    ") end
		if sTime == 12 then client_set_clan_tag("project      ") end
		if sTime == 13 then client_set_clan_tag("oject        ") end
		if sTime == 14 then client_set_clan_tag("ect          ") end
		if sTime == 15 then client_set_clan_tag("             ") end
		if sTime == 16 then client_set_clan_tag("            g") end
		if sTime == 17 then client_set_clan_tag("           g ") end
		if sTime == 18 then client_set_clan_tag("          g  ") end
		if sTime == 19 then client_set_clan_tag("         g  i") end
		if sTime == 20 then client_set_clan_tag("        g  i ") end
		if sTime == 21 then client_set_clan_tag("       g  i  ") end
		if sTime == 22 then client_set_clan_tag("      g  i  d") end
		if sTime == 23 then client_set_clan_tag("     g  i  d ") end
		if sTime == 24 then client_set_clan_tag("    g  i  d  ") end
		if sTime == 25 then client_set_clan_tag("   g  i  d  e") end
		if sTime == 26 then client_set_clan_tag("  g  i  d  e ") end
		if sTime == 27 then client_set_clan_tag(" g  i  d  e  ") end
		if sTime == 28 then client_set_clan_tag("g  i  d  e  o") end
		if sTime == 29 then client_set_clan_tag("g i  d  e  o ") end
		if sTime == 30 then client_set_clan_tag("gi  d  e  o  ") end
		if sTime == 31 then client_set_clan_tag("gid e  o  n  ") end
		if sTime == 32 then client_set_clan_tag("gide  o  n  p") end
		if sTime == 33 then client_set_clan_tag("gide o  n  p ") end
		if sTime == 34 then client_set_clan_tag("gideo  n  p  ") end
		if sTime == 35 then client_set_clan_tag("gideo n  p  r") end
		if sTime == 36 then client_set_clan_tag("gideon  p  r ") end
		if sTime == 37 then client_set_clan_tag("gideon p  r  ") end
		if sTime == 38 then client_set_clan_tag("gideonp  r  o") end
		if sTime == 39 then client_set_clan_tag("gideonp r  o ") end
		if sTime == 40 then client_set_clan_tag("gideonpr  o  ") end
		if sTime == 41 then client_set_clan_tag("gideonpr o  j") end
		if sTime == 42 then client_set_clan_tag("gideonpro  j ") end
		if sTime == 43 then client_set_clan_tag("gideonproj  e") end
		if sTime == 44 then client_set_clan_tag("gideonproj e ") end
		if sTime == 45 then client_set_clan_tag("gideonproje  ") end
		if sTime == 46 then client_set_clan_tag("gideonproje c") end
		if sTime == 47 then client_set_clan_tag("gideonprojec ") end

    elseif style == "Reverse" then

        if iClanTagReverseIndex <= iTagLength then

            client_set_clan_tag(string_sub(sClanTag, 1, index))

        else

            if iTagLength - index == 0 then

                sClanTag = "\0"
                index = iTagLength - 1

            end

            client_set_clan_tag(string_sub(sClanTag, 1, iTagLength - index))

        end

    end

end

-- Paint event (Used so clantag updates while dead)
client_set_event_callback("paint", function(ctx)

    if not ui_get(menu.enabled) or ui_get(menu.clantags) == "Disabled" then

        return

    end

    -- Run command only runs while alive, if you're dead you're not sending packets.
    if not entity_is_alive(entity_get_local_player()) then

        bSendPacket = false

    end

    -- Don't update animated tag if choking packets
    if bSendPacket then

        return

    end

    -- Updating clantag information
    handleClanTags()

    -- Checking if the clan tag is animated
    if not ui_get(menu.animated) then

        -- Setting the static clantag once
        if not bStaticStatus then

            client_set_clan_tag(sClanTag)
            bStaticStatus = true

        end

    else
        
        -- Adding a few characters onto the length to add a slight pause when animating.
        iTagLength = iTagLength + 3

    end

    -- Getting the current clantag indexes
    iClanTagIndex = math_floor((globals_curtime() * (ui_get(menu.speed) / 10)) % iTagLength + 1)
    iClanTagReverseIndex = math_floor((globals_curtime() * (ui_get(menu.speed) / 10)) % (iTagLength * 2) + 1)

    -- Checking if the last index is the same as the current index to avoid spamming
    if iClanTagIndex == iClantTagPreviousIndex then

        return

    end

    -- Setting the previous Index
    iClantTagPreviousIndex = iClanTagIndex

    -- Call animation function
    animateClanTag(ui_get(menu.style), iClanTagIndex)

end)