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

#ifndef QDENGINE_QDCORE_QD_MINIGAME_CONFIG_H
#define QDENGINE_QDCORE_QD_MINIGAME_CONFIG_H

namespace QDEngine {
/// Конфигурационные данные для миниигры.
/**
В .ini файле:
имя секции - название параметра

ключи
type = "string" / "file" / "object" - тип данных data_type_
count - количество данных, data_count_
value - строка данных, data_string_
comment - комментарий, comment_

Если тип данных не указан, то считается равным "string".

Пример:

[ParameterExample]
type = "object"
count = 2
value = "Object0"
comment = "пример параметра"
*/
class qdMinigameConfigParameter {
public:
	qdMinigameConfigParameter();
	~qdMinigameConfigParameter();

	bool operator == (const qdMinigameConfigParameter &prm) const {
		return (name_ == prm.name_);
	}
	bool operator == (const char *str) const {
		return !strcmp(str, name());
	}

	//! Тип данных.
	enum data_type_t {
		/// данные - строка
		PRM_DATA_STRING,
		/// данные - имя файла
		PRM_DATA_FILE,
		/// данные - имя объекта из игровой сцены
		PRM_DATA_OBJECT
	};

	const char *name() const {
		return name_.c_str();
	}
	void set_name(const char *name) {
		name_ = name;
	}

	data_type_t data_type() const {
		return data_type_;
	}
	void set_data_type(data_type_t tp) {
		data_type_ = tp;
	}

	const char *data_string() const {
		return data_string_.c_str();
	}
	void set_data_string(const char *str) {
		data_string_ = str;
	}

	const char *comment() const {
		return comment_.c_str();
	}
	void set_comment(const char *str) {
		comment_ = str;
	}

	int data_count() const {
		return data_count_;
	}
	void set_data_count(int cnt) {
		data_count_ = cnt;
	}

	/// Проверяет валидность данных.
	/**
	Если данные не того формата, то возвращает false и устанавливает
	is_data_valid_ в false.
	*/
	bool validate_data();
	bool is_data_valid() const {
		return is_data_valid_;
	}

	//! Загрузка данных из скрипта.
	bool load_script(const xml::tag *p);
	//! Запись данных в скрипт.
	bool save_script(Common::WriteStream &fh, int indent = 0) const;

	//! Загрузка данных из .ini файла.
	bool load_ini(const char *ini_file, const char *ini_section);

private:

	/// Имя параметра, данные из миниигры запрашиваются по нему.
	std::string name_;

	/// Тип данных.
	data_type_t data_type_;

	/// Количество данных.
	/**
	Используется для числовых данных (указывает, сколько чисел записано в data_string_)
	и объектов (в сцене создаётся соответствующее количество копий объекта по имени data_string_,
	к их именам добавляется четырёхзначный порядковый номер).

	По умолчанию = 1.
	*/
	int data_count_;

	/// Строка данных.
	/**
	Формат зависит от типа данных.
	Для числовых данных - числа в текстовом виде через пробел, для
	остальных типов - просто строка.
	*/
	std::string data_string_;

	/// Комментарий.
	std::string comment_;

	/// false если строка данных не того формата.
	bool is_data_valid_;
};

} // namespace QDEngine

#endif // QDENGINE_QDCORE_QD_MINIGAME_CONFIG_H
