//
//  OLMKitTests.m
//  OLMKitTests
//
//  Created by Chris Ballinger on 4/8/16.
//
//

#import <XCTest/XCTest.h>
#import <OLMKit/OLMKit.h>

@interface OLMKitTests : XCTestCase

@end

@implementation OLMKitTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testAliceAndBob {
    NSError *error;

    OLMAccount *alice = [[OLMAccount alloc] initNewAccount];
    OLMAccount *bob = [[OLMAccount alloc] initNewAccount];
    [bob generateOneTimeKeys:5];
    NSDictionary *bobIdKeys = bob.identityKeys;
    NSString *bobIdKey = bobIdKeys[@"curve25519"];
    NSDictionary *bobOneTimeKeys = bob.oneTimeKeys;
    NSParameterAssert(bobIdKey != nil);
    NSParameterAssert(bobOneTimeKeys != nil);
    __block NSString *bobOneTimeKey = nil;
    NSDictionary *bobOtkCurve25519 = bobOneTimeKeys[@"curve25519"];
    [bobOtkCurve25519 enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        bobOneTimeKey = obj;
    }];
    XCTAssert([bobOneTimeKey isKindOfClass:[NSString class]]);
    
    OLMSession *aliceSession = [[OLMSession alloc] initOutboundSessionWithAccount:alice theirIdentityKey:bobIdKey theirOneTimeKey:bobOneTimeKey error:nil];
    NSString *message = @"Hello!";
    OLMMessage *aliceToBobMsg = [aliceSession encryptMessage:message error:&error];
    XCTAssertNil(error);
    
    OLMSession *bobSession = [[OLMSession alloc] initInboundSessionWithAccount:bob oneTimeKeyMessage:aliceToBobMsg.ciphertext error:nil];
    NSString *plaintext = [bobSession decryptMessage:aliceToBobMsg error:&error];
    XCTAssertEqualObjects(message, plaintext);
    XCTAssertNil(error);
    BOOL success = [bob removeOneTimeKeysForSession:bobSession];
    XCTAssertTrue(success);
}

- (void) testBackAndForth {
    OLMAccount *alice = [[OLMAccount alloc] initNewAccount];
    OLMAccount *bob = [[OLMAccount alloc] initNewAccount];
    [bob generateOneTimeKeys:1];
    NSDictionary *bobIdKeys = bob.identityKeys;
    NSString *bobIdKey = bobIdKeys[@"curve25519"];
    NSDictionary *bobOneTimeKeys = bob.oneTimeKeys;
    NSParameterAssert(bobIdKey != nil);
    NSParameterAssert(bobOneTimeKeys != nil);
    __block NSString *bobOneTimeKey = nil;
    NSDictionary *bobOtkCurve25519 = bobOneTimeKeys[@"curve25519"];
    [bobOtkCurve25519 enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        bobOneTimeKey = obj;
    }];
    XCTAssert([bobOneTimeKey isKindOfClass:[NSString class]]);
    
    OLMSession *aliceSession = [[OLMSession alloc] initOutboundSessionWithAccount:alice theirIdentityKey:bobIdKey theirOneTimeKey:bobOneTimeKey error:nil];
    NSString *message = @"Hello I'm Alice!";
    OLMMessage *aliceToBobMsg = [aliceSession encryptMessage:message error:nil];
    
    OLMSession *bobSession = [[OLMSession alloc] initInboundSessionWithAccount:bob oneTimeKeyMessage:aliceToBobMsg.ciphertext error:nil];
    NSString *plaintext = [bobSession decryptMessage:aliceToBobMsg error:nil];
    XCTAssertEqualObjects(message, plaintext);
    BOOL success = [bob removeOneTimeKeysForSession:bobSession];
    XCTAssertTrue(success);
    
    NSString *msg1 = @"Hello I'm Bob!";
    NSString *msg2 = @"Isn't life grand?";
    NSString *msg3 = @"Let's go to the opera.";
    
    OLMMessage *eMsg1 = [bobSession encryptMessage:msg1 error:nil];
    OLMMessage *eMsg2 = [bobSession encryptMessage:msg2 error:nil];
    OLMMessage *eMsg3 = [bobSession encryptMessage:msg3 error:nil];
    
    NSString *dMsg1 = [aliceSession decryptMessage:eMsg1 error:nil];
    NSString *dMsg2 = [aliceSession decryptMessage:eMsg2 error:nil];
    NSString *dMsg3 = [aliceSession decryptMessage:eMsg3 error:nil];
    XCTAssertEqualObjects(msg1, dMsg1);
    XCTAssertEqualObjects(msg2, dMsg2);
    XCTAssertEqualObjects(msg3, dMsg3);

    
}

- (void) testAccountSerialization {
    OLMAccount *bob = [[OLMAccount alloc] initNewAccount];
    [bob generateOneTimeKeys:5];
    NSDictionary *bobIdKeys = bob.identityKeys;
    NSDictionary *bobOneTimeKeys = bob.oneTimeKeys;
    
    NSData *bobData = [NSKeyedArchiver archivedDataWithRootObject:bob];
    
    OLMAccount *bob2 = [NSKeyedUnarchiver unarchiveObjectWithData:bobData];
    NSDictionary *bobIdKeys2 = bob2.identityKeys;
    NSDictionary *bobOneTimeKeys2 = bob2.oneTimeKeys;
    
    XCTAssertEqualObjects(bobIdKeys, bobIdKeys2);
    XCTAssertEqualObjects(bobOneTimeKeys, bobOneTimeKeys2);
}

- (void) testSessionSerialization {
    NSError *error;

    OLMAccount *alice = [[OLMAccount alloc] initNewAccount];
    OLMAccount *bob = [[OLMAccount alloc] initNewAccount];
    [bob generateOneTimeKeys:1];
    NSDictionary *bobIdKeys = bob.identityKeys;
    NSString *bobIdKey = bobIdKeys[@"curve25519"];
    NSDictionary *bobOneTimeKeys = bob.oneTimeKeys;
    NSParameterAssert(bobIdKey != nil);
    NSParameterAssert(bobOneTimeKeys != nil);
    __block NSString *bobOneTimeKey = nil;
    NSDictionary *bobOtkCurve25519 = bobOneTimeKeys[@"curve25519"];
    [bobOtkCurve25519 enumerateKeysAndObjectsUsingBlock:^(id  _Nonnull key, id  _Nonnull obj, BOOL * _Nonnull stop) {
        bobOneTimeKey = obj;
    }];
    XCTAssert([bobOneTimeKey isKindOfClass:[NSString class]]);
    
    OLMSession *aliceSession = [[OLMSession alloc] initOutboundSessionWithAccount:alice theirIdentityKey:bobIdKey theirOneTimeKey:bobOneTimeKey error:nil];
    NSString *message = @"Hello I'm Alice!";
    OLMMessage *aliceToBobMsg = [aliceSession encryptMessage:message error:&error];
    XCTAssertNil(error);

    
    OLMSession *bobSession = [[OLMSession alloc] initInboundSessionWithAccount:bob oneTimeKeyMessage:aliceToBobMsg.ciphertext error:nil];
    NSString *plaintext = [bobSession decryptMessage:aliceToBobMsg error:nil];
    XCTAssertEqualObjects(message, plaintext);
    BOOL success = [bob removeOneTimeKeysForSession:bobSession];
    XCTAssertTrue(success);
    
    NSString *msg1 = @"Hello I'm Bob!";
    NSString *msg2 = @"Isn't life grand?";
    NSString *msg3 = @"Let's go to the opera.";
    
    OLMMessage *eMsg1 = [bobSession encryptMessage:msg1 error:nil];
    OLMMessage *eMsg2 = [bobSession encryptMessage:msg2 error:nil];
    OLMMessage *eMsg3 = [bobSession encryptMessage:msg3 error:nil];
    
    NSData *aliceData = [NSKeyedArchiver archivedDataWithRootObject:aliceSession];
    OLMSession *alice2 = [NSKeyedUnarchiver unarchiveObjectWithData:aliceData];
    
    NSString *dMsg1 = [alice2 decryptMessage:eMsg1 error:nil];
    NSString *dMsg2 = [alice2 decryptMessage:eMsg2 error:nil];
    NSString *dMsg3 = [alice2 decryptMessage:eMsg3 error:nil];
    XCTAssertEqualObjects(msg1, dMsg1);
    XCTAssertEqualObjects(msg2, dMsg2);
    XCTAssertEqualObjects(msg3, dMsg3);
}

- (void)testEd25519Signing {

    OLMUtility *olmUtility = [[OLMUtility alloc] init];
    OLMAccount *alice = [[OLMAccount alloc] initNewAccount];

    NSDictionary *aJSON = @{
                            @"key1": @"value1",
                            @"key2": @"value2"
                            };
    NSData *message = [NSKeyedArchiver archivedDataWithRootObject:aJSON];
    NSString *signature = [alice signMessage:message];


    NSString *aliceEd25519Key = alice.identityKeys[@"ed25519"];

    NSError *error;
    BOOL result = [olmUtility verifyEd25519Signature:signature key:aliceEd25519Key message:message error:&error];
    XCTAssert(result);
    XCTAssertNil(error);
}

@end
