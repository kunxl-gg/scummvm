/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define FORBIDDEN_SYMBOL_ALLOW_ALL
#include "common/debug.h"

#include "qdengine/qdengine.h"
#include "qdengine/qd_precomp.h"

#include "qdengine/parser/xml_tag_buffer.h"
#include "qdengine/parser/qdscr_parser.h"

#include "qd_interface_element.h"
#include "qd_interface_button.h"
#include "qd_interface_screen.h"
#include "qd_interface_dispatcher.h"

#include "qd_game_dispatcher.h"
#include "qd_game_scene.h"
#include "qd_game_object_moving.h"


namespace QDEngine {

struct qdInterfaceElementsOrdering {
	bool operator()(const qdInterfaceElement *p0, const qdInterfaceElement *p1) {
		return p0->screen_depth() < p1->screen_depth();
	}
};

qdInterfaceScreen::qdInterfaceScreen() : is_locked_(false),
	autohide_time_(0.0f),
	autohide_phase_(1.0f),
	autohide_offset_(Vect2i(0, 0)),
	modal_caller_(NULL) {
	sorted_elements_.reserve(20);
}

qdInterfaceScreen::~qdInterfaceScreen() {
	elements_.clear();
}

#ifdef _QUEST_EDITOR
bool qdInterfaceScreen::select_element(qdInterfaceElement *pele, bool bselect) {
	assert(static_cast<qdInterfaceScreen *>(pele->owner()) == this);

	element_list_t::iterator itr = std::find(selected_elements_.begin(),
	                               selected_elements_.end(), pele);
	if (bselect) {
		if (itr == selected_elements_.end())
			selected_elements_.push_back(pele);
	} else {
		if (itr == selected_elements_.end())
			return false;
		selected_elements_.erase(itr);
	}
	return true;
}

void qdInterfaceScreen::clear_selection() {
	selected_elements_.clear();
}

bool qdInterfaceScreen::is_selected(const qdInterfaceElement *p) const {
	return (std::find(selected_elements_.begin(), selected_elements_.end(), p)
	        != selected_elements_.end());
}
#endif // _QUEST_EDITOR

bool qdInterfaceScreen::redraw(int dx, int dy) const {
#ifdef _QUEST_EDITOR
	qdInterfaceElement::set_screen_offset(Vect2i(dx, dy));
	qdInventoryCell::set_screen_offset(Vect2i(dx, dy));
#endif

	for (sorted_element_list_t::const_reverse_iterator it = sorted_elements_.rbegin(); it != sorted_elements_.rend(); ++it)
		(*it)->redraw();

	return true;
}

bool qdInterfaceScreen::pre_redraw(bool force_full_redraw) {
	qdGameDispatcher *dp = qdGameDispatcher::get_dispatcher();
	if (!dp) return false;

	if (force_full_redraw) {
		for (sorted_element_list_t::iterator it = sorted_elements_.begin(); it != sorted_elements_.end(); ++it) {
			if ((*it)->last_screen_region() != (*it)->screen_region())
				dp->add_redraw_region((*it)->last_screen_region());

			dp->add_redraw_region((*it)->screen_region());
		}
	} else {
		for (sorted_element_list_t::iterator it = sorted_elements_.begin(); it != sorted_elements_.end(); ++it) {
			if ((*it)->need_redraw()) {
				if ((*it)->last_screen_region() != (*it)->screen_region())
					dp->add_redraw_region((*it)->last_screen_region());

				dp->add_redraw_region((*it)->screen_region());
			}
		}
	}

	return true;
}

bool qdInterfaceScreen::post_redraw() {
	for (sorted_element_list_t::iterator it = sorted_elements_.begin(); it != sorted_elements_.end(); ++it)
		(*it)->post_redraw();

	return true;
}

bool qdInterfaceScreen::quant(float dt) {
	debugC(9, "qdInterfaceScreen::quant(%f)", dt);
	if (autohide_time_ > FLT_EPS) {
		float delta = dt / autohide_time_;

		qdInterfaceDispatcher *dp = dynamic_cast<qdInterfaceDispatcher *>(owner());

		if (dp && !dp->is_autohide_enabled())
			autohide_phase_ -= delta;
		else
			autohide_phase_ += delta;

		if (autohide_phase_ < 0.0f) autohide_phase_ = 0.0f;
		if (autohide_phase_ > 1.0f) autohide_phase_ = 1.0f;

		int x = round(float(autohide_offset_.x) * autohide_phase_);
		int y = round(float(autohide_offset_.y) * autohide_phase_);

		qdInterfaceElement::set_screen_offset(Vect2i(x, y));
		qdInventoryCell::set_screen_offset(Vect2i(x, y));
	} else {
		qdInterfaceElement::set_screen_offset(Vect2i(0, 0));
		qdInventoryCell::set_screen_offset(Vect2i(0, 0));
	}

	for (element_list_t::const_iterator it = element_list().begin(); it != element_list().end(); ++it) {
		if ((*it)->linked_to_option() && qdInterfaceDispatcher::option_value((*it)->option_ID()) != (*it)->option_value())
			qdInterfaceDispatcher::set_option_value((*it)->option_ID(), (*it)->option_value());

		(*it)->quant(dt);
	}

	return true;
}

bool qdInterfaceScreen::save_script(Common::WriteStream &fh, int indent) const {
	for (int i = 0; i < indent; i++) {
		fh.writeString("\t");
	}
	fh.writeString("<interface_screen");

	if (name()) {
		fh.writeString(Common::String::format(" name=\"%s\"", qdscr_XML_string(name())));
	}

	if (autohide_time_ > FLT_EPS) {
		fh.writeString(Common::String::format(" hide_time=\"%f\"", autohide_time_));
	}

	if (autohide_offset_.x || autohide_offset_.y) {
		fh.writeString(Common::String::format(" hide_offset=\"%d %d\"", autohide_offset_.x, autohide_offset_.y));
	}
	fh.writeString(">\r\n");

	if (has_music_track()) {
		music_track_.save_script(fh, indent + 1);
	}

	for (auto &it : element_list()) {
		it->save_script(fh, indent + 1);
	}


	for (int i = 0; i < indent; i++) {
		fh.writeString("\t");
	}
	fh.writeString("</interface_screen>\r\n");

	return true;
}

bool qdInterfaceScreen::load_script(const xml::tag *p) {
	for (xml::tag::subtag_iterator it = p->subtags_begin(); it != p->subtags_end(); ++it) {
		switch (it->ID()) {
		case QDSCR_NAME:
			set_name(it->data());
			break;
		case QDSCR_INTERFACE_ELEMENT:
			if (const xml::tag * tg = it->search_subtag(QDSCR_TYPE)) {
				if (qdInterfaceElement * el = qdInterfaceElement::create_element(static_cast<qdInterfaceElement::element_type>(xml::tag_buffer(*tg).get_int()))) {
					el->set_owner(this);
					el->load_script(&*it);
					add_element(el);
				}
			}
			break;
		case QDSCR_INTERFACE_SCREEN_HIDE_TIME:
			xml::tag_buffer(*it) > autohide_time_;
			break;
		case QDSCR_INTERFACE_SCREEN_HIDE_OFFSET:
			xml::tag_buffer(*it) > autohide_offset_.x > autohide_offset_.y;
			break;
		case QDSCR_MUSIC_TRACK:
			music_track_.load_script(&*it);
			break;
		}
	}

	return true;
}

bool qdInterfaceScreen::add_element(qdInterfaceElement *p) {
	if (elements_.add_object(p)) {
		sorted_elements_.push_back(p);
		sort_elements();
		return true;
	}

	return false;
}

bool qdInterfaceScreen::rename_element(qdInterfaceElement *p, const char *name) {
	return elements_.rename_object(p, name);
}

bool qdInterfaceScreen::remove_element(qdInterfaceElement *p) {
	sorted_element_list_t::iterator it = std::find(sorted_elements_.begin(), sorted_elements_.end(), p);
	if (it != sorted_elements_.end())
		sorted_elements_.erase(it);

#ifdef _QUEST_EDITOR
	selected_elements_.remove(p);
#endif // _QUEST_EDITOR

	return elements_.remove_object(p);
}

qdInterfaceElement *qdInterfaceScreen::get_element(const char *el_name) {
	return elements_.get_object(el_name);
}

bool qdInterfaceScreen::is_element_in_list(const qdInterfaceElement *el) const {
	return elements_.is_in_list(el);
}

bool qdInterfaceScreen::mouse_handler(int x, int y, mouseDispatcher::mouseEvent ev) {
	debugC(9, kDebugInput, "qdInterfaceScreen::mouse_handler(%d, %d, %lu)", x, y, sorted_elements_.size());
	if (qdInterfaceDispatcher *dp = dynamic_cast<qdInterfaceDispatcher* >(owner())) {
		for (auto &it : sorted_elements_) {
			if (it->hit_test(x, y)) {
				dp->toggle_mouse_hover();
				if (it->get_element_type() != qdInterfaceElement::EL_TEXT_WINDOW)
					dp->disable_autohide();

				if (it->mouse_handler(x, y, ev) && !it->is_locked())
					return true;
			} else
				it->hover_clear();
		}
	}

	return false;
}

bool qdInterfaceScreen::keyboard_handler(int vkey) {
	for (element_list_t::const_iterator it = element_list().begin(); it != element_list().end(); ++it) {
		if (!(*it)->is_locked() && (*it)->keyboard_handler(vkey))
			return true;
	}

	return false;
}

bool qdInterfaceScreen::char_input_handler(int vkey) {
	for (element_list_t::const_iterator it = element_list().begin(); it != element_list().end(); ++it) {
		if (!(*it)->is_locked() && (*it)->char_input_handler(vkey))
			return true;
	}

	return false;
}

qdResource *qdInterfaceScreen::add_resource(const char *file_name, const qdInterfaceElementState *res_owner) {
	if (qdInterfaceDispatcher * dp = dynamic_cast<qdInterfaceDispatcher * >(owner())) {
		if (qdResource * p = dp->add_resource(file_name, res_owner)) {
			resources_.register_resource(p, res_owner);
			if (dp->is_screen_active(this) && !p->is_resource_loaded())
				p->load_resource();

			return p;
		}
	}

	return NULL;
}

bool qdInterfaceScreen::remove_resource(const char *file_name, const qdInterfaceElementState *res_owner) {
	if (qdInterfaceDispatcher * dp = dynamic_cast<qdInterfaceDispatcher * >(owner())) {
		if (qdResource * p = dp->get_resource(file_name)) {
			resources_.unregister_resource(p, res_owner);
			return dp->remove_resource(file_name, res_owner);
		}
	}

	return false;
}

bool qdInterfaceScreen::init(bool is_game_active) {
	for (element_list_t::const_iterator it = element_list().begin(); it != element_list().end(); ++it) {
		(*it)->init(is_game_active);
		if ((*it)->linked_to_option())
			(*it)->set_option_value(qdInterfaceDispatcher::option_value((*it)->option_ID()));
	}

	build_visible_elements_list();

	return true;
}

bool qdInterfaceScreen::hide_element(const char *element_name, bool temporary_hide) {
	if (qdInterfaceElement * p = get_element(element_name))
		return hide_element(p, temporary_hide);

	return false;
}

bool qdInterfaceScreen::hide_element(qdInterfaceElement *p, bool temporary_hide) {
	if (!temporary_hide)
		p->hide();

	sorted_element_list_t::iterator it = std::find(sorted_elements_.begin(), sorted_elements_.end(), p);
	if (it != sorted_elements_.end())
		sorted_elements_.erase(it);

	return true;
}

bool qdInterfaceScreen::show_element(const char *element_name) {
	if (qdInterfaceElement * p = get_element(element_name))
		return show_element(p);

	return false;
}

bool qdInterfaceScreen::show_element(qdInterfaceElement *p) {
	p->show();

	sorted_element_list_t::iterator it = std::find(sorted_elements_.begin(), sorted_elements_.end(), p);
	if (it == sorted_elements_.end()) {
		sorted_elements_.push_back(p);
		sort_elements();
	}

	return true;
}

bool qdInterfaceScreen::sort_elements() {
	std::sort(sorted_elements_.begin(), sorted_elements_.end(), qdInterfaceElementsOrdering());
	return true;
}

bool qdInterfaceScreen::build_visible_elements_list() {
	sorted_elements_.clear();

	for (element_list_t::const_iterator it = element_list().begin(); it != element_list().end(); ++it) {
		if ((*it)->is_visible())
			sorted_elements_.push_back(*it);
	}

	sort_elements();

	return true;
}

void qdInterfaceScreen::activate_personage_buttons(const qdNamedObject *p) {
	for (element_list_t::const_iterator it = element_list().begin(); it != element_list().end(); ++it) {
		if (qdInterfaceButton * bt = dynamic_cast<qdInterfaceButton * >(*it)) {
			if (const qdInterfaceEvent * ev = bt->find_event(qdInterfaceEvent::EVENT_ACTIVATE_PERSONAGE)) {
				if (p) {
					if (ev->has_data() && !strcmp(p->name(), ev->event_data()))
						bt->activate_state(1);
					else
						bt->activate_state(0);
				} else
					bt->activate_state(0);
			}
		}
	}
}

void qdInterfaceScreen::update_personage_buttons() {
	qdGameDispatcher *p = qdGameDispatcher::get_dispatcher();
	if (!p) return;

	qdGameScene *sp = p->get_active_scene();
	if (!sp) return;

	for (element_list_t::const_iterator it = element_list().begin(); it != element_list().end(); ++it) {
		if (qdInterfaceButton * bt = dynamic_cast<qdInterfaceButton * >(*it)) {
			if (!sp->set_personage_button(bt)) {
				if (bt->find_event(qdInterfaceEvent::EVENT_ACTIVATE_PERSONAGE))
					hide_element(bt);
			}
		}
	}
}
} // namespace QDEngine
