#ifndef EMBEDDEDICONEXTRACTOR_H
#define EMBEDDEDICONEXTRACTOR_H

#include <QtCore>
#include <QtWin>

class EmbeddedIconExtractor
{
	public:
		EmbeddedIconExtractor();
		EmbeddedIconExtractor(QString fName);
		EmbeddedIconExtractor(wchar_t *fName);
		EmbeddedIconExtractor(QString fName, QPixmap *outputPixmap);
		EmbeddedIconExtractor(wchar_t *fName, QPixmap *outputPixmap);

		QPixmap extract();
		QPixmap extract(QString fName);
		QPixmap extract(wchar_t *fName);
		void extract(QString fName, QPixmap *outputPixmap);
		void extract(wchar_t *fName, QPixmap *outputPixmap);

		QPixmap getExtractedPixmap();

		void setFile(QString fName);
		void setFile(wchar_t *fName);

	private:
		wchar_t *_fName;
		QPixmap _pixmap;
};

#endif // EMBEDDEDICONEXTRACTOR_H
