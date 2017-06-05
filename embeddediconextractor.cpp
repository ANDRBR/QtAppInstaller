#include "embeddediconextractor.h"

EmbeddedIconExtractor::EmbeddedIconExtractor() : _fName(nullptr){}

EmbeddedIconExtractor::EmbeddedIconExtractor(QString fName){
	setFile(fName);
}

EmbeddedIconExtractor::EmbeddedIconExtractor(wchar_t *fName) : _fName(fName){}

EmbeddedIconExtractor::EmbeddedIconExtractor(QString fName, QPixmap *outputPixmap){
	*outputPixmap = extract(fName);
}

EmbeddedIconExtractor::EmbeddedIconExtractor(wchar_t *fName, QPixmap *outputPixmap){
	*outputPixmap = extract(fName);
}

QPixmap EmbeddedIconExtractor::extract(){
	HINSTANCE hInstance = ::GetModuleHandle(NULL);
	HICON Icon = ::ExtractIcon(hInstance, _fName, 0);
	_pixmap = QtWin::fromHICON(Icon);
	return _pixmap;
}

QPixmap EmbeddedIconExtractor::extract(QString fName){
	setFile(fName);
	return extract();
}

QPixmap EmbeddedIconExtractor::extract(wchar_t *fName){
	setFile(fName);
	return extract();
}

void EmbeddedIconExtractor::extract(QString fName, QPixmap *outputPixmap){
	*outputPixmap = extract(fName);
}

void EmbeddedIconExtractor::extract(wchar_t *fName, QPixmap *outputPixmap){
	*outputPixmap = extract(fName);
}

QPixmap EmbeddedIconExtractor::getExtractedPixmap(){
	return _pixmap;
}

void EmbeddedIconExtractor::setFile(QString fName){
	_fName = new wchar_t[fName.length() + 1];
	fName.toWCharArray(_fName);
	_fName[fName.length()] = '\0';
}

void EmbeddedIconExtractor::setFile(wchar_t *fName){
	_fName = fName;
}
