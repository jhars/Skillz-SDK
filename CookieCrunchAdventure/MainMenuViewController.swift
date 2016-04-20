import UIKit
import SpriteKit
import AVFoundation
import Skillz


class MainMenuViewController : UIViewController {
    
    override func prefersStatusBarHidden() -> Bool {
        return true
    }
    
    override func shouldAutorotate() -> Bool {
        return false
    }
    
    override func supportedInterfaceOrientations() -> UIInterfaceOrientationMask {
        return UIInterfaceOrientationMask.Portrait
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Configure the view.
        let skView = view as! SKView
        skView.multipleTouchEnabled = false
    }
    @IBAction func skillzButton(sender: UIButton) {
        Skillz.skillzInstance().launchSkillz();
    }
}