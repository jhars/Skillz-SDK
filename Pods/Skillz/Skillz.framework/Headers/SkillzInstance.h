//
//  SkillzInstance.h
//  SkillzSDK-iOS
//
//  Copyright (c) 2014 Skillz. All rights reserved.
//  http://skillz.com/
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKit.h>

@class SKZMatchInfo;
@class SKZPlayer;
@class SKZTurnBasedMatchInfo;

/**
 * Skillz requires explicitly linking with the following Frameworks:
 *  1. libz
 *  2. libsqlite3
 *  3. libxml2
 *
 * Skillz requires enabling "Modules" (this auto-links all the required Frameworks):
 *
 *  Under "Apple LLVM 5.0 - Languages - Modules" in Build Settings, set
 *   - Enable Modules (C and Objective-C): YES
 *   - Link Frameworks Automatically: YES
 *
 * Skillz also requires the following linker flags:
 *  1. -ObjC
 *  2. -lc++
 *  3. -lz
 */


#pragma mark - Skillz SDK Enums

/**
 * NS_ENUM defining the different servers that Skillz can connect to.
 */
typedef NS_ENUM (NSInteger, SkillzEnvironment) {
    /** Used when connecting to the live production server. */
    SkillzProduction,
    /** Used when connecting to the test sandbox server. */
    SkillzSandbox
};

/**
 * NS_ENUM defining the orientations that Skillz can be launched in.
 */
typedef NS_ENUM (NSInteger, SkillzOrientation) {
    /** Used to launch Skillz in a portrait orientation. */
    SkillzPortrait,
    /** Used to launch Skillz in a landscape orientation. This will match the landscape orientation of your game. */
    SkillzLandscape
};


///----------------------------------------------------
///@name Skillz SDK Delegate Protocol
///----------------------------------------------------


#pragma mark - Skillz SDK Delegate Protocol

/**
 * SkillzBaseDelegate is the protocol used by applications to interface with Skillz.
 *
 * Do NOT define an object as conforming to SkillzBaseDelegate, rather your delegate should conform either to SkillzDelegate or
 * SkillzTurnBasedDelegate
 *
 * Required methods in this protocol should be implemented in your application for basic Skillz functionality.
 */
@protocol SkillzBaseDelegate <NSObject>

@required
/**
 * Skillz will only query for this value upon display of the Skillz UI, either via completion of game, or initial launch.
 *
 * @return The orientation that will be used to construct the Skillz UI. The Skillz UI will then be locked to this orientation
 *         until it is dismissed.
 */
- (SkillzOrientation)preferredSkillzInterfaceOrientation;

@optional
/**
 * This method will be called when the Skillz SDK will exit. It will NOT be called when a Skillz Tournament is launched.
 */
- (void)skillzWillExit;

/**
 * This method will be called before the Skillz UI launches. Use this to clean up any state needed before you launch Skillz.
 */
- (void)skillzWillLaunch;

/**
 * This method will be called once the Skillz UI has finished displaying. Use this to clean up your view hierarchy.
 */
- (void)skillzHasFinishedLaunching;

#pragma mark User Engagement

/**
 * This method must be overridden to allow your game to launch into Skillz from sources external to your application
 * (e.g. from Skillz-run advertisements). By default, this method returns FALSE and this functionality is disabled.
 *
 * Implementation:
 *
 *     - Return TRUE to signify that your application is in a state where it is safe for Skillz to launch.
 *     - Return FALSE to signify that it is NOT safe for Skillz to launch at this time.
 *
 * For example:
 *
 *     - If the user is mid-gameplay, return FALSE. This ensures that gameplay is not interrupted.
 *     - If the user is at a splash screen or in an options menu, return TRUE. This is a safe place to launch into Skillz from.
 *
 * When returning TRUE, be sure that any relevant state is cleaned up in the skillzWillLaunch method.
 *
 * @return If FALSE is returned, Skillz will not launch.
 */
- (BOOL)shouldSkillzLaunchFromURL;

@end

/**
 * If you plan to support standard Skillz tournaments, your Skillz delegate should conform to this protocol and implement its
 * required methods.
 *
 * NOTE: Your Skillz Delegate may also conform to SkillzTurnBasedDelegate if you plan to support both play types.
 */
@protocol SkillzDelegate <SkillzBaseDelegate>

@required
/**
 * This method will be called when a typical Skillz tournament will launch. You should use this method for constructing a new game
 * based on the supplied argument.
 *
 * @param gameParameters dictionary contains the Game Parameters that were configured in the Skillz Developer Portal
 * @param matchInfo class contain data relevant to the current match
 */
- (void)tournamentWillBegin:(NSDictionary *)gameParameters
              withMatchInfo:(SKZMatchInfo *)matchInfo;

@optional
/**
 * Deprecated, use tournamentWillBegin:withMatchInfo:
 */
- (void)tournamentWillBegin:(NSDictionary *)gameParameters __attribute__ ((deprecated));

@end


/**
 * If you plan to support Turn Based Gameplay, your Skillz delegate should conform to this protocol.
 *
 * NOTE: Your Skillz Delegate may also conform to SkillzDelegate if you plan to support both play types.
 */
@protocol SkillzTurnBasedDelegate <SkillzBaseDelegate>

#pragma mark Turn Based Methods

@required
/**
 * This method will be called when a turn based tournament type will launch. You should use this method to either construct a new
 * game, or continue an ongoing tournament.
 *
 * @param gameParameters       This dictionary contains Game Parameters that were configured in the Skillz Developer Portal
 * @param currentGameStateInfo The SKZTurnBasedMatchInfo object will give you all current information about an ongoing turn based
 *                             game, please refer to the Skillz documentation for more info.
 */
- (void)turnBasedTournamentWillBegin:(NSDictionary *)gameParameters
                 withTurnInformation:(SKZTurnBasedMatchInfo *)currentGameStateInfo;

@optional
/**
 * This method is optional, even for Turn Based Gameplay. By implementing this method, you allow the user to review ongoing turn
 * based tournaments from the Skillz UI.
 * When this method is called, construct your game as you would for turnBasedTournamentWillBegin:withTurnInformation:, but do not
 * allow the player to make a move.
 *
 * @param gameParameters       This dictionary contains the Game Parameters for the tournament that is about to be reviewed.
 * @param currentGameStateInfo The SKZTurnBasedMatchInfo object will give you all current information about an ongoing turn based 
 *                             game, please refer to the Skillz documentation for more info.
 */
- (void)turnBasedGameReviewWillBegin:(NSDictionary *)gameParameters
            withGameStateInformation:(SKZTurnBasedMatchInfo *)currentGameStateInfo;

@end

///----------------------------------------------------
///@name Skillz SDK Interface
///----------------------------------------------------

#pragma mark - Skillz SDK Interface

__attribute__ ((visibility("default")))
NS_AVAILABLE_IOS(8_0)

/**
 * Main interface for the Skillz SDK
 */
@interface Skillz : NSObject <CLLocationManagerDelegate, UIPopoverControllerDelegate>


///------------------------------
///@name Skillz SDK Properties
///------------------------------

/**
 * Whether or not a Skillz match is currently in progress.
 */
@property BOOL tournamentIsInProgress;

/**
 * The current SkillzBaseDelegate instance
 */
@property (readonly, strong) id<SkillzBaseDelegate> skillzDelegate;


#pragma mark - Skillz SDK Functions

///----------------------------------------------------
///@name Skillz SDK Class Methods
///----------------------------------------------------

/**
 * Get a singleton reference to the Skillz SDK
 *
 * @return The singleton instance of the Skillz SDK object
 */
+ (Skillz *)skillzInstance;

/**
 * Returns a random integer supplied by the Skillz SDK to ensure fairness across competition games.
 *
 * The range of this function is [0, RAND_MAX) (0 is inclusive, RAND_MAX is exclusive)
 *
 * On iOS, RAND_MAX is 2,147,483,647
 *
 * Players in the same tournament will receive the same sequence of random numbers.
 */
+ (NSInteger)getRandomNumber;

/**
 * Returns a random floating point value supplied by the Skillz SDK to ensure fairness across competition games.
 *
 * The range of this function is [0, 1.0) (0 is inclusive, 1.0 is exclusive)
*
 * Players in the same tournament will receive the same sequence of random numbers.
 */
+ (CGFloat)getRandomFloat;

/**
 * Returns a random unsigned integer supplied by the Skillz SDK to ensure fairness across competition games.
 *
 * Number will be in the range [min,max) (min is inclusive, max is exclusive), and the probability will be evenly distributed 
 * amongst all the possible values.
 *
 * [Skillz skillzgetRandomNumberWithMin:2 andMax:10] will return one of the following numbers with equal probability: 
 * 2,3,4,5,6,7,8,9
 *
 * Players in the same tournament will receive the same sequence of random numbers.
 *
 * @param min  The minimum possible value returned, inclusive.
 * @param max  The maximum possible value returned, exclusive.
 */
+ (NSUInteger)getRandomNumberWithMin:(NSUInteger)min
                              andMax:(NSUInteger)max;

///----------------------------------------------------
///@name Skillz SDK Instance Methods
///----------------------------------------------------

/**
 * Initialize Skillz
 *
 * This needs to be called within application:didFinishLaunchingWithOptions in your App Delegate.
 * Will not launch the Skillz experience, this function only establishes the connection between your game and the SkillzEnvironment
 * of choice.
 *
 * @param gameId           Your game ID as given to you on the Skillz developer portal
 * @param delegate         This delegate must implement all required methods of the SkillzBaseDelegate protocol
 * @param environment      SkillzSandbox for sandbox testing or SkillzProduction for app store submission.
 * @param allowExit        Whether to allow the user to exit the Skillz experience
 *
 */
- (void)initWithGameId:(NSString *)gameId
           forDelegate:(id <SkillzBaseDelegate>)delegate
         withEnvironment:(SkillzEnvironment)environment
             allowExit:(BOOL)flag;

/**
 * Launch the Skillz Experience
 *
 * This function is what will actually render the Skillz experience on screen. This function will draw a ViewController onto your
 * view hierarchy.
 *
 * Because of this, do not call this method while attempting to draw another ViewController on screen as well.
 */
- (void)launchSkillz;

/**
 * This method must be called each time the current player's score changes during a Skillz match.
 *
 * For example, in many games this method is called when the player scores points, when the player is penalized, and whenever a
 * time bonus is applied. It is OK for this method to
 * be called very often.
 *
 * If a continuous in-game score is displayed to the player, this method is generally called as often as that score display is
 * updated - usually by placing the updatePlayersCurrentScore call in the same place within the game loop.
 *
 * @param currentScoreForPlayer Current score value for the player
 */
- (void)updatePlayersCurrentScore:(NSNumber *)currentScoreForPlayer;

/**
 * Call this function to report the player's score to Skillz. Ends the current tournament, and returns the user to the Skillz
 * experience.
 *
 * @param score            Numeric value representing the player's final score
 * @param completion       Completion will be called on wrap up so that the developer can finish any ongoing processes, such as
 *                         saving game data or removing the game from the view hierarchy.
 */
- (void)displayTournamentResultsWithScore:(NSNumber *)score
                           withCompletion:(void (^)(void))completion;

/**
 * Call this function when a player aborts a Skillz match in progress. Forfeits the match and brings the user back into the Skillz
 * experience.
 *
 * @param completion      Completion will be called on wrap up so that the developer can finish any ongoing processes, such as
 *                        saving game data or removing the game from the view hierarchy.
 */
- (void)notifyPlayerAbortWithCompletion:(void (^)(void))completion;

/**
 * If your game plays its own background music that you'd like to play in the Skillz UI, set hasBackgroundMusic to YES to prevent 
 * the Skillz music from being played.
 *
 * @param hasBackgroundMusic       Whether or not your game uses background music.
 */
- (void)setGameHasBackgroundMusic:(BOOL)hasBackgroundMusic;

#pragma mark - Sklillz SDK Information

///----------------------------------------------------
///@name Skillz SDK Information
///----------------------------------------------------

/**
 * Get the version of the Skillz SDK
 *
 * @return The SDK Version
 */
+ (NSString *)SDKShortVersion;

/**
 * Display the long version of SDK Info in a UIAlertView
 */
+ (void)showSDKVersionInfo;

/**
 * This will return a value confirming whether or not the Skillz UI is currently presented
 *
 * @return True if Skillz UI is currently presented, otherwise false.
 */
+ (BOOL)isSkillzPresented;

/**
 * This will return the UIInterfaceOrientationMask set for Skillz, typically this is only used by our UnityViewControllerBase.
 *
 * @return UIInterfaceOrientationMask for Skillz
 */
+ (UIInterfaceOrientationMask)skillzOrientation;

/**
 * Get the current logged in player. Use this method if you need this
 * information outside of a tournament.
 *
 * @return SKZPlayer object that represent the current player. If there is no
 * player currently logged in, will return nil.
 */
+ (SKZPlayer *)player;

/**
 *  DEPRECATED: Use the player method instead.
 */
+ (NSString *)currentUserDisplayName __attribute__ ((deprecated));

/**
   Deprecated
 */
- (void)initWithGameId:(NSString *)gameId
           forDelegate:(id <SkillzBaseDelegate>)delegate
       withEnvironment:(SkillzEnvironment)environment;

@end

