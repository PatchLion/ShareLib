//
//  CNotifyWebView.h
//  Hare
//
//  Created by new on 15-5-20.
//  Copyright (c) 2015年 huang. All rights reserved.
//

#ifndef __CNOTIFYWEBVIEW__H__
#define __CNOTIFYWEBVIEW__H__
#import <WebKit/WebKit.h>

#include "IWebviewContainer.h"

@interface CNotifyWebView : WebView {
    
@private
    IWebviewContainer* webviewContainer;
}
-(id)init;
-(void)setContainer: (IWebviewContainer*)obj;

@end

#endif
