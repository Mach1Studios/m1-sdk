using System;
using assets.OSscripts.Utilities;
using UnityEngine;

namespace assets.Scripts.ButtonsScripts
{
    public class ButtonBehavior : AbstractButtonBehavior
    {
        public enum VideoPlayerButtonType
        {
            Back,
            Forward
        }

        [Header("Set button type")]
        public VideoPlayerButtonType ButtonType;

        // Use this for initialization
        void Start()
        {
            CallThisInStartMethod();
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
                switch (ButtonType)
                {
                    case VideoPlayerButtonType.Back:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.BackPressedMaterial);
                        break;
                    case VideoPlayerButtonType.Forward:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.ForwardPressedMaterial);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
            else
            {
                switch (ButtonType)
                {
                    case VideoPlayerButtonType.Back:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.BackMaterial);
                        break;
                    case VideoPlayerButtonType.Forward:
                        MeshRenderer.material = GetMaterial(MaterialsNameMapping.ForwardMaterial);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }
        }

        public override void ButtonPressed()
        {
            switch (ButtonType)
            {
                case VideoPlayerButtonType.Back:
                    if (OnButtonPressed != null)
                    {
                        OnButtonPressed.Invoke(VideoPlayerButtonEventType.Back);
                    }
                    break;
                case VideoPlayerButtonType.Forward:
                    if (OnButtonPressed != null)
                    {
                        OnButtonPressed.Invoke(VideoPlayerButtonEventType.Forward);
                    }
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }
        }
    }
}
