MODULE := engines/qdengine

MODULE_OBJS = \
	qdengine.o \
	console.o \
	metaengine.o \
	core/qd_precomp.o \
	core/parser/qdscr_convert.o \
	core/parser/qdscr_parser.o \
	core/parser/xml_parser.o \
	core/parser/xml_tag_buffer.o \
	core/runtime/qd_dialogs.o \
	core/runtime/qd_dialog_control.o \
	core/runtime/comline_parser.o \
	core/runtime/qd_runtime.o \
	core/system/app_log.o \
	core/system/app_log_file.o \
	core/system/app_error_handler.o \
	core/system/app_core.o \
	core/system/sound/ds_snd_dispatcher.o \
	core/system/sound/ds_sound.o \
	core/system/sound/snd_dispatcher.o \
	core/system/sound/wav_file.o \
	core/system/sound/wav_sound.o \
	core/system/graphics/gdi_gr_dispatcher.o \
	core/system/graphics/gr_core.o \
	core/system/graphics/gr_dispatcher.o \
	core/system/graphics/gr_draw_sprite_rle_z.o \
	core/system/graphics/gr_draw_sprite_rle.o \
	core/system/graphics/gr_draw_sprite_z.o \
	core/system/graphics/gr_draw_sprite.o \
	core/system/graphics/gr_font.o \
	core/system/graphics/gr_screen_region.o \
	core/system/graphics/gr_tile_animation.o \
	core/system/graphics/gr_tile_sprite.o \
	core/system/graphics/rle_compress.o \
	core/system/graphics/UI_TextParser.o \
	core/system/input/mouse_input.o \
	core/system/input/keyboard_input.o \
	core/system/input/input_wndproc.o \
	core/system/input/input_recorder.o \
	core/qdcore/util/AIAStar_API.o \
	core/qdcore/util/fps_counter.o \
	core/qdcore/util/LZ77.o \
	core/qdcore/util/ResourceDispatcher.o \
	core/qdcore/qd_animation_frame.o \
	core/qdcore/qd_animation_info.o \
	core/qdcore/qd_animation_maker.o \
	core/qdcore/qd_animation_set_info.o \
	core/qdcore/qd_animation_set_preview.o \
	core/qdcore/qd_animation_set.o \
	core/qdcore/qd_animation.o \
	core/qdcore/qd_camera_mode.o \
	core/qdcore/qd_camera.o \
	core/qdcore/qd_condition_data.o \
	core/qdcore/qd_condition_group.o \
	core/qdcore/qd_condition_object_reference.o \
	core/qdcore/qd_condition.o \
	core/qdcore/qd_conditional_object.o \
	core/qdcore/qd_contour.o \
	core/qdcore/qd_coords_animation_point.o \
	core/qdcore/qd_coords_animation.o \
	core/qdcore/qd_counter.o \
	core/qdcore/qd_d3dutils.o \
	core/qdcore/qd_file_owner.o \
	core/qdcore/qd_file_manager.o \
	core/qdcore/qd_font_info.o \
	core/qdcore/qd_game_dispatcher_base.o \
	core/qdcore/qd_game_dispatcher.o \
	core/qdcore/qd_game_end.o \
	core/qdcore/qd_game_object_animated.o \
	core/qdcore/qd_game_object_mouse.o \
	core/qdcore/qd_game_object_state.o \
	core/qdcore/qd_game_object_moving.o \
	core/qdcore/qd_game_object_static.o \
	core/qdcore/qd_game_object.o \
	core/qdcore/qd_game_scene.o \
	core/qdcore/qd_grid_zone_state.o \
	core/qdcore/qd_grid_zone.o \
	core/qdcore/qd_interface_background.o \
	core/qdcore/qd_interface_button.o \
	core/qdcore/qd_interface_counter.o \
	core/qdcore/qd_interface_object_base.o \
	core/qdcore/qd_interface_screen.o \
	core/qdcore/qd_interface_slider.o \
	core/qdcore/qd_interface_text_window.o \
	core/qdcore/qd_named_object_reference.o \
	core/qdcore/qd_interface_dispatcher.o \
	core/qdcore/qd_interface_element_state_mode.o \
	core/qdcore/qd_interface_element_state.o \
	core/qdcore/qd_interface_element.o \
	core/qdcore/qd_interface_save.o \
	core/qdcore/qd_inventory_cell.o \
	core/qdcore/qd_inventory.o \
	core/qdcore/qd_minigame_config.o \
	core/qdcore/qd_minigame_interface.o \
	core/qdcore/qd_minigame.o \
	core/qdcore/qd_music_track.o \
	core/qdcore/qd_named_object_base.o \
	core/qdcore/qd_named_object_indexer.o \
	core/qdcore/qd_named_object.o \
	core/qdcore/qd_resource_file.o \
	core/qdcore/qd_resource.o \
	core/qdcore/qd_rnd.o \
	core/qdcore/qd_scale_info.o \
	core/qdcore/qd_screen_text_set.o \
	core/qdcore/qd_screen_text.o \
	core/qdcore/qd_setup.o \
	core/qdcore/qd_sound.o \
	core/qdcore/qd_sprite.o \
	core/qdcore/qd_screen_text_dispatcher.o \
	core/qdcore/qd_sound_info.o \
	core/qdcore/qd_textdb.o \
	core/qdcore/qd_trigger_chain.o \
	core/qdcore/qd_trigger_element.o \
	core/qdcore/qd_trigger_profiler.o \
	core/qdcore/qd_video.o \
	xlibs/xutil/xbuffer/xbsearch.o \
	xlibs/xutil/xbuffer/xbcore.o \
	xlibs/xutil/xbuffer/xbcnvout.o \
	xlibs/xutil/xstream/xscnvout.o \
	xlibs/xutil/xstream/xscore.o \
	xlibs/xutil/xstream/xsgetlin.o \
	xlibs/xutil/xstream/xsinput.o \
	xlibs/xutil/xstream/xsoutput.o \

# This module can be built as a plugin
ifeq ($(ENABLE_QDENGINE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
