#ifndef _WIN32
// even if included by mistake this file should not be compiled under windows

#include "win_compat.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>

int MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	CFStringRef cfTitle =
	    CFStringCreateWithCString(nullptr, lpCaption, kCFStringEncodingUTF8);
	CFStringRef cfMessage =
	    CFStringCreateWithCString(nullptr, lpText, kCFStringEncodingUTF8);
	CFOptionFlags responseFlags;

	CFOptionFlags alertLevel = kCFUserNotificationPlainAlertLevel;
	if (uType & MB_ICONERROR)
		alertLevel = kCFUserNotificationStopAlertLevel;
	if (uType & MB_ICONINFORMATION)
		alertLevel = kCFUserNotificationNoteAlertLevel;

	CFUserNotificationDisplayAlert(
	    0, alertLevel, nullptr, nullptr, nullptr, cfTitle, cfMessage, nullptr,
	    (uType & MB_YESNO) ? CFSTR("") : nullptr, nullptr, &responseFlags);

	CFRelease(cfTitle);
	CFRelease(cfMessage);

	if (uType & MB_YESNO) {
		return (responseFlags == kCFUserNotificationDefaultResponse) ? IDYES
		                                                             : IDNO;
	}
	return IDOK;
}
#endif

#endif