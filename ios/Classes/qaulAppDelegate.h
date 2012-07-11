/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#import <UIKit/UIKit.h>

@class qaulViewController;

@interface qaulAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    qaulViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet qaulViewController *viewController;

@end

