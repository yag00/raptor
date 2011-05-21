/**
 * This file is part of the raptor project.
 *
 * Copyright (C) 2011 Christophe Duvernois <christophe.duvernois@gmail.com>
 *
 * Raptor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Raptor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __VERSION_H__
#define __VERSION_H__

#include <stdint.h>
#include <string>

class QString;

class Version {
	public:
		/** constructor
		 * @param version_ string */
		Version(const char* version_);
		/** constructor
		 * @param version_ string */
		Version(const std::string& version_);
		/** constructor
		 * @param version_ string */
		Version(const QString& version_);
		/** constructor
		 * @param major_ major version number
		 * @param minor_ minor version number
		 * @param patch_ patch version number */
		Version(uint32_t major_, uint32_t minor_, uint32_t patch_);

		/** destructor */
		virtual ~Version();

		/** @return filename */
		QString getVersion() const;
		/** @return major version number */
		uint32_t getMajor() const;
		/** @return minor version number */
		uint32_t getMinor() const;
		/** @return patch version number */
		uint32_t getPatch() const;

		bool isNull();
		
		bool operator==(const Version& v_);
		bool operator!=(const Version& v_);
		bool operator<(const Version& v_);
		bool operator<=(const Version& v_);
		bool operator>(const Version& v_);
		bool operator>=(const Version& v_);

	private:
		uint32_t _major;
		uint32_t _minor;
		uint32_t _patch;
};

static const Version RaptorVersion(PACKAGE_VERSION);

#endif /* __VERSION_H__ */
