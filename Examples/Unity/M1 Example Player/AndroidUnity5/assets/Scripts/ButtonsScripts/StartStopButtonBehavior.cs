using System;
using assets.Scripts.Utility;
using UnityEngine;

namespace assets.Scripts.ButtonsScripts
{
    public class StartStopButtonBehavior : AbstractButtonBehavior
    {
        private enum ButtonStateEnum
        {
            Start,
            Stop
        }

        [SerializeField]
        [Header("Set initial button value")]
        // ReSharper disable once InconsistentNaming
        private ButtonStateEnum InitialValue;

        private ButtonStateEnum _currentButtonState;

        // Use this for initialization
        void Start()
        {
            CallThisInStartMethod();
            _currentButtonState = InitialValue;
        }

        // Update is called once per frame
        void Update()
        {
            CallThisInUpdateMethod();
        }

        protected override void CheckIsButtonHit()
        {
            if (IsUserLookOnButton)
            {
                switch (_currentButtonState)
                {
                    case ButtonStateEnum.Start:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.StartPressedMaterial);
                        break;
                    case ButtonStateEnum.Stop:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.StopPressedMaterial);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            else
            {
                switch (_currentButtonState)
                {
                    case ButtonStateEnum.Start:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.StartMaterial);
                        break;
                    case ButtonStateEnum.Stop:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.StopMaterial);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        public override void ButtonPressed()
        {
            switch (_currentButtonState)
            {
                case ButtonStateEnum.Start:
                    _currentButtonState=ButtonStateEnum.Stop;
                    if (OnButtonPressed != null)
                    {
                        OnButtonPressed.Invoke(VideoPlayerButtonEventType.Start);
                    }
                    break;
                case ButtonStateEnum.Stop:
                    _currentButtonState = ButtonStateEnum.Start;
                    if (OnButtonPressed != null)
                    {
                        OnButtonPressed.Invoke(VideoPlayerButtonEventType.Stop);
                    }
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}