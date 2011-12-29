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

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <dtl/dtl.hpp>

#include <QFileInfo>
#include <QDateTime>

#include "document_editor.h"
#include "Unidiff.h"


Unidiff::Unidiff() {

}

Unidiff::~Unidiff(){

}

QString Unidiff::diff(DocumentEditor& doc1_, DocumentEditor& doc2_){
	std::vector<std::string> alines;
	std::vector<std::string> blines;

	for (int line = 0; line < doc1_.lines(); line++){
		std::string str = doc1_.text(line).remove("\r").remove("\n").toStdString();
		alines.push_back(str);
	}
	for (int line = 0; line < doc2_.lines(); line++){
		std::string str = doc2_.text(line).remove("\r").remove("\n").toStdString();
		blines.push_back(str);
	}

	dtl::Diff<std::string> diff(alines, blines);
    diff.onHuge();
    //diff.onUnserious();
    diff.compose();
    diff.composeUnifiedHunks();

	std::ostringstream oss;
    if (diff.getEditDistance() > 0) {
		// show file info
		QFileInfo file1(doc1_.getFullPath());
		if(file1.exists())
			oss << "+++ " << doc1_.getName().toStdString() << '\t' << file1.lastModified().toString("yyyy-MM-dd hh:mm:ss").toStdString() << std::endl;
		else
			oss << "+++ " << doc1_.getName().toStdString() << " (new file)" << std::endl;
		QFileInfo file2(doc2_.getFullPath());
		if(file2.exists())
			oss << "--- " << doc2_.getName().toStdString() << '\t' << file2.lastModified().toString("yyyy-MM-dd hh:mm:ss").toStdString() << std::endl;
		else
			oss << "--- " << doc2_.getName().toStdString() << " (new file)" << std::endl;
    }
    diff.printUnifiedFormat(oss);
	return QString(oss.str().c_str());
}

QString Unidiff::diff(const QString& file1_, const QString& file2_){
	std::vector<std::string> alines;
	std::vector<std::string> blines;

	std::ifstream file1(file1_.toStdString().c_str());
    std::ifstream file2(file2_.toStdString().c_str());
	
	std::string buffer;
    while(getline(file1, buffer)){
        alines.push_back(buffer);
    }
    while(getline(file2, buffer)){
        blines.push_back(buffer);
    }

	dtl::Diff<std::string> diff(alines, blines);
    diff.onHuge();
    //diff.onUnserious();
    diff.compose();
    diff.composeUnifiedHunks();

	std::ostringstream oss;
    if (diff.getEditDistance() > 0) {
		// show file info
		QFileInfo file1(file1_);
		if(file1.exists())
			oss << "+++ " << file1_.toStdString() << '\t' << file1.lastModified().toString("yyyy-MM-dd hh:mm:ss").toStdString() << std::endl;
		else
			oss << "+++ " << file1_.toStdString() << std::endl;
		QFileInfo file2(file2_);
		if(file2.exists())
			oss << "--- " << file2_.toStdString() << '\t' << file2.lastModified().toString("yyyy-MM-dd hh:mm:ss").toStdString() << std::endl;
		else
			oss << "--- " << file2_.toStdString() << std::endl;
    }
    diff.printUnifiedFormat(oss);
	return QString(oss.str().c_str());
}
