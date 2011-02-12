#ifndef __COMPARE_H__
#define __COMPARE_H__

class DocumentEditor;

class Compare {
  
	public:
		Compare(DocumentEditor* doc1_, DocumentEditor* doc2_);
		~Compare();
		
		void diff();
		void clear(DocumentEditor* doc_);
		
	private:
		void initMarker(DocumentEditor* doc_);
		void setMarkerAdd(DocumentEditor* doc_, int line_);
		void setMarkerDel(DocumentEditor* doc_, int line_);

		DocumentEditor* _doc1;
		DocumentEditor* _doc2;
};

#endif // __COMPARE_H__
