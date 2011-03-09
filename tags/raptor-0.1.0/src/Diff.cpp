#include <QList>
#include <QString>
#include <QVariant>
#include <QDebug>
//#include <diff_match_patch.h>
//#include <dtl/dtl.hpp>
#include "document_editor.h"
#include "Diff.h"

#include <iostream>
using namespace std;

namespace{
	const static int MARKER_ADD = 1;
	const static int MARKER_ADD_BACKGROUND = 2;
	const static int MARKER_DEL = 3;
	const static int MARKER_DEL_BACKGROUND = 4;
}

Compare::Compare(DocumentEditor* doc1_, DocumentEditor* doc2_) : _doc1(doc1_), _doc2(doc2_) {
	initMarker(_doc1);
	initMarker(_doc2);
}

Compare::~Compare(){
	//clear(_doc1);
	//clear(_doc2);
}

void Compare::initMarker(DocumentEditor* doc_){
	//marker Add
	doc_->markerDefine(QsciScintilla::Plus, MARKER_ADD);
	doc_->markerDefine(QsciScintilla::Background, MARKER_ADD_BACKGROUND);
	doc_->setMarkerForegroundColor(QColor(100, 100, 100) , MARKER_ADD);
	doc_->setMarkerForegroundColor(QColor(100, 100, 100) , MARKER_ADD_BACKGROUND);
	doc_->setMarkerBackgroundColor(QColor(0, 255, 0), MARKER_ADD);
	doc_->setMarkerBackgroundColor(QColor(0, 255, 0), MARKER_ADD_BACKGROUND);
	//marker Del
	doc_->markerDefine(QsciScintilla::Minus, MARKER_DEL);
	doc_->markerDefine(QsciScintilla::Background, MARKER_DEL_BACKGROUND);
	doc_->setMarkerForegroundColor(QColor(100, 100, 100), MARKER_DEL);
	doc_->setMarkerForegroundColor(QColor(100, 100, 100), MARKER_DEL_BACKGROUND);
	doc_->setMarkerBackgroundColor(QColor(255, 0, 0), MARKER_DEL);
	doc_->setMarkerBackgroundColor(QColor(255, 0, 0), MARKER_DEL_BACKGROUND);
}

void Compare::diff(){
	/*diff_match_patch dmp;
	QString QA = _doc1->text();
	QString QB = _doc2->text();
	QList<Diff> ddiff = dmp.diff_main(QA,QB);
	dmp.diff_cleanupSemantic(ddiff);
	foreach(Diff d, ddiff){
		qDebug() << d.toString() << "\n";
	}
	return;*/
	
	/*QString A("abc");
	QString B("abd");

	dtl::Diff<QChar, QString> d(A, B);
	d.compose();
	qDebug() << "DIFF" << __LINE__;
	
	// editDistance
    qDebug() << "editDistance:" << d.getEditDistance();
    
    // Longest Common Subsequence
    vector< QChar > lcs_v = d.getLcsVec();
    QString       lcs_s(lcs_v.begin(), lcs_v.end());
    qDebug() << "LCS:" << lcs_s << endl;
    
    // Shortest Edit Script
    qDebug() << "SES";
    d.printSES();*/

	//typedef char   elem;
    //typedef string sequence;
	
	/*std::string A("abc");
	std::string B("abd");
	dtl::Diff< char, std::string > d(_doc1->text().toStdString(), _doc2->text().toStdString());
    //d.onOnlyEditDistance();
    d.compose();
    
	dtl::Ses<char> ses = d.getSes();
	
	std::vector< std::pair<char, dtl::elemInfo> > result = ses.getSequence();
	std::vector< std::pair<char, dtl::elemInfo> >::iterator it;
	
	std::string currentStr = "";
	int current = dtl::SES_COMMON;
	
	for(it = result.begin(); it != result.end(); ++it){
		if(it->second.type == current)
			currentStr += it->first;
		else{
			std::cout << current << " " << currentStr << std::endl;
			current = it->second.type;
			currentStr = it->first;
		}
	}*/
	/*switch (result.second.type) {
		case SES_ADD:
			this->out_ << dtl::SES_MARK_ADD    << result.first << endl;
			break;
		case SES_DELETE:
			this->out_ << dtl::SES_MARK_DELETE << result.first << endl;
			break;
		case SES_COMMON:
			this->out_ << dtl::SES_MARK_COMMON << result.first << endl;
			break;
	}*/

    // editDistance
    //cout << "editDistance:" << d.getEditDistance() << endl;
    
    // Longest Common Subsequence
    //vector< elem > lcs_v = d.getLcsVec();
    //sequence       lcs_s(lcs_v.begin(), lcs_v.end());
    //cout << "LCS:" << lcs_s << endl;
    
    // Shortest Edit Script
    //cout << "SES" << endl;
    //d.printSES();
	
	/*setMarkerAdd(_doc1, 1);
	setMarkerDel(_doc1, 3);
	setMarkerDel(_doc2, 1);
	setMarkerAdd(_doc2, 3);*/
}

void Compare::clear(DocumentEditor* doc_){
	doc_->markerDeleteAll(MARKER_ADD);
	doc_->markerDeleteAll(MARKER_ADD_BACKGROUND);
	doc_->markerDeleteAll(MARKER_DEL);
	doc_->markerDeleteAll(MARKER_DEL_BACKGROUND);
}
		

void Compare::setMarkerAdd(DocumentEditor* doc_, int line_){
	doc_->markerAdd(line_, MARKER_ADD);
	doc_->markerAdd(line_, MARKER_ADD_BACKGROUND);
}

void Compare::setMarkerDel(DocumentEditor* doc_, int line_){
	doc_->markerAdd(line_, MARKER_DEL);
	doc_->markerAdd(line_, MARKER_DEL_BACKGROUND);
}
