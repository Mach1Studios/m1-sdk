//
//Filename: KeyboardCameraControl.cs
//
 
using UnityEngine;
 
[AddComponentMenu("Camera-Control/Keyboard")]
public class KeyboardCameraControl : MonoBehaviour
{
    // Keyboard axes buttons in the same order as Unity
    public enum KeyboardAxis { Horizontal = 0, Vertical = 1, None = 3 }
 
    [System.Serializable]
    // Handles left modifiers keys (Alt, Ctrl, Shift)
    public class Modifiers
    {
        public bool leftAlt;
        public bool leftControl;
        public bool leftShift;
 
        public bool checkModifiers()
        {
            return (!leftAlt ^ Input.GetKey(KeyCode.LeftAlt)) &&
                (!leftControl ^ Input.GetKey(KeyCode.LeftControl)) &&
                (!leftShift ^ Input.GetKey(KeyCode.LeftShift));
        }
    }
 
    [System.Serializable]
    // Handles common parameters for translations and rotations
    public class KeyboardControlConfiguration
    {
 
        public bool activate;
        public KeyboardAxis keyboardAxis;
        public Modifiers modifiers;
        public float sensitivity;
 
        public bool isActivated()
        {
            return activate && keyboardAxis != KeyboardAxis.None && modifiers.checkModifiers();
        }
    }
 
    // Yaw default configuration
    public KeyboardControlConfiguration yaw = new KeyboardControlConfiguration { keyboardAxis = KeyboardAxis.Horizontal, modifiers = new Modifiers { leftAlt = true }, sensitivity = 1F };
 
    // Pitch default configuration
    public KeyboardControlConfiguration pitch = new KeyboardControlConfiguration { keyboardAxis = KeyboardAxis.Vertical, modifiers = new Modifiers { leftAlt = true }, sensitivity = 1F };
 
    // Roll default configuration
    public KeyboardControlConfiguration roll = new KeyboardControlConfiguration { keyboardAxis = KeyboardAxis.Horizontal, modifiers = new Modifiers { leftAlt = true, leftControl = true}, sensitivity = 1F };
 
    // Vertical translation default configuration
    public KeyboardControlConfiguration verticalTranslation = new KeyboardControlConfiguration { keyboardAxis = KeyboardAxis.Vertical, modifiers = new Modifiers { leftControl = true }, sensitivity = 0.5F };
 
    // Horizontal translation default configuration
    public KeyboardControlConfiguration horizontalTranslation = new KeyboardControlConfiguration { keyboardAxis = KeyboardAxis.Horizontal, sensitivity = 0.5F };
 
    // Depth (forward/backward) translation default configuration
    public KeyboardControlConfiguration depthTranslation = new KeyboardControlConfiguration { keyboardAxis = KeyboardAxis.Vertical, sensitivity = 0.5F };
 
    // Default unity names for keyboard axes
    public string keyboardHorizontalAxisName = "Horizontal";
    public string keyboardVerticalAxisName = "Vertical";
 
 
    private string[] keyboardAxesNames;
 
    void Start()
    {
        keyboardAxesNames = new string[] { keyboardHorizontalAxisName, keyboardVerticalAxisName};
    }
 
 
    // LateUpdate  is called once per frame after all Update are done
    void LateUpdate()
    {
        if (yaw.isActivated())
        {
            float rotationX = Input.GetAxis(keyboardAxesNames[(int)yaw.keyboardAxis]) * yaw.sensitivity;
            transform.Rotate(0, rotationX, 0);
        }
        if (pitch.isActivated())
        {
            float rotationY = Input.GetAxis(keyboardAxesNames[(int)pitch.keyboardAxis]) * pitch.sensitivity;
            transform.Rotate(-rotationY, 0 , 0);
        }
        if (roll.isActivated())
        {
            float rotationZ = Input.GetAxis(keyboardAxesNames[(int)roll.keyboardAxis]) * roll.sensitivity;
            transform.Rotate(0, 0, rotationZ);
        }
        if (verticalTranslation.isActivated())
        {
            float translateY = Input.GetAxis(keyboardAxesNames[(int)verticalTranslation.keyboardAxis]) * verticalTranslation.sensitivity;
            transform.Translate(0, translateY, 0);
        }
        if (horizontalTranslation.isActivated())
        {
            float translateX = Input.GetAxis(keyboardAxesNames[(int)horizontalTranslation.keyboardAxis]) * horizontalTranslation.sensitivity;
            transform.Translate(translateX, 0, 0);
        }
        if (depthTranslation.isActivated())
        {
            float translateZ = Input.GetAxis(keyboardAxesNames[(int)depthTranslation.keyboardAxis]) * depthTranslation.sensitivity;
            transform.Translate(0, 0, translateZ);
        }
 
 
	}
}