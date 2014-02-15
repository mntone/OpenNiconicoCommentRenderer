/*
	about mntoneÅfs framework for Windows

	The MIT License (MIT)

	Copyright (c) 2014 mntone

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once
#include <Windows.h>

namespace mnfx {

	enum class window_message: UINT
	{
		null = WM_NULL,
		create = WM_CREATE,
		destroy = WM_DESTROY,
		move = WM_MOVE,
		size = WM_SIZE,
		activate = WM_ACTIVATE,
		set_focus = WM_SETFOCUS,
		kill_focus = WM_KILLFOCUS,
		enable = WM_ENABLE,
		set_redraw = WM_SETREDRAW,
		set_text = WM_SETTEXT,
		get_text = WM_GETTEXT,
		get_text_length = WM_GETTEXTLENGTH,
		paint = WM_PAINT,
		close = WM_CLOSE,
#ifndef _WIN32_WCE
		query_end_session = WM_QUERYENDSESSION,
		query_open = WM_QUERYOPEN,
		end_session = WM_ENDSESSION,
#endif
		quit = WM_QUIT,
		erase_background = WM_ERASEBKGND,
		system_color_change = WM_SYSCOLORCHANGE,
		show_window = WM_SHOWWINDOW,
		winini_change = WM_WININICHANGE,
#if( WINVER >= 0x0400 )
		setting_change = WM_SETTINGCHANGE,
#endif

		device_mode_change = WM_DEVMODECHANGE,
		activate_app = WM_ACTIVATEAPP,
		font_change = WM_FONTCHANGE,
		time_change = WM_TIMECHANGE,
		cancel_mode = WM_CANCELMODE,
		set_cursor = WM_SETCURSOR,
		mouse_activate = WM_MOUSEACTIVATE,
		child_activate = WM_CHILDACTIVATE,
		queue_sync = WM_QUEUESYNC,

		get_minmax_info = WM_GETMINMAXINFO,

		paint_icon = WM_PAINTICON,
		icon_erase_background = WM_ICONERASEBKGND,
		next_dialog_control = WM_NEXTDLGCTL,
		spooler_status = WM_SPOOLERSTATUS,
		draw_item = WM_DRAWITEM,
		measure_item = WM_MEASUREITEM,
		delete_imte = WM_DELETEITEM,
		virtual_key_to_item = WM_VKEYTOITEM,
		char_to_item = WM_CHARTOITEM,
		set_font = WM_SETFONT,
		get_font = WM_GETFONT,
		set_hotkey = WM_SETHOTKEY,
		get_hotkey = WM_GETHOTKEY,
		query_drag_icon = WM_QUERYDRAGICON,
		compare_item = WM_COMPAREITEM,
#if( WINVER >= 0x0500 )
#ifndef _WIN32_WCE
		get_object = WM_GETOBJECT,
#endif
#endif
		compacting = WM_COMPACTING,
		//comm_notify = WM_COMMNOTIFY, <- no longer support
		window_position_changing = WM_WINDOWPOSCHANGING,
		window_position_changed = WM_WINDOWPOSCHANGED,

		power = WM_POWER,

		copy_data = WM_COPYDATA,
		cancel_journal = WM_CANCELJOURNAL,

#if( WINVER >= 0x0400 )
		notify = WM_NOTIFY,
		input_language_change_request = WM_INPUTLANGCHANGEREQUEST,
		input_language_change = WM_INPUTLANGCHANGE,
		tcard = WM_TCARD,
		help = WM_HELP,
		user_changed = WM_USERCHANGED,
		notify_format = WM_NOTIFYFORMAT,

		context_menu = WM_CONTEXTMENU,
		style_changing = WM_STYLECHANGING,
		style_changed = WM_STYLECHANGED,
		display_change = WM_DISPLAYCHANGE,
		get_icon = WM_GETICON,
		set_icon = WM_SETICON,
#endif

		nc_create = WM_NCCREATE,
		nc_destroy = WM_NCDESTROY,
		nc_calculate_size = WM_NCCALCSIZE,
		nc_hittest = WM_NCHITTEST,
		nc_paint = WM_NCPAINT,
		nc_activate = WM_NCACTIVATE,
		get_dialog_code = WM_GETDLGCODE,
#ifndef _WIN32_WCE
		sync_paint = WM_SYNCPAINT,
#endif
		nc_mouse_move = WM_NCMOUSEMOVE,
		nc_left_button_down = WM_NCLBUTTONDOWN,
		nc_left_button_up = WM_NCLBUTTONUP,
		nc_left_button_double_click = WM_NCLBUTTONDBLCLK,
		nc_right_button_down = WM_NCRBUTTONDOWN,
		nc_right_button_up = WM_NCRBUTTONUP,
		nc_right_button_double_click = WM_NCRBUTTONDBLCLK,
		nc_middle_button_down = WM_NCMBUTTONDOWN,
		nc_middle_button_up = WM_NCMBUTTONUP,
		nc_middle_button_double_click = WM_NCMBUTTONDBLCLK,

#if( _WIN32_WINNT >= 0x0500 )
		nc_x_button_down = WM_NCXBUTTONDOWN,
		nc_x_button_up = WM_NCXBUTTONUP,
		nc_x_button_double_click = WM_NCXBUTTONDBLCLK,
#endif

#if( _WIN32_WINNT >= 0x0501 )
		input_device_change = WM_INPUT_DEVICE_CHANGE,
		input = WM_INPUT,
#endif

		key_first = WM_KEYFIRST,
		key_down = WM_KEYDOWN,
		key_up = WM_KEYUP,
		wmchar = WM_CHAR,
		dead_char = WM_DEADCHAR,
		system_key_down = WM_SYSKEYDOWN,
		system_key_up = WM_SYSKEYUP,
		system_char = WM_SYSCHAR,
		system_dead_char = WM_SYSDEADCHAR,
#if( _WIN32_WINNT >= 0x0501 )
		unicode_char = WM_UNICHAR,
#endif
		key_last = WM_KEYLAST,

#if( WINVER >= 0x0400 )
		ime_start_composition = WM_IME_STARTCOMPOSITION,
		ime_end_composition = WM_IME_ENDCOMPOSITION,
		ime_composition = WM_IME_COMPOSITION,
		ime_key_last = WM_IME_KEYLAST,
#endif

		initialize_dialog = WM_INITDIALOG,
		command = WM_COMMAND,
		system_command = WM_SYSCOMMAND,
		timer = WM_TIMER,
		horizontal_scroll = WM_HSCROLL,
		vertical_scroll = WM_VSCROLL,
		initialize_menu = WM_INITMENU,
		initialize_menu_popup = WM_INITMENUPOPUP,
#if( WINVER >= 0x0601 )
		gesture = WM_GESTURE,
		gesture_notify = WM_GESTURENOTIFY,
#endif
		menu_select = WM_MENUSELECT,
		menu_char = WM_MENUCHAR,
		enter_idle = WM_ENTERIDLE,
#if( WINVER >= 0x0500 )
#ifndef _WIN32_WCE
		menu_right_button_up = WM_MENURBUTTONUP,
		menu_drag = WM_MENUDRAG,
		menu_get_object = WM_MENUGETOBJECT,
		uninitialize_menu_popup = WM_UNINITMENUPOPUP,
		menu_command = WM_MENUCOMMAND,

		change_ui_state = WM_CHANGEUISTATE,
		update_ui_state = WM_UPDATEUISTATE,
		query_ui_state = WM_QUERYUISTATE,
#endif
#endif

		control_color_message_box = WM_CTLCOLORMSGBOX,
		control_color_edit_box = WM_CTLCOLOREDIT,
		control_color_list_box = WM_CTLCOLORLISTBOX,
		control_color_button = WM_CTLCOLORBTN,
		control_color_dialog = WM_CTLCOLORDLG,
		control_color_scroll_bar = WM_CTLCOLORSCROLLBAR,
		control_color_static = WM_CTLCOLORSTATIC,

		mouse_first = WM_MOUSEFIRST,
		mouse_move = WM_MOUSEMOVE,
		left_button_down = WM_LBUTTONDOWN,
		left_button_up = WM_LBUTTONUP,
		left_button_double_click = WM_LBUTTONDBLCLK,
		right_button_down = WM_RBUTTONDOWN,
		right_button_up = WM_RBUTTONUP,
		right_button_double_click = WM_RBUTTONDBLCLK,
		middle_button_down = WM_MBUTTONDOWN,
		middle_button_up = WM_MBUTTONUP,
		middle_button_double_click = WM_MBUTTONDBLCLK,
#if( _WIN32_WINNT >= 0x0400 ) || ( _WIN32_WINDOWS > 0x0400 )
		mouse_wheel = WM_MOUSEWHEEL,
#endif
#if( _WIN32_WINNT >= 0x0500 )
		x_button_down = WM_XBUTTONDOWN,
		x_button_up = WM_XBUTTONUP,
		x_button_double_click = WM_XBUTTONDBLCLK,
#endif
#if( _WIN32_WINNT >= 0x0600 )
		mouse_horizontal_wheel = WM_MOUSEHWHEEL,
#endif

		mouse_last = WM_MOUSELAST,

		parent_notify = WM_PARENTNOTIFY,
		enter_menu_loop = WM_ENTERMENULOOP,
		exit_menu_loop = WM_EXITMENULOOP,

#if( WINVER >= 0x0400 )
		next_menu = WM_NEXTMENU,
		sizing = WM_SIZING,
		capture_changed = WM_CAPTURECHANGED,
		moving = WM_MOVING,

		power_broadcast = WM_POWERBROADCAST,

		device_change = WM_DEVICECHANGE,
#endif

#ifdef _USE_MDI
		mdi_create = WM_MDICREATE,
		mdi_destroy = WM_MDIDESTROY,
		mdi_activate = WM_MDIACTIVATE,
		mdi_restore = WM_MDIRESTORE,
		mdi_next = WM_MDINEXT,
		mdi_maximize = WM_MDIMAXIMIZE,
		mdi_tile = WM_MDITILE,
		mdi_cascade = WM_MDICASCADE,
		mdi_icon_arrange = WM_MDIICONARRANGE,
		mdi_get_active = WM_MDIGETACTIVE,

		mdi_set_menu = WM_MDISETMENU,
#endif
		enter_size_move = WM_ENTERSIZEMOVE,
		exit_size_move = WM_EXITSIZEMOVE,
		drop_files = WM_DROPFILES,
#ifdef _USE_MDI
		mdi_refresh_menu = WM_MDIREFRESHMENU,
#endif

#if( WINVER >= 0x0602 )
		pointer_device_change = WM_POINTERDEVICECHANGE,
		pointer_device_in_range = WM_POINTERDEVICEINRANGE,
		pointer_device_out_of_range = WM_POINTERDEVICEOUTOFRANGE,
#endif

#if( WINVER >= 0x0601 )
		touch = WM_TOUCH,
#endif

#if( WINVER >= 0x0602 )
		nc_pointer_update = WM_NCPOINTERUPDATE,
		nc_pointer_down = WM_NCPOINTERDOWN,
		nc_pointer_up = WM_NCPOINTERUP,
		pointer_update = WM_POINTERUPDATE,
		pointer_down = WM_POINTERDOWN,
		pointer_up = WM_POINTERUP,
		pointer_enter = WM_POINTERENTER,
		pointer_leave = WM_POINTERLEAVE,
		pointer_activate = WM_POINTERACTIVATE,
		pointer_capture_changed = WM_POINTERCAPTURECHANGED,
		touch_hittesting = WM_TOUCHHITTSTING,
		pointer_wheel = WM_POINTERWHEEL,
		pointer_horizontal_wheel = WM_POINTERHWHEEL,
#endif

#if( WINVER >= 0x0400 )
		ime_set_context = WM_IME_SETCONTEXT,
		ime_notify = WM_IME_NOTIFY,
		ime_control = WM_IME_CONTROL,
		ime_composition_full = WM_IME_COMPOSITIONFULL,
		ime_select = WM_IME_SELECT,
		ime_char = WM_IME_CHAR,
#endif
#if( WINVER >= 0x0500 )
		ime_request = WM_IME_REQUEST,
#endif
#if( WINVER >= 0x0400 )
		ime_key_down = WM_IME_KEYDOWN,
		ime_key_up = WM_IME_KEYUP,
#endif

#if( _WIN32_WINNT >= 0x0400 ) || ( WINVER >= 0x0500 )
		mouse_hover = WM_MOUSEHOVER,
		mouse_leave = WM_MOUSELEAVE,
#endif

#if( WINVER >= 0x0500 )
		nc_mouse_hover = WM_NCMOUSEHOVER,
		nc_mouse_leave = WM_NCMOUSELEAVE,
#endif

#if( WINVER >= 0x0501 )
		wts_session_change = WM_WTSSESSION_CHANGE,

		tablet_first = WM_TABLET_FIRST,
		tablet_last = WM_TABLET_LAST,
#endif

#if( WINVER >= 0x0601 )
		dpi_changed = WM_DPICHANGED,
#endif

		cut = WM_CUT,
		copy = WM_COPY,
		paste = WM_PASTE,
		clear = WM_CLEAR,
		undo = WM_UNDO,
#ifdef _USE_CLIPBOARD
		render_format = WM_RENDERFORMAT,
		render_all_formats = WM_RENDERALLFORMATS,
		destroy_clipboard = WM_DESTROYCLIPBOARD,
		draw_clipboard = WM_DRAWCLIPBOARD,
		paint_clipboard = WM_PAINTCLIPBOARD,
		vertical_scroll_clipboard = WM_VSCROLLCLIPBOARD,
		size_clipboard = WM_SIZECLIPBOARD,
		ask_callback_format_name = WM_ASKCBFORMATNAME,
		change_callback_chain = WM_CHANGECBCHAIN,
		horizontal_scroll_clipboard = WM_HSCROLLCLIPBOARD,
#endif
		query_new_palettle = WM_QUERYNEWPALETTE,
		palette_is_changing = WM_PALETTEISCHANGING,
		palette_changed = WM_PALETTECHANGED,
		hotkey = WM_HOTKEY,

#if( _WIN32_WINNT >= 0x0500 )
		wmpaint = WM_PAINT,
		paint_client = WM_PRINTCLIENT,
#endif

#if( _WIN32_WINNT >= 0x0500 )
		app_command = WM_APPCOMMAND,
#endif

#if( _WIN32_WINNT >= 0x0501 )
		theme_changed = WM_THEMECHANGED,

#ifdef _USE_CLIPBOARD
		clipboard_update = WM_CLIPBOARDUPDATE,
#endif
#endif

#if( _WIN32_WINNT >= 0x0600 )
		dwm_composition_changed = WM_DWMCOMPOSITIONCHANGED,
		dwm_nc_rendering_changed = WM_DWMNCRENDERINGCHANGED,
		dwm_colorization_color_changed = WM_DWMCOLORIZATIONCOLORCHANGED,
		dwm_window_maximized_change = WM_DWMWINDOWMAXIMIZEDCHANGE,
#endif

#if( _WIN32_WINNT >= 0x0601 )
		dwm_send_iconic_thumbnail = WM_DWMSENDICONICTHUMBNAIL,
		dwm_send_iconic_live_preview_bitmap = WM_DWMSENDICONICLIVEPREVIEWBITMAP,
#endif

#if( WINVER >= 0x0600 )
		get_title_bar_info_extra = WM_GETTITLEBARINFOEX,
#endif

#if( WINVER >= 0x0400 )
		hand_held_first = WM_HANDHELDFIRST,
		hand_held_last = WM_HANDHELDLAST,

		afx_first = WM_AFXFIRST,
		afx_last = WM_AFXLAST,
#endif

		pen_win_first = WM_PENWINFIRST,
		pen_win_last = WM_PENWINLAST,

#if( WINVER >= 0x0400 )
		app = WM_APP,
#endif

		user = WM_USER,
	};

}