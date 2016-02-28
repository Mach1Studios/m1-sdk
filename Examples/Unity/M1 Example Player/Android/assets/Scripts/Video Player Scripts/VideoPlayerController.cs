using System;
using System.Collections.Generic;
using System.Linq;
using assets.OSscripts.Utilities;
using assets.Scripts.ButtonsScripts;
using assets.Scripts.Input_Management;
using assets.Scripts.Startup_Management;
using UnityEngine;

namespace assets.Scripts.Video_Player_Scripts
{
    public class VideoPlayerController : MonoBehaviour
    {
        private readonly Dictionary<string, AbstractButtonBehavior> _videoControlButtons = new Dictionary<string, AbstractButtonBehavior>();

        private DateTime _timer;

        [Header("Delay in milliseconds")]
        public float UpdateTime = 200f;

        private StartupManager _startupManager;
        private InputManager _inputManager;
        private ButtonsController _buttonsController;

        // Use this for initialization
        void Start()
        {
            var inputManagerObject = GameObject.FindGameObjectWithTag(Tags.InputManager);
            if (inputManagerObject == null)
            {
                Debug.LogError("Failed to find InputManager object");
            }
            else
            {
                _inputManager = inputManagerObject.GetComponent<InputManager>();
                _inputManager.InputDetected += _inputManager_InputDetected;
            }

            var startupManagerObject = GameObject.FindGameObjectWithTag(Tags.StartupManager);
            if (startupManagerObject == null)
            {
                Debug.LogError("Failed to find StartupManager object");
            }
            else
            {
                _startupManager = startupManagerObject.GetComponent<StartupManager>();
                _startupManager.ScriptWasFinished += _startupManager_ScriptWasFinished;
            }

            var buttonsControllerObject = GameObject.FindGameObjectWithTag(Tags.ButtonsController);
            if (buttonsControllerObject == null)
            {
                Debug.LogError("Failed to find ButtonsController object");
            }
            else
            {
                _buttonsController = buttonsControllerObject.GetComponent<ButtonsController>();
            }
        }

        // Update is called once per frame
        void Update()
        {
            if (_buttonsController.ShowButtons)
            {
                RaycastCameraLook();
            }
        }

        private void RaycastCameraLook()
        {
            if (_timer < DateTime.Now)
            {
                _timer = DateTime.Now.AddMilliseconds(UpdateTime);

                RaycastHit hit;
                if (Physics.Raycast(transform.position, transform.forward, out hit))
                {
                    //DiagnosticMessage.Print(hit.transform.name);
                    CheckIfRaycastHitButton(hit);
                }
                else
                {
                    SetThatUserLookOnThisButton(string.Empty);
                    //DiagnosticMessage.Print("");
                }
            }
        }

        private void CheckIfRaycastHitButton(RaycastHit hit)
        {
            var nameOfHitButton = hit.transform.name;
            if (_videoControlButtons.ContainsKey(nameOfHitButton))
            {
                SetThatUserLookOnThisButton(nameOfHitButton);
            }
            else
            {
                var buttonBehaviorController = hit.transform.GetComponent<AbstractButtonBehavior>();
                if (buttonBehaviorController != null)
                {
                    _videoControlButtons.Add(nameOfHitButton, buttonBehaviorController);
                    SetThatUserLookOnThisButton(nameOfHitButton);
                }
                else
                {
                    SetThatUserLookOnThisButton(string.Empty);
                }
            }
        }

        private void SetThatUserLookOnThisButton(string nameOfButton)
        {
            AbstractButtonBehavior fromDictionary = null;

            if (_videoControlButtons.ContainsKey(nameOfButton))
            {
                fromDictionary = _videoControlButtons[nameOfButton];
                fromDictionary.IsUserLookOnButton = true;
            }

            foreach (
                var buttonBehaviorController in
                    _videoControlButtons.Values.Where(x => !x.Equals(fromDictionary)))
            {
                buttonBehaviorController.IsUserLookOnButton = false;
            }
        }

        private void _startupManager_ScriptWasFinished(object sender, EventArgs e)
        {
            _buttonsController.ShowButtons = true;
        }

        private void _inputManager_InputDetected(object sender, EventArgs e)
        {
            if (_buttonsController.ShowButtons)
            {
                var userLookOnButton = _videoControlButtons.Values.FirstOrDefault(x => x.IsUserLookOnButton);
                if (userLookOnButton != null)
                {
                    userLookOnButton.ButtonPressed();
                }
                else
                {
                    _buttonsController.ShowButtons = false;
                }
            }
            else
            {
                _buttonsController.ShowButtons = true;
            }
        }
    }
}
