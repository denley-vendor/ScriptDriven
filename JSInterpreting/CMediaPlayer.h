//
// CMediaPlayer.h
//

#ifndef __H_CMediaPlayer__
#define __H_CMediaPlayer__

#include "IDispatchImpl.h"
#include "js_demo_h.h"

class CMediaPlayer : public IDispatchImpl<CMediaPlayer
										, IMediaPlayer
										, &IID_IMediaPlayer>
{
public:
	CMediaPlayer(ITypeLib* ptl);
	~CMediaPlayer();

	// APIs exposed to scripts
	STDMETHODIMP get_majorVersion(UINT *pVal);
	STDMETHODIMP get_minorVersion(UINT *pVal);
	STDMETHODIMP get_vendor(BSTR *pVal);
	STDMETHODIMP get_currentPosition(UINT *pVal);
	STDMETHODIMP put_currentPosition(UINT pVal);

	STDMETHODIMP play(BSTR mediaFile);
	STDMETHODIMP pauseOn();
	STDMETHODIMP pauseOff();
	STDMETHODIMP stop();

private:
	UINT		mCurrentPosition;
};

#endif // __H_CMediaPlayer__