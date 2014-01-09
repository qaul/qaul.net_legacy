/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#import "QaulView.h"


@implementation QaulView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		counter = 0;
    }
    return self;
}

- (void)drawRect:(NSRect)rect {
    // Drawing code here.
	// set white background color
    [[NSColor whiteColor] setFill];
    NSRectFill(rect);
	
	//Qaullib_IpcReceive();
	//NSLog(@"IpcReceive called");
	
	//if (counter < 10) 
	//{
	//	NSLog(@"drawRect called"); 
	//	counter++;
	//}
}

@end
