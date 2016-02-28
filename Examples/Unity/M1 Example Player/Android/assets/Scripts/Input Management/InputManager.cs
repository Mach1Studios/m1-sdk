using System;
using assets.Scripts.Utility;
using UnityEngine;

// InputManager is responsible for checking whether or not the user pressed/tapped from any input devices. If so,
// a flag is set and can be queried each frame by calling CheckForClick()
// TODO: Add additional input detection and utility functions.
namespace assets.Scripts.Input_Management
{
    public class InputManager: MonoBehaviour
    {
        public event EventHandler InputDetected; 

        //public bool IsPressed { get; private set; }

        // Update is called once per frame
        void Update()
        {
            //IsPressed = false;
            DetectInputEvents();
        }

        // Called every frame from update to check for input events.
        void DetectInputEvents()
        {
            if (Input.GetKeyDown(KeyCode.Mouse0))
            {
                DiagnosticMessage.Print("KeyCode.Mouse0");
                OnInputDetected();
            }

            if (Input.GetKeyDown(KeyCode.JoystickButton2))
            {
                DiagnosticMessage.Print("KeyCode.JoystickButton2");
                OnInputDetected();
            }

            if (Input.GetKeyDown(KeyCode.JoystickButton0))
            {
                DiagnosticMessage.Print("KeyCode.JoystickButton0");
                OnInputDetected();
            }
        }

        protected virtual void OnInputDetected()
        {
            var handler = InputDetected;
            if (handler != null) handler(this, EventArgs.Empty);
        }
    }
}