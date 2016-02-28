using System;
using System.Linq;
using UnityEngine;
using UnityEngine.Events;

namespace assets.Scripts.ButtonsScripts
{
    public enum VideoPlayerButtonEventType
    {
        Back,
        Start,
        Stop,
        Forward
    }

    public abstract class AbstractButtonBehavior : MonoBehaviour
    {
        public bool IsUserLookOnButton;

        [Serializable]
        public class ButtonPressedEventHandler : UnityEvent<VideoPlayerButtonEventType> { }

        [SerializeField]
        protected ButtonPressedEventHandler OnButtonPressed;

        protected ButtonsController ButtonsController;
        protected MeshRenderer MeshRenderer;

        protected void CallThisInStartMethod()
        {
            ButtonsController = FindObjectOfType<ButtonsController>();
            if (ButtonsController == null)
            {
                Debug.LogError("ButtonsController not found");
            }

            MeshRenderer = GetComponent<MeshRenderer>();
            if (MeshRenderer == null)
            {
                Debug.LogError("Mesh rendered not found");
            }
        }

        protected Material GetMaterial(string materialName)
        {
            var material = ButtonsController.Materials.First(
                                x => x.Name.Equals(materialName)).Materal;
            return material;
        }

        protected void CallThisInUpdateMethod()
        {
            CheckIsButtonHit();
        }

        protected abstract void CheckIsButtonHit();

        public abstract void ButtonPressed();
    }
}