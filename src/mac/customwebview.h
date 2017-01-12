#ifndef CUSTOMWEBVIEW_H
#define CUSTOMWEBVIEW_H

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>
#import <WebKit/WebView.h>
#import <WebKit/WebResourceLoadDelegate.h>
#import <WebKit/WebFrameLoadDelegate.h>
#include <QPointer>
#include "webviewinqt.h"

@interface CustomWebView: WebView<WebResourceLoadDelegate, WebFrameLoadDelegate> {

    QPointer<WebViewInQt> pTarget;
}

-(id) initWithObjects:(NSRect)frameRect frameName:(NSString *)frameName
                groupName:(NSString *)groupName target:(QPointer<WebViewInQt>) target;

@end

#endif
