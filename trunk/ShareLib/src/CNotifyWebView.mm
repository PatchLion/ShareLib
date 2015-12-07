//
//  CNotifyWebView.m
//  Hare
//
//  Created by new on 15-5-20.
//  Copyright (c) 2015年 huang. All rights reserved.
//

#import "CNotifyWebView.h"
#import <WebKit/WebFrameLoadDelegate.h>

@implementation CNotifyWebView

-(id)init
{
    self = [super init];
    if (self)
    {
        [self setFrameLoadDelegate:self];
        [self setUIDelegate:self];
    }
    
    return self;
}
-(id) free
{
    NSLog(@"CNotifyWebView release!");
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}
-(void)webView:(WebView *)sender didFailLoadWithError:(NSError *)error forFrame:(WebFrame *)frame
{
     if (webviewContainer)
    {
        webviewContainer->onPageLoadFinished(ShareLibrary::Result_OtherError);
    }
}

-(void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame *)frame
{
    if (webviewContainer)
    {
        webviewContainer->onPageLoadFinished(ShareLibrary::Result_Success);
    }
}
-(void)setContainer:(IWebviewContainer *)obj
{
    webviewContainer = obj;
}

@end
