using System;
using assets.Scripts.Utility;
using UnityEngine;

namespace assets.Scripts.ButtonsScripts
{
    public class ButtonsController : MonoBehaviour
    {
        public GameObject Aim;

        [Serializable]
        public struct NamedMaterial
        {
            public string Name;
            public Material Materal;
        }

        [Tooltip("back, start, stop, forward, back_pressed, start_pressed, stop_pressed, forward_pressed")]
        [Header("Required materials in tooltip")]
        public NamedMaterial[] Materials =
        {
            new NamedMaterial { Name = MaterialsNameMapping.BackMaterial },
            new NamedMaterial { Name = MaterialsNameMapping.StartMaterial },
            new NamedMaterial { Name = MaterialsNameMapping.StopMaterial},
            new NamedMaterial { Name = MaterialsNameMapping.ForwardMaterial },
            new NamedMaterial { Name = MaterialsNameMapping.BackPressedMaterial },
            new NamedMaterial { Name = MaterialsNameMapping.StartPressedMaterial},
            new NamedMaterial { Name = MaterialsNameMapping.StopPressedMaterial},
            new NamedMaterial { Name = MaterialsNameMapping.ForwardPressedMaterial },
        };

        public bool ShowButtons;

        // Use this for initialization
        void Start()
        {
            if (Aim == null)
            {
                Debug.LogError("Aim not set");
            }

            SetActiveButtonsAndAim(false);
        }

        // Update is called once per frame
        void Update()
        {
            SetActiveButtonsAndAim(ShowButtons);
        }

        private void SetActiveButtonsAndAim(bool isActive)
        {
            Aim.SetActive(isActive);
            for (int i = 0; i < transform.childCount; i++)
            {
                transform.GetChild(i).gameObject.SetActive(isActive);
            }
        }
    }
}
