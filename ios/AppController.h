//
//  ShaderTestAppController.h
//  ShaderTest
//
//  Created by  georgehsiang on 13/3/11.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

