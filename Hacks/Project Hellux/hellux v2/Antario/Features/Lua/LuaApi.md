# hellux API for Lua

<a name="-1"></a>

|Contents|
|--------|
|[print](#0)|
|[client.run_script](#1)|
|[client.set_event_callback](#2)|
|[client.is_key_down](#3)|
|[client.is_key_up](#4)|
|[client.exec](#5)|
|[client.userid_to_entindex](#6)|
|[client.draw_debug_text](#7)|
|[client.draw_hitbox](#8)|
|[client.random_int](#9)|
|[client.random_float](#q)|
|[client.screen_size](#w)|
|[client.visible](#e)|
|[client.trace_line](#r)|
|[client.trace_bullet](#a)|
|[client.scale_damage](#s)|
|[client.latency](#d)|
|[client.camera_angles](#f)|
|[client.timestamp](#z)|
|[client.eye_position](#x)|
|[client.set_clan_tag](#c)|
|[client.system_time](#v)|
|[client.cursor_pos](#b)|
|[ui.set](#11)|
|[ui.get](#12)|
|[ui.checkbox](#13)|
|[ui.text](#14)|
|[ui.keybind](#15)|
|[ui.slider](#16)|
|[ui.combobox](#17)|
|[ui.tooltip](#18)|
|[ui.is_menu_open](#19)|
|[ui.set_visible](#1q)|
|[globals.realtime](#21)|
|[globals.framecount](#22)|
|[globals.absoluteframetime](#23)|
|[globals.curtime](#24)|
|[globals.frametime](#25)|
|[globals.maxclients](#26)|
|[globals.tickcount](#27)|
|[globals.interval_per_tick](#28)|
|[globals.interpolation_amount](#29)|

---

## <a name="0"></a>print( text )
| Argument | Type | Description |
|-------------------------------|
| text | string | String to write. Must be wrapped in **tostring()** if it's not a string. |

Outputs text to console.

```lua
-- Print hello world to console
print("Hello world!")
-- Print sum
print(tostring(2 + 1))
```

[back to ToC](#-1)

## <a name="1"></a>client.run_script( script_name )
| Argument | Type | Description |
|-------------------------------|
| script_name | string | Name of script. Example: **autorun.lua** |

Loads script if not loaded by **script_name**. May be used in **autorun.lua**.

```lua
client.run_script("tetris.lua")
```

[back to ToC](#-1)

## <a name="2"></a>client.set_event_callback( event, function )
| Argument | Type | Description |
|-------------------------------|
| event | string | Name of event. Reference: [here](https://gideonhack.pw/index.php?/topic/29-event-list/) |
| function | function(...) | Function to call when event raises |

Subscribes to event **event**.

```lua
client.set_event_callback("on_paint", function()
	render.text(5, 5, "It works!", 255, 255, 0, 255)
end)
```

[back to ToC](#-1)

## <a name="3"></a>client.is_key_down( key ): *boolean*
| Argument | Type | Description |
|-------------------------------|
| key | number | Key to check. Reference: [here](https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes) |

Checks if **key** is currently down.

```lua
-- key pressed implementation
function client.is_key_pressed(key, active)
	if client.is_key_down(key) then -- if key is down
		if not active then -- and we hold it first time
			return true -- then key is pressed
		end

		active = true
	else
		active = false
	end

	return false -- else it's not
end

-- ...

local m1pressed = false
client.set_event_callback("on_paint", function()
	if client.is_key_pressed(1, m1pressed) then
		print("Mouse1 pressed!")
	end
end)
```

[back to ToC](#-1)

## <a name="4"></a>client.is_key_up( key ): *boolean*
| Argument | Type | Description |
|-------------------------------|
| key | number | Key to check. Reference: [here](https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes) |

Checks if **key** is currently up.

```lua
client.set_event_callback("on_paint", function()
	if client.is_key_up(1) then
		render.text(5, 5, "Mouse1 is up!", 255, 255, 255, 255)
	end
end)
```

[back to ToC](#-1)

## <a name="5"></a>client.exec( command, unrestrict )
| Argument | Type | Description |
|-------------------------------|
| command | string | Command to execute |
| unrestrict | boolean | If command is a cheat command (e.g. **sv_cheats 1**), then you must put true to let it be executed |

Executes CS:GO command.

```lua
client.exec('say "owned by gideonproject"', true)
```

[back to ToC](#-1)

## <a name="6"></a>client.userid_to_entindex( user_id ): *number*
| Argument | Type | Description |
|-------------------------------|
| user_id | number | ID of user to convert |

Converts **user_id** to entity index.

```lua
client.draw_hitbox(client.userid_to_entindex(1), 5, 1, 255, 255, 255, 255)
```

[back to ToC](#-1)

## <a name="7"></a>client.draw_debug_text( x, y, z, line_offset, duration, r, g, b, a, text )
| Argument | Type | Description |
|-------------------------------|
| x | float | X coordinate |
| y | float | Y coordinate |
| z | float | Z coordinate |
| line_offset | number | Used for line alignment |
| duration | float | Duration in seconds |
| r | number | Red color |
| g | number | Green color |
| b | number | Blue color |
| a | number | Alpha color |
| text | string | Text to draw. Must be wrapped in **tostring()** if not a string |

Draws text in world space at **x**,  **y**, **z**

```lua
client.draw_debug_text(50, 50, 50, 0, 10, 255, 255, 255, 255, "Sick")
```

[back to ToC](#-1)

## <a name="8"></a>client.draw_hitbox( ent_index, duration, hitbox_number, r, g, b, a )
| Argument | Type | Description |
|-------------------------------|
| ent_index | number | Index of entity |
| duration | float | Duration in seconds |
| hitbox_number | number | ID of hitbox |
| r | number | Red color |
| g | number | Green color |
| b | number | Blue color |
| a | number | Alpha color |

Draws hitbox capsule on entity. Use **hitbox_number** 19 to draw all hitboxes.

```lua
client.draw_hitbox(enitity.get_players()[1], 5, 19, 255, 255, 255, 255)
```

[back to ToC](#-1)

## <a name="9"></a>client.random_int( max, min ): *number*
| Argument | Type | Description |
|-------------------------------|
| max | number | Maximal number |
| min | *optional (0)* number | Minimal number |

Returns random number from **min** to **max**

```lua
local rnd = client.random_int(100)
```

[back to ToC](#-1)

## <a name="q"></a>client.random_float( max, min ): *float*
| Argument | Type | Description |
|-------------------------------|
| max | float | Maximal number |
| min | *optional (0.0)* float | Minimal number |

Returns random float from **min** to **max**

```lua
local rnd = client.random_float(50.0, 20.0)
```

[back to ToC](#-1)

## <a name="w"></a>client.screen_size(  ): *number, number*
Returns width and height of screen

```lua
local w, h = client.screen_size()
```

[back to ToC](#-1)

## <a name="e"></a>client.visible( x, y, z ): *boolean*
| Argument | Type | Description |
|-------------------------------|
| x | float | X coord |
| y | float | Y coord |
| z | float | Z coord |

Returns true if point is visible for localplayer.

```lua
client.visible(50, 50, 50)
```

[back to ToC](#-1)

## <a name="r"></a>client.trace_line( skip_entindex, from_x, from_y, from_z, to_x, to_y, to_z ): *float, number*
| Argument | Type | Description |
|-------------------------------|
| skip_entindex| number | Index of entity to skip |
| from_x, from_y, from_z | float, float, float | Start X, Y, Z coords |
| to_x, to_y, to_z | float, float, float | End X, Y, Z coords |

Traces line and returns fraction (how long line has been traced) and entity index (if entity was hit)

```lua
client.trace_line(entity.get_local_player(), 0, 0, 0, 50, 50, 50)
```

[back to ToC](#-1)

## <a name="a"></a>client.trace_bullet( from_entindex, from_x, from_y, from_z, to_x, to_y, to_z ): *number, number*
| Argument | Type | Description |
|-------------------------------|
| from_entindex | number | From what entity trace bullet |
| from_x, from_y, from_z | float, float, float | Start X, Y, Z coords |
| to_x, to_y, to_z | float, float, float | End X, Y, Z coords |

Tries to penetrate walls by entity. Returns what entity was hit and damage done to it

```lua
client.trace_bullet(entity.get_local_player(), 0, 0, 0, 50, 50, 50)
```

[back to ToC](#-1)

## <a name="s"></a>client.scale_damage( entindex, hitgroup, damage ): *number*
| Argument | Type | Description |
|-------------------------------|
| entindex | number | Entity to take weapon from |
| hitgroup | number | Hitgroup to aim |
| damage | number | Base damage |

Returns adjusted damage for specific hitgroup

```lua
local damage = 50
damage = client.scale_damage(entity.get_local_player(), 1, damage)
```

[back to ToC](#-1)

## <a name="d"></a>client.latency(  ): *float*
Returns your latency in seconds

```lua
local ping = client.latency()
```

[back to ToC](#-1)

## <a name="f"></a>client.camera_angles(  ): *float, float, float*
Returns pitch, yaw, roll of view camera

```lua
local x, y, z = client.camera_angles()
```

[back to ToC](#-1)

## <a name="z"></a>client.timestamp(  ): *number*
Returns time since system start in milliseconds

```lua
local time = client.timestamp()
```

[back to ToC](#-1)

## <a name="x"></a>client.eye_position(  ): *float, float, float*
Return x, y, z of localplayer eyes position

```lua
local x, y, z = client.eye_position()
```

[back to ToC](#-1)

## <a name="c"></a>client.set_clan_tag( tag )
| Argument | Type | Description |
|-------------------------------|
| tag | string | ClanTag to set. If tag is not string, then it must be wrapped in **tostring()** |

Sets clantag.

```lua
-- Note: you must NOT set clantag every tick because it will lead lags in movement.
-- It's better to set it each half of second for example (delay must be at least 100ms)
client.set_clan_tag("AIMWARE.net")
```

[back to ToC](#-1)

## <a name="v"></a>client.system_time(  ): *number, number, numer*
Returns hour, minute, seconds of localtime

```lua
local h, m, s = client.system_time()
```

[back to ToC](#-1)

## <a name="b"></a>client.cursor_pos(  ): *number, number*
Returns X and Y points of cursor

```lua
local mouse_x, mouse_y = client.cursor_pos()
```

[back to ToC](#-1)

## <a name="11"></a>ui.set( item, value )
| Argument | Type | Description |
|-------------------------------|
| item | string | Menu item (**TAB > Group > Item > Keybind/Color/Color 1/Color 2**) |
| value | boolean/number/float/r, g, b, a | Value to set |

Sets **value** to **item**

```lua
-- Checkboxes
ui.set("MISC > Cheat settings > Low FPS warning", true)
-- Sliders
ui.set("MISC > Misc preferences > Ping increase", 25)
-- Colors
ui.set("MISC > Cheat settings > Menu color", 255, 255, 255, 255) -- Note: "Color" is not appended because we don't have other element at the same line
-- Also you can store floats if you want, but that affect nothing
ui.set("SomeVariable", 0.0)
```

[back to ToC](#-1)

## <a name="12"></a>ui.get( item ): *boolean/number/float/r, g, b, a*
| Argument | Type | Description |
|-------------------------------|
| item | string | Menu item (see **ui.set** to understand how to use this) |

Returns value of **item**

```lua
-- Checkboxes
local enabled = ui.get_bool("MISC > Cheat settings > Low FPS warning")
-- Sliders
local ping = ui.get_int("MISC > Misc preferneces > Ping increase")
-- Colors
local r, g, b, a = ui.get_color("Misc > Cheat settings > Menu color")
-- Custom
local someFloat = ui.get_float("SomeVariable")
```

[back to ToC](#-1)

## <a name="13"></a>ui.checkbox( group, label, var )
| Argument | Type | Description |
|-------------------------------|
| group | string | Where to create checkbox |
| label | string | Label of checkbox |
| var | string | Variable to use in **ui.get** or **ui.set** |

Creates checkbox.

```lua
ui.checkbox("MISC > Cheat settings", "Rainbow menu", "var_RainbowMenuEnable")
```

[back to ToC](#-1)

## <a name="14"></a>ui.text( group, label )
| Argument | Type | Description |
|-------------------------------|
| group | string | Where to create text |
| label | string | Label of text |

Creates text label.

```lua
ui.text("MISC > Misc preferences", "You are noob")
```

[back to ToC](#-1)

## <a name="15"></a>ui.keybind( group, id, var )
| Argument | Type | Description |
|-------------------------------|
| group | string | Where to create keybind |
| id | string | Any random ID |
| var | string | Variable to use in **ui.get/set** |

Creates keybind after last element in menu (e.g. if you call it after checkbox then it will create a keybind there)

```lua
local g = "MISC > Cheat settings"
ui.text(g, "Force baim v2")
ui.keybind(g, "key_forcebaim", "key_forcebaim")
```

[back to ToC](#-1)

## <a name="16"></a>ui.slider( group, label, var, min, max, format )
| Argument | Type | Description |
|-------------------------------|
| group | string | Where to create slider |
| label | string | Label of slider |
| var | string | Variable to use in **ui.get/set** |
| min | number | Minimal value |
| max | number | Maximal value |
| format | *optional (%d)* string | Format string (check printf C function for reference) |

Creates slider.

```lua
ui.slider("MISC > Misc preferences", "Rainbow menu speed", "var_rainbotMenuSpd", 0, 100)
```

[back to ToC](#-1)

## <a name="17"></a>ui.combobox( group, label, var, values )
| Argument | Type | Description |
|-------------------------------|
| group | string | Where to create combobox |
| label | string | Label of combobox |
| var | string | Variable to use in **ui.get/set** |
| values | table | List of values to store in combobox |

Creates combobox.

```lua
ui.combobox("MISC > Misc preferences", "Custom clan-tag", "var_customCTag", { "AIMWARE.net", "gaysense" })
```

[back to ToC](#-1)

## <a name="18"></a>ui.tooltip( group, text )
| Argument | Type | Description |
|-------------------------------|
| group | string | Where to create tooltip |
| text | string | Text to put inside tooltip |

Creates tooltip after last element in menu.

```lua
local g = "MISC > Cheat settings"
ui.text(g, "This is text")
ui.tooltip(g, "And this little thing is tooltip!")
```

[back to ToC](#-1)

## <a name="19"></a>ui.is_menu_open(  ): *boolean*
Returns is menu open

```lua
local open = ui.is_menu_open()
```

[back to ToC](#-1)

## <a name="1q"></a>ui.set_visible( group, query, value )
| Argument | Type | Description |
|-------------------------------|
| group | string | Where to search for element |
| query | string | Variable or title of element |
| value | boolean | New is_visible value |

Changes visibility of menu item. Only affects to lua-created items.

```lua
ui.set_visible("MISC > Cheat preferences", "Rainbow menu", false)
```

[back to ToC](#-1)

## <a name="21"></a>globals.realtime(  )
Returns time since game start in seconds

```lua
local realtime = globals.realtime()
```

[back to ToC](#-1)

## <a name="22"></a>globals.framecount(  )
Returns total count of frames since game start

```lua
local frames = globals.framecount()
```

[back to ToC](#-1)

## <a name="23"></a>globals.absoluteframetime(  )
Returns absolute time of frame render

```lua
local aft = globals.absoluteframetime()
```

[back to ToC](#-1)

## <a name="24"></a>globals.curtime(  )
Returns time since you're on a server

```lua
local curtime = globals.curtime()
```

[back to ToC](#-1)

## <a name="25"></a>globals.frametime(  )
Returns render time of last frame

```lua
local realtime = globals.realtime()
```

[back to ToC](#-1)

## <a name="26"></a>globals.maxclients(  )
Returns maximal players count on server

```lua
local clients = globals.maxclients()
```

[back to ToC](#-1)

## <a name="27"></a>globals.tickcount(  )
Returns count of ticks

```lua
local ticks = globals.tickcount()
```

[back to ToC](#-1)

## <a name="28"></a>globals.interval_per_tick(  )
Returns time passed since last tick

```lua
local ipt = globals.interval_per_tick()
```

[back to ToC](#-1)

## <a name="29"></a>globals.interpolation_amount(  )
Returns amount of interp (backtrack)

```lua
local interp = globals.interpolation_amount()
```

[back to ToC](#-1)