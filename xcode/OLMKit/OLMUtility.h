//
//  OLMUtility.h
//  olm
//
//  Created by Chris Ballinger on 4/8/16.
//
//

#import <Foundation/Foundation.h>

@interface OLMUtility : NSObject

/**
 Verify an ed25519 signature.

 @param key the ed25519 key.
 @param message the message which was signed.
 @param signature the base64-encoded signature to be checked.
 @param the result error if there is a problem with the verification.
 If the key was too small then the message will be "OLM.INVALID_BASE64".
 If the signature was invalid then the message will be "OLM.BAD_MESSAGE_MAC".

 @return YES if valid.
 */
- (BOOL)ed25519Verify:(NSString*)key message:(NSString*)message signature:(NSString*)signature error:(NSError**)error;

+ (NSMutableData*) randomBytesOfLength:(NSUInteger)length;

@end