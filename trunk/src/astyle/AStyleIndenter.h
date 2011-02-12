#ifndef __ASTYLE_INDENTER_H__
#define __ASTYLE_INDENTER_H__

#include <astyle.h>
#include <QString>

enum {AS_INDENT_WITH_SPACES, AS_INDENT_WITH_TABS, AS_FORCE_INDENT_WITH_TABS};

class AStyleIndenter {
	public:
		AStyleIndenter();
		~AStyleIndenter();
		
		void setStyle(astyle::FormatStyle style_);
		
		/** format the text */
		QString format(const QString& text_);

		inline astyle::ASFormatter& formatter(){
			return _formatter;
		}
	private:
		astyle::ASFormatter _formatter;
};

#endif // __ASTYLE_INDENTER_H__